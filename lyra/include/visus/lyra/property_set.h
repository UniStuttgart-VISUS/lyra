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
namespace detail { void move(property_set&, property_set_impl&&); }


/// <summary>
/// Represents a group of properties about the system.
/// </summary>
class LYRA_API property_set final {

public:

    /// <summary>
    /// The type-erased value type used to represent a property.
    /// </summary>
    typedef void *value_type;

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
    /// Answer whether the property set contains a property with the specified
    /// name.
    /// </summary>
    /// <param name="name">The name of the property to search. The encoding of
    /// this string is assumed to be UTF-8. If this is not the case, the answer
    /// might be wrong.</param>
    /// <returns><see langword="true" /> if the property set contains a property
    /// with the specified name, <see langword="false" /> otherwise.</returns>
    bool contains(_In_z_ const char *name) const noexcept;

    /// <summary>
    /// Answer whether the property set is empty.
    /// </summary>
    /// <returns><see langword="true" /> if there is no property in the set,
    /// <see langword="false" /> otherwise.</returns>
    bool empty(void) const noexcept;

    /// <summary>
    /// Gets a pointer to the value of the property with the specified name.
    /// </summary>
    /// <param name="dst"></param>
    /// <param name="length"></param>
    /// <param name="type"></param>
    /// <param name="name"></param>
    /// <returns></returns>
    bool get(_Out_ value_type& dst, _Out_ std::size_t& length,
        _Out_ property_type& type, _In_z_ const char *name) const noexcept;

    /// <summary>
    /// Answer the number (and names) of the properties in the set.
    /// </summary>
    /// <param name="dst">A buffer to receive pointers to at most
    /// <paramref name="cnt" /> property names.</param>
    /// <param name="cnt">The number of elements in <paramref name="dst" />.
    /// </param>
    /// <returns>The total number of properties in the set.</returns>
    std::size_t properties(_Out_writes_opt_(cnt) const char **dst = nullptr,
        _In_ std::size_t cnt = 0) const noexcept;

    /// <summary>
    /// Answer the siez of the property set.
    /// </summary>
    /// <returns>The total number of properties in the set.</returns>
    std::size_t size(void) const noexcept;

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

    friend void detail::move(property_set&, detail::property_set_impl&&);
};

LYRA_NAMESPACE_END

#endif /* !defined(_LYRA_PROPERTY_SET_H) */
