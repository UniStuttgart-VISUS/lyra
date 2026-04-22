// <copyright file="property_type.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_PROPERTY_TYPE_H)
#define _LYRA_PROPERTY_TYPE_H
#pragma once

#include "visus/lyra/api.h"


LYRA_NAMESPACE_BEGIN

/// <summary>
/// Allows for runtime identification of the type of a property.
/// </summary>
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
    properties
};


LYRA_NAMESPACE_END

#endif /* !defined(_LYRA_PROPERTY_TYPE_H) */
