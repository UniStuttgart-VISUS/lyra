// <copyright file="string_manipulation.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_STRING_MANIPULATION_H)
#define _LYRA_STRING_MANIPULATION_H
#pragma once

#include <algorithm>
#include <iterator>
#include <string>
#include <vector>

#include "visus/lyra/api.h"


LYRA_DETAIL_NAMESPACE_BEGIN

/// <summary>
/// Makes sure that the given string is nullt-erminated.
/// </summary>
/// <typeparam name="TChar"></typeparam>
/// <param name="str"></param>
/// <returns></returns>
template<class TChar>
std::vector<TChar>& null_terminate(_In_ std::vector<TChar>& str);

/// <summary>
/// Returns an iterator to the last non-zero character at the end of the
/// given range.
/// </summary>
/// <typeparam name="TIterator"></typeparam>
/// <param name="begin"></param>
/// <param name="end"></param>
/// <returns></returns>
template<class TIterator> TIterator remove_trailing_nulls(
    _In_ const TIterator begin, _In_ const TIterator end);

/// <summary>
/// Erases all trailing nulls from the given string.
/// </summary>
/// <typeparam name="TChar"></typeparam>
/// <typeparam name="TTraits"></typeparam>
/// <typeparam name="TAllocator"></typeparam>
/// <param name="str"></param>
/// <returns></returns>
template<class TChar, class TTraits, class TAllocator>
std::basic_string<TChar, TTraits, TAllocator>& remove_trailing_nulls(
        _In_ std::basic_string<TChar, TTraits, TAllocator>& str) {
    const auto end = remove_trailing_nulls(str.begin(), str.end());
    str.erase(end, str.end());
    return str;
}

/// <summary>
/// Erases all trailing nulls from the given vector.
/// </summary>
/// <typeparam name="TChar"></typeparam>
/// <param name="str"></param>
/// <returns></returns>
template<class TChar>
std::vector<TChar>& remove_trailing_nulls(_In_ std::vector<TChar>& str) {
    const auto end = remove_trailing_nulls(str.begin(), str.end());
    str.erase(end, str.end());
    return str;
}

LYRA_DETAIL_NAMESPACE_END

#include "string_manipulation.inl"

#endif /* !defined(_LYRA_STRING_MANIPULATION_H) */
