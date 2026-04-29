// <copyright file="cpu_info.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2025 - 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_CPU_INFO_H)
#define _LYRA_CPU_INFO_H
#pragma once

#include <cinttypes>
#include <cstdlib>

#include "visus/lyra/api.h"


LYRA_NAMESPACE_BEGIN

/// <summary>
/// Represents the data that are received from the x86 CPUID instruction.
/// </summary>
/// <remarks>
/// <para>The values retrieved from the registers depend on the index queried
/// from the CPU. Our implementation retrieves these in order, i.e. the first
/// element is at index 0 and holds the vendor string, the one at index 1 holds
/// the model information and so on.</para>
/// </remarks>
union cpu_info {
    struct {
        /// <summary>
        /// The value of the EAX register.
        /// </summary>
        std::uint32_t eax;

        /// <summary>
        /// The value of the EBX register.
        /// </summary>
        std::uint32_t ebx;

        /// <summary>
        /// The value of the ECX register.
        /// </summary>
        std::uint32_t ecx;

        /// <summary>
        /// The value of the EDX register.
        /// </summary>
        std::uint32_t edx;
    } registers;

    /// <summary>
    /// A contiguous view on the registers.
    /// </summary>
    std::int32_t values[4];
};


namespace detail {

    /// <summary>
    /// Retrieves all available information for the current x86 CPU.
    /// </summary>
    /// <param name="dst"></param>
    /// <param name="cnt"></param>
    /// <param name="base">Controls whether the basic CPU information (0) or the
    /// extended information (0x80000000) are to be retrieved.</param>
    /// <returns>The number of <see cref="cpu_info" /> entries available.</returns>
    std::size_t LYRA_API get_cpu_info(
        _Out_writes_opt_(cnt) cpu_info *dst,
        _In_ std::size_t cnt,
        _In_ const std::size_t base = 0x0);

} /* namespace detail */


/// <summary>
/// Retrieves all available information for the current x86 CPU.
/// </summary>
/// <param name="dst">A buffer to receive the infos or <see cref="nullptr" /> to
/// measure the required buffer size.</param>
/// <param name="cnt">The number of elements that can be safely written to
/// <paramref name="dst" />.</param>
/// <returns>The number of <see cref="cpu_info" /> entries available.</returns>
std::size_t LYRA_API get_cpu_info(
    _Out_writes_opt_(cnt) cpu_info *dst = nullptr,
    _In_ const std::size_t cnt = 0);


/// <summary>
/// Gets the specified CPU info by index.
/// </summary>
/// <remarks>
/// The function will determine whether the function index is an extended one on
/// its own and select the appropriate base for the query.
/// </remarks>
/// <param name="index"></param>
/// <returns></returns>
bool LYRA_API get_cpu_info(_Out_ cpu_info& info, _In_ const std::uint32_t idx);


/// <summary>
/// Retrieves all available extended information for the current x86 CPU.
/// </summary>
/// <param name="dst">A buffer to receive the infos or <see cref="nullptr" /> to
/// measure the required buffer size.</param>
/// <param name="cnt">The number of elements that can be safely written to
/// <paramref name="dst" />.</param>
/// <returns>The number of <see cref="cpu_info" /> entries available.</returns>
std::size_t LYRA_API get_extended_cpu_info(
    _Out_writes_opt_(cnt) cpu_info *dst = nullptr,
    _In_ const std::size_t cnt = 0);

LYRA_NAMESPACE_END

#endif /* !defined(_LYRA_CPU_INFO_H) */
