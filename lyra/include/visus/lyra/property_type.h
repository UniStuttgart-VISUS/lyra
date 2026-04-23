// <copyright file="property_type.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_PROPERTY_TYPE_H)
#define _LYRA_PROPERTY_TYPE_H
#pragma once

#include <cinttypes>
#include <memory>
#include <string>
#include <type_traits>
#include <vector>

#include "visus/lyra/boolean.h"
#include "visus/lyra/dispatch_list.h"


LYRA_NAMESPACE_BEGIN

// Forward declarations.
class property_set;

/// <summary>
/// Allows for runtime identification of the type of a property.
/// </summary>
/// <remarks>
/// <para>Note to implementors: do not forget to add new values to the
/// <see cref="property_types" /> dispatch list below!</para>
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


LYRA_DETAIL_NAMESPACE_BEGIN

/// <summary>
/// A dispatch list for <see cref="property_type" /> values.
/// </summary>
/// <typeparam name="Types"></typeparam>
template<property_type... Types> using property_type_dispatch_list
    = detail::dispatch_list<property_type, Types...>;


/// <summary>
/// The dispatch list for all <see cref="property_type" /> values.
/// </summary>
typedef property_type_dispatch_list<
    property_type::none,
    property_type::string,
    property_type::properties,
    property_type::boolean,
    property_type::int32,
    property_type::int64,
    property_type::uint32,
    property_type::uint64,
    property_type::float32,
    property_type::float64
> property_types;


/// <summary>
/// Allows for reflecting on a <see cref="property_type" />.
/// </summary>
/// <typeparam name="Type">The property type to reflect on.</typeparam>
template<property_type Type> struct property_type_traits;

#define _LYRA_PROP_TRAITS(v, t)\
template<> struct property_type_traits<property_type::v> final {\
    typedef t type;\
}

_LYRA_PROP_TRAITS(none, std::nullptr_t);
_LYRA_PROP_TRAITS(string, std::string);
_LYRA_PROP_TRAITS(properties, property_set);
_LYRA_PROP_TRAITS(boolean, bool);
_LYRA_PROP_TRAITS(int32, std::int32_t);
_LYRA_PROP_TRAITS(int64, std::int64_t);
_LYRA_PROP_TRAITS(uint32, std::uint32_t);
_LYRA_PROP_TRAITS(uint64, std::uint64_t);
_LYRA_PROP_TRAITS(float32, float);
_LYRA_PROP_TRAITS(float64, double);

#undef _LYRA_PROP_TRAITS

/// <summary>
/// Derives the C/C++ type for a <see cref="property_type" />.
/// </summary>
/// <typeparam name="Type">The property type to reflect on.</typeparam>
template<property_type Type>
using property_type_t = typename property_type_traits<Type>::type;


/// <summary>
/// Derives the <see cref="property_type" /> from a C++ type.
/// </summary>
/// <typeparam name="TType">The C/C++ type to reflect on.</typeparam>
template<class TType> struct property_traits;

template<> struct property_traits<std::nullptr_t> final {
    static constexpr property_type value = property_type::none;
    typedef std::nullptr_t pointer;
    typedef property_type_t<value> type;
    static constexpr inline pointer data(_In_ const type&) noexcept {
        return nullptr;
    }
    static constexpr inline std::size_t count(_In_ type) noexcept { return 0; }
};

template<> struct property_traits<std::string> final {
    static constexpr property_type value = property_type::string;
    typedef const char *pointer;
    typedef property_type_t<value> type;
    static inline pointer data(_In_ const type& v) noexcept {
        return v.c_str();
    }
    static constexpr inline std::size_t count(_In_ const type&) noexcept {
        return 1;
    }
};

template<> struct property_traits<property_set> final {
    static constexpr property_type value = property_type::properties;
    typedef property_type_t<value> type;
    typedef const type *pointer;
    static inline pointer data(_In_ const type& v) noexcept {
        return std::addressof(v);
    }
    static constexpr inline std::size_t count(_In_ const type&) noexcept {
        return 1;
    }
};

template<> struct property_traits<boolean> final {
    static constexpr property_type value = property_type::boolean;
    typedef const bool *pointer;
    typedef property_type_t<value> type;
    static inline pointer data(_In_ const type& v) noexcept {
        return &v;
    }
    static constexpr inline std::size_t count(_In_ const type&) noexcept {
        return 1;
    }
};

#define _LYRA_PROP_TRAITS(v, t)\
template<> struct property_traits<t> final {\
    static constexpr property_type value = property_type::v;\
    typedef property_type_t<value> type;\
    typedef const type *pointer;\
    typedef std::conditional_t<std::is_scalar_v<type>, type, const type&> _arg;\
    static const type *data(_In_ const type& v) noexcept {\
        return std::addressof(v);\
    }\
    static constexpr inline std::size_t count(_In_ _arg) noexcept {\
        return 1;\
    }\
}

_LYRA_PROP_TRAITS(boolean, bool);
_LYRA_PROP_TRAITS(int32, std::int32_t);
_LYRA_PROP_TRAITS(int64, std::int64_t);
_LYRA_PROP_TRAITS(uint32, std::uint32_t);
_LYRA_PROP_TRAITS(uint64, std::uint64_t);
_LYRA_PROP_TRAITS(float32, float);
_LYRA_PROP_TRAITS(float64, double);

#undef _LYRA_PROP_TRAITS

template<class TType> struct property_traits<std::vector<TType>> final {
    static constexpr property_type value = property_traits<TType>::value;
    typedef property_type_t<value> type;
    typedef std::vector<type> _arg;
    static inline const type *data(_In_ const _arg& v) noexcept {
        return v.data();
    }
    static inline std::size_t count(_In_ const _arg& v) noexcept {
        return v.size();
    }
};

template<> struct property_traits<std::vector<boolean>> final {
    static constexpr property_type value = property_traits<bool>::value;
    typedef property_type_t<value> type;
    typedef std::vector<boolean> _arg;
    static inline const type *data(_In_ const _arg& v) noexcept {
        return &v[0];
    }
    static inline std::size_t count(_In_ const _arg& v) noexcept {
        return v.size();
    }
};

template<class TType> struct property_traits<std::unique_ptr<TType>> final {
    static constexpr property_type value = property_traits<TType>::value;
    typedef property_type_t<value> type;
    typedef std::unique_ptr<type> _arg;
    static inline const type *data(_In_ const _arg& v) noexcept {
        return v.get();
    }
    static inline std::size_t count(_In_ const _arg& v) noexcept {
        return (v != nullptr) ? 1 : 0;
    }
};


/// <summary>
/// Allows for deriving the <see cref="property_type" /> from a C++ type.
/// </summary>
/// <typeparam name="TType">The C/C++ type to reflect on.</typeparam>
template<class TType>
constexpr auto property_type_v = property_traits<TType>::value;

LYRA_DETAIL_NAMESPACE_END

#endif /* !defined(_LYRA_PROPERTY_TYPE_H) */
