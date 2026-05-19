// <copyright file="to_string.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "to_string.h"

#include "visus/autodoc/convert_string.h"

#include "string_manipulation.h"


/*
 * LYRA_DETAIL_NAMESPACE::to_quoted_string
 */
std::string LYRA_DETAIL_NAMESPACE::to_quoted_string(
        _In_opt_z_ const char *str) {
    std::string retval(u8"\"");

    if (str != nullptr) {
        retval += str;
    }

    retval += u8'"';
    return retval;
}


/*
 * LYRA_DETAIL_NAMESPACE::to_quoted_string
 */
std::string LYRA_DETAIL_NAMESPACE::to_quoted_string(
        _In_ const multi_sz& value,
        _In_ const char sep) {
    auto retval = to_unquoted_string(value, sep);
    retval.insert(retval.begin(), u8'\"');
    retval += u8'\"';
    return retval;
}


/*
 * LYRA_DETAIL_NAMESPACE::to_unquoted_string
 */
std::string LYRA_DETAIL_NAMESPACE::to_unquoted_string(
        _In_opt_z_ const char *str) {
    return (str != nullptr) ? str : u8"";
}

/*
 * LYRA_DETAIL_NAMESPACE::to_unquoted_string
 */
std::string LYRA_DETAIL_NAMESPACE::to_unquoted_string(
        _In_ const multi_sz& value,
        _In_ const char sep) {
    std::string retval;

    auto first = true;
    for (auto s : value) {
        if (first) {
            first = false;
        } else {
            retval += sep;
        }

        retval += s;
    }

    return retval;
}
