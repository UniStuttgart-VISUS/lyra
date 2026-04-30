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
#include <type_traits>

#include "visus/lyra/property_set.h"
#include "visus/lyra/property_traits.h"

#include "property_variant.h"


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
    typedef property_variant value_type;

    /// <summary>
    /// Cache location for the JSON representation.
    /// </summary>
    std::string json;

    /// <summary>
    /// Stores the properties.
    /// </summary>
    std::map<key_type, value_type> values;

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
        this->values.emplace(std::move(key), value);
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
        this->values.emplace(std::move(key), std::forward<TValue>(value));
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
        this->values.emplace(std::move(key), begin, end);
    }

    /// <summary>
    /// Convenience method for adding a new vector-valued property to the set.
    /// </summary>
    template<class TIterator> inline void add(
            _In_z_ const char *key,
            _In_ const TIterator begin,
            _In_ const TIterator end,
            const property_traits<iterated_type<TIterator>> *_ = nullptr) {
        asssert(key != nullptr);
        this->values.emplace(std::string(key), begin, end);
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
    /// Adds a string property if non-<see langword="nullptr" />.
    /// </summary>
    void add(_Inout_ key_type&& key, _In_opt_z_ const char *value);

    /// <summary>
    /// Adds a string property.
    /// </summary>
    inline void add(_In_z_ const key_type::value_type *key,
            _In_opt_z_ const char *value) {
        assert(key != nullptr);
        this->add(std::string(key), value);
    }

    /// <summary>
    /// Adds a string property if non-<see langword="nullptr" />.
    /// </summary>
    inline void add(_Inout_ key_type&& key, _In_opt_z_ char *value) {
        this->add(std::move(key), const_cast<const char *>(value));
    }

    /// <summary>
    /// Adds a string property.
    /// </summary>
    inline void add(_In_z_ const key_type::value_type *key,
            _In_opt_z_ char *value) {
        assert(key != nullptr);
        this->add(std::string(key), value);
    }

    /// <summary>
    /// Adds a string property.
    /// </summary>
    inline void add(_Inout_ key_type&& key, _In_ const std::string& value) {
        this->add(std::move(key), value.c_str());
    }

    /// <summary>
    /// Adds a string property.
    /// </summary>
    inline void add(_In_z_ const key_type::value_type *key,
            _In_ const std::string& value) {
        assert(key != nullptr);
        this->add(std::string(key), value);
    }

    /// <summary>
    /// Adds a string property.
    /// </summary>
    inline void add(_Inout_ key_type&& key, _In_ std::string&& value) {
        this->add(std::move(key), value.c_str());
    }

    /// <summary>
    /// Adds a string property.
    /// </summary>
    inline void add(_In_z_ const key_type::value_type *key,
            _In_ std::string&& value) {
        assert(key != nullptr);
        this->add(std::string(key), value.c_str());
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

    template<class TProp>
    inline void add(_In_ const std::vector<typename TProp::type>& value) {
        this->add(TProp::name, value.begin(), value.end());
    }

    template<class TProp>
    inline void add(_Inout_ std::vector<typename TProp::type>&& value) {
        this->values.emplace(TProp::name, std::move(value));
    }

    /// <summary>
    /// Find the property with the specified name, or return
    /// <see langword="nullptr" /> if no such property exists.
    /// </summary>
    _Ret_maybenull_ const value_type *find(
        _In_ const key_type& key) const noexcept;
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

/// <summary>
/// Moves the implementation into a new <see cref="property_set" />.
/// </summary>
/// <param name="src"></param>
/// <returns></returns>
LYRA_TEST_API property_set to_property_set(_Inout_ property_set_impl&& src);

LYRA_DETAIL_NAMESPACE_END

#endif /* !defined(_LYRA_PROPERTY_SET_IMPL_H) */
