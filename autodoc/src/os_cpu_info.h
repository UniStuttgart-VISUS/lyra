// <copyright file="os_cpu_info.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2025 - 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_OS_CPU_INFO_H)
#define _LYRA_OS_CPU_INFO_H
#pragma once

#include <cinttypes>
#include <cstdlib>
#include <vector>

#include "visus/autodoc/api.h"


LYRA_DETAIL_NAMESPACE_BEGIN

#if defined(_WIN32) && (_WIN32_WINNT >= 0x0601)
/// <summary>
/// Gets the logical processor information for the specified relationship type.
/// </summary>
/// <param name="buffer">A buffer to store the information. This buffer will be
/// resized if it is too small to hold the information.</param>
/// <param name="relationship">The relationship type for which to retrieve the
/// information.</param>
/// <returns>A pointer to the logical processor information, or
/// <see langword="nullptr" /> in case of an error. The pointer is only valid as
/// long as <see cref="buffer" /> is not modified.</returns>
SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX *get_logical_processor_info(
    _Out_ std::vector<std::uint8_t>& buffer,
    _In_ const LOGICAL_PROCESSOR_RELATIONSHIP relationship);
#endif /* defined(_WIN32) && (_WIN32_WINNT >= 0x0601) */

/// <summary>
/// Gets the maximum number of CPUs that the operating system can use.
/// </summary>
/// <returns>The maximum number of CPUs that the operating system can use at the
/// time of the call.</returns>
std::size_t get_os_max_cpus(void);

LYRA_DETAIL_NAMESPACE_END

#endif /* !defined(_LYRA_OS_CPU_INFO_H) */
