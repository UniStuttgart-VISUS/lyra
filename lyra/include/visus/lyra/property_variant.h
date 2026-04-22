// <copyright file="property_variant.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_PROPERTY_VARIANT_H)
#define _LYRA_PROPERTY_VARIANT_H
#pragma once

#include <variant>

#include "visus/lyra/property_type.h"


LYRA_DETAIL_NAMESPACE_BEGIN

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
    typedef std::variant<property_type_type_t<Types>...> type;
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
