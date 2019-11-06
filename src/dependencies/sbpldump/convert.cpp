//
// Created by Jakob Rieck on 14.02.18.
//
#include "convert.h"

#include <cassert>
#include <sstream>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

static std::string dump_scheme_modifier(const json &modifier_desc,
                                        size_t padding = 0) {
  std::string pad(padding, ' ');

  std::string mod_name = modifier_desc["name"];

  if (modifier_desc.find("argument") != modifier_desc.end()) {
    // Modifier with an argument
    if (modifier_desc["argument"].is_string()) {
      std::string mod_arg = modifier_desc["argument"];
      return pad + "(with " + mod_name + " \"" + mod_arg + "\")";
    } else {
      long mod_arg = modifier_desc["argument"];
      return pad + "(with " + mod_name + " " + std::to_string(mod_arg) + ")";
    }
  } else {
    // No argument modifier
    return pad + "(with " + mod_name + ")";
  }
}

static std::string dump_scheme_filter(const json &filter_desc,
                                      size_t padding = 0) {
  std::string pad(padding, ' ');

  std::ostringstream filter_out;

  const std::string filter_name = filter_desc["name"];
  if (filter_name == "require-all" || filter_name == "require-any" ||
      filter_name == "require-not") {
    filter_out << pad << "(" << filter_name << std::endl;
    for (const json &subfilter : filter_desc["subfilters"]) {
      filter_out << dump_scheme_filter(subfilter, padding + 4) << std::endl;
    }
    filter_out << pad << ")" << std::endl;
  } else {
    filter_out << pad << "(" << filter_name << " ";

    const json &arguments = filter_desc["arguments"];
    for (json::const_iterator it = arguments.cbegin(); it != arguments.cend();
         ++it) {
      const json &argument = *it;

      if (argument.find("alias") != argument.end()) {
        // Alias provided, use this one.
        const std::string alias = argument["alias"];
        filter_out << alias;
      } else {
        if (argument["type"] == "string") {
          filter_out << argument["value"];
        } else {
          const uint64_t int_value = argument["value"];
          if (argument["type"] == "boolean")
            filter_out << (int_value ? "#t" : "#f");
          else
            filter_out << int_value;
        }
      }
      if (it != --arguments.cend()) {
        // Place each argument on its own line
        filter_out << std::endl << pad << "    ";
      }
    }

    filter_out << ")";

    // Normal filter on first level (assuming 4 as default padding)
    if (padding == 4) {
      filter_out << std::endl;
    }
  }

  return filter_out.str();
}

static std::string dump_scheme_rule(const json &rule_desc) {
  std::ostringstream rule;

  rule_desc.dump(4);

  std::string action = rule_desc["action"];
  rule << "(" << action << std::endl;

  for (const std::string &op : rule_desc["operations"]) {
    rule << "    " << op << std::endl;
  }

  if (rule_desc.count("filters") > 0) {
    for (const json &filter : rule_desc["filters"]) {
      rule << dump_scheme_filter(filter, 4);
    }
  }

  if (rule_desc.count("modifiers") > 0) {
    for (const json &modifier : rule_desc["modifiers"]) {
      rule << dump_scheme_modifier(modifier, 4);
    }
  }
  rule << ")" << std::endl;

  return rule.str();
}

extern "C" const char *sandbox_rules_dump_scheme(const char *json_rules) {
  std::ostringstream output;

  auto rules = json::parse(json_rules);

  output << "(version 1)" << std::endl;

  for (const json &rule : rules) {
    output << dump_scheme_rule(rule);
  }

  std::string output_str = output.str();

  return strdup(output_str.c_str());
}
