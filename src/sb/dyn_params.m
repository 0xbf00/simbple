//
// Created by Jakob Rieck on 20.02.18.
//

#include "dyn_params.h"
#include <CoreFoundation/CoreFoundation.h>
#include <Foundation/Foundation.h>
#include <scheme_interpreter/scheme-private.h>
#include <misc/misc.h>

pointer cf2sc(scheme *, CFTypeRef);
typedef struct {
    scheme *sc;
    pointer result;
} conversion_data_t;

static NSDictionary *load_plist(const char *filename)
{
    NSString *filePath = [NSString stringWithUTF8String: filename];
    if ([[NSFileManager defaultManager] fileExistsAtPath: filePath])
    {
        return [NSDictionary dictionaryWithContentsOfFile: filePath];
    }

    return NULL;
}

static void process_key_value_pair(const void *key, const void *value, void *context)
{
    conversion_data_t *ctx = context;

    pointer sc_key = cf2sc(ctx->sc, key);
    pointer sc_value = cf2sc(ctx->sc, value);

    pointer pair = cons(ctx->sc, sc_key, sc_value);
    ctx->result  = cons(ctx->sc, pair, ctx->result);
}

/**
 * Convert CFStringRef to C-String.
 * The caller is responsible to free the object
 * @param input String to convert
 * @return Pointer to c string (char buffer)
 */
static char *cfstr_ascii(CFTypeRef input)
{
    assert(CFStringGetTypeID() == CFGetTypeID(input));

    CFStringRef str = input;

    const CFIndex len = CFStringGetMaximumSizeOfFileSystemRepresentation(str);
    char *out = calloc(len, 1);

    assert(TRUE == CFStringGetFileSystemRepresentation(str, out, len));
    return out;
}

/**
 * Convert Core Foundation type to scheme type. Inspired by xpc2sc (as implemented in libsandbox)
 * @param sc The current scheme context
 * @param input Core Foundation object
 * @return
 */
pointer cf2sc(scheme *sc, CFTypeRef input)
{
    if (CFGetTypeID(input) == CFStringGetTypeID()) {
        char *c_str = cfstr_ascii(input);
        pointer result = mk_string(sc, c_str);
        free(c_str);
        return result;
    }
    else if (CFGetTypeID(input) == CFBooleanGetTypeID()) {
        CFBooleanRef b = input;
        return (CFBooleanGetValue(b) == TRUE) ? sc->T : sc->F;
    }
    else if (CFGetTypeID(input) == CFArrayGetTypeID()) {
        CFArrayRef arr = input;
        const CFIndex count = CFArrayGetCount(arr);

        pointer out = sc->NIL;

        // We go through the array backwards because we push the elements backwards
        for (CFIndex i = count; i != 0; --i) {
            out = cons(sc, cf2sc(sc, CFArrayGetValueAtIndex(arr, i - 1)), out);
        }
        return out;
    }
    else if (CFGetTypeID(input) == CFNullGetTypeID()) {
        return sc->NIL;
    }
    else if (CFGetTypeID(input) == CFNumberGetTypeID()) {
        CFNumberRef num = input;
        CFNumberType num_type = CFNumberGetType(num);

        long value = 0;
        assert(sizeof(value) >= CFNumberGetByteSize(num));
        assert(TRUE == CFNumberGetValue(num, kCFNumberIntType, &value));

        return mk_integer(sc, value);
    }
    else if (CFGetTypeID(input) == CFDictionaryGetTypeID()) {
        conversion_data_t conversion = {
                .sc = sc,
                .result = sc->NIL
        };

        CFDictionaryApplyFunction(input, process_key_value_pair, &conversion);

        return conversion.result;
    } else {
        assert(false && "Invalid type specified.");
    }
}

NSDictionary *extract_params(const char *filename)
{
    NSDictionary *dict = load_plist(filename);

    NSDictionary *validation_info = dict[@"SandboxProfileDataValidationInfo"];
    // Key used in Container.plist files up to 10.15.3
    NSDictionary *result = validation_info[@"SandboxProfileDataValidationParametersKey"];

    if (!result) {
        // Key used in Container.plist files starting in 10.15.4
        result = validation_info[@"Parameters"];
    }
    return result;
}

void register_params(scheme *sc, const char *filename)
{
    pointer res = cf2sc(sc, (__bridge CFDictionaryRef) extract_params(filename));

    scheme_support.register_object(sc, "*params*", res);
}

NSArray *extract_additional_sandbox_snippets(const char *filename)
{
    NSDictionary *dict = load_plist(filename);

    NSDictionary *validation_info = dict[@"SandboxProfileDataValidationInfo"];
    // works on <= 10.15.3
    NSArray *result = validation_info[@"SandboxProfileDataValidationSnippetDictionariesKey"];

    if (!result) {
        // >= 10.15.4
        result = validation_info[@"SystemProfiles"];
    }

    return result;
}

/**
 * Load additional sandbox snippets that were part of the original compilation.
 * Thankfully, they are preserved in-order in the Container.plist file
 * @param sc
 * @param filename
 */
void load_additional_sandbox_snippets(scheme *sc, const char *filename)
{
    NSArray *snippets = extract_additional_sandbox_snippets(filename);
    
    for (NSDictionary *dict in snippets) {
        NSString *profilePath = dict[@"AppSandboxProfileSnippetPathKey"];

        // We've already loaded these profiles, do not load them again
        if ([profilePath hasSuffix: @"application.sb"] || 
            [profilePath hasSuffix: @"system.sb"])
            continue;

        scheme_support.load_file(sc, [profilePath UTF8String]);
    }
}

NSDictionary *extract_entitlements(const char *filename)
{
    NSDictionary *dict = load_plist(filename);
    NSDictionary *validation_info = dict[@"SandboxProfileDataValidationInfo"];
    if (!validation_info)
        return nil;

    NSMutableDictionary *entitlements = [NSMutableDictionary dictionaryWithCapacity: 5];
    if (validation_info[@"SandboxProfileDataValidationEntitlementsKey"]) {
        [entitlements addEntriesFromDictionary: validation_info[@"SandboxProfileDataValidationEntitlementsKey"]];
    } else {
        [entitlements addEntriesFromDictionary: validation_info[@"Entitlements"]];
    }

    NSArray *redirectable_paths = validation_info[@"SandboxProfileDataValidationRedirectablePathsKey"];
    if (!redirectable_paths) {
        redirectable_paths = validation_info[@"RedirectablePaths"];
    }
    [entitlements setObject: redirectable_paths forKey: @"com.apple.private.app-sandbox.redirectable-paths"];

    NSArray *redirected_paths = validation_info[@"SandboxProfileDataValidationRedirectedPathsKey"];
    if (!redirected_paths) {
        redirected_paths = validation_info[@"RedirectedPaths"];
    }
    [entitlements setObject: redirected_paths forKey: @"com.apple.private.app-sandbox.redirected-paths"];

    NSArray *system_images = validation_info[@"SystemImages"];
    if (system_images) {
        [entitlements setObject: system_images forKey: @"com.apple.private.app-sandbox.system-frameworks"];
    }

    return [NSDictionary dictionaryWithDictionary: entitlements];
}

void register_entitlements(scheme *sc, const char *filename)
{
    pointer res = cf2sc(sc, (__bridge CFDictionaryRef) extract_entitlements(filename));

    scheme_support.register_object(sc, "*entitlements*", res);
}

/**
 * Validate a container metadata file.
 * Check that all required keys are available and that the file is a valid plist file.
 *
 * @param filename Filename of the container metadata to check.
 * @return 0 on success
 */
int validate_container_file(const char *filename)
{
    if (!extract_entitlements(filename) ||
        !extract_params(filename) ||
        !extract_additional_sandbox_snippets(filename))
        return 1;

    return 0;
}

/**
 * Extracts the compiled profile bytecode from the container metadata
 * file
 *
 * @param filename The filename of the container file from which to read the bytecode
 * @return NULL on failure. Else a structure of type data_buffer that contains the
 * compiled profile bytecode. The caller is responsible for releasing the memory.
 */
struct data_buffer *extract_compiled_profile(const char *filename)
{
    NSDictionary *dict = load_plist(filename);
    NSData *bytecode = dict[@"SandboxProfileData"];

    if (!bytecode) {
        return NULL;
    }

    struct data_buffer *compiled_profile = calloc(sizeof(struct data_buffer), 1);
    assert(compiled_profile != NULL && "Failed to allocate memory for bytecode wrapper.");

    compiled_profile->length = [bytecode length];
    compiled_profile->data = calloc(compiled_profile->length, 1);
    assert(compiled_profile->data != NULL && "Failed to allocate memory for bytecode storage.");

    memcpy(compiled_profile->data, [bytecode bytes], compiled_profile->length);

    return compiled_profile;
}