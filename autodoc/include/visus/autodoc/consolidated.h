// <copyright file="consolidated.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_CONSOLIDATED_H)
#define _LYRA_CONSOLIDATED_H
#pragma once

#include "visus/autodoc/raw.h"


LYRA_NAMESPACE_BEGIN

namespace consolidated {

    /// <summary>
    /// Identifies the block of meta data about the calling application.
    /// </summary>
    typedef raw::application application;

    /// <summary>
    /// Identifies a block containing the environment.
    /// </summary>
    typedef raw::environment environment;

    /// <summary>
    /// Identifies a block containing the hardware.
    /// </summary>
    typedef raw::hardware hardware;

    /// <summary>
    /// Identifies a block containing information about network interfaces and
    /// network shares.
    /// </summary>
    typedef raw::network network;

    /// <summary>
    /// Identifies a block containing information about the operating system.
    /// </summary>
    typedef raw::operating_system operating_system;

    /// <summary>
    /// Identifies a block containing the information about a network share
    /// mounted on the system.
    /// </summary>
    struct share final {
        typedef property_set type;
        static constexpr auto name = u8"Share";
    };

    /// <summary>
    /// Identifies a property holding the timestamp when the information was
    /// collected.
    /// </summary>
    typedef raw::timestamp timestamp;

    /// <summary>
    /// Gets the consolidated system information, which is the same as the raw
    /// data after removing duplicates. This function tries to find the most
    /// data about the system it can, but removes the noise from the raw data.
    /// </summary>
    /// <param name="flags">Allows for customising the collection
    /// behaviour.</param>
    LYRA_API property_set get(_In_ const collection_flags flags
        = collection_flags::none);

} /* namespace consolidated */

LYRA_NAMESPACE_END

#endif /* !defined(_LYRA_CONSOLIDATED_H) */
