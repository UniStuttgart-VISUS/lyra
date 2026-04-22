// <copyright file="property_set_impl.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_PROPERTY_SET_IMPL_H)
#define _LYRA_PROPERTY_SET_IMPL_H
#pragma once

#include <vector>
#include <string>

#include "visus/lyra/property_variant.h"


LYRA_DETAIL_NAMESPACE_BEGIN

/// <summary>
/// Implementation details of the <see cref="property_set" />.
/// </summary>
struct property_set_impl final {

    /// <summary>
    /// The key type, which are UTF-8-encoded property names.
    /// </summary>
    typedef std::string key_type;

    /// <summary>
    /// The value type, which is a variant that can store any of the
    /// <see cref="property_type" />s.
    /// </summary>
    typedef property_variant value_type;

    /// <summary>
    /// Stores the names of the properties.
    /// </summary>
    std::vector<key_type> keys;

    /// <summary>
    /// Stores the values in the same order as the <see cref="keys" />.
    /// </summary>
    std::vector<value_type> values;
};

LYRA_DETAIL_NAMESPACE_END

#endif /* !defined(_LYRA_PROPERTY_SET_IMPL_H) */
