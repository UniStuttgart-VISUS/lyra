// <copyright file="ends_with.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_ENDS_WITH_H)
#define _LYRA_ENDS_WITH_H
#pragma once

#include "equals.h"


LYRA_DETAIL_NAMESPACE_BEGIN

/// <summary>
/// Answer whether <paramref name="str" /> ends with <paramref name="suffix" />.
/// </summary>
/// <typeparam name="TChar"></typeparam>
/// <param name="str"></param>
/// <param name="suffix"></param>
/// <param name="comparison"></param>
/// <returns></returns>
template<class TChar>
bool ends_with(_In_opt_z_ const TChar *str, _In_opt_z_ const TChar *suffix,
    _In_ const string_comparison comparison = string_comparison::dft) noexcept;

/// <summary>
/// Answer whether <paramref name="str" /> ends with <paramref name="suffix" />,
/// ignoring case.
/// </summary>
/// <typeparam name="TChar"></typeparam>
/// <param name="str"></param>
/// <param name="suffix"></param>
/// <returns></returns>
template<class TChar> inline bool iends_with(
        _In_opt_z_ const TChar *str,
        _In_opt_z_ const TChar *suffix) noexcept {
    return ends_with(str, suffix, string_comparison::case_insensitive);
}

LYRA_DETAIL_NAMESPACE_END

#include "ends_with.inl"

#endif /* !defined(_LYRA_ENDS_WITH_H) */
