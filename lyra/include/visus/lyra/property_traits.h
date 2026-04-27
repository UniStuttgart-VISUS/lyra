// <copyright file="property_traits.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_PROPERTY_TRAITS_H)
#define _LYRA_PROPERTY_TRAITS_H
#pragma once

#include <cinttypes>

#include "visus/lyra/dispatch_list.h"
#include "visus/lyra/property_type.h"


// Forward declarations
LYRA_NAMESPACE_BEGIN
class property_set;
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

_LYRA_PROP_TRAITS(string, const char *);
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


#define _LYRA_PROP_TRAITS(v, t)\
template<> struct property_traits<t> final {\
    static constexpr property_type value = property_type::v;\
    typedef property_type_t<value> type;\
    typedef const type *pointer;\
}

_LYRA_PROP_TRAITS(boolean, bool);
_LYRA_PROP_TRAITS(string, const char *);
_LYRA_PROP_TRAITS(properties, property_set);
_LYRA_PROP_TRAITS(int32, std::int32_t);
_LYRA_PROP_TRAITS(int64, std::int64_t);
_LYRA_PROP_TRAITS(uint32, std::uint32_t);
_LYRA_PROP_TRAITS(uint64, std::uint64_t);
_LYRA_PROP_TRAITS(float32, float);
_LYRA_PROP_TRAITS(float64, double);

#undef _LYRA_PROP_TRAITS

/// <summary>
/// Allows for deriving the <see cref="property_type" /> from a C++ type.
/// </summary>
/// <typeparam name="TType">The C/C++ type to reflect on.</typeparam>
template<class TType>
constexpr auto property_type_v = property_traits<TType>::value;

LYRA_DETAIL_NAMESPACE_END

#endif /* !defined(_LYRA_PROPERTY_TYPE_H) */
