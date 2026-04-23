// <copyright file="property_variant.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_PROPERTY_VARIANT_H)
#define _LYRA_PROPERTY_VARIANT_H
#pragma once

#include <variant>

#include "visus/lyra/boolean.h"
#include "visus/lyra/property_set.h"
#include "visus/lyra/property_type.h"


LYRA_DETAIL_NAMESPACE_BEGIN

/// <summary>
/// Creates a vectorised version of a <typeparamref name="TType" />.
/// </summary>
/// <typeparam name="TType"></typeparam>
template<class TType> struct make_vector_property final {
    typedef std::vector<TType> type;
};

/// <summary>
/// Forces Booleans to be wrapped with our <see cref="boolean" /> wrapper such
/// that the bitfield specialisation is not used, which would prevent the
/// callers from accessing the stored values via pointer arithmetic.
/// </summary>
template<> struct make_vector_property<bool> final {
    typedef std::vector<boolean> type;
};


/// <summary>
/// Derives a variant type from a dispatch list of
/// <see cref="property_type" />s.
/// </summary>
/// <typeparam name="TDispatch"></typeparam>
template<class TDispatch> struct make_property_variant;

/// <summary>
/// Specialisation for actual dispatch lists of <see cref="property_type" />s.
/// </summary>
/// <typeparam name="Types"></typeparam>
template<property_type... Types>
struct make_property_variant<property_type_dispatch_list<Types...>> final {
    typedef std::variant<property_type_t<Types>...,
        typename make_vector_property<property_type_t<Types>>::type...> type;
};

LYRA_DETAIL_NAMESPACE_END


LYRA_NAMESPACE_BEGIN

/// <summary>
/// A variant type that can store any of the <see cref="property_type" />s.
/// </summary>
typedef typename detail::make_property_variant<detail::property_types>::type
    property_variant;

LYRA_NAMESPACE_END

#endif /* !defined(_LYRA_PROPERTY_VARIANT_H) */
