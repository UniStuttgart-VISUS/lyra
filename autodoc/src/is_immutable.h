// <copyright file="is_immutable.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_is_immutable_H)
#define _LYRA_is_immutable_H
#pragma once

#include <type_traits>

#include "visus/autodoc/collection_flags.h"

#include "detector.h"


LYRA_DETAIL_NAMESPACE_BEGIN

/// <summary>
/// The detector function for a <c>is_immutable</c> constant.
/// </summary>
template<class TType>
using is_immutable_t = decltype(TType::is_immutable);

/// <summary>
/// Detects whether a type has a <c>is_immutable</c> constant.
/// </summary>
/// <typeparam name="TType"></typeparam>
template<class TType>
struct has_is_immutable final : detector<is_immutable_t, void, TType> { };


/// <summary>
/// Determines whether a property descriptor identifies a property which does
/// not change when collecting the data multiple time.
/// </summary>
/// <typeparam name="TProp">The type of a property descriptor.</typeparam>
/// <typeparam name="HasDecl">Specifies whether <typeparamref name="TProp" />
/// declares a <c>is_immutable</c> explicitly.</typeparam>
template<class TProp, bool HasDecl = has_is_immutable<TProp>::value>
struct is_immutable final : std::false_type { };

/// <summary>
/// Specialisation for types that actually declare whether they are immutable
/// or not.
/// </summary>
template<class TProp> struct is_immutable<TProp, true> final
    : std::bool_constant<TProp::is_immutable> { };

/// <summary>
/// Determines whether a property descriptor identifies a property which does
/// not change when collecting the data multiple time.
/// </summary>
/// <typeparam name="TProp">The type of a property descriptor.</typeparam>
template<class TProp>
constexpr auto is_immutable_v = is_immutable<TProp>::value;


/// <summary>
/// Checks whether a potentially immutable property should be collected based on
/// the given <paramref name="flags" />.
/// </summary>
/// <typeparam name="TProp"></typeparam>
/// <param name="flags"></param>
/// <returns></returns>
template<class TProp>
inline bool check_immutable(_In_ const collection_flags flags) {
    return !has_flag(flags, collection_flags::no_immutable)
        || !is_immutable_v<TProp>;
}

LYRA_DETAIL_NAMESPACE_END

#endif /* !defined(_LYRA_is_immutable_H) */
