//
// Created by Jakob Rieck on 15.05.18.
//

#ifndef MISC_FILESYSTEM_H
#define MISC_FILESYSTEM_H

/**
 * Computes an absolute path for a given program path and a relative path
 * @param program_name The program path
 * @param path The relative path
 * @return Absolute path
 */
const char *program_relative_path(const char *relative_path);
/**
 * Computes an absolute path for a given relative path from the user's home
 * directory.
 * @param relative_path The home-relative path
 * @return Absolute path of the resource
 */
const char *home_relative_path(const char *relative_path);

/**
 * Check whether a file exists.
 *
 * @param filename Filepath to file that may or may not exist.
 * @return True, iff the file exists, else false.
 */
int file_exists(const char *filename);

#endif // MISC_FILESYSTEM_H
