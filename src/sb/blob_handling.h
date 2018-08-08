//
// Created by Jakob Rieck on 16.02.18.
//

#ifndef SBPL_BLOB_HANDLING_H
#define SBPL_BLOB_HANDLING_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

struct cell;
typedef struct cell *pointer;
typedef struct scheme scheme;

enum sbpl_tag {
  SBPL_MODIFIER,
  SBPL_FILTER,
  SBPL_META_FILTER,
  SBPL_OPERATION,
  SBPL_ACTION,
  SBPL_RULE,
  SBPL_ERROR,
};

/**
 * Make a scheme blob.
 * @param sc The current scheme context
 * @param ptr The pointer to store in the blob
 * @param tag The type of data stored
 * @return Scheme's pointer denoting the blob stored
 */
pointer scheme_blob(scheme *sc, void *ptr, enum sbpl_tag tag);

#ifdef __cplusplus
}
#endif

#endif // SBPL_BLOB_HANDLING_H
