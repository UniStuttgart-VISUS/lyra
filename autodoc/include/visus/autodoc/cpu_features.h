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
    struct stepping_id final : cpu_info_selector<
            0x00000001, cpu_info_register::eax, 0, 3> {
        typedef std::uint32_t type;
        static constexpr auto name = u8"Stepping";
    };

    /// <summary>
    /// Gets the model number.
    /// </summary>
    struct model final : cpu_info_selector<
            0x00000001, cpu_info_register::eax, 4, 7> {
        typedef std::uint32_t type;
        static constexpr auto name = u8"Model";
    };

    /// <summary>
    /// Gets the family ID.
    /// </summary>
    struct family_id final : cpu_info_selector<
            0x00000001, cpu_info_register::eax, 8, 11> {
        typedef std::uint32_t type;
        static constexpr auto name = u8"Family";
    };

    /// <summary>
    /// Gets the processor type.
    /// </summary>
    struct processor_type final : cpu_info_selector<
           0x00000001, cpu_info_register::eax, 12, 13> {
        typedef std::uint32_t type;
        static constexpr auto name = u8"Processor Type";
    };

    /// <summary>
    /// Gets the extended model ID.
    /// </summary>
    struct extended_model final : cpu_info_selector<
            0x00000001, cpu_info_register::eax, 16, 19> {
        typedef std::uint32_t type;
        static constexpr auto name = u8"Extended Model";
    };

    /// <summary>
    /// Gets the extended family ID.
    /// </summary>
    struct extended_family_id final : cpu_info_selector<
            0x00000001, cpu_info_register::eax, 20, 27> {
        typedef std::uint32_t type;
        static constexpr auto name = u8"Extended Family";
    };

    /// <summary>
    /// Gets the brand index.
    /// </summary>
    struct brand final : cpu_info_selector<
            0x00000001, cpu_info_register::ebx, 0, 7> {
        typedef std::uint32_t type;
        static constexpr auto name = u8"Brand ID";
    };

    /// <summary>
    /// Gets the cache line flush size (value * 8 bytes).
    /// </summary>
    struct clflush_size final : cpu_info_selector<
            0x00000001, cpu_info_register::ebx, 8, 15> {
        typedef std::uint32_t type;
        static constexpr auto name = u8"Cache Line Flush Size";
    };


    /// <summary>
    /// Gets the maximum addressable logical processor ID in the physical
    /// package.
    /// </summary>
    struct max_cpu_id final : cpu_info_selector<
            0x00000001, cpu_info_register::ebx, 16, 23> {
        typedef std::uint32_t type;
        static constexpr auto name = u8"Maximum CPUs per Package";
    };

    /// <summary>
    /// Gets the initial APIC ID of the processor.
    /// </summary>
    struct apic_id final : cpu_info_selector<
            0x00000001, cpu_info_register::ebx, 24, 31> {
        typedef std::uint32_t type;
        static constexpr auto name = u8"APIC ID";
    };

    /// <summary>
    /// Detect SSE3 support.
    /// </summary>
    struct sse3 final : simd_detector<simd_instruction_set::sse3> {
        typedef bool type;
        static constexpr auto name = u8"Streaming SIMD Extensions 3 (sse3)";
    };

    /// <summary>
    /// Checks whether the PCLMULQDQ instruction is supported.
    /// </summary>
    struct pclmulqdq final : cpu_info_detector<
            0x00000001, cpu_info_register::ecx, detail::cpu_info_bit(1)> {
        typedef bool type;
        static constexpr auto name = u8"PCLMULQDQ";
    };

    /// <summary>
    /// Checks whether the DS area uses a 64-bit layout.
    /// </summary>
    struct dtes64 final : cpu_info_detector<
            0x00000001, cpu_info_register::ecx, detail::cpu_info_bit(2)> {
        typedef bool type;
        static constexpr auto name = u8"64-bit Debug Store (dtes64)";
    };

    /// <summary>
    /// Checks whether MONITOR/MWAIT instructions are supported.
    /// </summary>
    struct monitor final : cpu_info_detector<
            0x00000001, cpu_info_register::ecx, detail::cpu_info_bit(3)> {
        typedef bool type;
        static constexpr auto name = u8"MONITOR and MWAIT (monitor)";
    };

    /// <summary>
    /// Checks whether CPL qualified debug store is supported.
    /// </summary>
    struct ds_cpl final : cpu_info_detector<
            0x00000001, cpu_info_register::ecx, detail::cpu_info_bit(4)> {
        typedef bool type;
        static constexpr auto name = u8"CPL-Qualified Debug Store (ds-cpl)";
    };

    /// <summary>
    /// Checks whether Virtual Machine Extensions (VMX) are supported.
    /// </summary>
    struct vmx final : cpu_info_detector<
            0x00000001, cpu_info_register::ecx, detail::cpu_info_bit(5)> {
        typedef bool type;
        static constexpr auto name = u8"Virtual Machine Extensions (vmx)";
    };

    /// <summary>
    /// Checks whether Safer Mode Extensions (SMX) are supported.
    /// </summary>
    struct smx final : cpu_info_detector<
            0x00000001, cpu_info_register::ecx, detail::cpu_info_bit(6)> {
        typedef bool type;
        static constexpr auto name = u8"Safer Mode Extensions (smx)";
    };

    /// <summary>
    /// Checks whether Enhanced Intel SpeedStep Technology (EIST) is supported.
    /// </summary>
    struct est final : cpu_info_detector<
            0x00000001, cpu_info_register::ecx, detail::cpu_info_bit(7)> {
        typedef bool type;
        static constexpr auto name = u8"Enhanced Intel SpeedStep Technology "
            u8"(est)";
    };

    /// <summary>
    /// Checks whether Thermal Monitor 2 (TM2) is supported.
    /// </summary>
    struct tm2 final : cpu_info_detector<
            0x00000001, cpu_info_register::ecx, detail::cpu_info_bit(8)> {
        typedef bool type;
        static constexpr auto name = u8"Thermal Monitor 2 (tm2)";
    };

    /// <summary>
    /// Checks whether Supplemental SSE3 (SSSE3) is supported.
    /// </summary>
    struct ssse3 final : simd_detector<simd_instruction_set::ssse3> {
        typedef bool type;
        static constexpr auto name = u8"Supplemental SIMD Extensions 3 (ssse3)";
    };

    /// <summary>
    /// Checks the L1 context ID.
    /// </summary>
    struct cnxt_id final : cpu_info_detector<
            0x00000001, cpu_info_register::ecx, detail::cpu_info_bit(10)> {
        typedef bool type;
        static constexpr auto name = u8"L1 context ID (cnxt-id)";
    };

    /// <summary>
    /// Checks whether the Silicon Debug interface is supported.
    /// </summary>
    struct sdbg final : cpu_info_detector<
            0x00000001, cpu_info_register::ecx, detail::cpu_info_bit(11)> {
        typedef bool type;
        static constexpr auto name = u8"Silicon Debug interface (sdbg)";
    };

    /// <summary>
    /// Checks whether fused-multiply-add (FMA) instructions are supported.
    /// </summary>
    struct fma final : cpu_info_detector<
            0x00000001, cpu_info_register::ecx, detail::cpu_info_bit(12)> {
        typedef bool type;
        static constexpr auto name = u8"Fused Multiply-Add (fma)";
    };

    /// <summary>
    /// Checks whether the CMPXCHG16B instruction is supported.
    /// </summary>
    struct cx16 final : cpu_info_detector<0x00000001, cpu_info_register::ecx,
        detail::cpu_info_bit(13)> {
        typedef bool type;
        static constexpr auto name = u8"CMPXCHG16B";
    };

    /// <summary>
    /// Checks whether sending task priority messages can be disabled.
    /// </summary>
    struct xtpr final : cpu_info_detector<
            0x00000001, cpu_info_register::ecx, detail::cpu_info_bit(14)> {
        typedef bool type;
        static constexpr auto name = u8"Task Priority Messages (xtpr)";
    };

    /// <summary>
    /// Checks whether Performance and Debug Capability is supported.
    /// </summary>
    struct pdcm final : cpu_info_detector<
            0x00000001, cpu_info_register::ecx, detail::cpu_info_bit(15)> {
        typedef bool type;
        static constexpr auto name = u8"Performance and Debug Capability "
            u8"(pdcm)";
    };

    /// <summary>
    /// Checks whether process context identifiers (PCID) are supported.
    /// </summary>
    struct pcid final : cpu_info_detector<
            0x00000001, cpu_info_register::ecx, detail::cpu_info_bit(17)> {
        typedef bool type;
        static constexpr auto name = u8"Process Context Identifiers (pcid)";
    };

    /// <summary>
    /// Checks whether direct cache access (DCA) for DMA writes is supported.
    /// </summary>
    struct dca final : cpu_info_detector<
            0x00000001, cpu_info_register::ecx, detail::cpu_info_bit(18)> {
        typedef bool type;
        static constexpr auto name = u8"Direct Cache Access (dca)";
    };

    /// <summary>
    /// Checks whether SSE 4.1 is supported.
    /// </summary>
    struct sse4_1 final : simd_detector<simd_instruction_set::sse4_1> {
        typedef bool type;
        static constexpr auto name = u8"Streaming SIMD Extensions 4.1 (sse4.1)";
    };

    /// <summary>
    /// Checks whether SSE 4.2 is supported.
    /// </summary>
    struct sse4_2 final : simd_detector<simd_instruction_set::sse4_2> {
        typedef bool type;
        static constexpr auto name = u8"Streaming SIMD Extensions 4.2 (sse4.2)";
    };

    /// <summary>
    /// Checks whether enhanced APIC is supported.
    /// </summary>
    struct x2apic final : cpu_info_detector<
        0x00000001, cpu_info_register::ecx, detail::cpu_info_bit(21)> {
        typedef bool type;
        static constexpr auto name = u8"Extended APIC (x2apic)";
    };

    /// <summary>
    /// Checks whether the MOVBE instruction is supported.
    /// </summary>
    struct movbe final : cpu_info_detector<
            0x00000001, cpu_info_register::ecx, detail::cpu_info_bit(22)> {
        typedef bool type;
        static constexpr auto name = u8"MOVBE";
    };

    /// <summary>
    /// Checks whether the POPCNT instruction is supported.
    /// </summary>
    struct popcnt final : cpu_info_detector<
            0x00000001, cpu_info_register::ecx,detail::cpu_info_bit(23)> {
        typedef bool type;
        static constexpr auto name = u8"POPCNT";
    };

    /// <summary>
    /// Checks whether APIC implements one-shot operation using a TSC deadline.
    /// </summary>
    struct tsc_deadline final : cpu_info_detector<
            0x00000001, cpu_info_register::ecx, detail::cpu_info_bit(24)> {
        typedef bool type;
        static constexpr auto name = u8"TSC Deadline (tsc-deadline)";
    };

    /// <summary>
    /// Checks whether the AES instruction set is supported.
    /// </summary>
    struct aes final : cpu_info_detector<
            0x00000001, cpu_info_register::ecx, detail::cpu_info_bit(25)> {
        typedef bool type;
        static constexpr auto name = u8"AES (aes-ni)";
    };

    /// <summary>
    /// Checks whether extensible processor save/restore instructions XSAVE,
    /// XRSTOR, XSETBV and XGETBV are supported.
    /// </summary>
    struct xsave final : cpu_info_detector<
            0x00000001, cpu_info_register::ecx, detail::cpu_info_bit(26)> {
        typedef bool type;
        static constexpr auto name = u8"Extensible Processor Save/Restore "
            u8"(xsave)";
    };

    /// <summary>
    /// Checks whether XSAVE is enabled by the operating system.
    /// </summary>
    struct osxsave final : cpu_info_detector<
            0x00000001, cpu_info_register::ecx, detail::cpu_info_bit(27)> {
        typedef bool type;
        static constexpr auto name = u8"Extensible Processor Save/Restore "
            u8"Enabled by Operating System (osxsave)";
    };

    /// <summary>
    /// Checks whether AVX (256-bit SIMD) instructions are supported.
    /// </summary>
    struct avx final : simd_detector<simd_instruction_set::avx> {
        typedef bool type;
        static constexpr auto name = u8"Advanced Vector Extensions (avx)";
    };

    /// <summary>
    /// Checks whether conversion from and to FP16 (half-precision floating
    /// point numbers) is supported.
    /// </summary>
    struct f16c final : cpu_info_detector<
            0x00000001, cpu_info_register::ecx, detail::cpu_info_bit(29)> {
        typedef bool type;
        static constexpr auto name = u8"FP16 Conversion (f16c)";
    };

    /// <summary>
    /// Checks whether the on-chip RNG operation RDRAND is supported.
    /// </summary>
    struct rdrnd final : cpu_info_detector<
            0x00000001, cpu_info_register::ecx, detail::cpu_info_bit(30)> {
        typedef bool type;
        static constexpr auto name = u8"On-board Random Number Generator "
            u8"(rdrnd)";
    };

    /// <summary>
    /// Indicates whether a hypervisor is present (zero for physical CPUs).
    /// </summary>
    struct hypervisor final : cpu_info_detector<
            0x00000001, cpu_info_register::ecx, detail::cpu_info_bit(31)> {
        typedef bool type;
        static constexpr auto name = u8"Hypervisor Present (hypervisor)";
    };

    /// <summary>
    /// Indicates whether an on-board x87 FPU is supported.
    /// </summary>
    struct fpu final : cpu_info_detector<
            0x00000001, cpu_info_register::edx, detail::cpu_info_bit(0)> {
        typedef bool type;
        static constexpr auto name = u8"On-board x87 Floating-Point Unit (fpu)";
    };

    /// <summary>
    /// Indicates whether virtual 8086 mode extensions are supported.
    /// </summary>
    struct vme final : cpu_info_detector<
            0x00000001, cpu_info_register::edx, detail::cpu_info_bit(1)> {
        typedef bool type;
        static constexpr auto name = u8"Virtual 8086 Mode Extensions (vme)";
    };

    /// <summary>
    /// Indicates whether debugging extensions are present.
    /// </summary>
    struct de final : cpu_info_detector<
            0x00000001, cpu_info_register::edx, detail::cpu_info_bit(2)> {
        typedef bool type;
        static constexpr auto name = u8"Debugging Extensions (de)";
    };

    /// <summary>
    /// Indicates whether pages larger than 4MB are supported.
    /// </summary>
    struct pse final : cpu_info_detector<
            0x00000001, cpu_info_register::edx, detail::cpu_info_bit(3)> {
        typedef bool type;
        static constexpr auto name = u8"Page Size Extension (pse)";
    };

    /// <summary>
    /// Indicates whether the time stamp counter and the RDSTC instruction are
    /// supported.
    /// </summary>
    struct tsc final : cpu_info_detector<
            0x00000001, cpu_info_register::edx, detail::cpu_info_bit(4)> {
        typedef bool type;
        static constexpr auto name = u8"Time Stamp Counter and RDSTC (tsc)";
    };

    /// <summary>
    /// Indicates whether RDMSR and WRMSR instructions are supported.
    /// </summary>
    struct msr final : cpu_info_detector<
            0x00000001, cpu_info_register::edx, detail::cpu_info_bit(5)> {
        typedef bool type;
        static constexpr auto name = u8"Model-Specific Registers (msr)";
    };

    /// <summary>
    /// Indicates whether physical address extension (PAE) is supported.
    /// </summary>
    struct pae final : cpu_info_detector<
            0x00000001, cpu_info_register::edx, detail::cpu_info_bit(6)> {
        typedef bool type;
        static constexpr auto name = u8"Physical Address Extension (pae)";
    };

    /// <summary>
    /// Indicates whether machine check exception (MCE) is supported.
    /// </summary>
    struct mce final : cpu_info_detector<
            0x00000001, cpu_info_register::edx, detail::cpu_info_bit(7)> {
        typedef bool type;
        static constexpr auto name = u8"Machine Check Exception (mce)";
    };

    /// <summary>
    /// Indicates the CMPXCHG8B instruction is supported.
    /// </summary>
    struct cx8 final : cpu_info_detector<
            0x00000001, cpu_info_register::edx, detail::cpu_info_bit(8)> {
        typedef bool type;
        static constexpr auto name = u8"CMPXCHG8B";
    };

    /// <summary>
    /// Indicates whether an APIC is present.
    /// </summary>
    struct apic final : cpu_info_detector<
            0x00000001, cpu_info_register::edx, detail::cpu_info_bit(9)> {
        typedef bool type;
        static constexpr auto name = u8"Advanced Programmable Interrupt "
            u8"Controller (apic)";
    };

    /// <summary>
    /// Indicates whether the SYSENTER and SYSEXIT instructions are supported.
    /// </summary>
    struct sep final : cpu_info_detector<
            0x00000001, cpu_info_register::edx, detail::cpu_info_bit(11)> {
        typedef bool type;
        static constexpr auto name = u8"Fast System Call Instructions (sep)";
    };

    /// <summary>
    /// Indicates whether memory type range registers (MTRR) are present.
    /// </summary>
    struct mtrr final : cpu_info_detector<
            0x00000001, cpu_info_register::edx, detail::cpu_info_bit(12)> {
        typedef bool type;
        static constexpr auto name = u8"Memory Type Range Registers (mtrr)";
    };

    /// <summary>
    /// Indicates whether the Page Global Enable (PGE) feature is supported.
    /// </summary>
    struct pge final : cpu_info_detector<
            0x00000001, cpu_info_register::edx, detail::cpu_info_bit(13)> {
        typedef bool type;
        static constexpr auto name = u8"Page Global Enable (pge)";
    };

    /// <summary>
    /// Indicates whether the Machine Check Architecture (MCA) is supported.
    /// </summary>
    struct mca final : cpu_info_detector<0x00000001, cpu_info_register::edx,
        detail::cpu_info_bit(14)> {
        typedef bool type;
        static constexpr auto name = u8"Machine Check Architecture (mca)";
    };

    /// <summary>
    /// Indicates whether the conditional move instructions CMOV, FCMOV and
    /// FCOMI are supported.
    /// </summary>
    struct cmov final : cpu_info_detector<
            0x00000001, cpu_info_register::edx, detail::cpu_info_bit(15)> {
        typedef bool type;
        static constexpr auto name = u8"Conditional Move Instructions (cmov)";
    };

    /// <summary>
    /// Indicates whether the page attribute table (PAT) is supported.
    /// </summary>
    struct pat final : cpu_info_detector<
            0x00000001, cpu_info_register::edx, detail::cpu_info_bit(16)> {
        typedef bool type;
        static constexpr auto name = u8"Page Attribute Table (pat)";
    };

    /// <summary>
    /// Indicates whether 36-bit page size extensions (PSE-36) are supported.
    /// </summary>
    struct pse36 final : cpu_info_detector<
            0x00000001, cpu_info_register::edx, detail::cpu_info_bit(17)> {
        typedef bool type;
        static constexpr auto name = u8"36-bit Page Size Extensions (pse-36)";
    };

    /// <summary>
    /// Indicates whether processor serial number (PSN) is supported and
    /// enabled.
    /// </summary>
    struct psn final : cpu_info_detector<
            0x00000001, cpu_info_register::edx, detail::cpu_info_bit(18)> {
        typedef bool type;
        static constexpr auto name = u8"Processor Serial Number (psn)";
    };

    /// <summary>
    /// Indicates whether CLFLUSH instruction is supported.
    /// </summary>
    struct clfsh final : cpu_info_detector<
            0x00000001, cpu_info_register::edx, detail::cpu_info_bit(19)> {
        typedef bool type;
        static constexpr auto name = u8"CLFLUSH";
    };

    /// <summary>
    /// Indicates whether the no-execute bit on Itanium is supported.
    /// </summary>
    struct nx final : cpu_info_detector<
            0x00000001, cpu_info_register::edx, detail::cpu_info_bit(20)> {
        typedef bool type;
        static constexpr auto name = u8"No-Execute Bit (nx)";
    };

    /// <summary>
    /// Indicates whether debug store is supported.
    /// </summary>
    struct ds final : cpu_info_detector<
            0x00000001, cpu_info_register::edx, detail::cpu_info_bit(21)> {
        typedef bool type;
        static constexpr auto name = u8"Debug Store (ds)";
    };

    /// <summary>
    /// Indicates whether on-board thermal control MSRs for ACPI are supported.
    /// </summary>
    struct acpi final : cpu_info_detector<
            0x00000001, cpu_info_register::edx, detail::cpu_info_bit(22)> {
        typedef bool type;
        static constexpr auto name = u8"On-Board Thermal Control MSRs for ACPI "
            u8"(acpi)";
    };

    /// <summary>
    /// Indicates whether the MMX instruction set is supported.
    /// </summary>
    struct mmx final : simd_detector<simd_instruction_set::mmx> {
        typedef bool type;
        static constexpr auto name = u8"MMX";
    };

    /// <summary>
    /// Indicates whether the FXSAVE and FXSTOR instructions are supported.
    /// </summary>
    struct fxsr final : cpu_info_detector<
            0x00000001, cpu_info_register::edx, detail::cpu_info_bit(24)> {
        typedef bool type;
        static constexpr auto name = u8"FXSAVE and FXSTOR (fxsr)";
    };

    /// <summary>
    /// Indicates whether the SSE instruction set is supported.
    /// </summary>
    struct sse final : simd_detector<simd_instruction_set::sse> {
        typedef bool type;
        static constexpr auto name = u8"Streaming SIMD Extensions (sse)";
    };

    /// <summary>
    /// Indicates whether the SSE2 instruction set is supported.
    /// </summary>
    struct sse2 final : simd_detector<simd_instruction_set::sse2> {
        typedef bool type;
        static constexpr auto name = u8"Streaming SIMD Extensions 2 (sse2)";
    };

    /// <summary>
    /// Indicates whether the CPU cache implements self-snoop.
    /// </summary>
    struct ss final : cpu_info_detector<
            0x00000001, cpu_info_register::edx, detail::cpu_info_bit(27)> {
        typedef bool type;
        static constexpr auto name = u8"CPU Cache implements self-snoop (ss)";
    };

    /// <summary>
    /// Indicates whether the maximum APIC IDs reserved field is valid.
    /// </summary>
    struct htt final : cpu_info_detector<
            0x00000001, cpu_info_register::edx, detail::cpu_info_bit(28)> {
        typedef bool type;
        static constexpr auto name = u8"Maximum APIC IDs Reserved Field (htt)";
    };

    /// <summary>
    /// Indicates whether thermal monitor automatically limits temperature.
    /// </summary>
    struct tm final : cpu_info_detector<
            0x00000001, cpu_info_register::edx, detail::cpu_info_bit(29)> {
        typedef bool type;
        static constexpr auto name = u8"Thermal Monitor (tm)";
    };

    /// <summary>
    /// Indicates whether the CPU is an Itanium processor emulating x86.
    /// </summary>
    struct ia64 final : cpu_info_detector<
            0x00000001, cpu_info_register::edx, detail::cpu_info_bit(30)> {
        typedef bool type;
        static constexpr auto name = u8"Itanium Processor Emulating x86 (ia64)";
    };

    /// <summary>
    /// Indicates whether the pending break enable (PBE) feature is supported.
    /// </summary>
    struct pbe final : cpu_info_detector<
            0x00000001, cpu_info_register::edx, detail::cpu_info_bit(31)> {
        typedef bool type;
        static constexpr auto name = u8"Pending Break Enable (pbe)";
    };

    ///// <summary>
    ///// Indicates whether a cache type is instruction (2), data (1) or
    ///// unified (3).
    ///// </summary>
    //struct cache_type final : cpu_info_selector<
    //        0x00000004, cpu_info_register::eax, 0, 4> {
    //    typedef std::uint32_t type;
    //    static constexpr auto name = u8"Cache Type";
    //};

    ///// <summary>
    ///// Gets the cache level starting with 1 for L1, 2 for L2, etc.
    ///// </summary>
    //struct cache_level final : cpu_info_selector<
    //        0x00000004, cpu_info_register::eax, 5, 7> {
    //    typedef std::uint32_t type;
    //    static constexpr auto name = u8"Cache Level";
    //};

    ///// <summary>
    ///// Indicates whether the cache is self-initialising.
    ///// </summary>
    //struct cache_self_initialising final : cpu_info_detector<
    //        0x00000004, cpu_info_register::eax, detail::cpu_info_bit(8)> {
    //    typedef bool type;
    //    static constexpr auto name = u8"Self-Initialising Cache Level";
    //};

    ///// <summary>
    ///// Indicates whether the cache is fully associative.
    ///// </summary>
    //struct cache_fully_associative final : cpu_info_detector<
    //        0x00000004, cpu_info_register::eax, detail::cpu_info_bit(9)> {
    //    typedef bool type;
    //    static constexpr auto name = u8"Fully Associative Cache";
    //};

    /// <summary>
    /// Gets the smallest monitor line size in bytes.
    /// </summary>
    struct min_monitor_size final : cpu_info_selector<
            0x00000005, cpu_info_register::eax, 0, 15> {
        typedef std::uint32_t type;
        static constexpr auto name = u8"Minimum Monitor Line Size";
    };

    /// <summary>
    /// Gets the largest monitor line size in bytes.
    /// </summary>
    struct max_monitor_size final : cpu_info_selector<
            0x00000005, cpu_info_register::ebx, 0, 15> {
        typedef std::uint32_t type;
        static constexpr auto name = u8"Maximum Monitor Line Size";
    };

    /// <summary>
    /// Indicates whether enumeration of MONITOR/MWAIT extensions in
    /// ECX and EDX is supported.
    /// </summary>
    struct emx final : cpu_info_detector<
            0x00000005, cpu_info_register::ecx, detail::cpu_info_bit(0)> {
        typedef bool type;
        static constexpr auto name = u8"Monitor Enumeration (emx)";
    };

    /// <summary>
    /// Indicates whether interrups are treated as break events for MWAIT even
    /// when interrupts are disabled.
    /// </summary>
    struct ibe final : cpu_info_detector<
            0x00000005, cpu_info_register::ecx, detail::cpu_info_bit(1)> {
        typedef bool type;
        static constexpr auto name = u8"Interrupts as Break Events (ibe)";
    };

    /// <summary>
    /// Indicates whether MWAIT can be used for power management without setting
    /// up memory monitoring with MONITOR beforehand.
    /// </summary>
    struct monitorless_mwait final : cpu_info_detector<
            0x00000005, cpu_info_register::ecx, detail::cpu_info_bit(3)> {
        typedef bool type;
        static constexpr auto name = u8"Monitor-less MWAIT";
    };

    /// <summary>
    /// Gets the number of C0 sub-states supported for MWAIT.
    /// </summary>
    struct c0_sub_states final : cpu_info_selector<
            0x00000005, cpu_info_register::edx, 0, 3> {
        typedef std::uint32_t type;
        static constexpr auto name = u8"C0 Sub-States for MWAIT";
    };

    /// <summary>
    /// Gets the number of C1 sub-states supported for MWAIT.
    /// </summary>
    struct c1_sub_states final : cpu_info_selector<
            0x00000005, cpu_info_register::edx, 4, 7> {
        typedef std::uint32_t type;
        static constexpr auto name = u8"C1 Sub-States for MWAIT";
    };

    /// <summary>
    /// Gets the number of C2 sub-states supported for MWAIT.
    /// </summary>
    struct c2_sub_states final : cpu_info_selector<
            0x00000005, cpu_info_register::edx, 8, 11> {
        typedef std::uint32_t type;
        static constexpr auto name = u8"C2 Sub-States for MWAIT";
    };

    /// <summary>
    /// Gets the number of C3 sub-states supported for MWAIT.
    /// </summary>
    struct c3_sub_states final : cpu_info_selector<
            0x00000005, cpu_info_register::edx, 12, 15> {
        typedef std::uint32_t type;
        static constexpr auto name = u8"C3 Sub-States for MWAIT";
    };

    /// <summary>
    /// Gets the number of C4 sub-states supported for MWAIT.
    /// </summary>
    struct c4_sub_states final : cpu_info_selector<
            0x00000005, cpu_info_register::edx, 16, 19> {
        typedef std::uint32_t type;
        static constexpr auto name = u8"C4 Sub-States for MWAIT";
    };

    /// <summary>
    /// Gets the number of C5 sub-states supported for MWAIT.
    /// </summary>
    struct c5_sub_states final : cpu_info_selector<
            0x00000005, cpu_info_register::edx, 20, 23> {
        typedef std::uint32_t type;
        static constexpr auto name = u8"C5 Sub-States for MWAIT";
    };

    /// <summary>
    /// Gets the number of C6 sub-states supported for MWAIT.
    /// </summary>
    struct c6_sub_states final : cpu_info_selector<
            0x00000005, cpu_info_register::edx, 24, 27> {
        typedef std::uint32_t type;
        static constexpr auto name = u8"C6 Sub-States for MWAIT";
    };

    /// <summary>
    /// Gets the number of C7 sub-states supported for MWAIT.
    /// </summary>
    struct c7_sub_states final : cpu_info_selector<
            0x00000005, cpu_info_register::edx, 28, 31> {
        typedef std::uint32_t type;
        static constexpr auto name = u8"C7 Sub-States for MWAIT";
    };

    /// <summary>
    /// Indicates whether the digital thermal sensor is supported.
    /// </summary>
    struct dts final : cpu_info_detector<
            0x00000006, cpu_info_register::eax, detail::cpu_info_bit(0)> {
        typedef bool type;
        static constexpr auto name = u8"Digital Thermal Sensor (dts)";
    };

    /// <summary>
    /// Indicates whether the Intel Turbo Boost Technology is supported.
    /// </summary>
    struct turbo_boost final : cpu_info_detector<
            0x00000006, cpu_info_register::eax, detail::cpu_info_bit(1)> {
        typedef bool type;
        static constexpr auto name = u8"Turbo Boost Technology (turbo-boost)";
    };

    /// <summary>
    /// Indicates whether the APIC timer implements the always running mode.
    /// </summary>
    struct arat final : cpu_info_detector<
            0x00000006, cpu_info_register::eax, detail::cpu_info_bit(2)> {
        typedef bool type;
        static constexpr auto name = u8"Always Running APIC Timer (arat)";
    };

    /// <summary>
    /// Indicates whether the CPU supports power limit notification (PLN).
    /// </summary>
    struct pln final : cpu_info_detector<
            0x00000006, cpu_info_register::eax, detail::cpu_info_bit(4)> {
        typedef bool type;
        static constexpr auto name = u8"Power Limit Notification (pln)";
    };

    /// <summary>
    /// Indicates whether Clock-modulation duty cycle extensions are available.
    /// </summary>
    struct ecmd final : cpu_info_detector<
            0x00000006, cpu_info_register::eax, detail::cpu_info_bit(5)> {
        typedef bool type;
        static constexpr auto name = u8"Extended Clock Modulation Duty "
            u8"Capability (ecmd)";
    };

    /// <summary>
    /// Indicates whether the package thermal management capability is
    /// supported.
    /// </summary>
    struct ptm final : cpu_info_detector<
            0x00000006, cpu_info_register::eax, detail::cpu_info_bit(6)> {
        typedef bool type;
        static constexpr auto name = u8"Package Thermal Management Capability "
            u8"(ptm)";
    };

    /// <summary>
    /// Indicates whether hardware-controlled performance states are supported.
    /// </summary>
    struct hwp final : cpu_info_detector<
            0x00000006, cpu_info_register::eax, detail::cpu_info_bit(7)> {
        typedef bool type;
        static constexpr auto name = u8"Hardware-Controlled Performance States "
            u8"(hwp)";
    };

    /// <summary>
    /// Indicates whether the IA32_HWP_INTERRUPT MSR is supported.
    /// </summary>
    struct hwp_notification final : cpu_info_detector<
            0x00000006, cpu_info_register::eax, detail::cpu_info_bit(8)> {
        typedef bool type;
        static constexpr auto name = u8"Hardware-Controlled Performance States "
            u8"Notification (hwp-notification)";
    };

    /// <summary>
    /// Indicates whether the IA32_HWP_REQUEST MSR is supported.
    /// </summary>
    struct hwp_activity_window final : cpu_info_detector<
            0x00000006, cpu_info_register::eax, detail::cpu_info_bit(9)> {
        typedef bool type;
        static constexpr auto name = u8"Hardware-Controlled Performance States "
            u8"Activity Window Control (hwp-activity-window)";
    };

    /// <summary>
    /// Indicates whether the IA32_HWP_REQUEST MSR is supported.
    /// </summary>
    struct hwp_epp final : cpu_info_detector<
            0x00000006, cpu_info_register::eax, detail::cpu_info_bit(10)> {
        typedef bool type;
        static constexpr auto name = u8"Hardware-Controlled Performance States "
            u8"Energy Performance Preference Control "
            u8"(hwp-energy-performance-preference)";
    };

    /// <summary>
    /// Indicates whether the IA32_HWP_REQUEST_PKG MSR is supported.
    /// </summary>
    struct hwp_pkg final : cpu_info_detector<
            0x00000006, cpu_info_register::eax, detail::cpu_info_bit(11)> {
        typedef bool type;
        static constexpr auto name = u8"Hardware-Controlled Performance States "
            u8"Package-Level Control (hwp-package-level-request)";
    };

    /// <summary>
    /// Indicates whether hardware ducy cycling is supported.
    /// </summary>
    struct hdc final : cpu_info_detector<
            0x00000006, cpu_info_register::eax, detail::cpu_info_bit(13)> {
        typedef bool type;
        static constexpr auto name = u8"Hardware Duty Cycling (hdc)";
    };

    /// <summary>
    /// Indicates whether the Intel Turbo Boost Max Technology 3.0 is supported.
    /// </summary>
    struct turbo_boost_max final : cpu_info_detector<
            0x00000006, cpu_info_register::ebx, detail::cpu_info_bit(14)> {
        typedef bool type;
        static constexpr auto name = u8"Intel Turbo Boost Max Technology 3.0";
    };

    /// <summary>
    /// Interrupts upon changes to IA32_HWP_CAPABILITIES.Highest_Performance
    /// (bits 7:0) supported.
    /// </summary>
    struct hwp_capabilities final : cpu_info_selector<
            0x00000006, cpu_info_register::ebx, 0, 15> {
        typedef std::uint16_t type;
        static constexpr auto name = u8"Hardware-Controlled Performance States "
            u8"Capabilities (hwp-cap)";
    };

    /// <summary>
    /// Indicates whether HWP PECI override is supported.
    /// </summary>
    struct hwp_peci_override final : cpu_info_detector<
            0x00000006, cpu_info_register::ebx, detail::cpu_info_bit(16)> {
        typedef bool type;
        static constexpr auto name = u8"Hardware-Controlled Performance States "
            u8"Platform Environment Control Interface Override "
            u8"(hwp-peci-override)";
    };

    /// <summary>
    /// Indicates whether flexible HWP is supported.
    /// </summary>
    struct flexible_hwp final : cpu_info_detector<
            0x00000006, cpu_info_register::ebx, detail::cpu_info_bit(17)> {
        typedef bool type;
        static constexpr auto name = u8"Flexible Hardware-Controlled "
            u8"Performance States (flexible-hwp)";
    };

    /// <summary>
    /// Indicates whether fast access mode for IA32_HWP_REQUEST MSR is
    /// supported.
    /// </summary>
    struct hwp_request_fast_access final : cpu_info_detector<
            0x00000006, cpu_info_register::ebx, detail::cpu_info_bit(18)> {
        typedef bool type;
        static constexpr auto name = u8"Hardware-Controlled Performance States "
            u8"Request Fast Access (hwp-request-fast-access)";
    };

    /// <summary>
    /// Indicates whether the hardware feedback interface is supported.
    /// </summary>
    struct hw_feedback_interface final : cpu_info_detector<
            0x00000006, cpu_info_register::ebx, detail::cpu_info_bit(19)> {
        typedef bool type;
        static constexpr auto name = u8"Hardware Feedback Interface "
            u8"(hw-feedback)";
    };

    /// <summary>
    /// IA32_HWP_REQUEST of idle logical processor ignored when only one of two
    /// logical processors that share a physical processor is active.
    /// </summary>
    struct hwp_request_ignore_idle final : cpu_info_detector<
            0x00000006, cpu_info_register::ebx, detail::cpu_info_bit(20)> {
        typedef bool type;
        static constexpr auto name = u8"Hardware-Controlled Performance States "
            u8"Request Ignore Idle (hwp-request-ignore-idle)";
    };

    /// <summary>
    /// IA32_HWP_CTL MSR is supported.
    /// </summary>
    struct hwp_control_msr final : cpu_info_detector<
            0x00000006, cpu_info_register::ebx, detail::cpu_info_bit(22)> {
        typedef bool type;
        static constexpr auto name = u8"Hardware-Controlled Performance States "
            u8"Control MSR (hwp-control-msr)";
    };

    /// <summary>
    /// Intel Thread Director is supported.
    /// </summary>
    struct thread_director final : cpu_info_detector<
            0x00000006, cpu_info_register::ebx, detail::cpu_info_bit(23)> {
        typedef bool type;
        static constexpr auto name = u8"Intel Thread Director "
            u8"(thread-director)";
    };

    /// ECX and EDX missing

    /// <summary>
    /// SHA-512 instructions are supported.
    /// </summary>
    struct sha512 final : cpu_info_detector<
            0x00000007, cpu_info_register::eax, detail::cpu_info_bit(0)> {
        typedef bool type;
        static constexpr auto name = u8"SHA-512 Extensions (sha512)";
    };

    /// <summary>
    /// SM3 hash instructions are supported.
    /// </summary>
    struct sm3 final : cpu_info_detector<
            0x00000007, cpu_info_register::eax, detail::cpu_info_bit(1)> {
        typedef bool type;
        static constexpr auto name = u8"SM3 Hash Extensions (sm3)";
    };

    /// <summary>
    /// SM4 cipher instructions are supported.
    /// </summary>
    struct sm4 final : cpu_info_detector<
            0x00000007, cpu_info_register::eax, detail::cpu_info_bit(2)> {
        typedef bool type;
        static constexpr auto name = u8"SM4 Cipher Extensions (sm4)";
    };

    /// <summary>
    /// Remote atomic operations on integers are supported.
    /// </summary>
    struct rao_int final : cpu_info_detector<
            0x00000007, cpu_info_register::eax, detail::cpu_info_bit(3)> {
        typedef bool type;
        static constexpr auto name = u8"Remote Atomic Operations on Integers "
            u8"(rao-int)";
    };

    /// <summary>
    /// AVX Vector Neural Network Instructions (AVX-VNNI) are supported.
    /// </summary>
    struct avx_vnni final : simd_detector<simd_instruction_set::avxvnni> {
        typedef bool type;
        static constexpr auto name = u8"AVX Vector Neural Network Instructions "
            u8"(avx-vnni)";
    };

    /// <summary>
    /// AVX-512 BF16 is supported.
    /// </summary>
    struct avx512_bf16 final : simd_detector<simd_instruction_set::avx512bf16> {
        typedef bool type;
        static constexpr auto name = u8"AVX-512 Instructions for BFLOAT16 "
            u8"Numbers (avx512-bf16)";
    };

    /// <summary>
    /// Indicates whether linear address space separation (LASS) is supported.
    /// </summary>
    struct lass final : cpu_info_detector<
            0x00000007, cpu_info_register::eax, detail::cpu_info_bit(6)> {
        typedef bool type;
        static constexpr auto name = u8"Linear Address Space Separation (lass)";
    };

    /// <summary>
    /// Indicates whether CMPccXADD instructions are supported.
    /// </summary>
    struct cmpccxadd final : cpu_info_detector<
            0x00000007, cpu_info_register::eax, detail::cpu_info_bit(7)> {
        typedef bool type;
        static constexpr auto name = u8"CMPccXADD";
    };

    /// <summary>
    /// Indicates whether LEAF 0x23 is supported.
    /// </summary>
    struct architectural_performance_monitoring : cpu_info_detector<
            0x00000007, cpu_info_register::eax, detail::cpu_info_bit(8)> {
        typedef bool type;
        static constexpr auto name = u8"Architectural Performance Monitoring "
            u8"Extended Leaf (archperf­monext)";
    };

    /// <summary>
    /// Indicates whether fast zero-length REP MOVSB is supported.
    /// </summary>
    struct fzrm final : cpu_info_detector<
            0x00000007, cpu_info_register::eax, detail::cpu_info_bit(10)> {
        typedef bool type;
        static constexpr auto name = u8"Fast Zero-Length REP MOVSB (fzrm)";
    };

    /// <summary>
    /// Indicates whether fast short REP STOSB is supported.
    /// </summary>
    struct fsrs final : cpu_info_detector<
            0x00000007, cpu_info_register::eax, detail::cpu_info_bit(11)> {
        typedef bool type;
        static constexpr auto name = u8"Fast Short REP STOSB (fsrs)";
    };

    /// <summary>
    /// Indicates whether fast short REP CMPSB and REP SCASB are supported.
    /// </summary>
    struct rsrcs final : cpu_info_detector<
            0x00000007, cpu_info_register::eax, detail::cpu_info_bit(12)> {
        typedef bool type;
        static constexpr auto name = u8"Fast short REP CMPSB and REP SCASB "
            u8"(rsrcs)";
    };

    /// <summary>
    /// Indicates whether flexible return and event delivery is supported.
    /// </summary>
    struct fred final : cpu_info_detector<
            0x00000007, cpu_info_register::eax, detail::cpu_info_bit(17)> {
        typedef bool type;
        static constexpr auto name = u8"Flexible Return and Event Delivery "
            u8"(fred)";
    };

    /// <summary>
    /// Indicates whether the LKGS instruction is supported.
    /// </summary>
    struct lkgs final : cpu_info_detector<
            0x00000007, cpu_info_register::eax, detail::cpu_info_bit(18)> {
        typedef bool type;
        static constexpr auto name = u8"LKGS";
    };

    /// <summary>
    /// Indicates whether WRMSRNS is supported.
    /// </summary>
    struct non_serialising_wrmsr final : cpu_info_detector<
            0x00000007, cpu_info_register::eax, detail::cpu_info_bit(19)> {
        typedef bool type;
        static constexpr auto name = u8"Non-Serialising Write to MSRs "
            u8"(wrmsrns)";
    };

    /// <summary>
    /// Indicates whether non-maskable interrupt source reporting is supported.
    /// </summary>
    struct nmi_source_reporting final : cpu_info_detector<
            0x00000007, cpu_info_register::eax, detail::cpu_info_bit(20)> {
        typedef bool type;
        static constexpr auto name = u8"Non-Maskable Interrupt Source "
            "Reporting (nmi-src)";
    };

    /// <summary>
    /// Indicates whether AMX instructions for FP16 numbers are supported.
    /// </summary>
    struct amx_fp16 final : cpu_info_detector<
            0x00000007, cpu_info_register::eax, detail::cpu_info_bit(21)> {
        typedef bool type;
        static constexpr auto name = u8"AMX instructions for FP16 numbers "
            u8"(amx-fp16)";
    };

    /// <summary>
    /// Indicates whether the HRESET instruction, the IA32_HRESET_ENABLE MSR and
    /// leaf 0x20 are supported.
    /// </summary>
    struct hreset final : cpu_info_detector<
            0x00000007, cpu_info_register::eax, detail::cpu_info_bit(22)> {
        typedef bool type;
        static constexpr auto name = u8"Processor History Reset (hreset)";
    };

    /// <summary>
    /// AVX IFMA instructions are supported.
    /// </summary>
    struct avx_ifma : simd_detector<simd_instruction_set::avxifma> {
        typedef bool type;
        static constexpr auto name = u8"AVX Integer Fused Multiply Add "
            u8"(avx-ifma)";
    };

    /// <summary>
    /// Indicates whether linear address masking is supported.
    /// </summary>
    struct linear_address_masking final : cpu_info_detector<
            0x00000007, cpu_info_register::eax, detail::cpu_info_bit(26)> {
        typedef bool type;
        static constexpr auto name = u8"Linear Address Masking (lam)";
    };

    /// <summary>
    /// Indicates whether RDMSRLIST and WRMSRLIST instructions are supported.
    /// </summary>
    struct msr_list final : cpu_info_detector<
            0x00000007, cpu_info_register::eax, detail::cpu_info_bit(27)> {
        typedef bool type;
        static constexpr auto name = u8"RDMSRLIST and WRMSRLIST (msr-list)";
    };

    /// <summary>
    /// If set, supports INVD instruction execution prevention after BIOS done.
    /// </summary>
    struct invd final : cpu_info_detector<
            0x00000007, cpu_info_register::eax, detail::cpu_info_bit(30)> {
        typedef bool type;
        static constexpr auto name = u8"INVD";
    };

    /// <summary>
    /// MOVRS and PREFETCHRST2 instructions supported (memory read/prefetch
    /// with read-shared hint)
    /// </summary>
    struct movrs final : cpu_info_detector<
            0x00000007, cpu_info_register::eax, detail::cpu_info_bit(31)> {
        typedef bool type;
        static constexpr auto name = u8"Memory Read/Prefetch with Read-Shared "
            u8"Hint (movrs)";
    };

    /// <summary>
    /// Checks whether the V2 extended topology enumeration leaf 0x1F is supported.
    /// </summary>
    struct extended_toplogy_enumeration final : cpu_info_any<
            0x0000001F, cpu_info_register::ebx> {
        typedef bool type;
        static constexpr auto name = u8"Extended Topology Enumeration "
            u8"(leaf 0x1F)";
    };

    /// <summary>
    /// Test for extended APIC ID. See also
    /// https://docs.kernel.org/arch/x86/topology.html
    /// </summary>
    struct topology_extensions final : cpu_info_detector<
            0x80000001, cpu_info_register::ecx, detail::cpu_info_bit(22)> {
        typedef bool type;
        static constexpr auto name = u8"Extended APIC ID (topology_extensions)";
    };

    /// <summary>
    /// Answer whether the CPU supports leaf B of the CPUID instruction, which
    /// contains information about the topology of the CPU. See also
    /// https://github.com/tpn/cpuid-topo/blob/f9b24d25220965087c1d855f716c52d75bc7bbe0/cpu_topo.c#L1009-L1057
    /// </summary>
    struct topology_leaf_b final : cpu_info_any<
            0x0000000b, cpu_info_register::ebx> {
        typedef bool type;
        static constexpr auto name = u8"Topology Leaf B (leaf 0xB)";
    };

} /* namespace cpu_features */

LYRA_NAMESPACE_END

#endif /* !defined(_LYRA_CPU_FEATURES_H) */
