//
// Created by Jakob Rieck on 15.05.18.
//

#ifndef SBPL_EVALUATE_H
#define SBPL_EVALUATE_H

#include <platform_data/platforms.h>
#include <stdbool.h>

enum output_format { OUTPUT_JSON = 10000, OUTPUT_SCHEME };

/**
 * Evaluate an SBPL input profile.
 *
 * @param profile The profile to evaluate
 * @param container_metadata The container metadata to use for evaluation
 * @param should_patch Whether to patch the generated rules to enable logging
 * for all rules
 * @param format The output format (either OUTPUT_JSON or OUTPUT_SCHEME)
 * @return Generated rules as a string.
 */
const char *sb_evaluate(const char *profile, const char *container_metadata,
                        enum target_platform platform, bool should_patch,
                        enum output_format format);

#endif // SBPL_EVALUATE_H
