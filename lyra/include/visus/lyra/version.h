// <copyright file="version.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_VERSION_H)
#define _LYRA_VERSION_H
#pragma once

#include "visus/lyra/property_set.h"


LYRA_NAMESPACE_BEGIN

namespace version {

    /// <summary>
    /// Identifies the major version component.
    /// </summary>
    struct major final {
        typedef std::uint32_t type;
        static constexpr auto name = u8"major";
    };

    /// <summary>
    /// Identifies the minor version component.
    /// </summary>
    struct minor final {
        typedef std::uint32_t type;
        static constexpr auto name = u8"minor";
    };

    /// <summary>
    /// Identifies the patch level.
    /// </summary>
    struct patch final {
        typedef std::uint32_t type;
        static constexpr auto name = u8"patch";
    };

    /// <summary>
    /// Identifies whether the version is a pre-release version.
    /// </summary>
    struct prerelease final {
        typedef std::string type;
        static constexpr auto name = u8"prerelease";
    };

} /* namespace version */

LYRA_NAMESPACE_END

#endif /* !defined(_LYRA_VERSION_H) */
