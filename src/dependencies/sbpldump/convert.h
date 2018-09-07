//
// Created by Jakob Rieck on 14.02.18.
//

#ifndef JSON_TO_SBPL_CONVERT_H
#define JSON_TO_SBPL_CONVERT_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Dumps a json encoded rulebase (see simbple's rule_base.cpp)
 * as proper SBPL code.
 *
 * @param json_rules The JSON encoded rulebase
 * @return SBPL code corresponding to the rulebase
 */
const char *sandbox_rules_dump_scheme(const char *json_rules);

#ifdef __cplusplus
}
#endif

#endif // JSON_TO_SBPL_CONVERT_H
