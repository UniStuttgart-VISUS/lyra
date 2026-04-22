// <copyright file="property_type.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_PROPERTY_TYPE_H)
#define _LYRA_PROPERTY_TYPE_H
#pragma once

#include <cinttypes>
#include <string>

#include "visus/lyra/dispatch_list.h"


LYRA_NAMESPACE_BEGIN

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
    /// An unsigned 32-bit integer.
    /// </summary>
    uint32,

    /// <summary>
    /// A 32-but floating-point number.
    /// </summary>
    float32
};


/// <summary>
/// Derives the C/C++ type for a <see cref="property_type" />.
/// </summary>
/// <typeparam name="Type">The property type to reflect on.</typeparam>
template<property_type Type> struct property_type_type;

/// <summary>
/// Specialisation for <see cref="property_type::none" />.
/// </summary>
template<> struct property_type_type<property_type::none> final {
    typedef std::nullptr_t type;
};

/// <summary>
/// Specialisation for <see cref="property_type::string" />.
/// </summary>
template<> struct property_type_type<property_type::string> final {
    typedef std::string type;
};

/// <summary>
/// Specialisation for <see cref="property_type::properties" />.
/// </summary>
template<> struct property_type_type<property_type::properties> final {
    typedef class property_set type;
};

/// <summary>
/// Specialisation for <see cref="property_type::boolean" />.
/// </summary>
template<> struct property_type_type<property_type::boolean> final {
    typedef bool type;
};

/// <summary>
/// Specialisation for <see cref="property_type::int32" />.
/// </summary>
template<> struct property_type_type<property_type::int32> final {
    typedef std::int32_t type;
};

/// <summary>
/// Specialisation for <see cref="property_type::uint32" />.
/// </summary>
template<> struct property_type_type<property_type::uint32> final {
    typedef std::uint32_t type;
};

/// <summary>
/// Specialisation for <see cref="property_type::float32" />.
/// </summary>
template<> struct property_type_type<property_type::float32> final {
    typedef float type;
};


/// <summary>
/// Derives the C/C++ type for a <see cref="property_type" />.
/// </summary>
/// <typeparam name="Type">The property type to reflect on.</typeparam>
template<property_type Type>
using property_type_type_t = typename property_type_type<Type>::type;


/// <summary>
/// Derives the <see cref="property_type" /> from a C++ type.
/// </summary>
/// <typeparam name="TType">The C/C++ type to reflect on.</typeparam>
template<class TType> struct property_type_value;

/// <summary>
/// Specialisation for <see cref="std::nullptr_t" />.
/// </summary>
template<> struct property_type_value<std::nullptr_t> final {
    static constexpr property_type value = property_type::none;
};

/// <summary>
/// Specialisation for <see cref="std::string" />.
/// </summary>
template<> struct property_type_value<std::string> final {
    static constexpr property_type value = property_type::string;
};

/// <summary>
/// Specialisation for <see cref="property_set" />.
/// </summary>
template<> struct property_type_value<class property_set> final {
    static constexpr property_type value = property_type::properties;
};

/// <summary>
/// Specialisation for <see langword="bool" />.
/// </summary>
template<> struct property_type_value<bool> final {
    static constexpr property_type value = property_type::boolean;
};

/// <summary>
/// Specialisation for <see cref="std::int32_t" />.
/// </summary>
template<> struct property_type_value<std::int32_t> final {
    static constexpr property_type value = property_type::int32;
};

/// <summary>
/// Specialisation for <see cref="std::uint32_t" />.
/// </summary>
template<> struct property_type_value<std::uint32_t> final {
    static constexpr property_type value = property_type::uint32;
};

/// <summary>
/// Specialisation for <see langword="float" />.
/// </summary>
template<> struct property_type_value<float> final {
    static constexpr property_type value = property_type::float32;
};


/// <summary>
/// Allows for deriving the <see cref="property_type" /> from a C++ type.
/// </summary>
/// <typeparam name="TType">The C/C++ type to reflect on.</typeparam>
template<class TType>
constexpr auto property_type_value_v = property_type_value<TType>::value;

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
    property_type::uint32,
    property_type::float32
> property_types;

LYRA_DETAIL_NAMESPACE_END

#endif /* !defined(_LYRA_PROPERTY_TYPE_H) */
