// <copyright file="operating_system.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_OPERATING_SYSTEM_H)
#define _LYRA_OPERATING_SYSTEM_H
#pragma once

#include "visus/autodoc/collection_flags.h"
#include "visus/autodoc/property_set.h"
#include "visus/autodoc/version.h"


LYRA_NAMESPACE_BEGIN

namespace operating_system {

    /// <summary>
    /// Denotes a property identifying the OS family.
    /// </summary>
    struct family final {
        typedef const char *type;
        static constexpr auto name = u8"Family";
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
    /// Gets all we know about the operating system.
    /// </summary>
    /// <param name="flags">Allows for customising the collection
    /// behaviour.</param>
    /// <returns>A property set describing the version of the operating
    /// system as detailed as possible.</returns>
    LYRA_API property_set get(_In_ const collection_flags flags
        = collection_flags::none);

    /// <summary>
    /// Gets the registry settings related to the Windows TDR (Timeout Detection
    /// and Recovery) mechanism.
    /// </summary>
    /// <param name="flags"></param>
    /// <returns></returns>
    LYRA_API property_set get_tdr_settings(_In_ const collection_flags flags
        = collection_flags::none);

    /// <summary>
    /// Gets a <see cref="version::version" /> property set describing the
    /// version of the operating system the code is running on.
    /// </summary>
    /// <param name="flags">Allows for customising the collection
    /// behaviour.</param>
    /// <returns>A property set describing the version of the operating
    /// system as detailed as possible.</returns>
    LYRA_API property_set get_version(_In_ const collection_flags flags
        = collection_flags::none);

} /* namespace operating_system */

LYRA_NAMESPACE_END

#endif /* !defined(_LYRA_OPERATING_SYSTEM_H) */
