//
// Created by Jakob Rieck on 15.05.18.
//

#include "filesystem.h"

#include <dirent.h>
#include <libgen.h>
#include <limits.h>
#include <memory.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <mach-o/dyld.h>

/**
 * Get a path to the currently running program.
 * @return Path to currently running program.
 */
static char *program_name() {
  uint32_t size = 0;
  _NSGetExecutablePath(NULL, &size);

  char *buffer = calloc(1, size + 1);

  if (size == 0 || buffer == NULL)
    return NULL;

  _NSGetExecutablePath(buffer, &size);
  return buffer;
}

/**
 * Combine base path and relative path to form absolute path.
 */
static char *path_mk_absolute(const char *base, const char *relative) {
  char *out;
  asprintf(&out, "%s/%s", base, relative);

  return out;
}

/**
 * Computes an absolute path for a given program path and a relative path
 * @param program_name The program path
 * @param path The relative path
 * @return Absolute path
 */
const char *program_relative_path(const char *relative_path) {
  char *program_copy = program_name();
  char *program_dir = dirname(program_copy);

  const char *res = path_mk_absolute(program_dir, relative_path);
  free(program_copy);

  return res;
}

/**
 * Computes an absolute path for a given relative path from the user's home
 * directory.
 * @param relative_path The home-relative path
 * @return Absolute path of the resource
 */
const char *home_relative_path(const char *relative_path) {
  struct passwd *pw = getpwuid(getuid());
  const char *homedir = pw->pw_dir;

  return path_mk_absolute(homedir, relative_path);
}

/**
 * Check whether a file exists.
 *
 * @param filename Filepath to file that may or may not exist.
 * @return True, iff the file exists, else false.
 */
int file_exists(const char *filename) {
  // fopen() does not differentiate between files and directories
  // Therefore, the following workaround is used here.
  DIR *dir = opendir(filename);
  if (dir) {
    closedir(dir);
    return 0;
  }

  FILE *f = fopen(filename, "r");
  if (f) {
    fclose(f);
    return 1;
  }

  return 0;
}