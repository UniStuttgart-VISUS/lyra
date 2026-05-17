// <copyright file="cpu_features.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_CPU_FEATURES_H)
#define _LYRA_CPU_FEATURES_H
#pragma once

#include "visus/autodoc/cpu_info_value.h"


LYRA_NAMESPACE_BEGIN

namespace cpu_features {

    // https://en.wikipedia.org/wiki/CPUID
    // https://blog.winny.tech/posts/cpuid/
    // https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Register/Amd/Cpuid.h

    /// <summary>
    /// Gets the stepping ID.
    /// </summary>
    struct stepping_id final : cpu_info_bits<
            0x00000001, cpu_info_register::eax, 0, 3> {
        typedef std::uint32_t type;
        static constexpr auto name = u8"Stepping";
    };

    /// <summary>
    /// Gets the model number.
    /// </summary>
    struct model final : cpu_info_bits<
            0x00000001, cpu_info_register::eax, 4, 7> {
        typedef std::uint32_t type;
        static constexpr auto name = u8"Model";
    };

    /// <summary>
    /// Gets the family ID.
    /// </summary>
    struct family_id final : cpu_info_bits<
            0x00000001, cpu_info_register::eax, 8, 11> {
        typedef std::uint32_t type;
        static constexpr auto name = u8"Family";
    };

    /// <summary>
    /// Gets the processor type.
    /// </summary>
    struct processor_type final : cpu_info_bits<
           0x00000001, cpu_info_register::eax, 12, 13> {
        typedef std::uint32_t type;
        static constexpr auto name = u8"Processor Type";
    };

    /// <summary>
    /// Gets the extended model ID.
    /// </summary>
    struct extended_model final : cpu_info_bits<
            0x00000001, cpu_info_register::eax, 16, 19> {
        typedef std::uint32_t type;
        static constexpr auto name = u8"Extended Model";
    };

    /// <summary>
    /// Gets the extended family ID.
    /// </summary>
    struct extended_family_id final : cpu_info_bits<
            0x00000001, cpu_info_register::eax, 20, 27> {
        typedef std::uint32_t type;
        static constexpr auto name = u8"Extended Family";
    };

    /// <summary>
    /// Gets the brand index.
    /// </summary>
    struct brand final : cpu_info_bits<
            0x00000001, cpu_info_register::ebx, 0, 7> {
        typedef std::uint32_t type;
        static constexpr auto name = u8"Brand ID";
    };

    /// <summary>
    /// Gets the cache line flush size (value * 8 bytes).
    /// </summary>
    struct clflush_size final : cpu_info_bits<
            0x00000001, cpu_info_register::ebx, 8, 15> {
        typedef std::uint32_t type;
        static constexpr auto name = u8"Cache Line Flush Size";
    };


    /// <summary>
    /// Gets the maximum addressable logical processor ID in the physical
    /// package.
    /// </summary>
    struct max_cpu_id final : cpu_info_bits<
            0x00000001, cpu_info_register::ebx, 16, 23> {
        typedef std::uint32_t type;
        static constexpr auto name = u8"Maximum CPUs per Package";
    };

    /// <summary>
    /// Gets the initial APIC ID of the processor.
    /// </summary>
    struct apic_id final : cpu_info_bits<
            0x00000001, cpu_info_register::ebx, 24, 31> {
        typedef std::uint32_t type;
        static constexpr auto name = u8"APIC ID";
    };

    /// <summary>
    /// Detect SSE3 support.
    /// </summary>
    struct sse3 final : cpu_info_bit<
            0x00000001, cpu_info_register::ecx, 0> {
        typedef bool type;
        static constexpr auto name = u8"Streaming SIMD Extensions 3 (sse3)";
    };

    /// <summary>
    /// Checks whether the PCLMULQDQ instruction is supported.
    /// </summary>
    struct pclmulqdq final : cpu_info_bit<
            0x00000001, cpu_info_register::ecx, 1> {
        typedef bool type;
        static constexpr auto name = u8"PCLMULQDQ";
    };

    /// <summary>
    /// Checks whether the DS area uses a 64-bit layout.
    /// </summary>
    struct dtes64 final : cpu_info_bit<
            0x00000001, cpu_info_register::ecx, 2> {
        typedef bool type;
        static constexpr auto name = u8"64-bit Debug Store (dtes64)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Checks whether MONITOR/MWAIT instructions are supported.
    /// </summary>
    struct monitor final : cpu_info_bit<
            0x00000001, cpu_info_register::ecx, 3> {
        typedef bool type;
        static constexpr auto name = u8"MONITOR and MWAIT (monitor)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Checks whether CPL qualified debug store is supported.
    /// </summary>
    struct ds_cpl final : cpu_info_bit<
            0x00000001, cpu_info_register::ecx, 4> {
        typedef bool type;
        static constexpr auto name = u8"CPL-Qualified Debug Store (ds-cpl)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Checks whether Virtual Machine Extensions (VMX) are supported.
    /// </summary>
    struct vmx final : cpu_info_bit<
            0x00000001, cpu_info_register::ecx, 5> {
        typedef bool type;
        static constexpr auto name = u8"Virtual Machine Extensions (vmx)";
    };

    /// <summary>
    /// Checks whether Safer Mode Extensions (SMX) are supported.
    /// </summary>
    struct smx final : cpu_info_bit<
            0x00000001, cpu_info_register::ecx, 6> {
        typedef bool type;
        static constexpr auto name = u8"Safer Mode Extensions (smx)";
    };

    /// <summary>
    /// Checks whether Enhanced Intel SpeedStep Technology (EIST) is supported.
    /// </summary>
    struct est final : cpu_info_bit<
            0x00000001, cpu_info_register::ecx, 7> {
        typedef bool type;
        static constexpr auto name = u8"Enhanced Intel SpeedStep Technology "
            u8"(est)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Checks whether Thermal Monitor 2 (TM2) is supported.
    /// </summary>
    struct tm2 final : cpu_info_bit<
            0x00000001, cpu_info_register::ecx, 8> {
        typedef bool type;
        static constexpr auto name = u8"Thermal Monitor 2 (tm2)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Checks whether Supplemental SSE3 (SSSE3) is supported.
    /// </summary>
    struct ssse3 final : cpu_info_bit<
            0x00000001, cpu_info_register::ecx, 9> {
        typedef bool type;
        static constexpr auto name = u8"Supplemental SIMD Extensions 3 (ssse3)";
    };

    /// <summary>
    /// Checks the L1 context ID.
    /// </summary>
    struct cnxt_id final : cpu_info_bit<
            0x00000001, cpu_info_register::ecx, 10> {
        typedef bool type;
        static constexpr auto name = u8"L1 context ID (cnxt-id)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Checks whether the Silicon Debug interface is supported.
    /// </summary>
    struct sdbg final : cpu_info_bit<
            0x00000001, cpu_info_register::ecx, 11> {
        typedef bool type;
        static constexpr auto name = u8"Silicon Debug interface (sdbg)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Checks whether fused-multiply-add (FMA) instructions are supported.
    /// </summary>
    struct fma final : cpu_info_bit<
            0x00000001, cpu_info_register::ecx, 12> {
        typedef bool type;
        static constexpr auto name = u8"Fused Multiply-Add (fma)";
    };

    /// <summary>
    /// Checks whether the CMPXCHG16B instruction is supported.
    /// </summary>
    struct cx16 final : cpu_info_bit<
            0x00000001, cpu_info_register::ecx, 13> {
        typedef bool type;
        static constexpr auto name = u8"CMPXCHG16B";
    };

    /// <summary>
    /// Checks whether sending task priority messages can be disabled.
    /// </summary>
    struct xtpr final : cpu_info_bit<
            0x00000001, cpu_info_register::ecx, 14> {
        typedef bool type;
        static constexpr auto name = u8"Task Priority Messages (xtpr)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Checks whether Performance and Debug Capability is supported.
    /// </summary>
    struct pdcm final : cpu_info_bit<
            0x00000001, cpu_info_register::ecx, 15> {
        typedef bool type;
        static constexpr auto name = u8"Performance and Debug Capability "
            u8"(pdcm)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Checks whether process context identifiers (PCID) are supported.
    /// </summary>
    struct pcid final : cpu_info_bit<
            0x00000001, cpu_info_register::ecx, 17> {
        typedef bool type;
        static constexpr auto name = u8"Process Context Identifiers (pcid)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Checks whether direct cache access (DCA) for DMA writes is supported.
    /// </summary>
    struct dca final : cpu_info_bit<
            0x00000001, cpu_info_register::ecx, 18> {
        typedef bool type;
        static constexpr auto name = u8"Direct Cache Access (dca)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Checks whether SSE 4.1 is supported.
    /// </summary>
    struct sse4_1 final : cpu_info_bit<
            0x00000001, cpu_info_register::ecx, 19> {
        typedef bool type;
        static constexpr auto name = u8"Streaming SIMD Extensions 4.1 (sse4.1)";
    };

    /// <summary>
    /// Checks whether SSE 4.2 is supported.
    /// </summary>
    struct sse4_2 final : cpu_info_bit<
            0x00000001, cpu_info_register::ecx, 20> {
        typedef bool type;
        static constexpr auto name = u8"Streaming SIMD Extensions 4.2 (sse4.2)";
    };

    /// <summary>
    /// Checks whether enhanced APIC is supported.
    /// </summary>
    struct x2apic final : cpu_info_bit<
        0x00000001, cpu_info_register::ecx, 21> {
        typedef bool type;
        static constexpr auto name = u8"Extended APIC (x2apic)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Checks whether the MOVBE instruction is supported.
    /// </summary>
    struct movbe final : cpu_info_bit<
            0x00000001, cpu_info_register::ecx, 22> {
        typedef bool type;
        static constexpr auto name = u8"MOVBE";
    };

    /// <summary>
    /// Checks whether the POPCNT instruction is supported.
    /// </summary>
    struct popcnt final : cpu_info_bit<
            0x00000001, cpu_info_register::ecx,23> {
        typedef bool type;
        static constexpr auto name = u8"POPCNT";
    };

    /// <summary>
    /// Checks whether APIC implements one-shot operation using a TSC deadline.
    /// </summary>
    struct tsc_deadline final : cpu_info_bit<
            0x00000001, cpu_info_register::ecx, 24> {
        typedef bool type;
        static constexpr auto name = u8"TSC Deadline (tsc-deadline)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Checks whether the AES instruction set is supported.
    /// </summary>
    struct aes final : cpu_info_bit<
            0x00000001, cpu_info_register::ecx, 25> {
        typedef bool type;
        static constexpr auto name = u8"AES (aes-ni)";
    };

    /// <summary>
    /// Checks whether extensible processor save/restore instructions XSAVE,
    /// XRSTOR, XSETBV and XGETBV are supported.
    /// </summary>
    struct xsave final : cpu_info_bit<
            0x00000001, cpu_info_register::ecx, 26> {
        typedef bool type;
        static constexpr auto name = u8"Extensible Processor Save/Restore "
            u8"(xsave)";
    };

    /// <summary>
    /// Checks whether XSAVE is enabled by the operating system.
    /// </summary>
    struct osxsave final : cpu_info_bit<
            0x00000001, cpu_info_register::ecx, 27> {
        typedef bool type;
        static constexpr auto name = u8"Extensible Processor Save/Restore "
            u8"Enabled by Operating System (osxsave)";
    };

    /// <summary>
    /// Checks whether AVX (256-bit SIMD) instructions are supported.
    /// </summary>
    struct avx final : cpu_info_bit<
            0x00000001, cpu_info_register::ecx, 28> {
        typedef bool type;
        static constexpr auto name = u8"Advanced Vector Extensions (avx)";
    };

    /// <summary>
    /// Checks whether conversion from and to FP16 (half-precision floating
    /// point numbers) is supported.
    /// </summary>
    struct f16c final : cpu_info_bit<
            0x00000001, cpu_info_register::ecx, 29> {
        typedef bool type;
        static constexpr auto name = u8"FP16 Conversion (f16c)";
    };

    /// <summary>
    /// Checks whether the on-chip RNG operation RDRAND is supported.
    /// </summary>
    struct rdrnd final : cpu_info_bit<
            0x00000001, cpu_info_register::ecx, 30> {
        typedef bool type;
        static constexpr auto name = u8"On-board Random Number Generator "
            u8"(rdrnd)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Indicates whether a hypervisor is present (zero for physical CPUs).
    /// </summary>
    struct hypervisor final : cpu_info_bit<
            0x00000001, cpu_info_register::ecx, 31> {
        typedef bool type;
        static constexpr auto name = u8"Hypervisor Present (hypervisor)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Indicates whether an on-board x87 FPU is supported.
    /// </summary>
    struct fpu final : cpu_info_bit<
            0x00000001, cpu_info_register::edx, 0> {
        typedef bool type;
        static constexpr auto name = u8"On-board x87 Floating-Point Unit (fpu)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Indicates whether virtual 8086 mode extensions are supported.
    /// </summary>
    struct vme final : cpu_info_bit<
            0x00000001, cpu_info_register::edx, 1> {
        typedef bool type;
        static constexpr auto name = u8"Virtual 8086 Mode Extensions (vme)";
    };

    /// <summary>
    /// Indicates whether debugging extensions are present.
    /// </summary>
    struct de final : cpu_info_bit<
            0x00000001, cpu_info_register::edx, 2> {
        typedef bool type;
        static constexpr auto name = u8"Debugging Extensions (de)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Indicates whether pages larger than 4MB are supported.
    /// </summary>
    struct pse final : cpu_info_bit<
            0x00000001, cpu_info_register::edx, 3> {
        typedef bool type;
        static constexpr auto name = u8"Page Size Extension (pse)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Indicates whether the time stamp counter and the RDSTC instruction are
    /// supported.
    /// </summary>
    struct tsc final : cpu_info_bit<
            0x00000001, cpu_info_register::edx, 4> {
        typedef bool type;
        static constexpr auto name = u8"Time Stamp Counter and RDSTC (tsc)";
    };

    /// <summary>
    /// Indicates whether RDMSR and WRMSR instructions are supported.
    /// </summary>
    struct msr final : cpu_info_bit<
            0x00000001, cpu_info_register::edx, 5> {
        typedef bool type;
        static constexpr auto name = u8"Model-Specific Registers (msr)";
    };

    /// <summary>
    /// Indicates whether physical address extension (PAE) is supported.
    /// </summary>
    struct pae final : cpu_info_bit<
            0x00000001, cpu_info_register::edx, 6> {
        typedef bool type;
        static constexpr auto name = u8"Physical Address Extension (pae)";
    };

    /// <summary>
    /// Indicates whether machine check exception (MCE) is supported.
    /// </summary>
    struct mce final : cpu_info_bit<
            0x00000001, cpu_info_register::edx, 7> {
        typedef bool type;
        static constexpr auto name = u8"Machine Check Exception (mce)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Indicates the CMPXCHG8B instruction is supported.
    /// </summary>
    struct cx8 final : cpu_info_bit<
            0x00000001, cpu_info_register::edx, 8> {
        typedef bool type;
        static constexpr auto name = u8"CMPXCHG8B";
    };

    /// <summary>
    /// Indicates whether an APIC is present.
    /// </summary>
    struct apic final : cpu_info_bit<
            0x00000001, cpu_info_register::edx, 9> {
        typedef bool type;
        static constexpr auto name = u8"Advanced Programmable Interrupt "
            u8"Controller (apic)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Indicates whether the SYSENTER and SYSEXIT instructions are supported.
    /// </summary>
    struct sep final : cpu_info_bit<
            0x00000001, cpu_info_register::edx, 11> {
        typedef bool type;
        static constexpr auto name = u8"Fast System Call Instructions (sep)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Indicates whether memory type range registers (MTRR) are present.
    /// </summary>
    struct mtrr final : cpu_info_bit<
            0x00000001, cpu_info_register::edx, 12> {
        typedef bool type;
        static constexpr auto name = u8"Memory Type Range Registers (mtrr)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Indicates whether the Page Global Enable (PGE) feature is supported.
    /// </summary>
    struct pge final : cpu_info_bit<
            0x00000001, cpu_info_register::edx, 13> {
        typedef bool type;
        static constexpr auto name = u8"Page Global Enable (pge)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Indicates whether the Machine Check Architecture (MCA) is supported.
    /// </summary>
    struct mca final : cpu_info_bit<0x00000001, cpu_info_register::edx,
        14> {
        typedef bool type;
        static constexpr auto name = u8"Machine Check Architecture (mca)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Indicates whether the conditional move instructions CMOV, FCMOV and
    /// FCOMI are supported.
    /// </summary>
    struct cmov final : cpu_info_bit<
            0x00000001, cpu_info_register::edx, 15> {
        typedef bool type;
        static constexpr auto name = u8"Conditional Move Instructions (cmov)";
    };

    /// <summary>
    /// Indicates whether the page attribute table (PAT) is supported.
    /// </summary>
    struct pat final : cpu_info_bit<
            0x00000001, cpu_info_register::edx, 16> {
        typedef bool type;
        static constexpr auto name = u8"Page Attribute Table (pat)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Indicates whether 36-bit page size extensions (PSE-36) are supported.
    /// </summary>
    struct pse36 final : cpu_info_bit<
            0x00000001, cpu_info_register::edx, 17> {
        typedef bool type;
        static constexpr auto name = u8"36-bit Page Size Extensions (pse-36)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Indicates whether processor serial number (PSN) is supported and
    /// enabled.
    /// </summary>
    struct psn final : cpu_info_bit<
            0x00000001, cpu_info_register::edx, 18> {
        typedef bool type;
        static constexpr auto name = u8"Processor Serial Number (psn)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Indicates whether CLFLUSH instruction is supported.
    /// </summary>
    struct clfsh final : cpu_info_bit<
            0x00000001, cpu_info_register::edx, 19> {
        typedef bool type;
        static constexpr auto name = u8"CLFLUSH";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Indicates whether the no-execute bit on Itanium is supported.
    /// </summary>
    struct nx final : cpu_info_bit<
            0x00000001, cpu_info_register::edx, 20> {
        typedef bool type;
        static constexpr auto name = u8"No-Execute Bit (nx)";
        // TODO: Itanium only
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Indicates whether debug store is supported.
    /// </summary>
    struct ds final : cpu_info_bit<
            0x00000001, cpu_info_register::edx, 21> {
        typedef bool type;
        static constexpr auto name = u8"Debug Store (ds)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Indicates whether on-board thermal control MSRs for ACPI are supported.
    /// </summary>
    struct acpi final : cpu_info_bit<
            0x00000001, cpu_info_register::edx, 22> {
        typedef bool type;
        static constexpr auto name = u8"On-Board Thermal Control MSRs for ACPI "
            u8"(acpi)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Indicates whether the MMX instruction set is supported.
    /// </summary>
    struct mmx final : cpu_info_bit<
            0x00000001, cpu_info_register::edx, 23> {
        typedef bool type;
        static constexpr auto name = u8"MMX";
    };

    /// <summary>
    /// Indicates whether the FXSAVE and FXSTOR instructions are supported.
    /// </summary>
    struct fxsr final : cpu_info_bit<
            0x00000001, cpu_info_register::edx, 24> {
        typedef bool type;
        static constexpr auto name = u8"FXSAVE and FXSTOR (fxsr)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Indicates whether the SSE instruction set is supported.
    /// </summary>
    struct sse final : cpu_info_bit<
            0x00000001, cpu_info_register::ecx, 25> {
        typedef bool type;
        static constexpr auto name = u8"Streaming SIMD Extensions (sse)";
    };

    /// <summary>
    /// Indicates whether the SSE2 instruction set is supported.
    /// </summary>
    struct sse2 final : cpu_info_bit<
            0x00000001, cpu_info_register::ecx, 26> {
        typedef bool type;
        static constexpr auto name = u8"Streaming SIMD Extensions 2 (sse2)";
    };

    /// <summary>
    /// Indicates whether the CPU cache implements self-snoop.
    /// </summary>
    struct ss final : cpu_info_bit<
            0x00000001, cpu_info_register::edx, 27> {
        typedef bool type;
        static constexpr auto name = u8"CPU Cache implements self-snoop (ss)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Indicates whether the maximum APIC IDs reserved field is valid.
    /// </summary>
    struct htt final : cpu_info_bit<
            0x00000001, cpu_info_register::edx, 28> {
        typedef bool type;
        static constexpr auto name = u8"Maximum APIC IDs Reserved Field (htt)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Indicates whether thermal monitor automatically limits temperature.
    /// </summary>
    struct tm final : cpu_info_bit<
            0x00000001, cpu_info_register::edx, 29> {
        typedef bool type;
        static constexpr auto name = u8"Thermal Monitor (tm)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Indicates whether the CPU is an Itanium processor emulating x86.
    /// </summary>
    struct ia64 final : cpu_info_bit<
            0x00000001, cpu_info_register::edx, 30> {
        typedef bool type;
        static constexpr auto name = u8"Itanium Processor Emulating x86 (ia64)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Indicates whether the pending break enable (PBE) feature is supported.
    /// </summary>
    struct pbe final : cpu_info_bit<
            0x00000001, cpu_info_register::edx, 31> {
        typedef bool type;
        static constexpr auto name = u8"Pending Break Enable (pbe)";
        static constexpr auto uncommon = true;
    };

    ///// <summary>
    ///// Indicates whether a cache type is instruction (2), data (1) or
    ///// unified (3).
    ///// </summary>
    //struct cache_type final : cpu_info_bits<
    //        0x00000004, cpu_info_register::eax, 0, 4> {
    //    typedef std::uint32_t type;
    //    static constexpr auto name = u8"Cache Type";
    //};

    ///// <summary>
    ///// Gets the cache level starting with 1 for L1, 2 for L2, etc.
    ///// </summary>
    //struct cache_level final : cpu_info_bits<
    //        0x00000004, cpu_info_register::eax, 5, 7> {
    //    typedef std::uint32_t type;
    //    static constexpr auto name = u8"Cache Level";
    //};

    ///// <summary>
    ///// Indicates whether the cache is self-initialising.
    ///// </summary>
    //struct cache_self_initialising final : cpu_info_bit<
    //        0x00000004, cpu_info_register::eax, 8> {
    //    typedef bool type;
    //    static constexpr auto name = u8"Self-Initialising Cache Level";
    //};

    ///// <summary>
    ///// Indicates whether the cache is fully associative.
    ///// </summary>
    //struct cache_fully_associative final : cpu_info_bit<
    //        0x00000004, cpu_info_register::eax, 9> {
    //    typedef bool type;
    //    static constexpr auto name = u8"Fully Associative Cache";
    //};

    /// <summary>
    /// Gets the smallest monitor line size in bytes.
    /// </summary>
    struct min_monitor_size final : cpu_info_bits<
            0x00000005, cpu_info_register::eax, 0, 15> {
        typedef std::uint32_t type;
        static constexpr auto name = u8"Minimum Monitor Line Size";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Gets the largest monitor line size in bytes.
    /// </summary>
    struct max_monitor_size final : cpu_info_bits<
            0x00000005, cpu_info_register::ebx, 0, 15> {
        typedef std::uint32_t type;
        static constexpr auto name = u8"Maximum Monitor Line Size";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Indicates whether enumeration of MONITOR/MWAIT extensions in
    /// ECX and EDX is supported.
    /// </summary>
    struct emx final : cpu_info_bit<
            0x00000005, cpu_info_register::ecx, 0> {
        typedef bool type;
        static constexpr auto name = u8"Monitor Enumeration (emx)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Indicates whether interrups are treated as break events for MWAIT even
    /// when interrupts are disabled.
    /// </summary>
    struct ibe final : cpu_info_bit<
            0x00000005, cpu_info_register::ecx, 1> {
        typedef bool type;
        static constexpr auto name = u8"Interrupts as Break Events (ibe)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Indicates whether MWAIT can be used for power management without setting
    /// up memory monitoring with MONITOR beforehand.
    /// </summary>
    struct monitorless_mwait final : cpu_info_bit<
            0x00000005, cpu_info_register::ecx, 3> {
        typedef bool type;
        static constexpr auto name = u8"Monitor-less MWAIT";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Gets the number of C0 sub-states supported for MWAIT.
    /// </summary>
    struct c0_sub_states final : cpu_info_bits<
            0x00000005, cpu_info_register::edx, 0, 3> {
        typedef std::uint32_t type;
        static constexpr auto name = u8"C0 Sub-States for MWAIT";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Gets the number of C1 sub-states supported for MWAIT.
    /// </summary>
    struct c1_sub_states final : cpu_info_bits<
            0x00000005, cpu_info_register::edx, 4, 7> {
        typedef std::uint32_t type;
        static constexpr auto name = u8"C1 Sub-States for MWAIT";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Gets the number of C2 sub-states supported for MWAIT.
    /// </summary>
    struct c2_sub_states final : cpu_info_bits<
            0x00000005, cpu_info_register::edx, 8, 11> {
        typedef std::uint32_t type;
        static constexpr auto name = u8"C2 Sub-States for MWAIT";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Gets the number of C3 sub-states supported for MWAIT.
    /// </summary>
    struct c3_sub_states final : cpu_info_bits<
            0x00000005, cpu_info_register::edx, 12, 15> {
        typedef std::uint32_t type;
        static constexpr auto name = u8"C3 Sub-States for MWAIT";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Gets the number of C4 sub-states supported for MWAIT.
    /// </summary>
    struct c4_sub_states final : cpu_info_bits<
            0x00000005, cpu_info_register::edx, 16, 19> {
        typedef std::uint32_t type;
        static constexpr auto name = u8"C4 Sub-States for MWAIT";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Gets the number of C5 sub-states supported for MWAIT.
    /// </summary>
    struct c5_sub_states final : cpu_info_bits<
            0x00000005, cpu_info_register::edx, 20, 23> {
        typedef std::uint32_t type;
        static constexpr auto name = u8"C5 Sub-States for MWAIT";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Gets the number of C6 sub-states supported for MWAIT.
    /// </summary>
    struct c6_sub_states final : cpu_info_bits<
            0x00000005, cpu_info_register::edx, 24, 27> {
        typedef std::uint32_t type;
        static constexpr auto name = u8"C6 Sub-States for MWAIT";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Gets the number of C7 sub-states supported for MWAIT.
    /// </summary>
    struct c7_sub_states final : cpu_info_bits<
            0x00000005, cpu_info_register::edx, 28, 31> {
        typedef std::uint32_t type;
        static constexpr auto name = u8"C7 Sub-States for MWAIT";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Indicates whether the digital thermal sensor is supported.
    /// </summary>
    struct dts final : cpu_info_bit<
            0x00000006, cpu_info_register::eax, 0> {
        typedef bool type;
        static constexpr auto name = u8"Digital Thermal Sensor (dts)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Indicates whether the Intel Turbo Boost Technology is supported.
    /// </summary>
    struct turbo_boost final : cpu_info_bit<
            0x00000006, cpu_info_register::eax, 1> {
        typedef bool type;
        static constexpr auto name = u8"Turbo Boost Technology (turbo-boost)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Indicates whether the APIC timer implements the always running mode.
    /// </summary>
    struct arat final : cpu_info_bit<
            0x00000006, cpu_info_register::eax, 2> {
        typedef bool type;
        static constexpr auto name = u8"Always Running APIC Timer (arat)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Indicates whether the CPU supports power limit notification (PLN).
    /// </summary>
    struct pln final : cpu_info_bit<
            0x00000006, cpu_info_register::eax, 4> {
        typedef bool type;
        static constexpr auto name = u8"Power Limit Notification (pln)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Indicates whether Clock-modulation duty cycle extensions are available.
    /// </summary>
    struct ecmd final : cpu_info_bit<
            0x00000006, cpu_info_register::eax, 5> {
        typedef bool type;
        static constexpr auto name = u8"Extended Clock Modulation Duty "
            u8"Capability (ecmd)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Indicates whether the package thermal management capability is
    /// supported.
    /// </summary>
    struct ptm final : cpu_info_bit<
            0x00000006, cpu_info_register::eax, 6> {
        typedef bool type;
        static constexpr auto name = u8"Package Thermal Management Capability "
            u8"(ptm)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Indicates whether hardware-controlled performance states are supported.
    /// </summary>
    struct hwp final : cpu_info_bit<
            0x00000006, cpu_info_register::eax, 7> {
        typedef bool type;
        static constexpr auto name = u8"Hardware-Controlled Performance States "
            u8"(hwp)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Indicates whether the IA32_HWP_INTERRUPT MSR is supported.
    /// </summary>
    struct hwp_notification final : cpu_info_bit<
            0x00000006, cpu_info_register::eax, 8> {
        typedef bool type;
        static constexpr auto name = u8"Hardware-Controlled Performance States "
            u8"Notification (hwp-notification)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Indicates whether the IA32_HWP_REQUEST MSR is supported.
    /// </summary>
    struct hwp_activity_window final : cpu_info_bit<
            0x00000006, cpu_info_register::eax, 9> {
        typedef bool type;
        static constexpr auto name = u8"Hardware-Controlled Performance States "
            u8"Activity Window Control (hwp-activity-window)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Indicates whether the IA32_HWP_REQUEST MSR is supported.
    /// </summary>
    struct hwp_epp final : cpu_info_bit<
            0x00000006, cpu_info_register::eax, 10> {
        typedef bool type;
        static constexpr auto name = u8"Hardware-Controlled Performance States "
            u8"Energy Performance Preference Control "
            u8"(hwp-energy-performance-preference)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Indicates whether the IA32_HWP_REQUEST_PKG MSR is supported.
    /// </summary>
    struct hwp_pkg final : cpu_info_bit<
            0x00000006, cpu_info_register::eax, 11> {
        typedef bool type;
        static constexpr auto name = u8"Hardware-Controlled Performance States "
            u8"Package-Level Control (hwp-package-level-request)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Indicates whether hardware ducy cycling is supported.
    /// </summary>
    struct hdc final : cpu_info_bit<
            0x00000006, cpu_info_register::eax, 13> {
        typedef bool type;
        static constexpr auto name = u8"Hardware Duty Cycling (hdc)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Indicates whether the Intel Turbo Boost Max Technology 3.0 is supported.
    /// </summary>
    struct turbo_boost_max final : cpu_info_bit<
            0x00000006, cpu_info_register::ebx, 14> {
        typedef bool type;
        static constexpr auto name = u8"Intel Turbo Boost Max Technology 3.0";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Interrupts upon changes to IA32_HWP_CAPABILITIES.Highest_Performance
    /// (bits 7:0) supported.
    /// </summary>
    struct hwp_capabilities final : cpu_info_bits<
            0x00000006, cpu_info_register::ebx, 0, 15> {
        typedef std::uint16_t type;
        static constexpr auto name = u8"Hardware-Controlled Performance States "
            u8"Capabilities (hwp-cap)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Indicates whether HWP PECI override is supported.
    /// </summary>
    struct hwp_peci_override final : cpu_info_bit<
            0x00000006, cpu_info_register::ebx, 16> {
        typedef bool type;
        static constexpr auto name = u8"Hardware-Controlled Performance States "
            u8"Platform Environment Control Interface Override "
            u8"(hwp-peci-override)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Indicates whether flexible HWP is supported.
    /// </summary>
    struct flexible_hwp final : cpu_info_bit<
            0x00000006, cpu_info_register::ebx, 17> {
        typedef bool type;
        static constexpr auto name = u8"Flexible Hardware-Controlled "
            u8"Performance States (flexible-hwp)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Indicates whether fast access mode for IA32_HWP_REQUEST MSR is
    /// supported.
    /// </summary>
    struct hwp_request_fast_access final : cpu_info_bit<
            0x00000006, cpu_info_register::ebx, 18> {
        typedef bool type;
        static constexpr auto name = u8"Hardware-Controlled Performance States "
            u8"Request Fast Access (hwp-request-fast-access)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Indicates whether the hardware feedback interface is supported.
    /// </summary>
    struct hw_feedback_interface final : cpu_info_bit<
            0x00000006, cpu_info_register::ebx, 19> {
        typedef bool type;
        static constexpr auto name = u8"Hardware Feedback Interface "
            u8"(hw-feedback)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// IA32_HWP_REQUEST of idle logical processor ignored when only one of two
    /// logical processors that share a physical processor is active.
    /// </summary>
    struct hwp_request_ignore_idle final : cpu_info_bit<
            0x00000006, cpu_info_register::ebx, 20> {
        typedef bool type;
        static constexpr auto name = u8"Hardware-Controlled Performance States "
            u8"Request Ignore Idle (hwp-request-ignore-idle)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// IA32_HWP_CTL MSR is supported.
    /// </summary>
    struct hwp_control_msr final : cpu_info_bit<
            0x00000006, cpu_info_register::ebx, 22> {
        typedef bool type;
        static constexpr auto name = u8"Hardware-Controlled Performance States "
            u8"Control MSR (hwp-control-msr)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Intel Thread Director is supported.
    /// </summary>
    struct thread_director final : cpu_info_bit<
            0x00000006, cpu_info_register::ebx, 23> {
        typedef bool type;
        static constexpr auto name = u8"Intel Thread Director "
            u8"(thread-director)";
        static constexpr auto uncommon = true;
    };

    /// ECX and EDX missing

    /// <summary>
    /// Answer the number of sub-leafs (possible values for ECX) of leaf 0x7.
    /// </summary>
    struct extended_features final : cpu_info_bits<
            0x00000007, cpu_info_register::eax, 0, 31> {
        typedef std::uint32_t type;
        static constexpr auto name = u8"Number of Sub-Leaves of Leaf 0x7";
    };

    struct fsgsbase final : cpu_info_bit<
            0x00000007, cpu_info_register::ebx, 0> {
        typedef bool type;
        static constexpr auto name = u8"Access to Base of %fs and %gs "
            u8"(fsgsbase)";
        static constexpr auto uncommon = true;
    };

    struct tsc_adjust final : cpu_info_bit<
            0x00000007, cpu_info_register::ebx, 1> {
        typedef bool type;
        static constexpr auto name = u8"TSC Adjust MSR (tsc-adjust)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Checks whether the Software Guard Extensions (SGX) are supported.
    /// </summary>
    struct sgx final : cpu_info_bit<
            0x00000007, cpu_info_register::ebx, 2> {
        typedef bool type;
        static constexpr auto name = u8"Software Guard Extensions (sgx)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Checks whether Bit Manipulation Instruction Set 1 (BMI1) is supported.
    /// </summary>
    struct bmi1 final : cpu_info_bit<
            0x00000007, cpu_info_register::ebx, 3> {
        typedef bool type;
        static constexpr auto name = u8"Bit Manipulation Instruction Set 1 "
            u8"(bmi1)";
    };

    /// <summary>
    /// Checks whether TSX Hardware Lock Elision is supported.
    /// </summary>
    struct hle final : cpu_info_bit<
            0x00000007, cpu_info_register::ebx, 4> {
        typedef bool type;
        static constexpr auto name = u8"TSX Hardware Lock Elision (hle)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Checks whether AVX2 instructions are supported.
    /// </summary>
    struct avx2 final : cpu_info_bit<
            0x00000007, cpu_info_register::ebx, 5> {
        typedef bool type;
        static constexpr auto name = u8"Advanced Vector Extensions 2 (avx2)";
    };

    struct fdp_exception_only final : cpu_info_bit<
            0x00000007, cpu_info_register::ebx, 6> {
        typedef bool type;
        static constexpr auto name = u8"x87 FPU Data Pointer Register Updated "
            u8"on Exceptions Only (fdp-excptn-only)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Checks whether Supervisor Mode Execution Prevention is supported.
    /// </summary>
    struct smep final : cpu_info_bit<
            0x00000007, cpu_info_register::ebx, 7> {
        typedef bool type;
        static constexpr auto name = u8"Supervisor Mode Execution Prevention "
            u8"(smep)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Checks whether Bit Manipulation Instruction Set 2 (BMI2) is supported.
    /// </summary>
    struct bmi2 final : cpu_info_bit<
            0x00000007, cpu_info_register::ebx, 8> {
        typedef bool type;
        static constexpr auto name = u8"Bit Manipulation Instruction Set 2 "
            u8"(bmi2)";
    };

    /// <summary>
    /// Checks whether Enhanced REP MOVSB/STOSB is supported.
    /// </summary>
    struct erms final : cpu_info_bit<
            0x00000007, cpu_info_register::ebx, 9> {
        typedef bool type;
        static constexpr auto name = u8"Enhanced REP MOVSB/STOSB (erms)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Checks whether the INVPCID instruction is supported.
    /// </summary>
    struct invpcid final : cpu_info_bit<
            0x00000007, cpu_info_register::ebx, 10> {
        typedef bool type;
        static constexpr auto name = u8"INVPCID";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Checks whether TSX Restricted Transactional Memory is supported.
    /// </summary>
    struct rtm final : cpu_info_bit<
            0x00000007, cpu_info_register::ebx, 11> {
        typedef bool type;
        static constexpr auto name = u8"TSX Restricted Transactional Memory "
            u8"(rtm)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Checks whether Intel Resource Director (RDT) Monitoring or AMD Platform
    /// QOS Monitoring is supported.
    /// </summary>
    struct intel_res_dir_monitoring final : cpu_info_bit<
            0x00000007, cpu_info_register::ebx, 12,
            detail::vendor_condition<cpu_vendor::intel>> {
        typedef bool type;
        static constexpr auto name = u8"Intel Resource Director Monitoring "
            u8"(rdt-m)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Checks whether Intel Resource Director (RDT) Monitoring or AMD Platform
    /// QOS Monitoring is supported.
    /// </summary>
    struct adm_plat_qos_monitoring : cpu_info_bit<
            0x00000007, cpu_info_register::ebx, 12,
            detail::vendor_condition<cpu_vendor::amd>> {
            typedef bool type;
            static constexpr auto name = u8"AMD Platform QOS Monitoring "
                u8"(pqm)";
            static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Checks whether x87 FPU CS and DS are deprecated.
    /// </summary>
    struct fcs_fds_deprecated final : cpu_info_bit<
            0x00000007, cpu_info_register::ebx, 13> {
        typedef bool type;
        static constexpr auto name = u8"x87 FPU CS and DS Deprecated "
            u8"(fcs-fds-­deprecation)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Detects whether Intel MPX is supported.
    /// </summary>
    struct mpx final: cpu_info_bit<
            0x00000007, cpu_info_register::ebx, 14> {
        typedef bool type;
        static constexpr auto name = u8"Intel Memory Protection Extensions "
            u8"(mpx)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Detects whether Intel Resource Director (RDT) Allocation or AMD Platform
    /// QOS Enforcement are present.
    /// </summary>
    struct intel_res_dir_allocation final: cpu_info_bit<
            0x00000007, cpu_info_register::ebx, 15,
            detail::vendor_condition<cpu_vendor::intel>> {
        typedef bool type;
        static constexpr auto name = u8"Intel Resource Director Allocation or "
            u8"(rdt-a)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Detects whether Intel Resource Director (RDT) Allocation or AMD Platform
    /// QOS Enforcement are present.
    /// </summary>
    struct amd_plat_qos_enforcement final : cpu_info_bit<
            0x00000007, cpu_info_register::ebx, 15,
            detail::vendor_condition<cpu_vendor::amd>> {
        typedef bool type;
        static constexpr auto name = u8"AMD Platform QOS Enforcement (pqe)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Detects whether the foundation instruction set of AVX-512 is supported.
    /// </summary>
    struct avx512_f final : cpu_info_bit<
            0x00000007, cpu_info_register::ebx, 16> {
        typedef bool type;
        static constexpr auto name = u8"AVX-512 Foundation (avx512-f)";
    };

    /// <summary>
    /// Detects whether AVX-512 DQ is supported.
    /// </summary>
    struct avx512_dq final : cpu_info_bit<
            0x00000007, cpu_info_register::ebx, 17> {
        typedef bool type;
        static constexpr auto name = u8"AVX-512 Double Word and Quad Word "
            u8"Instructions (avx512-dq)";
    };

    /// <summary>
    /// Checks whether the RDSEED instruction is supported.
    /// </summary>
    struct rdseed final : cpu_info_bit<
            0x00000007, cpu_info_register::ebx, 18> {
        typedef bool type;
        static constexpr auto name = u8"RDSEED";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Checks whether Multi-Precision Add-Carry Instruction Extensions are present.
    /// </summary>
    struct adx final : cpu_info_bit<
            0x00000007, cpu_info_register::ebx, 19> {
        typedef bool type;
        static constexpr auto name = u8"Multi-Precision Add-Carry "
            u8"Instruction Extensions (adx)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Checks whether Supervisor Mode Access Prevention is supported.
    /// </summary>
    struct smap final : cpu_info_bit<
            0x00000007, cpu_info_register::ebx, 20> {
        typedef bool type;
        static constexpr auto name = u8"Supervisor Mode Access Prevention "
            u8"(smap)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Detects whether AVX-512 IFMA is supported.
    /// </summary>
    struct avx512_ifma final : cpu_info_bit<
            0x00000007, cpu_info_register::ebx, 21> {
        typedef bool type;
        static constexpr auto name = u8"AVX-512 Integer Fused Multiply-Add "
            u8"Instructions (avx512-ifma)";
    };

    /// <summary>
    /// Checks whether PCOMMIT is supported.
    /// </summary>
    struct pcommit final : cpu_info_bit<
            0x00000007, cpu_info_register::ebx, 22> {
        typedef bool type;
        static constexpr auto name = u8"PCOMMIT";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Checks whether CLFLUSHOPT is supported.
    /// </summary>
    struct clflushopt final : cpu_info_bit<
            0x00000007, cpu_info_register::ebx, 23> {
        typedef bool type;
        static constexpr auto name = u8"CLFLUSHOPT";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Checks whether CLWB is supported.
    /// </summary>
    struct clwb final : cpu_info_bit<
            0x00000007, cpu_info_register::ebx, 24> {
        typedef bool type;
        static constexpr auto name = u8"CLWB";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Checks whether Intel Processor Trace is supported.
    /// </summary>
    struct pt final : cpu_info_bit<
            0x00000007, cpu_info_register::ebx, 25> {
        typedef bool type;
        static constexpr auto name = u8"Intel Procesor Trace (pt)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Detects whether AVX-512 PF is supported.
    /// </summary>
    struct avx512_pf final : cpu_info_bit<
            0x00000007, cpu_info_register::ebx, 26> {
        typedef bool type;
        static constexpr auto name = u8"AVX-512 Prefetch Instructions "
            u8"(avx512-pf)";
    };

    /// <summary>
    /// Detects whether AVX-512 ER is supported.
    /// </summary>
    struct avx512_er final : cpu_info_bit<
        0x00000007, cpu_info_register::ebx, 27> {
        typedef bool type;
        static constexpr auto name = u8"AVX-512 Exponential and Reciprocal "
            u8"Instructions (avx512-er)";
    };

    /// <summary>
    /// Detects whether AVX-512 CD is supported.
    /// </summary>
    struct avx512_cd final : cpu_info_bit<
            0x00000007, cpu_info_register::ebx, 28> {
        typedef bool type;
        static constexpr auto name = u8"AVX-512 Conflict Detection "
            u8"Instructions (avx512-cd)";
    };

    /// <summary>
    /// Checks whether SHA-1 and SHA-256 extensions are supported.
    /// </summary>
    struct sha final : cpu_info_bit<
            0x00000007, cpu_info_register::ebx, 29> {
        typedef bool type;
        static constexpr auto name = u8"SHA-1 and SHA-256 extensions (sha)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Detects whether AVX-512 BW is supported.
    /// </summary>
    struct avx512_bw final : cpu_info_bit<
            0x00000007, cpu_info_register::ebx, 30> {
        typedef bool type;
        static constexpr auto name = u8"AVX-512 Byte and Word Instructions "
            u8"(avx512-bw)";
    };

    /// <summary>
    /// Detects whether AVX-512 VL is supported.
    /// </summary>
    struct avx512_vl final : cpu_info_bit<
            0x00000007, cpu_info_register::ebx, 31> {
        typedef bool type;
        static constexpr auto name = u8"AVX-512 Vector Length Extensions "
            u8"(avx512-vl)";
    };

    /// <summary>
    /// Checks whether the PREFETCHWT1 instruction is supported.
    /// </summary>
    struct prefetchwt1 final : cpu_info_bit<
            0x00000007, cpu_info_register::ecx, 0> {
        typedef bool type;
        static constexpr auto name = u8"PREFETCHWT1";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Detects whether AVX-512 VBMI is supported.
    /// </summary>
    struct avx512_vbmi final : cpu_info_bit<
            0x00000007, cpu_info_register::ecx, 1> {
        typedef bool type;
        static constexpr auto name = u8"AVX-512 Vector Bit Manipulation "
            u8"Instructions (avx512-vbmi)";
    };

    /// <summary>
    /// Checks whether the user-mode instruction prevention is supported.
    /// </summary>
    struct umip final : cpu_info_bit<
            0x00000007, cpu_info_register::ecx, 2> {
        typedef bool type;
        static constexpr auto name = u8"User-mode Instruction Prevention "
            u8"(umip)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Checks whether the memory protection keys for user-mode pages are
    /// supported.
    /// </summary>
    struct pku final : cpu_info_bit<
            0x00000007, cpu_info_register::ecx, 3> {
        typedef bool type;
        static constexpr auto name = u8"Memory Protection Keys for User-Mode "
            u8"Pages (pku)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Checks whether <paramref name="pku" /> is enabled by the OS.
    /// </summary>
    struct ospke final : cpu_info_bit<
            0x00000007, cpu_info_register::ecx, 4> {
        typedef bool type;
        static constexpr auto name = u8"Memory Protection Keys for User-Mode "
            u8"Pages Enabled by Operating System (ospke)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Checks whether timed pause and user-level monitor/wait instructions are
    /// supported.
    /// </summary>
    struct timed_wait_pause final : cpu_info_bit<
            0x00000007, cpu_info_register::ecx, 5> {
        typedef bool type;
        static constexpr auto name = u8"Timed Pause and User-Level "
            u8"Monitor/Wait Instructions (waitpkg)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Checks whether AVX-512 Vector Bit Manipulation Instructions 2 are
    /// supported.
    /// </summary>
    struct avx512_vmbi2 final : cpu_info_bit<
            0x00000007, cpu_info_register::ecx, 6> {
        typedef bool type;
        static constexpr auto name = u8"AVX-512 Vector Bit Manipulation "
            u8"Instructions 2  (avx512-vbmi2)";
    };

    /// <summary>
    /// Checks whether control flow enforcement is supported.
    /// </summary>
    struct control_flow_enforcement final : cpu_info_bit<
            0x00000007, cpu_info_register::ecx, 7> {
        typedef bool type;
        static constexpr auto name = u8"Control Flow Enforcement (cet-ss)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Checks whether Galois Field instructions are supported.
    /// </summary>
    struct gfni final : cpu_info_bit<
            0x00000007, cpu_info_register::ecx, 8> {
        typedef bool type;
        static constexpr auto name = u8"Galois Field Instructions (gfni)";
    };

    /// <summary>
    /// Checks whether Vector AES instructions are supported.
    /// </summary>
    struct vaes final : cpu_info_bit<
            0x00000007, cpu_info_register::ecx, 9> {
        typedef bool type;
        static constexpr auto name = u8"Vector AES Instructions (vaes)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Checks whether CLMUL instructions are supported.
    /// </summary>
    struct vpclmulqdq final : cpu_info_bit<
            0x00000007, cpu_info_register::ecx, 10> {
        typedef bool type;
        static constexpr auto name = u8"CLMUL Instructions (vpclmulqdq)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Checks whether AVX-512 VNNI instructions are supported.
    /// </summary>
    struct avx512_vnni final : cpu_info_bit<
            0x00000007, cpu_info_register::ecx, 11> {
        typedef bool type;
        static constexpr auto name = u8"AVX-512 Vector Neural Network "
            u8"Instructions (avx512-vnni)";
    };

    /// <summary>
    /// Checks whether AVX-512 BITALG instructions are supported.
    /// </summary>
    struct avx512_bitalg final : cpu_info_bit<
            0x00000007, cpu_info_register::ecx, 12> {
        typedef bool type;
        static constexpr auto name = u8"AVX-512 Bit Algorithm Instructions "
            u8"(avx512-bitalg)";
    };

    /// <summary>
    /// Checks whether Total Memory Encryption MSRs are supported.
    /// </summary>
    struct tme_en final : cpu_info_bit<
            0x00000007, cpu_info_register::ecx, 13> {
        typedef bool type;
        static constexpr auto name = u8"Total Memory Encryption MSRs "
            u8"(tme-en)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Checks whether AVX-512 Vector Population Count Double-Word and
    /// Quad-Word instructions are supported.
    /// </summary>
    struct avx512_vpopcntdq final : cpu_info_bit<
            0x00000007, cpu_info_register::ecx, 14> {
        typedef bool type;
        static constexpr auto name = u8"AVX-512 Vector Population Count "
            u8"Double Word and Quad Word (avx512-vpopcntdq)";
    };

    /// <summary>
    /// Checks whether 5-level paging is supported.
    /// </summary>
    struct la57 final : cpu_info_bit<
            0x00000007, cpu_info_register::ecx, 15> {
        typedef bool type;
        static constexpr auto name = u8"5-Level Paging (la57)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// The value of user space MPX address-width adjust used by the BNDLDX
    /// and BNDSTX Intel MPX instructions in 64-bit mode.
    /// </summary>
    struct mawau final : cpu_info_bits<
            0x00000007, cpu_info_register::ecx, 16, 21> {
        typedef bool type;
        static constexpr auto name = u8"The Value of User Space MPX "
            u8"Address-Width Adjust used by the BNDLDX and BNDSTX Intel MPX "
            "Instructions in 64-bit Mode (mawau)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Checks whether read processor ID is supported.
    /// </summary>
    struct read_processor_id final : cpu_info_bit<
            0x00000007, cpu_info_register::ecx, 22> {
        typedef bool type;
        static constexpr auto name = u8"RDPID";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Checks whether AES key locker is supported.
    /// </summary>
    struct key_locker final : cpu_info_bit<
            0x00000007, cpu_info_register::ecx, 23> {
        typedef bool type;
        static constexpr auto name = u8"AES Key Locker (kl)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Checks whether bus lock debug exceptions are supported.
    /// </summary>
    struct bus_lock_detect final : cpu_info_bit<
            0x00000007, cpu_info_register::ecx, 24> {
        typedef bool type;
        static constexpr auto name = u8"Bus lock debug exceptions "
            u8"(bus-lock-detect)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Checks whether the cache line demote instruction is supported.
    /// </summary>
    struct cldemote final : cpu_info_bit<
            0x00000007, cpu_info_register::ecx, 25> {
        typedef bool type;
        static constexpr auto name = u8"CLDEMOTE";
        static constexpr auto uncommon = true;
    };

    struct mprr final : cpu_info_bit<
            0x00000007, cpu_info_register::ecx, 26> {
        typedef bool type;
        static constexpr auto name = u8"mprr";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Checks whether the MOVDIRI instruction is supported.
    /// </summary>
    struct movdiri final : cpu_info_bit<
            0x00000007, cpu_info_register::ecx, 27> {
        typedef bool type;
        static constexpr auto name = u8"MOVDIRI";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Checks whether the MOVDIR64B instruction is supported.
    /// </summary>
    struct movdir64b final : cpu_info_bit<
            0x00000007, cpu_info_register::ecx, 28> {
        typedef bool type;
        static constexpr auto name = u8"MOVDIR64B";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Checks whether enqueue stores are supported.
    /// </summary>
    struct enqcmd final : cpu_info_bit<
            0x00000007, cpu_info_register::ecx, 29> {
        typedef bool type;
        static constexpr auto name = u8"Enqueue Stores and "
            u8"EMQCMD and EMQCMDS Instructions (enqcmd)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Checks whether SGX launch configuration is supported.
    /// </summary>
    struct sgx_launch_config final : cpu_info_bit<
            0x00000007, cpu_info_register::ecx, 30> {
        typedef bool type;
        static constexpr auto name = u8"SGX Launch Configuration (sgx-lc)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Checks whether protection keys for supervisor-mode pages are supported.
    /// </summary>
    struct pks final : cpu_info_bit<
        0x00000007, cpu_info_register::ecx, 30> {
        typedef bool type;
        static constexpr auto name = u8"Protection Keys for Supervisor-Mode "
            u8"Pages (pks)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Checks whether attestation services for Intel SGX are supported.
    /// </summary>
    struct sgx_attestation final : cpu_info_bit<
            0x00000007, cpu_info_register::edx, 1> {
        typedef bool type;
        static constexpr auto name = u8"Attestation Services for Intel SGX "
            u8"(sgx-keys)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Checks whether AVX-512 4-register Neural Network Instructions are
    /// supported.
    /// </summary>
    struct avx512_4vnniw final : cpu_info_bit<
            0x00000007, cpu_info_register::edx, 2> {
        typedef bool type;
        static constexpr auto name = u8"AVX-512 4-register Neural Network "
            u8"Instructions (avx512-4vnniw)";
    };

    /// <summary>
    /// Checks whether AVX-512 4-register Multiply Accumulation Single 
    /// precision instructions are supported.
    /// </summary>
    struct avx512_4fmaps final : cpu_info_bit<
            0x00000007, cpu_info_register::edx, 3> {
        typedef bool type;
        static constexpr auto name = u8"AVX-512 4-register  Multiply "
            "Accumulation Single Precision Instructions (avx512-4fmaps)";
    };

    /// <summary>
    /// Checks whether fast short REP MOVSB is supported.
    /// </summary>
    struct fsrm final : cpu_info_bit<
            0x00000007, cpu_info_register::edx, 4> {
        typedef bool type;
        static constexpr auto name = u8"Fast Short REP MOVSB (fsrm)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Checks whether user inter-processor interrupts are supported.
    /// </summary>
    struct uintr final : cpu_info_bit<
            0x00000007, cpu_info_register::edx, 5> {
        typedef bool type;
        static constexpr auto name = u8"User Inter-Processor Interrupts "
            u8"(uintr)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Checks whether AVX-512 vector intersection instructions on
    /// 32/64-bit integers are supported.
    /// </summary>
    struct avx512_vp2intersect final : cpu_info_bit<
            0x00000007, cpu_info_register::edx, 8> {
        typedef bool type;
        static constexpr auto name = u8"AVX-512 Vector Intersection "
            u8"Instructions on 32/64-Bit Integers (avx512-vp2intersect)";
    };

    /// <summary>
    /// Checks whether special register buffer data sampling mitigations
    /// are supported.
    /// </summary>
    struct srbds_ctrl final : cpu_info_bit<
            0x00000007, cpu_info_register::edx, 9> {
        typedef bool type;
        static constexpr auto name = u8"Special Register Buffer Data "
            u8"Sampling Mitigations (srbds-ctrl)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Checks whether the VERW instruction clears CPU buffers.
    /// </summary>
    struct md_clear final : cpu_info_bit<
            0x00000007, cpu_info_register::edx, 10> {
        typedef bool type;
        static constexpr auto name = u8"VERW Instruction Clears CPU Buffers "
            "(md-clear)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Checks whether the all TSX transactions are aborted.
    /// </summary>
    struct rtm_always_abort final : cpu_info_bit<
            0x00000007, cpu_info_register::edx, 11> {
        typedef bool type;
        static constexpr auto name = u8"All TSX Transactions are Aborted "
            "(rtm-always-abort)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Checks whether the TSX_FORCE_ABORT MSR is available.
    /// </summary>
    struct rtm_force_abort final : cpu_info_bit<
            0x00000007, cpu_info_register::edx, 13> {
        typedef bool type;
        static constexpr auto name = u8"TSX_FORCE_ABORT MSR Available "
            "(rtm-force-abort)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Checks whether the SERIALIZE instruction is supported.
    /// </summary>
    struct serialise final : cpu_info_bit<
            0x00000007, cpu_info_register::edx, 14> {
        typedef bool type;
        static constexpr auto name = u8"SERIALIZE";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Checks whether there is a mixture of CPU types.
    /// </summary>
    struct hybrid final : cpu_info_bit<
            0x00000007, cpu_info_register::edx, 15> {
        typedef bool type;
        static constexpr auto name = u8"Mixture of CPU Types in "
            u8"Processor Topology (hybrid)";
    };

    /// <summary>
    /// Checks whether TSX load address tracking suspend/resume
    /// instructions are supported.
    /// </summary>
    struct tsxldtrk final : cpu_info_bit<
            0x00000007, cpu_info_register::edx, 16> {
        typedef bool type;
        static constexpr auto name = u8"TSUSLDTRK and TRESLDTRK"
            u8"(tsxldtrk)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Checks whether platform configuration (memory encryption 
    /// technologies instructions) are supported.
    /// </summary>
    struct pconfig final : cpu_info_bit<
            0x00000007, cpu_info_register::edx, 18> {
        typedef bool type;
        static constexpr auto name = u8"Platform Configuration "
            u8"(Memory Encryption Technologies Instructions) "
            u8"(pconfig)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Checks whether architectural last branch records are supported.
    /// </summary>
    struct lbr final : cpu_info_bit<
            0x00000007, cpu_info_register::edx, 19> {
        typedef bool type;
        static constexpr auto name = u8"Architectural Last Branch Records "
            u8"(lbr)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Checks whether control flow enforcement is supported.
    /// </summary>
    struct cet_ibt final : cpu_info_bit<
        0x00000007, cpu_info_register::edx, 20> {
        typedef bool type;
        static constexpr auto name = u8"Control Flow Enforcement: Indirect "
            u8"Branch Tracking (lbr)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Checks whether AMX tile computation on BFLOAT16 numbers is supported.
    /// </summary>
    struct amx_bf16 final : cpu_info_bit<
            0x00000007, cpu_info_register::edx, 22> {
        typedef bool type;
        static constexpr auto name = u8"AMX Tile Computation on BFLOAT16 "
            u8"Numbers (amx-bf16)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Checks whether AVX-512 half-precision floating-point arithmetic
    /// instructions are supported.
    /// </summary>
    struct avx512_fp16 final : cpu_info_bit<
            0x00000007, cpu_info_register::edx, 23> {
        typedef bool type;
        static constexpr auto name = u8"AVX-512 Half-Precision "
            u8"Floating-Point Arithmetic Instructions (avx512-fp16)";
    };

    /// <summary>
    /// Checks whether AMX tile load/store instructions are supported.
    /// </summary>
    struct amx_tile final : cpu_info_bit<
            0x00000007, cpu_info_register::edx, 24> {
        typedef bool type;
        static constexpr auto name = u8"AMX Tile Load and Store Instructions "
            u8"(amx-tile)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Checks whether AMX tile computation on 8 bits is supported are
    /// supported.
    /// </summary>
    struct amx_int8 final : cpu_info_bit<
            0x00000007, cpu_info_register::edx, 25> {
        typedef bool type;
        static constexpr auto name = u8"AMX Tile Computation on 8-Bit "
            u8"Integers (amx-int8)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Speculation Control, part of Indirect Branch Control (IBC): Indirect
    /// Branch Restricted Speculation(IBRS) and Indirect Branch Prediction
    /// Barrier(IBPB)
    /// </summary>
    struct speculation_control final : cpu_info_bit<
            0x00000007, cpu_info_register::edx, 26> {
        typedef bool type;
        static constexpr auto name = u8"Speculation Control (spec-ctrl)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Single Thread Indirect Branch Predictor.
    /// </summary>
    struct stibp final : cpu_info_bit<
            0x00000007, cpu_info_register::edx, 27> {
        typedef bool type;
        static constexpr auto name = u8"Single Thread Indirect Branch "
            "Predictor (stibp)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// IA32_FLUSH_CMD MSR is supported.
    /// </summary>
    struct l1d_flush final : cpu_info_bit<
            0x00000007, cpu_info_register::edx, 28> {
        typedef bool type;
        static constexpr auto name = u8"IA32_FLUSH_CMD MSR (l1d-flush)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// IA32_ARCH_CAPABILITIES MSR is supported.
    /// </summary>
    struct arch_capabilities final : cpu_info_bit<
            0x00000007, cpu_info_register::edx, 29> {
        typedef bool type;
        static constexpr auto name = u8"IA32_ARCH_CAPABILITIES MSR "
            u8"(arch-­capabilities)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// IA32_CORE_CAPABILITIES MSR is supported.
    /// </summary>
    struct core_capabilities final : cpu_info_bit<
            0x00000007, cpu_info_register::edx, 30> {
        typedef bool type;
        static constexpr auto name = u8"IA32_CORE_CAPABILITIES MSR "
            u8"(core-­capabilities)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Speculative Store Bypass Disable is supported.
    /// </summary>
    struct ssbd final : cpu_info_bit<
        0x00000007, cpu_info_register::edx, 31> {
        typedef bool type;
        static constexpr auto name = u8"Speculative Store Bypass Disable "
            u8"(ssbd)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// SHA-512 instructions are supported.
    /// </summary>
    struct sha512 final : cpu_info_bit_ex<
            0x00000007, 0x00000001, cpu_info_register::eax, 0> {
        typedef bool type;
        static constexpr auto name = u8"SHA-512 Extensions (sha512)";
    };

    /// <summary>
    /// SM3 hash instructions are supported.
    /// </summary>
    struct sm3 final : cpu_info_bit_ex<
            0x00000007, 0x00000001, cpu_info_register::eax, 1> {
        typedef bool type;
        static constexpr auto name = u8"SM3 Hash Extensions (sm3)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// SM4 cipher instructions are supported.
    /// </summary>
    struct sm4 final : cpu_info_bit_ex<
            0x00000007, 0x00000001, cpu_info_register::eax, 2> {
        typedef bool type;
        static constexpr auto name = u8"SM4 Cipher Extensions (sm4)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Remote atomic operations on integers are supported.
    /// </summary>
    struct rao_int final : cpu_info_bit_ex<
            0x00000007, 0x00000001, cpu_info_register::eax, 3> {
        typedef bool type;
        static constexpr auto name = u8"Remote Atomic Operations on Integers "
            u8"(rao-int)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// AVX Vector Neural Network Instructions (AVX-VNNI) are supported.
    /// </summary>
    struct avx_vnni final : cpu_info_bit_ex<
            0x00000007, 0x00000001, cpu_info_register::eax, 4> {
        typedef bool type;
        static constexpr auto name = u8"AVX Vector Neural Network Instructions "
            u8"(avx-vnni)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// AVX-512 BF16 is supported.
    /// </summary>
    struct avx512_bf16 final : cpu_info_bit_ex<
            0x00000007, 0x00000001, cpu_info_register::eax, 5> {
        typedef bool type;
        static constexpr auto name = u8"AVX-512 Instructions for BFLOAT16 "
            u8"Numbers (avx512-bf16)";
    };

    /// <summary>
    /// Indicates whether linear address space separation (LASS) is supported.
    /// </summary>
    struct lass final : cpu_info_bit_ex<
            0x00000007, 0x00000001, cpu_info_register::eax, 6> {
        typedef bool type;
        static constexpr auto name = u8"Linear Address Space Separation (lass)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Indicates whether CMPccXADD instructions are supported.
    /// </summary>
    struct cmpccxadd final : cpu_info_bit_ex<
            0x00000007, 0x00000001, cpu_info_register::eax, 7> {
        typedef bool type;
        static constexpr auto name = u8"CMPccXADD";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Indicates whether LEAF 0x23 is supported.
    /// </summary>
    struct arch_perf_monitoring final : cpu_info_bit_ex<
            0x00000007, 0x00000001, cpu_info_register::eax, 8> {
        typedef bool type;
        static constexpr auto name = u8"Architectural Performance Monitoring "
            u8"Extended Leaf (archperf­monext)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Indicates whether fast zero-length REP MOVSB is supported.
    /// </summary>
    struct fzrm final : cpu_info_bit_ex<
            0x00000007, 0x00000001, cpu_info_register::eax, 10> {
        typedef bool type;
        static constexpr auto name = u8"Fast Zero-Length REP MOVSB (fzrm)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Indicates whether fast short REP STOSB is supported.
    /// </summary>
    struct fsrs final : cpu_info_bit_ex<
            0x00000007, 0x00000001, cpu_info_register::eax, 11> {
        typedef bool type;
        static constexpr auto name = u8"Fast Short REP STOSB (fsrs)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Indicates whether fast short REP CMPSB and REP SCASB are supported.
    /// </summary>
    struct rsrcs final : cpu_info_bit_ex<
            0x00000007, 0x00000001, cpu_info_register::eax, 12> {
        typedef bool type;
        static constexpr auto name = u8"Fast short REP CMPSB and REP SCASB "
            u8"(rsrcs)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Indicates whether flexible return and event delivery is supported.
    /// </summary>
    struct fred final : cpu_info_bit_ex<
            0x00000007, 0x00000001, cpu_info_register::eax, 17> {
        typedef bool type;
        static constexpr auto name = u8"Flexible Return and Event Delivery "
            u8"(fred)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Indicates whether the LKGS instruction is supported.
    /// </summary>
    struct lkgs final : cpu_info_bit_ex<
            0x00000007, 0x00000001, cpu_info_register::eax, 18> {
        typedef bool type;
        static constexpr auto name = u8"LKGS";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Indicates whether WRMSRNS is supported.
    /// </summary>
    struct non_serialising_wrmsr final : cpu_info_bit_ex<
            0x00000007, 0x00000001, cpu_info_register::eax, 19> {
        typedef bool type;
        static constexpr auto name = u8"Non-Serialising Write to MSRs "
            u8"(wrmsrns)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Indicates whether non-maskable interrupt source reporting is supported.
    /// </summary>
    struct nmi_source_reporting final : cpu_info_bit_ex<
            0x00000007, 0x00000001, cpu_info_register::eax, 20> {
        typedef bool type;
        static constexpr auto name = u8"Non-Maskable Interrupt Source "
            "Reporting (nmi-src)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Indicates whether AMX instructions for FP16 numbers are supported.
    /// </summary>
    struct amx_fp16 final : cpu_info_bit_ex<
            0x00000007, 0x00000001, cpu_info_register::eax, 21> {
        typedef bool type;
        static constexpr auto name = u8"AMX instructions for FP16 numbers "
            u8"(amx-fp16)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Indicates whether the HRESET instruction, the IA32_HRESET_ENABLE MSR and
    /// leaf 0x20 are supported.
    /// </summary>
    struct hreset final : cpu_info_bit_ex<
            0x00000007, 0x00000001, cpu_info_register::eax, 22> {
        typedef bool type;
        static constexpr auto name = u8"Processor History Reset (hreset)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// AVX IFMA instructions are supported.
    /// </summary>
    struct avx_ifma : cpu_info_bit_ex<
            0x00000007, 0x00000001, cpu_info_register::eax, 23> {
        typedef bool type;
        static constexpr auto name = u8"AVX Integer Fused Multiply Add "
            u8"(avx-ifma)";
    };

    /// <summary>
    /// Indicates whether linear address masking is supported.
    /// </summary>
    struct linear_address_masking final : cpu_info_bit_ex<
            0x00000007, 0x00000001, cpu_info_register::eax, 26> {
        typedef bool type;
        static constexpr auto name = u8"Linear Address Masking (lam)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Indicates whether RDMSRLIST and WRMSRLIST instructions are supported.
    /// </summary>
    struct msr_list final : cpu_info_bit_ex<
            0x00000007, 0x00000001, cpu_info_register::eax, 27> {
        typedef bool type;
        static constexpr auto name = u8"RDMSRLIST and WRMSRLIST (msr-list)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// If set, supports INVD instruction execution prevention after BIOS done.
    /// </summary>
    struct invd final : cpu_info_bit_ex<
            0x00000007, 0x00000001, cpu_info_register::eax, 30> {
        typedef bool type;
        static constexpr auto name = u8"INVD";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// MOVRS and PREFETCHRST2 instructions supported (memory read/prefetch
    /// with read-shared hint)
    /// </summary>
    struct movrs final : cpu_info_bit_ex<
            0x00000007, 0x00000001, cpu_info_register::eax, 31> {
        typedef bool type;
        static constexpr auto name = u8"Memory Read/Prefetch with Read-Shared "
            u8"Hint (movrs)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Intel Protected Processor Inventory Number MSRs are available.
    /// </summary>
    struct ppin final : cpu_info_bit_ex<
            0x00000007, 0x00000001, cpu_info_register::ebx, 0> {
        typedef bool type;
        static constexpr auto name = u8"Intel Protected Processor Inventory "
            u8"Number MSRs (ppin)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Total storage encryption is supported.
    /// </summary>
    struct pbndkb final : cpu_info_bit_ex<
            0x00000007, 0x00000001, cpu_info_register::ebx, 1> {
        typedef bool type;
        static constexpr auto name = u8"Total Storage Encryption (pbndkb)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// If 1, then bit 22 of IA32_MISC_ENABLE cannot be set to 1 to limit
    /// the value returned by CPUID.
    /// </summary>
    struct cpuid_max_value final : cpu_info_bit_ex<
            0x00000007, 0x00000001, cpu_info_register::ebx, 3> {
        typedef bool type;
        static constexpr auto name = u8"CPUID Limit (cpuidmaxval-lim-rmv)";
        static constexpr auto uncommon = true;
    };

    //struct mpsadbw_512 final : cpu_info_bit_ex<
    //        0x00000007, 0x00000001, cpu_info_register::ebx, 28> {
    //    typedef bool type;
    //    static constexpr auto name = u8"mpsadbw_512";
    //};

    //struct avx512_rao_fp final : cpu_info_bit_ex<
    //        0x00000007, 0x00000001, cpu_info_register::ebx, 30> {
    //    typedef bool type;
    //    static constexpr auto name = u8"mpsadbw_512";
    //};

    //struct avx512_rao_fp final : cpu_info_bit_ex<
    //        0x00000007, 0x00000001, cpu_info_register::ebx, 31> {
    //    typedef bool type;
    //    static constexpr auto name = u8"mpsadbw_512";
    //};

    /// <summary>
    /// Asymmetric RDT Monitoring capability is available.
    /// </summary>
    struct asymmetric_rdt_mon final : cpu_info_bit_ex<
            0x00000007, 0x00000001, cpu_info_register::ecx, 0> {
        typedef bool type;
        static constexpr auto name = u8"Asymmetric RDT Monitoring "
            u8"Capability (rdt-m-asym)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Asymmetric RDT Allocation capability is available.
    /// </summary>
    struct asymmetric_rdt_alloc final : cpu_info_bit_ex<
            0x00000007, 0x00000001, cpu_info_register::ecx, 1> {
        typedef bool type;
        static constexpr auto name = u8"Asymmetric RDT Allocation "
            u8"Capability (rdt-a-asym)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// X86S (cancelled).
    /// </summary>
    struct legacy_­reduced_­isa final : cpu_info_bit_ex<
            0x00000007, 0x00000001, cpu_info_register::ecx, 2> {
        typedef bool type;
        static constexpr auto name = u8"X86S (legacy-­reduced-isa)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// 64-bit Startup Inter-Processor Interrupt (part of cancelled X86S).
    /// </summary>
    struct sipi64 final : cpu_info_bit_ex<
            0x00000007, 0x00000001, cpu_info_register::ecx, 4> {
        typedef bool type;
        static constexpr auto name = u8"64-bit Startup Inter-Processor "
            u8"Interrupt (sipi64)";
        static constexpr auto uncommon = true;
    };

    /// <summary>
    /// Immediate forms of the RDMSR and WRMSRNS instructions are available.
    /// </summary>
    struct immediate_msr final : cpu_info_bit_ex<
            0x00000007, 0x00000001, cpu_info_register::ecx, 5> {
        typedef bool type;
        static constexpr auto name = u8"Immediate forms of the RDMSR and "
            u8"WRMSRNS Instructions (msr-imm)";
        static constexpr auto uncommon = true;
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
    struct topology_extensions final : cpu_info_bit<
            0x80000001, cpu_info_register::ecx, 22> {
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
