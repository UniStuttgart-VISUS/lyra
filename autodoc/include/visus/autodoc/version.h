// <copyright file="version.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_VERSION_H)
#define _LYRA_VERSION_H
#pragma once

#include "visus/autodoc/property_set.h"


LYRA_NAMESPACE_BEGIN

namespace version {

    /// <summary>
    /// Identifies the major version component.
    /// </summary>
    struct major final {
        typedef std::uint32_t type;
        static constexpr auto name = u8"Major";
    };

    /// <summary>
    /// Identifies the minor version component.
    /// </summary>
    struct minor final {
        typedef std::uint32_t type;
        static constexpr auto name = u8"Minor";
    };

    /// <summary>
    /// Identifies the patch level.
    /// </summary>
    struct patch final {
        typedef std::uint32_t type;
        static constexpr auto name = u8"Patch";
    };

    /// <summary>
    /// Identifies whether the version is a pre-release version.
    /// </summary>
    struct prerelease final {
        typedef const char *type;
        static constexpr auto name = u8"PreRelease";
    };

    /// <summary>
    /// Identifies the version as a whole, which is a <see cref="property_set" />
    /// containing the individual version components as properties.
    /// </summary>
    struct version final {
        typedef property_set type;
        static constexpr auto name = u8"Version";
    };

    /// <summary>
    /// Constructs a new property set representing the specified version.
    /// </summary>
    /// <param name="major">The major version component.</param>
    /// <param name="minor">The minor version component.</param>
    /// <param name="patch">The patch level.</param>
    /// <param name="prerelease">The pre-release identifier. This is optional
    /// and will be skipped if <see langword="nullptr" /> is passed.</param>
    /// <returns>A property set representing the specified version.</returns>
    LYRA_API property_set make(
        _In_ const std::uint32_t major,
        _In_ const std::uint32_t minor,
        _In_ const std::uint32_t patch,
        _In_opt_z_ const char *prerelease = nullptr);

    /// <summary>
    /// Constructs a new property set representing the specified version.
    /// </summary>
    /// <param name="major">The major version component.</param>
    /// <param name="minor">The minor version component.</param>
    /// <returns>A property set representing the specified version.</returns>
    LYRA_API property_set make(_In_ const std::uint32_t major,
        _In_ const std::uint32_t minor);

} /* namespace version */

LYRA_NAMESPACE_END

#endif /* !defined(_LYRA_VERSION_H) */
