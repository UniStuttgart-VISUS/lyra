// <copyright file="hardware.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_HARDWARE_H)
#define _LYRA_HARDWARE_H
#pragma once

#include "visus/lyra/collection_flags.h"
#include "visus/lyra/property_set.h"


LYRA_NAMESPACE_BEGIN

namespace hardware {

    /// <summary>
    /// Identifies a property holding a description of the device.
    /// </summary>
    struct description final {
        typedef const char *type;
        static constexpr auto name = u8"Description";
    };

    /// <summary>
    /// Identifies a property holding the class of a device.
    /// </summary>
    struct device_class final {
        typedef const char *type;
        static constexpr auto name = u8"Class";
    };

    /// <summary>
    /// Identifies a property holding the human-readable name of a device.
    /// </summary>
    struct friendly_name final {
        typedef const char *type;
        static constexpr auto name = u8"Friendly Name";
    };

    /// <summary>
    /// Identifies a property holding the hardware identifier of a device.
    /// </summary>
    struct hardware_id final {
        typedef const char *type;
        static constexpr auto name = u8"Hardware ID";
    };

    /// <summary>
    /// Identifies a property holding the location of a device.
    /// </summary>
    struct location final {
        typedef const char *type;
        static constexpr auto name = u8"Location";
    };

    /// <summary>
    /// Identifies a property holding the name of the manufacturer.
    /// </summary>
    struct manufacturer final {
        typedef const char *type;
        static constexpr auto name = u8"Manufacturer";
    };

    /// <summary>
    /// Gets all all devices we can identify on the current system.
    /// </summary>
    /// <param name="flags">Allows for customising the collection
    /// behaviour.</param>
    /// <returns>The hardware properties of the calling process.</returns>
    LYRA_API property_set get(_In_ const collection_flags flags
        = collection_flags::none);

} /* namespace hardware */

LYRA_NAMESPACE_END

#endif /* !defined(_LYRA_HARDWARE_H) */
