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

    struct tdr_ddi_delay final {
        typedef std::uint32_t type;
        static constexpr auto name = u8"TDR DDI Delay";
    };

    struct tdr_debug_mode final {
        typedef std::uint32_t type;
        static constexpr auto name = u8"TDR Debug Mode";
    };

    struct tdr_delay final {
        typedef std::uint32_t type;
        static constexpr auto name = u8"TDR Delay";
    };

    struct tdr_level final {
        typedef std::uint32_t type;
        static constexpr auto name = u8"TDR Level";
    };

    struct tdr_limit_count final {
        typedef std::uint32_t type;
        static constexpr auto name = u8"TDR Limit Count";
    };

    struct tdr_limit_time final {
        typedef std::uint32_t type;
        static constexpr auto name = u8"TDR Limit Time";
    };

    /// <summary>
    /// Identifies the property holding the Windows TDR (Timeout Detection and
    /// Recovery) settings.
    /// </summary>
    struct tdr_settings final {
        typedef property_set type;
        static constexpr auto name = u8"TDR Settings";
    };

    /// <summary>
    /// Gets all graphics-related information.
    /// </summary>
    /// <param name="flags"></param>
    /// <returns></returns>
    property_set get(_In_ const collection_flags flags
        = collection_flags::none);

    /// <summary>
    /// Gets all graphics adapters accessible via the DirectX Graphics
    /// Infrastructure (DXGI).
    /// </summary>
    /// <param name="flags"></param>
    /// <returns></returns>
    LYRA_API property_set get_dxgi_adapters(_In_ const collection_flags flags
        = collection_flags::none);

    /// <summary>
    /// Gets the registry settings related to the Windows TDR (Timeout Detection
    /// and Recovery) mechanism.
    /// </summary>
    /// <param name="flags"></param>
    /// <returns></returns>
    LYRA_API property_set get_tdr_settings(_In_ const collection_flags flags
        = collection_flags::none);

} /* namespace graphics */

LYRA_NAMESPACE_END

#endif /* !defined(_LYRA_GRAPHICS_H) */
