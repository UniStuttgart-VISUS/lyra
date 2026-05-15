// <copyright file="cpu_vendor.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_CPU_VENDOR_H)
#define _LYRA_CPU_VENDOR_H
#pragma once

#include <array>
#include <cinttypes>
#include <tuple>
#include <utility>

#include "visus/autodoc/cpu_info.h"


LYRA_DETAIL_NAMESPACE_BEGIN

/// <summary>
/// Generates the expected value for the vendor string in the CPUID result.
/// </summary>
template<std::uint32_t... Values> class cpu_vendor_identifier final {

private:

    static inline constexpr std::uint32_t swap(
            _In_ std::uint32_t value) noexcept {
        return ((value & 0x000000ff) << 24)
            | ((value & 0x0000ff00) << 8)
            | ((value & 0x00ff0000) >> 8)
            | ((value & 0xff000000) >> 24);
    }

    template<std::size_t... Idx>
    static inline constexpr std::array<std::uint32_t, sizeof...(Values)> make(
            _In_ std::index_sequence<Idx...>) noexcept {
        constexpr std::uint32_t values[] = { Values... };
        return std::array<std::uint32_t, sizeof...(Idx)> {
            swap(values[Idx])...
        };
    }

    static constexpr std::size_t size = (sizeof...(Values) < 3)
        ? sizeof...(Values) : 3;

public:

    static constexpr auto value = make(std::make_index_sequence<size>());
};


/// <summary>
/// Generates a list of expected values for the vendor string in the CPUID
/// result.
/// </summary>
template<class... TIdentifiers> class cpu_vendor final {

private:

    template<std::size_t N, std::size_t... I> static constexpr bool chk_elms(
            _In_ const cpu_info& info,
            _In_ const std::array<std::uint32_t, N>& expected,
            _In_ const std::index_sequence<I...>) noexcept {
        constexpr std::array<std::size_t, 3> swizzle { 1, 3, 2 };
        return ((info.values[swizzle[I]] == expected[I]) && ...);
    }

    template<std::size_t N> static constexpr bool chk_one(
            _In_ const cpu_info& info,
            _In_ const std::array<std::uint32_t, N>& expected) noexcept {
        return chk_elms(info, expected, std::make_index_sequence<N>());
    }

    template<std::size_t... Idx> static constexpr bool chk(
            _In_ const cpu_info& info,
            _In_ const std::index_sequence<Idx...>) noexcept {
        return (chk_one(info, TIdentifiers::value) || ...);
    }

public:

    /// <summary>
    /// The values expected to be found in the CPUID result to identify the
    /// vendor declared by an instance of this template.
    /// </summary>
    static constexpr auto value = std::make_tuple(TIdentifiers::value...);

    /// <summary>
    /// Checke whether the given CPUID results match any of the
    /// <see cref="value" />s of this type.
    /// </summary>
    /// <param name="info"></param>
    /// <returns></returns>
    static constexpr bool check(const cpu_info& info) noexcept {
        return chk(info, std::make_index_sequence<sizeof...(TIdentifiers)>());
    }

    /// <summary>
    /// Retrieves the CPUID information and checks whether the vendor string 
    /// vendor strinch matches the <see cref="value" />s of this type.
    /// </summary>
    /// <returns></returns>
    static constexpr bool check(void) noexcept {
        cpu_info info;
        return (get_cpu_info(info, 0) && check(info));
    }
};

LYRA_DETAIL_NAMESPACE_END


LYRA_NAMESPACE_BEGIN

namespace cpu_vendor {

    /// <summary>
    /// Identifies AMD CPUs via CPUID.
    /// </summary>
    typedef detail::cpu_vendor<
        detail::cpu_vendor_identifier<'Auth', 'enti', 'cAMD'>,
        detail::cpu_vendor_identifier<'AMD ', 'ISBE', 'TTER'>> amd;

    /// <summary>
    /// Identifies ao486 soft CPUs via CPUID.
    /// </summary>
    typedef detail::cpu_vendor<
        detail::cpu_vendor_identifier<'Genu', 'ineA', 'O486'>,
        detail::cpu_vendor_identifier<'MiST', 'er A', 'O486'>> ao486;

    /// <summary>
    /// Identifies IDT WinChip/Centaur CPUs via CPUID.
    /// </summary>
    typedef detail::cpu_vendor<
        detail::cpu_vendor_identifier<'Cent', 'aurH', 'auls'>> centaur;

    /// <summary>
    /// Identifies Compaq FX!32 via CPUID.
    /// </summary>
    typedef detail::cpu_vendor<
        detail::cpu_vendor_identifier<'Comp', 'aq F', 'X!32'>> fx32;

    /// <summary>
    /// Identifies Connectix Virtual PC via CPUID.
    /// </summary>
    typedef detail::cpu_vendor<
        detail::cpu_vendor_identifier<'Conn', 'ecti', 'xCPU'>> connectix;

    /// <summary>
    /// Identifies Cyrix CPUs via CPUID.
    /// </summary>
    typedef detail::cpu_vendor<
        detail::cpu_vendor_identifier<'Cyri', 'xIns', 'tead'>> cyrix;

    /// <summary>
    /// Identifies MCST Elbrus CPUs via CPUID.
    /// </summary>
    typedef detail::cpu_vendor<
        detail::cpu_vendor_identifier<'E2K ', 'MACH', 'INE '>> elbrus;

    /// <summary>
    /// Identifies Hygon CPUs via CPUID.
    /// </summary>
    typedef detail::cpu_vendor<
        detail::cpu_vendor_identifier<'Hygo', 'nGen', 'uine'>> hygon;

    /// <summary>
    /// Identifies Insignia RealPC via CPUID.
    /// </summary>
    typedef detail::cpu_vendor<
        detail::cpu_vendor_identifier<'Insi', 'gnia', ' 586'>> insignia;

    /// <summary>
    /// Identifies Intel CPUs via CPUID.
    /// </summary>
    typedef detail::cpu_vendor<
        detail::cpu_vendor_identifier<'Genu', 'ineI', 'ntel'>,
        detail::cpu_vendor_identifier<'Genu', 'ineI', 'otel'>> intel;

    /// <summary>
    /// Identifies Neko Project II via CPUID.
    /// </summary>
    typedef detail::cpu_vendor<
        detail::cpu_vendor_identifier<'Neko', ' Pro', 'ject'>> neko_project;

    /// <summary>
    /// Identifies NexGen CPUs via CPUID.
    /// </summary>
    typedef detail::cpu_vendor<
        detail::cpu_vendor_identifier<'NexG', 'enDr', 'iven'>> nexgen;

    /// <summary>
    /// Identifies National Semiconductor CPUs via CPUID.
    /// </summary>
    typedef detail::cpu_vendor<
        detail::cpu_vendor_identifier<'Geod', 'e by', ' NSC'>> nsc;

    /// <summary>
    /// Identifies PowerVM Lx86 via CPUID.
    /// </summary>
    typedef detail::cpu_vendor<
        detail::cpu_vendor_identifier<'Powe', 'rVM ', 'Lx86'>> power_vm;

    /// <summary>
    /// Identifies RDC Semiconductor CPUs via CPUID.
    /// </summary>
    typedef detail::cpu_vendor<
        detail::cpu_vendor_identifier<'Genu', 'ine ', ' RDC'>> rdc;

    /// <summary>
    /// Identifies Rise CPUs via CPUID.
    /// </summary>
    typedef detail::cpu_vendor<
        detail::cpu_vendor_identifier<'Rise', 'Rise', 'Rise'>> rise;

    /// <summary>
    /// Identifies SiS CPUs via CPUID.
    /// </summary>
    typedef detail::cpu_vendor<
        detail::cpu_vendor_identifier<'SiS ', 'SiS ', 'SiS '>> sis;

    /// <summary>
    /// Identifies Transmeta CPUs via CPUID.
    /// </summary>
    typedef detail::cpu_vendor<
        detail::cpu_vendor_identifier<'Tran', 'smet', 'aCPU'>,
        detail::cpu_vendor_identifier<'Genu', 'ineT', 'Mx86'>> transmeta;

    /// <summary>
    /// Identifies UMC CPUs via CPUID.
    /// </summary>
    typedef detail::cpu_vendor<
        detail::cpu_vendor_identifier<'UMC ', 'UMC ', 'UMC '>> umc;

    /// <summary>
    /// Identifies VIA CPUs via CPUID.
    /// </summary>
    typedef detail::cpu_vendor<
        detail::cpu_vendor_identifier<'VIA ', 'VIA ', 'VIA '>> via;

    /// <summary>
    /// Identifies Microsoft Virtual PC via CPUID.
    /// </summary>
    typedef detail::cpu_vendor<
        detail::cpu_vendor_identifier<'Virt', 'ual ', 'CPU '>> virtual_pc;

    /// <summary>
    /// Identifies DM&amp;P Vortex86 CPUs via CPUID.
    /// </summary>
    typedef detail::cpu_vendor<
        detail::cpu_vendor_identifier<'Vort', 'ex86', ' SoC'>> vortex;

    /// <summary>
    /// Identifies Zhaoxin CPUs via CPUID.
    /// </summary>
    typedef detail::cpu_vendor<
        detail::cpu_vendor_identifier<'  Sh', 'angh', 'ai  '>> zhaoxin;

} /* namespace cpu_vendor */


namespace hypervisor {

    /// <summary>
    /// Identifies Project ACRN via CPUID with EAX=0x40000000.
    /// </summary>
    typedef detail::cpu_vendor<
        detail::cpu_vendor_identifier<'ACRN', 'ACRN', 'ACRN'>> acrn;

    /// <summary>
    /// Identifies bhyve via CPUID with EAX=0x40000000.
    /// </summary>
    typedef detail::cpu_vendor<
        detail::cpu_vendor_identifier<'BHyV', 'E BH', 'yVE '>,
        detail::cpu_vendor_identifier<'bhyv', 'e bh', 'yve '>> bhyve;

    /// <summary>
    /// Identifies FEX-Emu via CPUID with EAX=0x40000000.
    /// </summary>
    typedef detail::cpu_vendor<
        detail::cpu_vendor_identifier<'FEXI', 'FEXI', 'EMU\0'>> fex_emu;

    /// <summary>
    /// Identifies Microsoft Hyper-V via CPUID with EAX=0x40000000.
    /// </summary>
    typedef detail::cpu_vendor<
        detail::cpu_vendor_identifier<'Micr', 'osof', 't Hv'>> hyperv;

    /// <summary>
    /// Identifies Lockheed Martin LMHS via CPUID with EAX=0x40000000.
    /// </summary>
    typedef detail::cpu_vendor<
        detail::cpu_vendor_identifier<'SRES', 'RESR', 'ESRE'>> lmhs;

    /// <summary>
    /// Identifies Bitdefender Napoca via CPUID with EAX=0x40000000.
    /// </summary>
    typedef detail::cpu_vendor<
        detail::cpu_vendor_identifier<'Napo', 'cahv', '    '>> napoca;

    /// <summary>
    /// Identifies NetBSD NVMM via CPUID with EAX=0x40000000.
    /// </summary>
    typedef detail::cpu_vendor<
        detail::cpu_vendor_identifier<'___ ', 'NVMM', ' ___'>> nvmm;

    /// <summary>
    /// Identifies OpenBSD VMM via CPUID with EAX=0x40000000.
    /// </summary>
    typedef detail::cpu_vendor<
        detail::cpu_vendor_identifier<'Open', 'BSDV', 'MM58'>> open_bsd_vmm;

    /// <summary>
    /// Identifies Parallels via CPUID with EAX=0x40000000.
    /// </summary>
    typedef detail::cpu_vendor<
        detail::cpu_vendor_identifier<'prl ', 'hype', 'rv  '>,
        detail::cpu_vendor_identifier<' lrp', 'epyh', '  vr'>> parallels;

    /// <summary>
    /// Identifies QEMU via CPUID with EAX=0x40000000.
    /// </summary>
    typedef detail::cpu_vendor<
        detail::cpu_vendor_identifier<'TCGT', 'CGTC', 'GTCG'>> qemu;

    /// <summary>
    /// Identifies QNX via CPUID with EAX=0x40000000.
    /// </summary>
    typedef detail::cpu_vendor<
        detail::cpu_vendor_identifier<'QXNQ', 'SBMV'>> qnx;

    /// <summary>
    /// Identifies Siemens Jailhouse via CPUID with EAX=0x40000000.
    /// </summary>
    typedef detail::cpu_vendor<
        detail::cpu_vendor_identifier<'Jail', 'hous', 'e\0\0\0'>> siemens;

    /// <summary>
    /// Identifies Intel KGT (Trusty) via CPUID with EAX=0x40000000.
    /// </summary>
    typedef detail::cpu_vendor<
        detail::cpu_vendor_identifier<'EVMM', 'EVMM', 'EVMM'>> trusty;

    /// <summary>
    /// Identifies Unisys s-Par via CPUID with EAX=0x40000000.
    /// </summary>
    typedef detail::cpu_vendor<
        detail::cpu_vendor_identifier<'Unis', 'ysSp', 'ar64'>> unisys;

    /// <summary>
    /// Identifies Virtual Box via CPUID with EAX=0x40000000.
    /// </summary>
    typedef detail::cpu_vendor<
        detail::cpu_vendor_identifier<'VBox', 'VBox', 'VBox'>> virtual_box;

    /// <summary>
    /// Identifies VMWare via CPUID with EAX=0x40000000.
    /// </summary>
    typedef detail::cpu_vendor<
        detail::cpu_vendor_identifier<'VMwa', 'reVM', 'ware'>> vmware;

    /// <summary>
    /// Identifies Xen via CPUID with EAX=0x40000000.
    /// </summary>
    typedef detail::cpu_vendor<
        detail::cpu_vendor_identifier<'XenV', 'MMXe', 'nVMM'>> xen;
}

LYRA_NAMESPACE_END

#endif /* !defined(_LYRA_CPU_VENDOR_H) */
