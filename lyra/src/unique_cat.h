// <copyright file="unique_cat.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_UNIQUE_CAT_H)
#define _LYRA_UNIQUE_CAT_H
#pragma once

#include <type_traits>


LYRA_DETAIL_NAMESPACE_BEGIN

/// <summary>
/// Appends all unique and non-<see langword="void" /> elements of
/// <paramref name="TTypes" /> with a variadic template.
/// </summary>
template<class TExisting, class... TTypes> struct unique_cat;

/// <summary>
/// Recursion step.
/// </summary>
template<template<class...> class TTpl, class... TEx, class TCur, class... TRem>
struct unique_cat<TTpl<TEx...>, TCur, TRem...> final {
    static constexpr auto _unique = !std::disjunction_v<
        std::is_same<TEx, TCur>...>;
    static constexpr auto _valid = !std::is_same_v<TCur, void>;

    typedef std::conditional_t<
        _unique && _valid,
        typename unique_cat<TTpl<TEx..., TCur>, TRem...>::type,
        typename unique_cat<TTpl<TEx...>, TRem...>::type>
        type;
};

/// <summary>
/// Recursion stop.
/// </summary>
template<template<class...> class TTpl, class... TEx>
struct unique_cat<TTpl<TEx...>> final {
    typedef TTpl<TEx...> type;
};


/// <summary>
/// Appends all unique and non-<see langword="void" /> elements of
/// <paramref name="TTypes" /> with a variadic template.
/// </summary>
template<class TExisting, class... TTypes>
using unique_cat_t = typename unique_cat<TExisting, TTypes...>::type;

LYRA_DETAIL_NAMESPACE_END

#endif /* !defined(_LYRA_UNIQUE_CAT_H) */
