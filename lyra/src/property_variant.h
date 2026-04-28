// <copyright file="property_variant.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_PROPERTY_VARIANT_H)
#define _LYRA_PROPERTY_VARIANT_H
#pragma once

#include <memory>
#include <string>
#include <type_traits>
#include <variant>
#include <vector>

#include "visus/lyra/multi_sz.h"
#include "visus/lyra/property_traits.h"

#include "boolean.h"
#include "unique_cat.h"


LYRA_DETAIL_NAMESPACE_BEGIN

/// <summary>
/// Derives the way how a property of a specific type is stored.
/// </summary>
template<property_type Type> struct make_scalar_storage final {
    typedef property_type_t<Type> type;
};

/// <summary>
/// Store strings as multi-sz strings rather than plain C stuff.
/// </summary>
template<> struct make_scalar_storage<property_type::string> final {
    typedef multi_sz type;
};

/// <summary>
/// Derives the way how a vector-valued property is stored.
/// </summary>
template<property_type Type> struct make_vector_storage final {
    typedef typename make_scalar_storage<Type>::type _scalar;
    typedef std::vector<_scalar> type;
};

/// <summary>
/// Store <see langword="bool" />s in a wrapper to prevent the bitfield
/// optimisation in STL from kicking in, which would ruin pointer arithmetics
/// in the public API.
/// </summary>
template<> struct make_vector_storage<property_type::boolean> final {
    typedef LYRA_DETAIL_NAMESPACE::boolean _scalar;
    typedef std::vector<_scalar> type;
};

/// <summary>
/// Disable vector specialisation for strings as <see cref="multi_sz" />s are
/// already vector-like.
/// </summary>
template<> struct make_vector_storage<property_type::string> final {
    typedef void type;
};


/// <summary>
/// Builds an STL variant for storing any allowed types of properties.
/// </summary>
template<class TList> struct make_property_variant;

/// <summary>
/// Specialisation for actual dispatch lists of property types.
/// </summary>
template<property_type... Types>
struct make_property_variant<property_type_dispatch_list<Types...>> final {
    typedef unique_cat_t<std::variant<std::monostate>,
        typename make_scalar_storage<Types>::type...> _scalars;
    typedef unique_cat_t<_scalars,
        typename make_vector_storage<Types>::type...> type;
};


/// <summary>
/// A variant type for storing any of the allowed property types.
/// </summary>
typedef make_property_variant<property_types>::type property_variant;


/// <summary>
/// Provides access to the raw data of a variant.
/// </summary>
template<class TType> struct property_variant_access final {
    typedef property_traits<TType> _traits;
    typedef typename _traits::pointer pointer;
    typedef typename _traits::type storage;
    static constexpr auto type = _traits::value;
    static inline pointer get(_In_ const storage& v) noexcept {
        return std::addressof(v);
    }
    static inline constexpr std::size_t count(const storage&) noexcept {
        return 1;
    }
};

/// <summary>
/// Specialisation for empty variants.
/// </summary>
template<> struct property_variant_access<std::monostate> final {
    typedef const void *pointer;
    typedef std::monostate storage;
    static constexpr auto type = property_type::none;
    static inline constexpr pointer get(_In_ const storage&) noexcept {
        return nullptr;
    }
    static inline constexpr std::size_t count(const storage&) noexcept {
        return 0;
    }
};

/// <summary>
/// Specialisation for vector-valued properties.
/// </summary>
template<class TType> struct property_variant_access<std::vector<TType>> final {
    typedef property_traits<TType> _traits;
    typedef typename _traits::pointer pointer;
    typedef typename make_vector_storage<_traits::value>::type storage;
    static constexpr auto type = _traits::value;
    static inline pointer get(_In_ const storage& v) noexcept {
        return v.data();
    }
    static inline std::size_t count(_In_ const storage& v) noexcept {
        return v.size();
    }
};

/// <summary>
/// Specialisation for Boolean vectors.
/// </summary>
template<> struct property_variant_access<std::vector<boolean>> final {
    typedef const bool *pointer;
    typedef std::vector<boolean> storage;
    static constexpr auto type = property_type::boolean;
    static inline pointer get(_In_ const storage& v) noexcept {
        return &v[0];
    }
    static inline std::size_t count(const storage& v) noexcept {
        return v.size();
    }
};


LYRA_DETAIL_NAMESPACE_END

#endif /* !defined(_LYRA_PROPERTY_VARIANT_H) */
