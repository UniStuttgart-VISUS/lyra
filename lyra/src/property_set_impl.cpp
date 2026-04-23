// <copyright file="invoke.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "property_set_impl.h"


/*
 * LYRA_DETAIL_NAMESPACE::move
 */
LYRA_NAMESPACE::property_set& LYRA_DETAIL_NAMESPACE::move(
        _In_ property_set& dst,
        _Inout_ property_set_impl&& src) {
    assert(dst._impl == nullptr);
    dst._impl = new property_set_impl();
    dst._impl->values = std::move(src.values);
    return dst;
}
