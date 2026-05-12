// <copyright file="is_uncommon.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_IS_UNCOMMON_H)
#define _LYRA_IS_UNCOMMON_H
#pragma once

#include <type_traits>

#include "visus/autodoc/collection_flags.h"

#include "detector.h"


LYRA_DETAIL_NAMESPACE_BEGIN

/// <summary>
/// The detector function for a <c>is_uncommon</c> constant.
/// </summary>
template<class TType>
using is_uncommon_t = decltype(TType::is_uncommon);

/// <summary>
/// Detects whether a type has a <c>is_uncommon</c> constant.
/// </summary>
/// <typeparam name="TType"></typeparam>
template<class TType>
struct has_is_uncommon final : detector<is_uncommon_t, void, TType> { };


/// <summary>
/// Determines whether a property descriptor identifies an uncommon property.
/// </summary>
/// <typeparam name="TProp">The type of a property descriptor.</typeparam>
/// <typeparam name="HasDecl">Specifies whether <typeparamref name="TProp" />
/// declares a <c>is_uncommon</c> explicitly.</typeparam>
template<class TProp, bool HasDecl = has_is_uncommon<TProp>::value>
struct is_uncommon final : std::false_type { };

/// <summary>
/// Specialisation for types that actually declare whether they are uncommon
/// or not.
/// </summary>
template<class TProp> struct is_uncommon<TProp, true> final
    : std::bool_constant<TProp::is_uncommon> { };

/// <summary>
/// Determines whether a property descriptor identifies an uncommon property.
/// </summary>
/// <typeparam name="TProp">The type of a property descriptor.</typeparam>
template<class TProp>
constexpr auto is_uncommon_v = is_uncommon<TProp>::value;


/// <summary>
/// Checks whether a potentially uncommon property should be collected based on
/// the given <paramref name="flags" />.
/// </summary>
/// <typeparam name="TProp"></typeparam>
/// <param name="flags"></param>
/// <returns></returns>
template<class TProp>
inline bool check_uncommon(_In_ const collection_flags flags) {
    return !has_flag(flags, collection_flags::no_uncommon)
        || !is_uncommon_v<TProp>;
}

LYRA_DETAIL_NAMESPACE_END

#endif /* !defined(_LYRA_IS_UNCOMMON_H) */
