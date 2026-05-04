// <copyright file="equals.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2025 - 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_EQUALS_H)
#define _LYRA_EQUALS_H
#pragma once

#include <cstdlib>
#include <cstring>
#include <cwchar>
#include <string>

#include "visus/autodoc/api.h"


LYRA_DETAIL_NAMESPACE_BEGIN

/// <summary>
/// Controls how strings are compared.
/// </summary>
enum class string_comparison {

    /// <summary>
    /// Perform the default case-sensitive comparison.
    /// </summary>
    dft = 0,

    /// <summary>
    /// Perform the default case-sensitive comparison.
    /// </summary>
    case_sensitive = 0,

    /// <summary>
    /// Performs a case-insensitive comparison.
    /// </summary>
    case_insensitive
};


/// <summary>
/// Tests whether two strings are equal.
/// </summary>
/// <param name="lhs">The left-hand side operand.</param>
/// <param name="rhs">The right-hand side operand.</param>
/// <param name="comparison">Details the type comparison to be performed.
/// </param>
/// <returns><see langword="true" /> if <paramref name="lhs" /> and
/// <paramref name="rhs" /> are equal.</returns>
LYRA_TEST_API bool equals(_In_opt_z_ const char *lhs,
    _In_opt_z_ const char *rhs,
    _In_ const string_comparison comparison = string_comparison::dft) noexcept;

/// <summary>
/// Tests whether two strings are equal.
/// </summary>
/// <param name="lhs">The left-hand side operand.</param>
/// <param name="rhs">The right-hand side operand.</param>
/// <param name="comparison">Details the type comparison to be performed.
/// </param>
/// <returns><see langword="true" /> if <paramref name="lhs" /> and
/// <paramref name="rhs" /> are equal.</returns>
LYRA_TEST_API bool equals(_In_opt_z_ const wchar_t *lhs,
    _In_opt_z_ const wchar_t *rhs,
    _In_ const string_comparison comparison = string_comparison::dft) noexcept;

/// <summary>
/// Tests whether two strings are equal.
/// </summary>
/// <typeparam name="TChar">The type of the characters.</typeparam>
/// <typeparam name="TLTraits">The traits class for the characters.</typeparam>
/// <typeparam name="TLAlloc">The allocator of the string.</typeparam>
/// <typeparam name="TRTraits">The traits class for the characters.</typeparam>
/// <typeparam name="TRAlloc">The allocator of the string.</typeparam>
/// <param name="lhs">The left-hand side operand.</param>
/// <param name="rhs">The right-hand side operand.</param>
/// <param name="comparison">Details the type comparison to be performed.
/// </param>
/// <returns><see langword="true" /> if <paramref name="lhs" /> and
/// <paramref name="rhs" /> are equal.</returns>
template<class TChar, class TLTraits, class TLAlloc, class TRTraits,
    class TRAlloc>
inline bool equals(
        _In_ const std::basic_string<TChar, TLTraits, TLAlloc>& lhs,
        _In_ const std::basic_string<TChar, TRTraits, TRAlloc>& rhs,
        _In_ const string_comparison comparison
        = string_comparison::dft) noexcept {
    return equals(lhs.c_str(), rhs.c_str(), comparison);
}

/// <summary>
/// Tests whether two strings are equal.
/// </summary>
/// <typeparam name="TChar">The type of the characters.</typeparam>
/// <typeparam name="TTraits">The traits class for the characters.</typeparam>
/// <typeparam name="TAlloc">The allocator of the string.</typeparam>
/// <param name="lhs">The left-hand side operand.</param>
/// <param name="rhs">The right-hand side operand.</param>
/// <param name="comparison">Details the type comparison to be performed.
/// </param>
/// <returns><see langword="true" /> if <paramref name="lhs" /> and
/// <paramref name="rhs" /> are equal.</returns>
template<class TChar, class TTraits, class TAlloc>
inline bool equals(
        _In_opt_z_ const TChar *lhs,
        _In_ const std::basic_string<TChar, TTraits, TAlloc>& rhs,
        _In_ const string_comparison comparison
        = string_comparison::dft) noexcept {
    return equals(lhs, rhs.c_str(), comparison);
}

/// <summary>
/// Tests whether two strings are equal.
/// </summary>
/// <typeparam name="TChar">The type of the characters.</typeparam>
/// <typeparam name="TTraits">The traits class for the characters.</typeparam>
/// <typeparam name="TAlloc">The allocator of the string.</typeparam>
/// <param name="lhs">The left-hand side operand.</param>
/// <param name="rhs">The right-hand side operand.</param>
/// <param name="comparison">Details the type comparison to be performed.
/// </param>
/// <returns><see langword="true" /> if <paramref name="lhs" /> and
/// <paramref name="rhs" /> are equal.</returns>
template<class TChar, class TTraits, class TAlloc>
inline bool equals(
        _In_ const std::basic_string<TChar, TTraits, TAlloc>& lhs,
        _In_opt_z_ const TChar *rhs,
        _In_ const string_comparison comparison
        = string_comparison::dft) noexcept {
    return equals(lhs.c_str(), rhs, comparison);
}

/// <summary>
/// Tests whether two strings are equal, ignoring case.
/// </summary>
/// <typeparam name="TLhs"></typeparam>
/// <typeparam name="TRhs"></typeparam>
/// <param name="lhs"></param>
/// <param name="rhs"></param>
/// <returns></returns>
template<class TLhs, class TRhs>
inline bool iequals(_In_ TLhs&& lhs, _In_ TRhs&& rhs) noexcept {
    return equals(std::forward<TLhs>(lhs),
        std::forward<TRhs>(rhs),
        string_comparison::case_insensitive);
}

LYRA_DETAIL_NAMESPACE_END

#endif /* !defined(_LYRA_EQUALS_H) */
