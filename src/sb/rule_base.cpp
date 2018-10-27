//
// Created by Jakob Rieck on 14.02.18.
//
#include "rule_base.h"

#include <cassert>
#include <sstream>
#include <string>
#include <vector>

#include <sb/filters/mem.h>
#include <sb/filters/types.h>
#include <sb/meta-filters/types.h>
#include <sb/operations/types.h>

#include <scheme_interpreter/scheme-private.h>

#include <nlohmann/json.hpp>

#include <sbpldump/convert.h>

using json = nlohmann::json;

static std::vector<json> rules;

static json rule_base_json_for_rule(const sbpl_action_t *rule);

// Add a rule to the internal rule base.
// None of these functions are thread-safe
// and are not to be called in a multi-threaded
// application
extern "C" void rule_base_add_rule(sbpl_action_t *rule) {
  // We should not hold onto sbpl_action_t rules, because the scheme runtime
  // will garbage collect our data out from under us. This has been addressed
  // by Apple by introducing counters along with each object, but we don't have
  // that.
  rules.push_back(rule_base_json_for_rule(rule));
}

static const char *op_name(pointer op) {
  assert(is_blob(op));

  auto bi = blob_info(op);
  assert(bi.tag == SBPL_OPERATION);

  auto *operation = static_cast<sbpl_operation_t *>(bi.blob);

  return operation->operation_name;
}

static json dump_json_operation(pointer op) { return op_name(op); }

static json dump_json_filter(pointer filter) {
  assert(is_blob(filter));

  auto bi = blob_info(filter);
  assert(bi.tag == SBPL_FILTER || SBPL_META_FILTER);

  if (bi.tag == SBPL_META_FILTER) {
    // Dump nested filter
    auto *meta_filter = static_cast<sbpl_meta_filter_t *>(bi.blob);

    std::vector<json> sub_filters;

    // Recursively invoke the dump routine
    for (size_t i = 0; i < meta_filter->filters_count; ++i) {
      sub_filters.push_back(dump_json_filter(meta_filter->filters[i]));
    }

    std::string meta_filter_name;
    switch (meta_filter->type) {
    case METAFILTER_ALL:
      meta_filter_name = "require-all";
      break;
    case METAFILTER_ANY:
      meta_filter_name = "require-any";
      break;
    case METAFILTER_NOT:
      meta_filter_name = "require-not";
      break;
    }

    return {{"name", meta_filter_name}, {"subfilters", sub_filters}};
  } else {
    // Dump a normal, non-nested filter
    auto *normal_filter = static_cast<sbpl_filter_t *>(bi.blob);

    std::vector<json> arguments;

    // Iterate over all arguments
    for (size_t i = 0; i < normal_filter->n_arguments; ++i) {
      sbpl_filter_argument_t *arg = filter_get_argument(normal_filter, i);
      const char *alias = arg->alias;

      json argument_details;
      if (alias) {
        argument_details["alias"] = alias;
      }

      switch (arg->type) {
        case STRING:
          argument_details["type"] = "string";
          argument_details["value"] = arg->argument.as_str;
          break;
        case BOOLEAN:
          argument_details["type"] = "boolean";
          argument_details["value"] = arg->argument.as_int;
          break;
        case PRIMITIVE:
          argument_details["type"] = "primitive";
          argument_details["value"] = arg->argument.as_int;
      }

      arguments.push_back(argument_details);
    }

    return {{"name", normal_filter->filter_name}, {"arguments", arguments}};
  }
}

static json dump_json_modifier(pointer modifier) {
  assert(is_blob(modifier));
  assert(blob_info(modifier).tag == SBPL_MODIFIER);

  auto *unpacked = static_cast<sbpl_modifier_t *>(blob_info(modifier).blob);
  json result = {{"name", unpacked->modifier_name}};

  switch (unpacked->argument_type) {
  case MODIFIER_ARGUMENT_NONE:
    break;
  case MODIFIER_ARGUMENT_STR:
    result["argument"] = unpacked->argument.string_arg;
    break;
  case MODIFIER_ARGUMENT_INT:
    result["argument"] = unpacked->argument.int_arg;
    break;
  }

  return result;
}

static json rule_base_json_for_rule(const sbpl_action_t *rule) {
  json rule_obj = {{"action", rule->type == ALLOW ? "allow" : "deny"}};

  // Dump operations
  for (size_t i = 0; i < rule->noperations; ++i) {
    rule_obj["operations"].push_back(dump_json_operation(rule->operations[i]));
  }

  // Dump filters
  for (size_t i = 0; i < rule->nfilters; ++i) {
    rule_obj["filters"].push_back(dump_json_filter(rule->filters[i]));
  }

  // Dump modifiers
  for (size_t i = 0; i < rule->nmodifiers; ++i) {
    rule_obj["modifiers"].push_back(dump_json_modifier(rule->modifiers[i]));
  }

  return rule_obj;
}

extern "C" const char *rule_base_dump_json() {
  json output = json::array();

  for (const json &rule : rules) {
    output.emplace_back(rule);
  }

  std::string res = output.dump(4);
  return strdup(res.c_str());
}

/**
 * Patch the current sandbox profile rules in such a way that each allow
 * decision reports to syslog.
 */
void patch_report() {
  for (json &rule : rules) {
    // (with report) statements can only be coupled with allow actions.
    if (rule["action"] == "allow") {
      // (with report) statements may only occur once per rule.
      bool should_patch = true;

      // Check whether the (with report) modifier has already been
      // set for this rule.
      for (const json &modifier : rule["modifiers"]) {
        if (modifier["name"] == "report") {
          should_patch = false;
          break;
        }
      }

      if (should_patch)
        rule["modifiers"].push_back({{"name", "report"}});
    }
  }
}

extern "C" const char *rule_base_dump_scheme() {
  return sandbox_rules_dump_scheme(rule_base_dump_json());
}
