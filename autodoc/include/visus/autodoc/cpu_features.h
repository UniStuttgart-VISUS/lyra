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
        static constexpr auto name = u8"SSE 3";
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
        static constexpr auto name = u8"SSSE3";
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
        static constexpr auto name = u8"SSE 4.1";
    };

    /// <summary>
    /// Checks whether SSE 4.2 is supported.
    /// </summary>
    struct sse4_2 final : simd_detector<simd_instruction_set::sse4_2> {
        typedef bool type;
        static constexpr auto name = u8"SSE 4.2";
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
        static constexpr auto name = u8"AVX";
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
        static constexpr auto name = u8"SSE";
    };

    /// <summary>
    /// Indicates whether the SSE2 instruction set is supported.
    /// </summary>
    struct sse2 final : simd_detector<simd_instruction_set::sse2> {
        typedef bool type;
        static constexpr auto name = u8"SSE 2";
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
