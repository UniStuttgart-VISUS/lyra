// <copyright file="tdr_level.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2016 - 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_TDR_LEVEL_H)
#define _LYRA_TDR_LEVEL_H
#pragma once

#include <cinttypes>
#include <limits>

#include "visus/autodoc/api.h"


LYRA_NAMESPACE_BEGIN

#if defined(_WIN32)
namespace detail { typedef DWORD tdr_level_t; }
#else /* defined(_WIN32) */
namespace detail { typedef std::uint32_t tdr_level_t; }
#endif /* defined(_WIN32) */


/// <summary>
/// Lists possible GPU timeout detection and recovery levels.
/// </summary>
enum class tdr_level : detail::tdr_level_t {

    /// <summary>
    /// The state of timeout detection is unknown.
    /// </summary>
    unknown = (std::numeric_limits<detail::tdr_level_t>::max)(),

    /// <summary>
    /// Timeout detection is disabled.
    /// </summary>
    off = static_cast<detail::tdr_level_t>(0),

    /// <summary>
    /// The system bug checks (bluescreens) on timeout.
    /// </summary>
    bug_check = static_cast<detail::tdr_level_t>(1),

    /// <summary>
    /// Recover to VGA on timeout.
    /// </summary>
    recover_vga = static_cast<detail::tdr_level_t>(2),

    /// <summary>
    /// Fully recover on timeout.
    /// </summary>
    recover = static_cast<detail::tdr_level_t>(3),

    /// <summary>
    /// The default level according to
    /// https://docs.microsoft.com/en-us/windows-hardware/drivers/display/tdr-registry-keys
    /// </summary>
    default_behaviour = recover
};

LYRA_NAMESPACE_END

#endif /* !defined(_LYRA_TDR_LEVEL_H) */
