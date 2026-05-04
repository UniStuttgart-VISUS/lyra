// <copyright file="graphics.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_GRAPHICS_H)
#define _LYRA_GRAPHICS_H
#pragma once

#include "visus/autodoc/collection_flags.h"
#include "visus/autodoc/property_set.h"


LYRA_NAMESPACE_BEGIN

namespace graphics {

    /// <summary>
    /// Identifies the representation of a DXGI adapter descriptor.
    /// </summary>
    struct dxgi_adapter final {
        typedef property_set type;
        static constexpr auto is_immutable = true;
        static constexpr auto name = u8"DXGI Adapter";
    };

    /// <summary>
    /// Gets all graphics adapters accessible via the DirectX Graphics
    /// Infrastructure (DXGI).
    /// </summary>
    property_set LYRA_API get_dxgi_adapters(_In_ const collection_flags flags
        = collection_flags::none);

} /* namespace graphics */

LYRA_NAMESPACE_END

#endif /* !defined(_LYRA_GRAPHICS_H) */
