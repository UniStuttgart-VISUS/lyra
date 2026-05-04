// <copyright file="equals.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2025 - 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "equals.h"


/*
 * LYRA_DETAIL_NAMESPACE::equals
 */
bool LYRA_DETAIL_NAMESPACE::equals(
        _In_opt_z_ const char *lhs,
        _In_opt_z_ const char *rhs,
        _In_ const string_comparison comparison) noexcept {
    if (lhs == rhs) {
        // Trivial accept.
        return true;
    }

    if ((lhs == nullptr) || (rhs == nullptr)) {
        // If any of the strings is nullptr, they cannot be equal, because if
        // both were nullptr, we would have entered the trivial accept case.
        return false;
    }

    switch (comparison) {
        case string_comparison::case_insensitive:
#if defined(_WIN32)
            return (::_stricmp(lhs, rhs) == 0);
#else /* defined(_WIN32) */
            return (::strcasecmp(lhs, rhs) == 0);
#endif /* defined(_WIN32) */

        case string_comparison::case_sensitive:
        default:
            return (::strcmp(lhs, rhs) == 0);
    }
}


/*
 * LYRA_DETAIL_NAMESPACE::equals
 */
bool LYRA_DETAIL_NAMESPACE::equals(
        _In_opt_z_ const wchar_t *lhs,
        _In_opt_z_ const wchar_t *rhs,
        _In_ const string_comparison comparison) noexcept {
    if (lhs == rhs) {
        // Trivial accept.
        return true;
    }

    if ((lhs == nullptr) || (rhs == nullptr)) {
        // If any of the strings is nullptr, they cannot be equal, because if
        // both were nullptr, we would have entered the trivial accept case.
        return false;
    }

    switch (comparison) {
        case string_comparison::case_insensitive:
#if defined(_WIN32)
            return (::_wcsicmp(lhs, rhs) == 0);
#else /* defined(_WIN32) */
            return (::wcscasecmp(lhs, rhs) == 0);
#endif /* defined(_WIN32) */

        case string_comparison::case_sensitive:
        default:
            return (::wcscmp(lhs, rhs) == 0);
    }
}
