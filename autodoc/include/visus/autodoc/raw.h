// <copyright file="raw.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_RAW_H)
#define _LYRA_RAW_H
#pragma once

#include "visus/autodoc/collection_flags.h"
#include "visus/autodoc/property_set.h"
#include "visus/autodoc/timestamp.h"


LYRA_NAMESPACE_BEGIN

namespace raw {

    /// <summary>
    /// Identifies the block of meta data about the calling application.
    /// </summary>
    struct application final {
        typedef property_set type;
        static constexpr auto name = u8"Application";
    };

    /// <summary>
    /// Identifies a block containing the environment.
    /// </summary>
    struct environment final {
        typedef property_set type;
        static constexpr auto name = u8"Environment";
    };

    /// <summary>
    /// Identifies a block containing the hardware.
    /// </summary>
    struct hardware final {
        typedef property_set type;
        static constexpr auto name = u8"Hardware";
    };

    /// <summary>
    /// Identifies a block containing information about the RAM.
    /// </summary>
    struct memory final {
        typedef property_set type;
        static constexpr auto name = u8"Memory";
    };

    /// <summary>
    /// Identifies a block containing information about the operating system.
    /// </summary>
    struct operating_system final {
        typedef property_set type;
        static constexpr auto name = u8"Operating System";
    };

    /// <summary>
    /// Identifies the information that can be obtained from SMBIOS.
    /// </summary>
    struct smbios final {
        typedef property_set type;
        static constexpr auto name = u8"SMBIOS";
    };

    /// <summary>
    /// Identifies a property holding the timestamp when the information was
    /// collected.
    /// </summary>
    struct timestamp final {
        typedef LYRA_NAMESPACE::timestamp::value_type type;
        static constexpr auto is_variable = true;
        static constexpr auto name = u8"Timestamp";
    };

    /// <summary>
    /// Retrieves all raw, unprocessed property sets without filtering out any
    /// duplicate information that can be obtained in multiple ways.
    /// </summary>
    /// <param name="flags"></param>
    /// <returns></returns>
    property_set LYRA_API get(_In_ const collection_flags flags
        = collection_flags::none);

} /* namespace raw */

LYRA_NAMESPACE_END

#endif /* !defined(_LYRA_RAW_H) */
