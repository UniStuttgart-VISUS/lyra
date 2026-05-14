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

    // https://en.wikipedia.org/wiki/CPUID
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
    /// Checks whether the DS area uses a 64-bit layout.
    /// </summary>
    typedef cpu_info_detector<0x00000001, cpu_info_register::ecx,
        detail::cpu_info_bit(2)> dtes64;

    /// <summary>
    /// Checks whether MONITOR/MWAIT instructions are supported.
    /// </summary>
    typedef cpu_info_detector<0x00000001, cpu_info_register::ecx,
        detail::cpu_info_bit(3)> monitor;

    /// <summary>
    /// Checks whether CPL qualified debug store is supported.
    /// </summary>
    typedef cpu_info_detector<0x00000001, cpu_info_register::ecx,
        detail::cpu_info_bit(4)> ds_cpl;

    /// <summary>
    /// Checks whether Virtual Machine Extensions (VMX) are supported.
    /// </summary>
    typedef cpu_info_detector<0x00000001, cpu_info_register::ecx,
        detail::cpu_info_bit(5)> vmx;

    /// <summary>
    /// Checks whether Safer Mode Extensions (SMX) are supported.
    /// </summary>
    typedef cpu_info_detector<0x00000001, cpu_info_register::ecx,
        detail::cpu_info_bit(6)> smx;

    /// <summary>
    /// Checks whether Enhanced Intel SpeedStep Technology (EIST) is supported.
    /// </summary>
    typedef cpu_info_detector<0x00000001, cpu_info_register::ecx,
        detail::cpu_info_bit(7)> est;

    /// <summary>
    /// Checks whether Thermal Monitor 2 (TM2) is supported.
    /// </summary>
    typedef cpu_info_detector<0x00000001, cpu_info_register::ecx,
        detail::cpu_info_bit(8)> tm2;

    /// <summary>
    /// Checks whether Supplemental SSE3 (SSSE3) is supported.
    /// </summary>
    typedef simd_detector<simd_instruction_set::ssse3> ssse3;

    /// <summary>
    /// Checks the L1 context ID.
    /// </summary>
    typedef cpu_info_detector<0x00000001, cpu_info_register::ecx,
        detail::cpu_info_bit(10)> cnxt_id;

    /// <summary>
    /// Checks whether the Silicon Debug interface is supported.
    /// </summary>
    typedef cpu_info_detector<0x00000001, cpu_info_register::ecx,
        detail::cpu_info_bit(11)> sdbg;

    /// <summary>
    /// Checks whether fused-multiply-add (FMA) instructions are supported.
    /// </summary>
    typedef cpu_info_detector<0x00000001, cpu_info_register::ecx,
        detail::cpu_info_bit(12)> fma;

    /// <summary>
    /// Checks whether the CMPXCHG16B instruction is supported.
    /// </summary>
    typedef cpu_info_detector<0x00000001, cpu_info_register::ecx,
        detail::cpu_info_bit(13)> cx16;

    /// <summary>
    /// Checks whether sending task priority messages can be disabled.
    /// </summary>
    typedef cpu_info_detector<0x00000001, cpu_info_register::ecx,
        detail::cpu_info_bit(14)> xtpr;

    /// <summary>
    /// Checks whether Performance and Debug Capability is supported.
    /// </summary>
    typedef cpu_info_detector<0x00000001, cpu_info_register::ecx,
        detail::cpu_info_bit(15)> pdcm;

    /// <summary>
    /// Checks whether process context identifiers (PCID) are supported.
    /// </summary>
    typedef cpu_info_detector<0x00000001, cpu_info_register::ecx,
        detail::cpu_info_bit(17)> pcid;

    /// <summary>
    /// Checks whether direct cache access (DCA) for DMA writes is supported.
    /// </summary>
    typedef cpu_info_detector<0x00000001, cpu_info_register::ecx,
        detail::cpu_info_bit(18)> dca;

    /// <summary>
    /// Checks whether SSE 4.1 is supported.
    /// </summary>
    typedef simd_detector<simd_instruction_set::sse4_1> sse4_1;

    /// <summary>
    /// Checks whether SSE 4.2 is supported.
    /// </summary>
    typedef simd_detector<simd_instruction_set::sse4_2> sse4_2;

    /// <summary>
    /// Checks whether enhanced APIC is supported.
    /// </summary>
    typedef cpu_info_detector<0x00000001, cpu_info_register::ecx,
        detail::cpu_info_bit(21)> x2apic;

    /// <summary>
    /// Checks whether the MOVBE instruction is supported.
    /// </summary>
    typedef cpu_info_detector<0x00000001, cpu_info_register::ecx,
        detail::cpu_info_bit(22)> movbe;

    /// <summary>
    /// Checks whether the POPCNT instruction is supported.
    /// </summary>
    typedef cpu_info_detector<0x00000001, cpu_info_register::ecx,
        detail::cpu_info_bit(23)> popcnt;

    /// <summary>
    /// Checks whether APIC implements one-shot operation using a TSC deadline.
    /// </summary>
    typedef cpu_info_detector<0x00000001, cpu_info_register::ecx,
        detail::cpu_info_bit(24)> tsc_deadline;

    /// <summary>
    /// Checks whether the AES instruction set is supported.
    /// </summary>
    typedef cpu_info_detector<0x00000001, cpu_info_register::ecx,
        detail::cpu_info_bit(25)> aes;

    /// <summary>
    /// Checks whether extensible processor save/restore instructions XSAVE,
    /// XRSTOR, XSETBV and XGETBV are supported.
    /// </summary>
    typedef cpu_info_detector<0x00000001, cpu_info_register::ecx,
        detail::cpu_info_bit(26)> xsave;

    /// <summary>
    /// Checks whether XSAVE is enabled by the operating system.
    /// </summary>
    typedef cpu_info_detector<0x00000001, cpu_info_register::ecx,
        detail::cpu_info_bit(27)> osxsave;

    /// <summary>
    /// Checks whether AVX (256-bit SIMD) instructions are supported.
    /// </summary>
    typedef simd_detector<simd_instruction_set::avx> avx;

    /// <summary>
    /// Checks whether conversion from and to FP16 (half-precision floating
    /// point numbers) is supported.
    /// </summary>
    typedef cpu_info_detector<0x00000001, cpu_info_register::ecx,
        detail::cpu_info_bit(29)> f16c;

    /// <summary>
    /// Checks whether the on-chip RNG operation RDRAND is supported.
    /// </summary>
    typedef cpu_info_detector<0x00000001, cpu_info_register::ecx,
        detail::cpu_info_bit(30)> rdrnd;

    /// <summary>
    /// Indicates whether a hypervisor is present (zero for physical CPUs).
    /// </summary>
    typedef cpu_info_detector<0x00000001, cpu_info_register::ecx,
        detail::cpu_info_bit(31)> hypervisor;

    /// <summary>
    /// Indicates whether an on-board x87 FPU is supported.
    /// </summary>
    typedef cpu_info_detector<0x00000001, cpu_info_register::edx,
        detail::cpu_info_bit(0)> fpu;

    /// <summary>
    /// Indicates whether virtual 8086 mode extensions are supported.
    /// </summary>
    typedef cpu_info_detector<0x00000001, cpu_info_register::edx,
        detail::cpu_info_bit(1)> vme;

    /// <summary>
    /// Indicates whether debugging extensions are present.
    /// </summary>
    typedef cpu_info_detector<0x00000001, cpu_info_register::edx,
        detail::cpu_info_bit(2)> de;

    /// <summary>
    /// Indicates whether pages larger than 4MB are supported.
    /// </summary>
    typedef cpu_info_detector<0x00000001, cpu_info_register::edx,
        detail::cpu_info_bit(3)> pse;

    /// <summary>
    /// Indicates whether the time stamp counter and the RDSTC instruction are
    /// supported.
    /// </summary>
    typedef cpu_info_detector<0x00000001, cpu_info_register::edx,
        detail::cpu_info_bit(4)> tsc;

    /// <summary>
    /// Indicates whether RDMSR and WRMSR instructions are supported.
    /// </summary>
    typedef cpu_info_detector<0x00000001, cpu_info_register::edx,
        detail::cpu_info_bit(5)> msr;

    /// <summary>
    /// Indicates whether physical address extension (PAE) is supported.
    /// </summary>
    typedef cpu_info_detector<0x00000001, cpu_info_register::edx,
        detail::cpu_info_bit(6)> pae;

    /// <summary>
    /// Indicates whether machine check exception (MCE) is supported.
    /// </summary>
    typedef cpu_info_detector<0x00000001, cpu_info_register::edx,
        detail::cpu_info_bit(7)> mce;

    /// <summary>
    /// Indicates the CMPXCHG8B instruction is supported.
    /// </summary>
    typedef cpu_info_detector<0x00000001, cpu_info_register::edx,
        detail::cpu_info_bit(8)> cx8;

    /// <summary>
    /// Indicates whether an APIC is present.
    /// </summary>
    typedef cpu_info_detector<0x00000001, cpu_info_register::edx,
        detail::cpu_info_bit(9)> apic;

    /// <summary>
    /// Indicates whether the SYSENTER and SYSEXIT instructions are supported.
    /// </summary>
    typedef cpu_info_detector<0x00000001, cpu_info_register::edx,
        detail::cpu_info_bit(11)> sep;

    /// <summary>
    /// Indicates whether memory type range registers (MTRR) are present.
    /// </summary>
    typedef cpu_info_detector<0x00000001, cpu_info_register::edx,
        detail::cpu_info_bit(12)> mtrr;

    /// <summary>
    /// Indicates whether the Page Global Enable (PGE) feature is supported.
    /// </summary>
    typedef cpu_info_detector<0x00000001, cpu_info_register::edx,
        detail::cpu_info_bit(13)> pge;

    /// <summary>
    /// Indicates whether the Machine Check Architecture (MCA) is supported.
    /// </summary>
    typedef cpu_info_detector<0x00000001, cpu_info_register::edx,
        detail::cpu_info_bit(14)> mca;

    /// <summary>
    /// Indicates whether the conditional move instructions CMOV, FCMOV and
    /// FCOMI are supported.
    /// </summary>
    typedef cpu_info_detector<0x00000001, cpu_info_register::edx,
        detail::cpu_info_bit(15)> cmov;

    /// <summary>
    /// Indicates whether the page attribute table (PAT) is supported.
    /// </summary>
    typedef cpu_info_detector<0x00000001, cpu_info_register::edx,
        detail::cpu_info_bit(16)> pat;

    /// <summary>
    /// Indicates whether 36-bit page size extensions (PSE-36) are supported.
    /// </summary>
    typedef cpu_info_detector<0x00000001, cpu_info_register::edx,
        detail::cpu_info_bit(17)> pse36;

    /// <summary>
    /// Indicates whether processor serial number (PSN) is supported and
    /// enabled.
    /// </summary>
    typedef cpu_info_detector<0x00000001, cpu_info_register::edx,
        detail::cpu_info_bit(18)> psn;

    /// <summary>
    /// Indicates whether CLFLUSH instruction is supported.
    /// </summary>
    typedef cpu_info_detector<0x00000001, cpu_info_register::edx,
        detail::cpu_info_bit(19)> clfsh;

    /// <summary>
    /// Indicates whether the no-execute bit on Itanium is supported.
    /// </summary>
    typedef cpu_info_detector<0x00000001, cpu_info_register::edx,
        detail::cpu_info_bit(20)> nx;

    /// <summary>
    /// Indicates whether debug store is supported.
    /// </summary>
    typedef cpu_info_detector<0x00000001, cpu_info_register::edx,
        detail::cpu_info_bit(21)> ds;

    /// <summary>
    /// Indicates whether on-board thermal control MSRs for ACPI are supported.
    /// </summary>
    typedef cpu_info_detector<0x00000001, cpu_info_register::edx,
        detail::cpu_info_bit(22)> acpi;

    /// <summary>
    /// Indicates whether the MMX instruction set is supported.
    /// </summary>
    typedef simd_detector<simd_instruction_set::mmx> mmx;

    /// <summary>
    /// Indicates whether the FXSAVE and FXSTOR instructions are supported.
    /// </summary>
    typedef cpu_info_detector<0x00000001, cpu_info_register::edx,
        detail::cpu_info_bit(24)> fxsr;

    /// <summary>
    /// Indicates whether the SSE instruction set is supported.
    /// </summary>
    typedef simd_detector<simd_instruction_set::sse> sse;

    /// <summary>
    /// Indicates whether the SSE2 instruction set is supported.
    /// </summary>
    typedef simd_detector<simd_instruction_set::sse2> sse2;

    /// <summary>
    /// Indicates whether the CPU cache implements self-snoop.
    /// </summary>
    typedef cpu_info_detector<0x00000001, cpu_info_register::edx,
        detail::cpu_info_bit(27)> ss;

    /// <summary>
    /// Indicates whether the maximum APIC IDs reserved field is valid.
    /// </summary>
    typedef cpu_info_detector<0x00000001, cpu_info_register::edx,
        detail::cpu_info_bit(28)> htt;

    /// <summary>
    /// Indicates whether thermal monitor automatically limits temperature.
    /// </summary>
    typedef cpu_info_detector<0x00000001, cpu_info_register::edx,
        detail::cpu_info_bit(29)> tm;

    /// <summary>
    /// Indicates whether the CPU is an Itanium processor emulating x86.
    /// </summary>
    typedef cpu_info_detector<0x00000001, cpu_info_register::edx,
        detail::cpu_info_bit(30)> ia64;

    /// <summary>
    /// Indicates whether the pending break enable (PBE) feature is supported.
    /// </summary>
    typedef cpu_info_detector<0x00000001, cpu_info_register::edx,
        detail::cpu_info_bit(31)> pbe;

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
