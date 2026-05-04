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

#include "visus/autodoc/property_set.h"
#include "visus/autodoc/property_traits.h"

#include "is_immutable.h"
#include "is_sensitive.h"
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
        assert(!key.empty());
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
        assert(!key.empty());
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
        assert(!key.empty());
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
        assert(key != nullptr);
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

    template<class TProp> inline std::enable_if_t<
        std::is_same_v<typename TProp::type, const char *>>
    add(_In_ const multi_sz& value) {
        this->add(TProp::name, value.begin(), value.end());
    }

    template<class TProp> inline std::enable_if_t<
        std::is_same_v<typename TProp::type, const char *>>
    add(_Inout_ multi_sz&& value) {
        this->values.emplace(TProp::name, std::move(value));
    }

    /// <summary>
    /// Find the property with the specified name, or return
    /// <see langword="nullptr" /> if no such property exists.
    /// </summary>
    _Ret_maybenull_ const value_type *find(
        _In_ const key_type& key) const noexcept;

    /// <summary>
    /// Mersge the properties in <paramref name="other" /> into this property
    /// set, unless the same property already exists in <see cref="values" />.
    /// </summary>
    inline void merge(_In_ property_set&& other) {
        other.merge_to(*this);
    }
};


/// <summary>
/// Checks whether adding the property <typeparamref name="TProp" /> is allowed
/// based on the given <paramref name="flags" /> and adds it
/// <paramref name="ps" /> if this is the case.
/// </summary>
template<class TProp, class... TArgs> bool checked_add(
        _Inout_ property_set_impl& ps,
        _In_ const collection_flags flags,
        _In_ TArgs&&... args) {
    const auto retval = check_sensitive<TProp>(flags)
        && check_immutable<TProp>(flags);
    if (retval) {
        ps.add<TProp>(std::forward<TArgs>(args)...);
    }
    return retval;
}


/// <summary>
/// Checks whether adding the (undeclared) property <paramref name="prop" /> is
/// allowed based on the given <paramref name="flags" /> and adds it
/// <paramref name="ps" /> if this is the case.
/// </summary>
template<class TName, class... TArgs> bool checked_add(
        _In_ TName&& prop,
        _Inout_ property_set_impl& ps,
        _In_ const collection_flags flags,
        _In_ TArgs&&... args) {
    const auto retval = !has_flag(flags, collection_flags::no_undeclared);
    if (retval) {
        ps.add(std::forward<TName>(prop), std::forward<TArgs>(args)...);
    }
    return retval;
}

LYRA_DETAIL_NAMESPACE_END

#endif /* !defined(_LYRA_PROPERTY_SET_IMPL_H) */
