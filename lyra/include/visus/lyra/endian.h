// <copyright file="endian.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2024 - 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_ENDIAN_H)
#define _LYRA_ENDIAN_H
#pragma once

#include <cinttypes>

#include "visus/lyra/api.h"


LYRA_NAMESPACE_BEGIN

/// <summary>
/// Possible endian orders of CPUs.
/// </summary>
enum class endian : std::uint32_t {

    /// <summary>
    /// Little endian representation with least significant byte coming
    /// first in memory.
    /// </summary>
    little = 0x03020100,

    /// <summary>
    /// Big endian representation with most significant byte coming first in
    /// memory.
    /// </summary>
    big = 0x00010203,

    /// <summary>
    /// Endian of the system the code is running on.
    /// </summary>
    system = '\0\1\2\3'
};

LYRA_NAMESPACE_END

#endif /* !defined(_LYRA_ENDIAN_H) */
