// <copyright file="cpu_features.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_CPU_FEATURES_H)
#define _LYRA_CPU_FEATURES_H
#pragma once

#include "visus/autodoc/simd_detector.h"


LYRA_NAMESPACE_BEGIN

namespace cpu_features {

    // https://blog.winny.tech/posts/cpuid/
    // https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Register/Amd/Cpuid.h

    /// <summary>
    /// Gets the stepping ID.
    /// </summary>
    typedef cpu_info_selector<0x00000001, cpu_info_register::eax, 0, 3> stepping_id;

    /// <summary>
    /// Gets the model number.
    /// </summary>
    typedef cpu_info_selector<0x00000001, cpu_info_register::eax, 4, 7> model;

    /// <summary>
    /// Gets the family ID.
    /// </summary>
    typedef cpu_info_selector<0x00000001, cpu_info_register::eax, 8, 11> family_id;

    /// <summary>
    /// Gets the processor type.
    /// </summary>
    typedef cpu_info_selector<0x00000001, cpu_info_register::eax, 12, 13>
        processor_type;

    /// <summary>
    /// Gets the extended model ID.
    /// </summary>
    typedef cpu_info_selector<0x00000001, cpu_info_register::eax, 16, 19>
        extended_model;

    /// <summary>
    /// Gets the extended family ID.
    /// </summary>
    typedef cpu_info_selector<0x00000001, cpu_info_register::eax, 20, 27>
        extended_family_id;

    /// <summary>
    /// Gets the brand index.
    /// </summary>
    typedef cpu_info_selector<0x00000001, cpu_info_register::ebx, 0, 7>
        brand;

    /// <summary>
    /// Gets the cache line flush size (value * 8 bytes).
    /// </summary>
    typedef cpu_info_selector<0x00000001, cpu_info_register::ebx, 8, 15>
        clflush_size;

    /// <summary>
    /// Gets the maximum addressable logical processor ID in the physical
    /// package.
    /// </summary>
    typedef cpu_info_selector<0x00000001, cpu_info_register::ebx, 16, 23>
        max_cpu_id;

    /// <summary>
    /// Gets the initial APIC ID of the processor.
    /// </summary>
    typedef cpu_info_selector<0x00000001, cpu_info_register::ebx, 24, 31>
        apic_id;

    /// <summary>
    /// Detect SSE3 support.
    /// </summary>
    typedef simd_detector<simd_instruction_set::sse3> sse3;

    /// <summary>
    /// Checks whether the PCLMULQDQ instruction is supported.
    /// </summary>
    typedef cpu_info_detector<0x00000001, cpu_info_register::ecx,
        detail::cpu_info_bit(1)> pclmulqdq;

    /// <summary>
    /// Checks whether the POPCNT instruction is supported via bit 23 of ECX in
    /// function 1. POPCNT allows for counting the number of set bits in a word.
    /// </summary>
    typedef cpu_info_detector<0x00000001, cpu_info_register::ecx,
        detail::cpu_info_bit(23)> popcnt;

    /// <summary>
    /// Checks whether the <see cref="_xgetbv" /> intrinsic is supported via bit
    /// 27 of ECX in function 1.
    /// </summary>
    typedef cpu_info_detector<0x00000001, cpu_info_register::ecx,
        detail::cpu_info_bit(27)> xgetbv;



    /// <summary>
    /// Checks whether the V2 extended topology enumeration leaf 0x1F is supported.
    /// </summary>
    typedef cpu_info_any<0x0000001F, cpu_info_register::ebx>
        extended_toplogy_enumeration;

    /// <summary>
    /// Test for extended APIC ID. See also
    /// https://docs.kernel.org/arch/x86/topology.html
    /// </summary>
    typedef cpu_info_detector<0x80000001, cpu_info_register::ecx,
        detail::cpu_info_bit(22)> topology_extensions;

    /// <summary>
    /// Answer whether the CPU supports leaf B of the CPUID instruction, which
    /// contains information about the topology of the CPU. See also
    /// https://github.com/tpn/cpuid-topo/blob/f9b24d25220965087c1d855f716c52d75bc7bbe0/cpu_topo.c#L1009-L1057
    /// </summary>
    typedef cpu_info_any<0x0000000b, cpu_info_register::ebx> topology_leaf_b;

} /* namespace cpu_features */

LYRA_NAMESPACE_END

#endif /* !defined(_LYRA_CPU_FEATURES_H) */
