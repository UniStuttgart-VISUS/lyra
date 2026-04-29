// <copyright file="cpu_info_detector.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2025 - 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_CPU_INFO_DETECTOR_H)
#define _LYRA_CPU_INFO_DETECTOR_H
#pragma once

#include <vector>

#include "visus/lyra/cpu_info.h"


LYRA_DETAIL_NAMESPACE_BEGIN

/// <summary>
/// Creates a mask for the <paramref name="bit" />th bit.
/// </summary>
/// <param name="bit">The zero-based index of the bit to mask.</param>
/// <returns>The mask for the specified bit.</returns>
inline constexpr std::uint32_t cpu_info_bit(
        _In_ const std::uint32_t bit) noexcept {
    return static_cast<std::size_t>(1) << bit;
}

/// <summary>
/// Creates a mask for selecting the bits in the range
/// [<paramref name="first" />, <paramref name="last" />].
/// </summary>
/// <param name="first">The zero-based index of the first bit to select.</param>
/// <param name="last">The zero-based index of the last bit to select.</param>
/// <returns>The bitmask for the selected range of bits.</returns>
inline constexpr std::uint32_t cpu_info_range(
        _In_ const std::uint32_t first,
        _In_ const std::uint32_t last) noexcept {
    return (cpu_info_bit(last + 1) - 1) & ~((cpu_info_bit(first) - 1));
}

LYRA_DETAIL_NAMESPACE_END


LYRA_NAMESPACE_BEGIN

/// <summary>
/// Provides names for the indices of <see cref="cpu_info.values" /> to improve
/// readability of the <see cref="cpu_info_detector" /> and
/// <see cref="cpu_info_selector" />.
/// </summary>
enum cpu_info_register : std::size_t {
    /// <summary>
    /// Identifies the EAX register.
    /// </summary>
    eax = 0,

    /// <summary>
    /// Identifies the EBX register.
    /// </summary>
    ebx = 1,

    /// <summary>
    /// Identifies the ECX register.
    /// </summary>
    ecx = 2,

    /// <summary>
    /// Identifies the EDX register.
    /// </summary>
    edx = 3
};


/// <summary>
/// Base implementation for detecting the presence of bits in the CPU info
/// of x86 CPUs.
/// </summary>
/// <typeparam name="Fun">The zero-based index of the CPUID function to check.
/// If the function is above the threashold of the extended functions, this
/// will be handled implicitly. If this function is not supported on the
/// machine, the result will be <see langword="false" />.</typeparam>
/// <typeparam name="Reg">The zero-based index of the register to be checked,
/// which must be within [0, 4[.</typeparam>
/// <typeparam name="Mask">The bitmask to check.</typeparam>
template<std::size_t Fun, std::size_t Reg, std::uint32_t Mask>
class cpu_info_detector {
    static_assert(Reg < 4, "The register must be within [0, 4[.");

public:

    /// <summary>
    /// Initialises a new instance.
    /// </summary>
    cpu_info_detector(void);

    /// <summary>
    /// Answer whether the instruction identified by
    /// <typeparamref name="Fun" />, <typename="Reg" /> and
    /// <typeparamref name="Mask" /> is supported.
    /// </summary>
    /// <returns><see langword="true" /> if the bit identified by the template
    /// is set, <see langword="false" /> otherwise.</returns>
    inline operator bool(void) const noexcept {
        return this->_value;
    }

private:

    bool _value;
};


/// <summary>
/// Exracts the value of a specific bit range from the CPU info of x86 CPUs.
/// </summary>
/// <typeparam name="Fun">The zero-based index of the CPUID function to check.
/// If this function is not supported on the machine, the result will be
/// <see langword="false" />.</typeparam>
/// <typeparam name="Reg">The zero-based index of the register to be checked,
/// which must be within [0, 4[.</typeparam>
/// <typeparam name="Mask">The bitmask to extract.</typeparam>
template<std::size_t Fun, std::size_t Reg, std::uint32_t Mask>
class cpu_info_selector {
    static_assert(Reg < 4, "The register must be within [0, 4[.");

public:

    /// <summary>
    /// Initialises a new instance.
    /// </summary>
    cpu_info_selector(void);

    /// <summary>
    /// Answer the value selected by the <typeparamref name="Mask" /> applied
    /// on register <typename="Reg" /> when calling the CPUID instruction
    /// <typeparamref name="Fun" />.
    /// </summary>
    /// <returns>The value selected by the mask.</returns>
    inline operator std::uint32_t(void) const noexcept {
        return this->_value;
    }

private:

    std::uint32_t _value;
};


LYRA_NAMESPACE_END

#include "visus/lyra/cpu_info_detector.inl"

#endif /* !defined(_LYRA_CPU_INFO_DETECTOR_H) */
