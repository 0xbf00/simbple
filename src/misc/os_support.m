#include "os_support.h"
#include <Foundation/Foundation.h>

char *macos_version()
{
	NSOperatingSystemVersion version = [[NSProcessInfo processInfo] operatingSystemVersion];

	char *result = NULL;
	int success = asprintf(&result, "%lu.%lu.%lu", 
		version.majorVersion, version.minorVersion, version.patchVersion);
	if (success == -1 || result == NULL)
		return NULL;

	return result;
}