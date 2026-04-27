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
#include <string>
#include <type_traits>
#include <vector>

#include "visus/lyra/property_set.h"
#include "visus/lyra/property_traits.h"

#include "property.h"


LYRA_DETAIL_NAMESPACE_BEGIN

/// <summary>
/// Implementation details of the <see cref="property_set" />.
/// </summary>
struct LYRA_TEST_API property_set_impl final {

    /// <summary>
    /// Determines the type iterated by <typeparamref name="TIterator" />.
    /// </summary>
    template<class TIterator>
    using iterated_type = typename std::iterator_traits<TIterator>::value_type;

    /// <summary>
    /// The key type, which are UTF-8-encoded property names.
    /// </summary>
    typedef std::string key_type;

    /// <summary>
    /// The value type, which is a variant that can store any of the
    /// <see cref="property_type" />s.
    /// </summary>
    typedef property value_type;

    /// <summary>
    /// Stores the properties.
    /// </summary>
    std::vector<value_type> values;

    /// <summary>
    /// Initialises a new instance.
    /// </summary>
    property_set_impl(void) = default;

    property_set_impl(const property_set_impl&) = delete;

    /// <summary>
    /// Convenience method for adding a new property to the set.
    /// </summary>
    /// <remarks>
    /// The last parameter <paramref name="_" /> makes sure that the traits can
    /// be instantiated for the value type.
    /// </remarks>
    template<class TValue> inline void add(
            _Inout_ key_type&& key,
            _In_ const TValue& value,
            const property_traits<std::decay_t<TValue>> *_ = nullptr) {
        assert(this->find(key) == nullptr);
        this->values.emplace_back(std::move(key), value);
    }

    /// <summary>
    /// Convenience method for adding a new property to the set.
    /// </summary>
    /// <remarks>
    /// The last parameter <paramref name="_" /> makes sure that the traits can
    /// be instantiated for the value type.
    /// </remarks>
    template<class TValue> inline void add(
            _Inout_ key_type&& key,
            _In_ TValue&& value,
            const property_traits<std::decay_t<TValue>> *_ = nullptr) {
        assert(this->find(key) == nullptr);
        this->values.emplace_back(std::move(key), std::forward<TValue>(value));
    }

    /// <summary>
    /// Convenience method for adding a new vector-valued property to the set.
    /// </summary>
    /// <remarks>
    /// The last parameter <paramref name="_" /> makes sure that the traits can
    /// be instantiated for the value type.
    /// </remarks>
    template<class TIterator> inline void add(
            _Inout_ key_type&& key,
            _In_ const TIterator begin,
            _In_ const TIterator end,
            const property_traits<iterated_type<TIterator>> *_ = nullptr) {
        assert(this->find(key) == nullptr);
        this->values.emplace_back(std::move(key), begin, end);
    }

    /// <summary>
    /// Convenience method for adding a new property to the set.
    /// </summary>
    template<class TValue> inline void add(
            _In_z_ const key_type::value_type *key,
            _In_ const TValue& value) {
        assert(key != nullptr);
        this->add(std::string(key), value);
    }

    /// <summary>
    /// Convenience method for adding a new property to the set.
    /// </summary>
    template<class TValue> inline void add(
            _In_z_ const key_type::value_type *key,
            _Inout_ TValue&& value) {
        assert(key != nullptr);
        this->add(std::string(key), std::forward<TValue>(value));
    }

    /// <summary>
    /// Convenience method for adding a new vector-valued property to the set.
    /// </summary>
    template<class TIterator> inline void add(
            _In_z_ const key_type::value_type *key,
            _In_ TIterator&& begin,
            _In_ TIterator&& end) {
        assert(key != nullptr);
        this->add(std::string(key),
            std::forward<TIterator>(begin),
            std::forward<TIterator>(end));
    }

        /// <summary>
    /// Adds a string property in the form of a single multi-sz string.
    /// </summary>
    inline void add(_Inout_ key_type&& key,
            _In_opt_z_ const char *value) {
        this->add(std::move(key), multi_sz::for_string(value));
    }

    /// <summary>
    /// Adds a string property in the form of a single multi-sz string.
    /// </summary>
    inline void add(_In_z_ const key_type::value_type *key,
            _In_opt_z_ const char *value) {
        assert(key != nullptr);
        this->add(std::string(key), multi_sz::for_string(value));
    }

    /// <summary>
    /// Adds a wide string property, converting its value to UTF-8 first.
    /// </summary>
    void add(_In_z_ const key_type::value_type *key,
        _In_opt_z_ const wchar_t *value);

    /// <summary>
    /// Adds a UTF-16 string property, converting its value to UTF-8 first.
    /// </summary>
    void add(_In_z_ const key_type::value_type *key,
        _In_opt_z_ const char16_t *value);

    /// <summary>
    /// Convenience method for adding a new property using a property
    /// descriptor, which will enforce the expected type and set the correct
    /// name implicitly.
    /// </summary>
    template<class TProp>
    inline void add(_In_ const typename TProp::type& value) {
        this->add(TProp::name, value);
    }

    /// <summary>
    /// Convenience method for adding a new property using a property
    /// descriptor, which will enforce the expected type and set the correct
    /// name implicitly.
    /// </summary>
    template<class TProp>
    inline void add(_In_ typename TProp::type&& value) {
        this->add(TProp::name, std::forward<typename TProp::type>(value));
    }

    /// <summary>
    /// Find the property with the specified name, or return
    /// <see langword="nullptr" /> if no such property exists.
    /// </summary>
    _Ret_maybenull_ const value_type *find(
        _In_ const key_type& key) const noexcept;

    property_set_impl& operator =(const property_set_impl&) = delete;
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
LYRA_TEST_API property_set& realise(_In_ property_set& dst,
    _Inout_ property_set_impl&& src);

LYRA_DETAIL_NAMESPACE_END

#endif /* !defined(_LYRA_PROPERTY_SET_IMPL_H) */
