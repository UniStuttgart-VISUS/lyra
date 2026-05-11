// <copyright file="contains.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_CONTAINS_H)
#define _LYRA_CONTAINS_H
#pragma once

#include <algorithm>
#include <iterator>
#include <type_traits>

#include "visus/autodoc/api.h"


LYRA_DETAIL_NAMESPACE_BEGIN

/// <summary>
/// Derives the value type iterated by <typeparamref name="TIterator" />.
/// </summary>
template<class TIterator>
using iterated_type_t = typename std::iterator_traits<TIterator>::value_type;


/// <summary>
/// Answer whether <paramref name="value" /> is contained in the range defined
/// by <paramref name="begin" /> and <paramref name="end" />.
/// </summary>
template<class TIterator> inline bool contains(
        _In_ const TIterator begin,
        _In_ const TIterator end,
        _In_ const iterated_type_t<TIterator>& value) {
    return (std::find(begin, end, value) != end);
}

/// <summary>
/// Answer whether <paramref name="value" /> is contained in the given
/// <paramref name="container" />.
/// </summary>
template<class TContainer> inline bool contains(
        _In_ const TContainer& container,
        _In_ const typename TContainer::value_type& value) {
    return contains(std::begin(container), std::end(container), value);
}

LYRA_DETAIL_NAMESPACE_END

#endif /* !defined(_LYRA_CONTAINS_H) */
