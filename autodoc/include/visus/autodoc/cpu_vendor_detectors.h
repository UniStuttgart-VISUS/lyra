// <copyright file="cpu_vendor_detectors.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_CPU_VENDOR_DETECTORS_H)
#define _LYRA_CPU_VENDOR_DETECTORS_H
#pragma once

#include "visus/autodoc/cpu_info_detector.h"


LYRA_NAMESPACE_BEGIN

namespace cpu_vendor_detectors {

    /// <summary>
    /// Detects whether the CPU vendor is AMD by checking the CPUID.
    /// </summary>
    typedef cpu_info_vendor_detector<
        detail::cpu_info_vendor_register('Auth'),
        detail::cpu_info_vendor_register('enti'),
        detail::cpu_info_vendor_register('cAMD')> amd;

    /// <summary>
    /// Detects whether the CPU vendor is Centraur by checking the CPUID.
    /// </summary>
    typedef cpu_info_vendor_detector<
        detail::cpu_info_vendor_register('Cent'),
        detail::cpu_info_vendor_register('aurH'),
        detail::cpu_info_vendor_register('auls')> centaur;

    /// <summary>
    /// Detects whether the CPU vendor is Cyrix by checking the CPUID.
    /// </summary>
    typedef cpu_info_vendor_detector<
        detail::cpu_info_vendor_register('Cyri'),
        detail::cpu_info_vendor_register('xIns'),
        detail::cpu_info_vendor_register('tead')> cyrix;

    /// <summary>
    /// Detects whether the CPU is a Hyper-V hypervisor by checking the CPUID.
    /// </summary>
    typedef cpu_info_vendor_detector<
        detail::cpu_info_vendor_register('Micr'),
        detail::cpu_info_vendor_register('osof'),
        detail::cpu_info_vendor_register('t Hv')> hyperv;

    /// <summary>
    /// Detects whether the CPU vendor is Intel by checking the CPUID.
    /// </summary>
    typedef cpu_info_vendor_detector<
        detail::cpu_info_vendor_register('Genu'),
        detail::cpu_info_vendor_register('ineI'),
        detail::cpu_info_vendor_register('ntel')> intel;

    /// <summary>
    /// Detects whether the CPU is a KVM hypervisor by checking the CPUID.
    /// </summary>
    typedef cpu_info_vendor_detector<
        detail::cpu_info_vendor_register('KVMK'),
        detail::cpu_info_vendor_register('VMKV'),
        detail::cpu_info_vendor_register('MKVM')> kvm;

    /// <summary>
    /// Detects whether the CPU vendor is Transmeta by checking the CPUID.
    /// </summary>
    typedef cpu_info_vendor_detector<
        detail::cpu_info_vendor_register('Tran'),
        detail::cpu_info_vendor_register('smet'),
        detail::cpu_info_vendor_register('aCPU')> transmeta;


    /// <summary>
    /// Detects whether the CPU is a Virtual Box hypervisor by checking the
    /// CPUID.
    /// </summary>
    typedef cpu_info_vendor_detector<
        detail::cpu_info_vendor_register('VBox'),
        detail::cpu_info_vendor_register('VBox'),
        detail::cpu_info_vendor_register('VBox')> virtual_box;

    /// <summary>
    /// Detects whether the CPU is a Virtual PC hypervisor by checking the
    /// CPUID.
    /// </summary>
    typedef cpu_info_vendor_detector<
        detail::cpu_info_vendor_register('Virt'),
        detail::cpu_info_vendor_register('ual '),
        detail::cpu_info_vendor_register('CPU ')> virtual_pc;

    /// <summary>
    /// Detects whether the CPU is a VMware hypervisor by checking the CPUID.
    /// </summary>
    typedef cpu_info_vendor_detector<
        detail::cpu_info_vendor_register('VMwa'),
        detail::cpu_info_vendor_register('reVM'),
        detail::cpu_info_vendor_register('ware')> vmware;

    /// <summary>
    /// Detects whether the CPU is a Xen hypervisor by checking the CPUID.
    /// </summary>
    typedef cpu_info_vendor_detector<
        detail::cpu_info_vendor_register('XenV'),
        detail::cpu_info_vendor_register('MMXe'),
        detail::cpu_info_vendor_register('nVMM')> xen;

} /* namespace cpu_vendor_detectors */

LYRA_NAMESPACE_END

#endif /* !defined(_LYRA_CPU_VENDOR_DETECTORS_H) */
