// <copyright file="property_type.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_PROPERTY_TYPE_H)
#define _LYRA_PROPERTY_TYPE_H
#pragma once

#include "visus/lyra/dispatch_list.h"


LYRA_NAMESPACE_BEGIN

/// <summary>
/// Allows for runtime identification of the type of a property.
/// </summary>
/// <remarks>
/// <para>Note to implementors: do not forget to add new values to the
/// <see cref="property_types" /> dispatch list below!</para>
/// </remarks>
enum class property_type {

    /// <summary>
    /// The property is not set (equal to <see langword="nullptr" />.
    /// </summary>
    none,

    /// <summary>
    /// The property is an UTF-8 string.
    /// </summary>
    string,

    /// <summary>
    /// The property is a nested <see cref="property_set" />.
    /// </summary>
    properties,

    /// <summary>
    /// A boolean value (<see langword="true" /> or <see langword="false" />).
    /// </summary>
    boolean,

    /// <summary>
    /// A signed 32-bit integer.
    /// </summary>
    int32,

    /// <summary>
    /// An unsigned 32-bit integer.
    /// </summary>
    uint32,

    /// <summary>
    /// A 32-but floating-point number.
    /// </summary>
    float32
};

LYRA_NAMESPACE_END


LYRA_DETAIL_NAMESPACE_BEGIN

/// <summary>
/// A dispatch list for <see cref="property_type" /> values.
/// </summary>
/// <typeparam name="Types"></typeparam>
template<property_type... Types> using property_type_dispatch_list
    = detail::dispatch_list<property_type, Types...>;


/// <summary>
/// The dispatch list for all <see cref="property_type" /> values.
/// </summary>
typedef property_type_dispatch_list<
    property_type::none,
    property_type::string,
    property_type::properties,
    property_type::boolean,
    property_type::int32,
    property_type::uint32,
    property_type::float32
> property_types;

LYRA_DETAIL_NAMESPACE_END

#endif /* !defined(_LYRA_PROPERTY_TYPE_H) */
