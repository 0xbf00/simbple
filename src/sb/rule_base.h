//
// Created by Jakob Rieck on 14.02.18.
//

#ifndef SBPL_RULE_BASE_H
#define SBPL_RULE_BASE_H

#include "sb.h"

#ifdef __cplusplus
extern "C" {
#endif

void rule_base_add_rule(sbpl_action_t *rule);
const char *rule_base_dump_json();
const char *rule_base_dump_scheme();
void patch_report();

#ifdef __cplusplus
}
#endif

#endif // SBPL_RULE_BASE_H
