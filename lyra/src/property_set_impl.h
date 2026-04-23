// <copyright file="property_set_impl.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_PROPERTY_SET_IMPL_H)
#define _LYRA_PROPERTY_SET_IMPL_H
#pragma once

#include <algorithm>
#include <cassert>
#include <map>
#include <string>

#include "visus/lyra/convert_string.h"
#include "visus/lyra/property_set.h"
#include "visus/lyra/property_variant.h"


LYRA_DETAIL_NAMESPACE_BEGIN

/// <summary>
/// Implementation details of the <see cref="property_set" />.
/// </summary>
struct LYRA_TEST_API property_set_impl final {

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
    /// Stores the properties.
    /// </summary>
    std::map<key_type, value_type> values;

    /// <summary>
    /// Convenience method for adding a new property to the set.
    /// </summary>
    /// <typeparam name="TValue"></typeparam>
    /// <param name="key"></param>
    /// <param name="value"></param>
    template<class TValue> inline void add(
            _In_z_ const key_type::value_type *key,
            _In_ TValue&& value) {
        assert(key != nullptr);
        assert(this->values.find(key) == this->values.cend());
        this->values.emplace(key, std::forward<TValue>(value));
    }

    /// <summary>
    /// Adds a wide string property, converting its value to UTF-8 first.
    /// </summary>
    /// <param name="key"></param>
    /// <param name="value"></param>
    inline void add(
            _In_z_ const key_type::value_type *key,
            _In_z_ const wchar_t *value) {
        this->add(key, to_utf8(value));
    }

    /// <summary>
    /// Adds a UTF-16 string property, converting its value to UTF-8 first.
    /// </summary>
    /// <param name="key"></param>
    /// <param name="value"></param>
    inline void add(
            _In_z_ const key_type::value_type *key,
            _In_z_ const char16_t *value) {
        this->add(key, to_utf8(value));
    }

    /// <summary>
    /// Convenience method for adding a new property using a property
    /// descriptor, which will enforce the expected type and set the correct
    /// name implicitly.
    /// </summary>
    /// <typeparam name="TProp"></typeparam>
    /// <param name="value"></param>
    template<class TProp>
    inline void add(_In_ typename TProp::type&& value) {
        this->add(TProp::name, std::forward<typename TProp::type>(value));
    }

};


/// <summary>
/// Moves the implementation into a <see cref="property_set" />.
/// </summary>
/// <remarks>
/// This function is intended for implementors that want to construct a publicly
/// accessible <see cref="property_set" /> after filling an implementation.
/// </remarks>
/// <param name="dst"></param>
/// <param name="src"></param>
/// <returns><paramref name="dst" />.</returns>
LYRA_TEST_API property_set& move(_In_ property_set& dst,
    _Inout_ property_set_impl&& src);

LYRA_DETAIL_NAMESPACE_END

#endif /* !defined(_LYRA_PROPERTY_SET_IMPL_H) */
