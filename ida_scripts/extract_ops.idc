#include <idc.idc>

#define true 1

/*
 * Convert the current string into a valid identifier in C programs.
 * This function assumes a operation is given as input, meaning that
 * the only characters that are assumed to be in this string are
 * letters, stars (*) and hyphons (-).
 */
static make_c_identifier(str)
{
    auto result = "";

    auto len = strlen(str);
    auto i = 0;
    for (i = 0; i < len; i = i + 1) {
        if (str[i] == "*") {
            result = result + "_all";
        } else if (str[i] == "-") {
            result = result + "_";
        } else {
            result = result + str[i];
        }
    }

    return result;
}

/*
 * Get the number of currently registered operations
 */
static get_op_count()
{
    auto op_name_base = LocByName("_operation_names");
    if (op_name_base == BADADDR) {
        return 0;
    }

    auto offset = 0x0;
    auto ops_count = 0x0;
    while (true) {
        auto current_name = Name(op_name_base + offset);

        // If the current memory location has a name and it is not
        // the one we are looking for, stop (because then we reached another
        // memory structure and should stop)
        if (current_name != "" && current_name != "_operation_names") {
            break;
        }

        ops_count = ops_count + 1;
        offset = offset + 0x8;
    }

    return ops_count;
}

/*
 * Get the name of an operation (given its index).
 * Returns "" if the index is invalid.
 */
static get_op_name(idx)
{
    auto op_name_base = LocByName("_operation_names");
    if (op_name_base == BADADDR) {
        return "";
    }

    return GetString(Qword(op_name_base + 0x8 * idx), -1, ASCSTR_C);
}

/*
 * Get the base address of the operation info structure for the specified
 * index. Returns BADADDR if the operation is unsuccessful.
 */
static get_op_base(idx)
{
    auto op_name_base = LocByName("_operation_info");
    if (op_name_base == BADADDR) {
        return BADADDR;
    }

    // 0x20 is the size of the operation_info structure
    return op_name_base + 0x20 * idx;
}

/*
 * Dump the sandbox operations to the provided file as a valid C declaration.
 */
static dump_op_names(out_file)
{
    // Type declaration
    fprintf(out_file, "static const char *operation_names[] = {\n");

    // Dump the actual operation names
    auto i = 0;
    auto cnt_ops = get_op_count();

    for (i = 0; i < cnt_ops; i = i + 1) {
        auto op_name = get_op_name(i);
        fprintf(out_file, "        \"%s\",\n", op_name);
    }

    // Closing definition
    fprintf(out_file, "};\n");

    // Convenience function definition
    fprintf(out_file, "size_t operations_get_count()\n");
    fprintf(out_file, "{\n    return sizeof(operation_names) / sizeof(*operation_names);\n");
    fprintf(out_file, "}\n");
}

/*
 * Dump definitions of filters and modifiers required by the definition of the
 * operations to the file provided.
 */
static dump_filters_modifiers(out_file)
{
    auto i = 0;
    auto cnt_ops = get_op_count();

    // dump the allowed filters and allowed modifiers.
    for (i = 0; i < cnt_ops; i = i + 1) {
        auto current_op = get_op_base(i);
        auto op_name = make_c_identifier(get_op_name(i));

        auto filters_out = "static const char *" + op_name + "_filters[] = {\n";

        // See also the layout of memory described in dump_op_infos
        auto allowed_filters = Qword(current_op + 0x10);

        // Dump the filters.
        auto current_filter = allowed_filters;
        while (true) {
            if (Qword(current_filter) == 0x0) {
                break;
            }
            filters_out = filters_out + "    \"" + GetString(Qword(current_filter), -1, 0) + "\",\n";
            current_filter = current_filter + 0x8;
        }
        filters_out = filters_out + "};\n";

        // Dump the modifiers
        auto allowed_modifiers = Qword(current_op + 0x18);

        auto modifiers_out = "static const char *" + op_name + "_modifiers[] = {\n";

        auto current_modifier = allowed_modifiers;
        while (true) {
            if (Qword(current_modifier) == 0x0) {
                break;
            }

            modifiers_out = modifiers_out + "    \"" + GetString(Qword(current_modifier), -1, 0) + "\",\n";
            current_modifier = current_modifier + 0x8;
        }

        modifiers_out = modifiers_out + "};\n";

        fprintf(out_file, filters_out);
        fprintf(out_file, modifiers_out);
    }
}

/*
 * Dump the actual information about the operation to the file provided.
 */
static dump_op_infos(out_file)
{
    /*
        A single operation info entry looks like the following:
        {
            uint32_t terminal_node; // 0 for TERMINAL_NODE, 1 for NONTERMINAL_NODE
            uint32_t fallback_op; // The index of the operation to try next. Mostly used to implement things like "file-read-data" -> "file*"
            uint32_t action; // Not entirely clear what this purpose is. Could be an allow / deny decision.
            char **allowed_filters; // A number of categories of filters that are allowed. Ends with a NULL pointer
            char **allowed_modifiers; // A number of modifiers that are allowed. Ends with a NULL pointer
        }
    */
    auto i = 0;
    auto cnt_ops = get_op_count();

    // Type declaration
    fprintf(out_file, "\nstatic const operation_info_t operation_infos[] = {\n");

    // Actual data dumping
    for (i = 0; i < cnt_ops; i = i + 1) {
        auto current_op = get_op_base(i);
        auto op_name = make_c_identifier(get_op_name(i));

        auto terminal_node = Dword(current_op);
        auto fallback_op = Dword(current_op + 0x4);
        auto action = Dword(current_op + 0x8);

        auto str_terminal_node = "";
        if (terminal_node == 0) {
            str_terminal_node = "TERMINAL_NODE";
        } else {
            str_terminal_node = "NONTERMINAL_NODE";
        }

        auto op_name_fallback = get_op_name(fallback_op);
        fprintf(out_file, "    { // %s\n        %s,\n        %d, // -> %s\n        %d,\n        %s,\n        %s\n    },\n",
            get_op_name(i), str_terminal_node, fallback_op, op_name_fallback, action, op_name + "_filters", op_name + "_modifiers");
    }

    // Closing the data declaration
    fprintf(out_file, "};\n");
}

static main()
{
    auto i = 0;
    auto cnt_ops = get_op_count();

    auto out_name = AskFile(1, "*.c", "Where would you like to save the structure definition to?");

    auto out_file = 0;

    if (out_name != 0) {
        out_file = fopen(out_name, "w");
    }

    if (out_name == 0 || out_file == 0) {
        Message("Invalid or no file provided.\n");
        return;
    }

    Message("Found %d symbols\n", cnt_ops);

    // Starting include line
    fprintf(out_file, "#include \"definition.h\"\n");

    // First off, dump the operation names
    dump_op_names(out_file);
    fprintf(out_file, "\n");

    // Dump data definitions for filters and modifiers, which will be needed
    // by final data definitions
    dump_filters_modifiers(out_file);
    fprintf(out_file, "\n");

    // Dump actual operation definitions. These definitions require the definitions for filters
    // and modifiers.
    dump_op_infos(out_file);
    fprintf(out_file, "\n");

    fclose(out_file);
}