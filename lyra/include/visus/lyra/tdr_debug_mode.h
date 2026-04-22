// <copyright file="tdr_debug_mode.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2016 - 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_TDR_DEBUG_MODE_H)
#define _LYRA_TDR_DEBUG_MODE_H
#pragma once

#include <cinttypes>
#include <limits>

#include "visus/lyra/api.h"


LYRA_NAMESPACE_BEGIN

#if defined(_WIN32)
namespace detail { typedef DWORD tdr_debug_mode_t; }
#else /* defined(_WIN32) */
namespace detail { typedef std::uint32_t tdr_debug_mode_t; }
#endif /* defined(_WIN32) */


/// <summary>
/// Lists debugging-related behaviours of the TDR process.
/// </summary>
enum class tdr_debug_mode : detail::tdr_debug_mode_t {

    /// <summary>
    /// The debugging-related behaviour is unknown.
    /// </summary>
    unknown = (std::numeric_limits<detail::tdr_debug_mode_t>::max)(),

    /// <summary>
    /// Break to kernel debugger before the recovery to allow investigation
    /// of the timeout.
    /// </summary>
    off = static_cast<detail::tdr_debug_mode_t>(0),

    /// <summary>
    /// Ignore any timeout.
    /// </summary>
    ignore_timeout = static_cast<detail::tdr_debug_mode_t>(1),

    /// <summary>
    /// Recover without breaking into the debugger.
    /// </summary>
    recover_no_prompt = static_cast<detail::tdr_debug_mode_t>(2),

    /// <summary>
    /// Recover even if some recovery conditions are not met (eg recover on
    /// consecutive timeouts).
    /// </summary>
    recover_unconditional = static_cast<detail::tdr_debug_mode_t>(3),

    /// <summary>
    /// The default behaviour according to
    /// https://docs.microsoft.com/en-us/windows-hardware/drivers/display/tdr-registry-keys
    /// </summary>
    default_behaviour = recover_no_prompt
};

LYRA_NAMESPACE_END

#endif /* !defined(_LYRA_TDR_DEBUG_MODE_H) */
