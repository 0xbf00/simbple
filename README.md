# simbple

This project aims to document the inner workings of Apple's App Sandbox and to provide a way to obtain plaintext sandbox profiles for third-party apps.

Sandboxing for third-party applications is configured using entitlements. Under the hood, these entitlements are turned into a full sandboxing profile by libsandbox. For this, an abstract base profile, containing both SBPL statements and Scheme code, is evaluated, which generates the final ruleset for an app. Unfortunately, only the compiled profile is returned and the plaintext version cannot be audited.

The `simbple` tool reimplements the profile evaluation process done in `libsandbox` but outputs correct, human-readable SBPL output. The output can be manually audited, automatically patched, manually modified or simply be compiled to profile bytecode using Stefan Esser's tools.

## Compilation

`simbple` depends on `cmake`, `argp` and `nlohmann/json`. The dependencies can be installed using homebrew:

```sh
$ brew tap nlohmann/json
$ brew install cmake nlohmann_json argp-standalone
```

To build the project

```sh
$ mkdir build
$ cd build
$ cmake ../src
$ make
```

At this time, the resulting binary cannot be moved away from its compilation directory, as certain relative filepaths are hardcoded in the program.

## Usage

As input, the `simbple` tool takes the `Container.plist` file for the target app. This is done so that the same parameters used during the initial sandbox compilation process can be used and the resulting profiles can be verified to be correct. In case an application does not have the required file, simply launch the app once to generate it. If it's still not there, ensure the application is sandboxed, for example by consulting `Activity Monitor.app`.

Here is the full command-line interface to the tool:

```sh
$ ./simbple --help

Usage: simbple [OPTION...] CONTAINER_METADATA
Evaluate a SBPL (+ Scheme) profile

  -o, --output=FILE          Output file
  -p, --profile=PROFILE      Base profile to evaluate. Defaults to
                             application.sb profile.
      --platforms=PLATFORM   sierra, high_sierra (default)

 Output formats:
      --json                 Output as JSON
      --scheme               Output as SCHEME / SBPL

 Misc options:
      --patch                Patch the output profile to log all statements.
      --verify               Verify semantic correctness of generated results

  -?, --help                 Give this help list
      --usage                Give a short usage message
  -V, --version              Print program version

Mandatory or optional arguments to long options are also mandatory or optional
for any corresponding short options.

The output is a simplified SBPL profile that can be analysed, modified and
compiled as is.
```

To obtain the plaintext sandbox profile for `Calculator.app`, execute the following command

```sh
$ ./simbple ~/Library/Containers/com.apple.calculator/Container.plist
```

The output can be verified to be correct using the `--verify` flag. The program exits with a non-zero status code if verification fails.