// <copyright file="property_set.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_PROPERTY_SET_H)
#define _LYRA_PROPERTY_SET_H
#pragma once

#include "visus/lyra/property_type.h"


LYRA_NAMESPACE_BEGIN

// Forward declarations.
namespace detail { struct property_set_impl; }


/// <summary>
/// Represents a group of properties about the system.
/// </summary>
class LYRA_API property_set final {

public:

    /// <summary>
    /// Initialises a new instance.
    /// </summary>
    inline property_set(void) noexcept : _impl(nullptr) { }

    /// <summary>
    /// Clone <see cref="other" />
    /// </summary>
    /// <param name="other">The object to be cloned.</param>
    property_set(_In_ const property_set& other);

    /// <summary>
    /// Move <see cref="other" />
    /// </summary>
    /// <param name="other">The object to be moved.</param>
    inline property_set(_Inout_ property_set&& other) noexcept
            : _impl(other._impl) {
        other._impl = nullptr;
    }

    /// <summary>
    /// Answer whether the property set is empty.
    /// </summary>
    /// <returns><see langword="true" /> if there is no property in the set,
    /// <see langword="false" /> otherwise.</returns>
    bool empty(void) const noexcept;

    /// <summary>
    /// Assignment.
    /// </summary>
    /// <param name="rhs">The right-hand-side operand.</param>
    /// <returns><c>*<see langword="this" /></c>.</returns>
    property_set& operator =(_In_ const property_set& rhs);

    /// <summary>
    /// Move assignment.
    /// </summary>
    /// <param name="rhs">The right-hand-side operand.</param>
    /// <returns><c>*<see langword="this" /></c>.</returns>
    property_set& operator =(_Inout_ property_set&& rhs) noexcept;

private:

    detail::property_set_impl *_impl;

    friend class detail::property_set_impl;
};

LYRA_NAMESPACE_END

#endif /* !defined(_LYRA_PROPERTY_SET_H) */
