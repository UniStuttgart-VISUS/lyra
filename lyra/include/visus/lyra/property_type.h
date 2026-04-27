// <copyright file="property_type.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_PROPERTY_TYPE_H)
#define _LYRA_PROPERTY_TYPE_H
#pragma once

#include "visus/lyra/multi_sz.h"


LYRA_NAMESPACE_BEGIN


/// <summary>
/// Allows for runtime identification of the type of a property.
/// </summary>
/// <remarks>
/// <para>Note to implementors: do not forget to add new values to the
/// dispatch lists and property traits in property_traits.h!</para>
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
    /// A signed 64-bit integer.
    /// </summary>
    int64,

    /// <summary>
    /// An unsigned 32-bit integer.
    /// </summary>
    uint32,

    /// <summary>
    /// An unsigned 64-bit integer.
    /// </summary>
    uint64,

    /// <summary>
    /// A 32-bit floating-point number.
    /// </summary>
    float32,

    /// <summary>
    /// A 64-bit floating-point number.
    /// </summary>
    float64
};

LYRA_NAMESPACE_END

#endif /* !defined(_LYRA_PROPERTY_TYPE_H) */
