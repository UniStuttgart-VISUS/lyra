// <copyright file="hardware.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_HARDWARE_H)
#define _LYRA_HARDWARE_H
#pragma once

#include "visus/autodoc/collection_flags.h"
#include "visus/autodoc/property_set.h"
#include "visus/autodoc/timestamp.h"


LYRA_NAMESPACE_BEGIN

namespace hardware {

    /// <summary>
    /// Identifies a property holding a date, most importantly the date of the
    /// driver.
    /// </summary>
    struct date final {
        typedef timestamp::value_type type;
        static constexpr auto name = u8"Date";
    };

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
    /// Identifies a property holding the device type.
    /// </summary>
    struct device_type final {
        typedef std::uint32_t type;
        static constexpr auto name = u8"Device Type";
    };

    /// <summary>
    /// Identifies a property holding information about the device driver.
    /// </summary>
    struct driver final {
        typedef property_set type;
        static constexpr auto name = u8"Driver";
    };

    /// <summary>
    /// Identifies a property holding the driver type.
    /// </summary>
    struct driver_type final {
        typedef std::uint32_t type;
        static constexpr auto name = u8"Driver Type";
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
    /// Identifies a property holding the name of the driver provider.
    /// </summary>
    struct provider final {
        typedef const char *type;
        static constexpr auto name = u8"Provider";
    };

    /// <summary>
    /// Identifies a property holding the name of the manufacturer.
    /// </summary>
    struct manufacturer final {
        typedef const char *type;
        static constexpr auto name = u8"Manufacturer";
    };

    /// <summary>
    /// Identifies a property holding a version string, most importantly the
    /// version of the driver.
    /// </summary>
    struct version final {
        typedef const char *type;
        static constexpr auto name = u8"Version";
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
