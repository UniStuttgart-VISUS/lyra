// <copyright file="is_variable.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_IS_VARIABLE_H)
#define _LYRA_IS_VARIABLE_H
#pragma once

#include <type_traits>

#include "visus/autodoc/collection_flags.h"

#include "detector.h"


LYRA_DETAIL_NAMESPACE_BEGIN

/// <summary>
/// The detector function for a <c>is_variable</c> constant.
/// </summary>
template<class TType>
using is_variable_t = decltype(TType::is_variable);

/// <summary>
/// Detects whether a type has a <c>is_variable</c> constant.
/// </summary>
/// <typeparam name="TType"></typeparam>
template<class TType>
struct has_is_variable final : detector<is_variable_t, void, TType> { };


/// <summary>
/// Determines whether a property descriptor identifies a variable property
/// which might change over time.
/// </summary>
/// <typeparam name="TProp">The type of a property descriptor.</typeparam>
/// <typeparam name="HasDecl">Specifies whether <typeparamref name="TProp" />
/// declares a <c>is_variable</c> explicitly.</typeparam>
template<class TProp, bool HasDecl = has_is_variable<TProp>::value>
struct is_variable final : std::false_type { };

/// <summary>
/// Specialisation for types that actually declare whether they are variable
/// or not.
/// </summary>
template<class TProp> struct is_variable<TProp, true> final
    : std::bool_constant<TProp::is_variable> { };

/// <summary>
/// Determines whether a property descriptor identifies a variable property
/// which might change over time.
/// </summary>
/// <typeparam name="TProp">The type of a property descriptor.</typeparam>
template<class TProp>
constexpr auto is_variable_v = is_variable<TProp>::value;


/// <summary>
/// Checks whether a potentially variable property should be collected based on
/// the given <paramref name="flags" />.
/// </summary>
/// <typeparam name="TProp"></typeparam>
/// <param name="flags"></param>
/// <returns></returns>
template<class TProp>
inline bool check_variable(_In_ const collection_flags flags) {
    return !has_flag(flags, collection_flags::variable_only)
        || is_variable_v<TProp>;
}

LYRA_DETAIL_NAMESPACE_END

#endif /* !defined(_LYRA_IS_VARIABLE_H) */
