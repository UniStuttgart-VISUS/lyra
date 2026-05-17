// <copyright file="cpu.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "visus/autodoc/cpu.h"

#include <algorithm>
#include <array>
#include <cstring>
#include <iomanip>
#include <map>
#include <set>
#include <sstream>

#include "visus/autodoc/cpu_features.h"
#include "visus/autodoc/cpu_info.h"
#include "visus/autodoc/multi_sz.h"
#include "visus/autodoc/simd_detector.h"

#include "os_cpu_info.h"
#include "property_set_impl.h"


/// <summary>
/// Adds the CPU topology from CPUID queries to <paramref name="ps" />.
/// </summary>
/// <remarks>See https://docs.kernel.org/arch/x86/topology.html</remarks>
bool add_amd_topology(_Inout_ LYRA_DETAIL_NAMESPACE::property_set_impl& ps) {
    using namespace LYRA_NAMESPACE;
    const cpu_features::topology_leaf_b leaf_b;
    const cpu_features::topology_extensions topo_ext;
    cpu_info info;

    if (get_cpu_info(info, 0x80000026)) {
        // Extended CPU Topology is supported.
    }

    if (leaf_b && get_cpu_info(info, 0x0000000b)) {
        // Extended Topology Enumeration is supported.
    }

    if (get_cpu_info(info, 0x80000008)) {
        // Size Identifiers is supported.
    }

    if (topo_ext && get_cpu_info(info, 0x8000001e)) {
        // Extended APIC ID, Core Identifiers, Node Identifiers is supported.
    }

    return false;
}


/// <summary>
/// Adds a CPU feature or property to <paramref name="ps" />.
/// </summary>
template<class TFeature>
void add_cpu_feature(_Inout_ LYRA_DETAIL_NAMESPACE::property_set_impl& ps,
        _In_ const LYRA_NAMESPACE::cpu_info& info) {
    ps.add<TFeature>(TFeature::get(info));
};


/// <summary>
/// Adds the CPU topology from CPUID queries to <paramref name="ps" />.
/// </summary>
/// <remarks>
/// <para>See https://docs.kernel.org/arch/x86/topology.html. This function can
/// also be used for Centaur and Zhaoxin CPUs.</para>
/// </remarks>
bool add_intel_topology(_Inout_ LYRA_DETAIL_NAMESPACE::property_set_impl& ps) {
    using namespace LYRA_NAMESPACE;
    const cpu_features::extended_toplogy_enumeration ext_topo;
    const cpu_features::topology_leaf_b leaf_b;
    const cpu_features::topology_extensions topo_ext;
    cpu_info info;

    if (ext_topo && get_cpu_info(info, 0x0000001f)) {

    }

    if (leaf_b && get_cpu_info(info, 0x0000000b)) {
    }

    if (get_cpu_info(info, 0x00000004)) {
    }

    if (get_cpu_info(info, 0x00000001)) {
    }

    return false;
}


/*
 * LYRA_NAMESPACE::cpu::get
 */
LYRA_NAMESPACE::property_set LYRA_NAMESPACE::cpu::get(
        _In_ const collection_flags flags) {
    detail::property_set_impl ps;

    detail::checked_add<cpu::cpuid>(ps, flags, get_cpuid(flags));
    ps.merge(get_topology(flags));

    return property_set(std::move(ps));
}


/*
 * LYRA_NAMESPACE::cpu::get_cpuid
 */
LYRA_NAMESPACE::property_set LYRA_NAMESPACE::cpu::get_cpuid(
        _In_ const collection_flags flags) {
    constexpr auto reg_size = sizeof(std::uint32_t);
    detail::property_set_impl ps;

    {
        constexpr auto brand_comps = 3;
        std::array<char, brand_comps * sizeof(cpu_info) + 1> brand { 0 };
        cpu_info info;

        std::uint32_t i = 0;
        for (; i < brand_comps; ++i) {
            if (get_cpu_info(info, 0x80000002 + i)) {
                ::memcpy(brand.data() + i * sizeof(cpu_info),
                    &info,
                    sizeof(cpu_info));

            } else {
                i = 0;
                break;
            }
        }

        if (detail::check_flags<cpu::brand_string>(flags)
                && (i == brand_comps)) {
            ps.add<cpu::brand_string>(brand.data());
        }
    }

    if (detail::check_flags<cpu::cpuid>(flags)) {
        std::vector<cpu_info> infos(get_cpu_info());
        get_cpu_info(infos.data(), infos.size());
        std::vector<cpu_info> ex_infos(get_extended_cpu_info());
        get_extended_cpu_info(ex_infos.data(), ex_infos.size());

        detail::property_set_impl info_set;
        const auto add_infos = [&](const std::vector<cpu_info>& infos,
                const std::uint32_t base) {
            for (auto it = infos.begin(); it != infos.end(); ++it) {
                std::uint32_t fun = base + std::distance(infos.begin(), it);
                std::stringstream ss;
                ss << "0x"
                    << std::setw(2 * sizeof(fun))
                    << std::setfill('0')
                    << std::hex
                    << fun;

                multi_sz values;
                for (auto val : it->values) {
                    std::stringstream ss;
                    ss << "0x"
                        << std::setw(2 * sizeof(val))
                        << std::setfill('0')
                        << std::hex
                        << static_cast<std::uint32_t>(val);
                    const auto s = ss.str();
                    values.add(s.c_str());
                }

                info_set.add(ss.str(), values);
            }
        };

        add_infos(infos, 0);
        add_infos(ex_infos, 0x80000000);

        ps.add<cpu::cpuid>(property_set(std::move(info_set)));
    }

    {
        cpu_info info;
        if (get_cpu_info(info, 0)) {
            std::array<char, 3 * reg_size + 1> vendor{ 0 };
            ::memcpy(vendor.data() + 0, &info.registers.ebx, reg_size);
            ::memcpy(vendor.data() + 4, &info.registers.edx, reg_size);
            ::memcpy(vendor.data() + 8, &info.registers.ecx, reg_size);

            if (detail::check_sensitive<cpu::vendor>(flags)) {
                ps.add<cpu::vendor>(vendor.data());
            }
        }
    }

    if (detail::check_flags<cpu::features>(flags)) {
        cpu_info info;
        detail::property_set_impl props;

        if (get_cpu_info(info, 0x00000001)) {
            // EAX
            ::add_cpu_feature<cpu_features::stepping_id>(ps, info);
            const auto model = cpu_features::model::get(info);
            const auto ext_model = cpu_features::extended_model::get(info);
            ps.add(u8"Model", (ext_model << 4) | model);
            const auto family = cpu_features::family_id::get(info);
            const auto ext_family = cpu_features::extended_family_id::get(info);
            ps.add(u8"Family", (ext_family << 4) | family);
            ::add_cpu_feature<cpu_features::processor_type>(ps, info);
            ::add_cpu_feature<cpu_features::sse3>(ps, info);
            ::add_cpu_feature<cpu_features::pclmulqdq>(ps, info);
            ::add_cpu_feature<cpu_features::dtes64>(ps, info);
            ::add_cpu_feature<cpu_features::monitor>(ps, info);
            ::add_cpu_feature<cpu_features::ds_cpl>(ps, info);
            ::add_cpu_feature<cpu_features::vmx>(ps, info);
            ::add_cpu_feature<cpu_features::smx>(ps, info);
            ::add_cpu_feature<cpu_features::est>(ps, info);
            ::add_cpu_feature<cpu_features::tm2>(ps, info);
            ::add_cpu_feature<cpu_features::ssse3>(ps, info);
            ::add_cpu_feature<cpu_features::cnxt_id>(ps, info);
            ::add_cpu_feature<cpu_features::sdbg>(ps, info);
            ::add_cpu_feature<cpu_features::fma>(ps, info);
            ::add_cpu_feature<cpu_features::cx16>(ps, info);
            ::add_cpu_feature<cpu_features::xtpr>(ps, info);
            ::add_cpu_feature<cpu_features::pdcm>(ps, info);
            ::add_cpu_feature<cpu_features::pcid>(ps, info);
            ::add_cpu_feature<cpu_features::dca>(ps, info);
            ::add_cpu_feature<cpu_features::sse4_1>(ps, info);
            ::add_cpu_feature<cpu_features::sse4_2>(ps, info);
            ::add_cpu_feature<cpu_features::x2apic>(ps, info);
            ::add_cpu_feature<cpu_features::movbe>(ps, info);
            ::add_cpu_feature<cpu_features::popcnt>(ps, info);
            ::add_cpu_feature<cpu_features::tsc_deadline>(ps, info);
            ::add_cpu_feature<cpu_features::aes>(ps, info);
            ::add_cpu_feature<cpu_features::xsave>(ps, info);
            ::add_cpu_feature<cpu_features::osxsave>(ps, info);
            ::add_cpu_feature<cpu_features::avx>(ps, info);
            ::add_cpu_feature<cpu_features::f16c>(ps, info);
            ::add_cpu_feature<cpu_features::rdrnd>(ps, info);
            ::add_cpu_feature<cpu_features::hypervisor>(ps, info);

            // EBX
            ::add_cpu_feature<cpu_features::brand>(ps, info);
            ::add_cpu_feature<cpu_features::clflush_size>(ps, info);
            ::add_cpu_feature<cpu_features::max_cpu_id>(ps, info);
            ::add_cpu_feature<cpu_features::apic_id>(ps, info);

            // ECX


            // EDX
            ::add_cpu_feature<cpu_features::fpu>(ps, info);
            ::add_cpu_feature<cpu_features::vme>(ps, info);
            ::add_cpu_feature<cpu_features::de>(ps, info);
            ::add_cpu_feature<cpu_features::pse>(ps, info);
            ::add_cpu_feature<cpu_features::tsc>(ps, info);
            ::add_cpu_feature<cpu_features::msr>(ps, info);
            ::add_cpu_feature<cpu_features::pae>(ps, info);
            ::add_cpu_feature<cpu_features::mce>(ps, info);
            ::add_cpu_feature<cpu_features::cx8>(ps, info);
            ::add_cpu_feature<cpu_features::apic>(ps, info);
            ::add_cpu_feature<cpu_features::sep>(ps, info);
            ::add_cpu_feature<cpu_features::mtrr>(ps, info);
            ::add_cpu_feature<cpu_features::pge>(ps, info);
            ::add_cpu_feature<cpu_features::mca>(ps, info);
            ::add_cpu_feature<cpu_features::cmov>(ps, info);
            ::add_cpu_feature<cpu_features::pse36>(ps, info);
            ::add_cpu_feature<cpu_features::psn>(ps, info);
            ::add_cpu_feature<cpu_features::clfsh>(ps, info);
            ::add_cpu_feature<cpu_features::nx>(ps, info);
            ::add_cpu_feature<cpu_features::ds>(ps, info);
            ::add_cpu_feature<cpu_features::acpi>(ps, info);
            ::add_cpu_feature<cpu_features::mmx>(ps, info);
            ::add_cpu_feature<cpu_features::fxsr>(ps, info);
            ::add_cpu_feature<cpu_features::sse>(ps, info);
            ::add_cpu_feature<cpu_features::sse2>(ps, info);
            ::add_cpu_feature<cpu_features::ss>(ps, info);
            ::add_cpu_feature<cpu_features::htt>(ps, info);
            ::add_cpu_feature<cpu_features::ia64>(ps, info);
            ::add_cpu_feature<cpu_features::pbe>(ps, info);
        }

        if (get_cpu_info(info, 0x00000005)) {
            ::add_cpu_feature<cpu_features::emx>(ps, info);
            if (cpu_features::emx::get(info)) {
                ::add_cpu_feature<cpu_features::min_monitor_size>(ps, info);
                ::add_cpu_feature<cpu_features::max_monitor_size>(ps, info);
                ::add_cpu_feature<cpu_features::ibe>(ps, info);
                ::add_cpu_feature<cpu_features::monitorless_mwait>(ps, info);
                ::add_cpu_feature<cpu_features::c0_sub_states>(ps, info);
                ::add_cpu_feature<cpu_features::c1_sub_states>(ps, info);
                ::add_cpu_feature<cpu_features::c2_sub_states>(ps, info);
                ::add_cpu_feature<cpu_features::c3_sub_states>(ps, info);
                ::add_cpu_feature<cpu_features::c4_sub_states>(ps, info);
                ::add_cpu_feature<cpu_features::c5_sub_states>(ps, info);
                ::add_cpu_feature<cpu_features::c6_sub_states>(ps, info);
                ::add_cpu_feature<cpu_features::c7_sub_states>(ps, info);
            }
        }

        if (get_cpu_info(info, 0x00000006)) {
            // EAX
            ::add_cpu_feature<cpu_features::dts>(ps, info);
            ::add_cpu_feature<cpu_features::turbo_boost>(ps, info);
            ::add_cpu_feature<cpu_features::arat>(ps, info);
            ::add_cpu_feature<cpu_features::pln>(ps, info);
            ::add_cpu_feature<cpu_features::ecmd>(ps, info);
            ::add_cpu_feature<cpu_features::ptm>(ps, info);
            ::add_cpu_feature<cpu_features::hwp>(ps, info);
            ::add_cpu_feature<cpu_features::hwp_notification>(ps, info);
            ::add_cpu_feature<cpu_features::hwp_activity_window>(ps, info);
            ::add_cpu_feature<cpu_features::hwp_epp>(ps, info);
            ::add_cpu_feature<cpu_features::hwp_pkg>(ps, info);
            ::add_cpu_feature<cpu_features::hdc>(ps, info);
            ::add_cpu_feature<cpu_features::turbo_boost_max>(ps, info);
            ::add_cpu_feature<cpu_features::hwp_capabilities>(ps, info);
            ::add_cpu_feature<cpu_features::hwp_peci_override>(ps, info);
            ::add_cpu_feature<cpu_features::flexible_hwp>(ps, info);
            ::add_cpu_feature<cpu_features::hwp_request_fast_access>(ps, info);
            ::add_cpu_feature<cpu_features::hw_feedback_interface>(ps, info);
            ::add_cpu_feature<cpu_features::hwp_request_ignore_idle>(ps, info);
            ::add_cpu_feature<cpu_features::hwp_control_msr>(ps, info);
            ::add_cpu_feature<cpu_features::thread_director>(ps, info);

            // EBX
            // ECX
            // EDX
        }

        if (get_cpu_info(info, 0x00000007)) {
            ::add_cpu_feature<cpu_features::extended_features>(ps, info);

            // EAX
            ::add_cpu_feature<cpu_features::fsgsbase>(ps, info);
            ::add_cpu_feature<cpu_features::tsc_adjust>(ps, info);
            ::add_cpu_feature<cpu_features::sgx>(ps, info);
            ::add_cpu_feature<cpu_features::bmi1>(ps, info);
            ::add_cpu_feature<cpu_features::hle>(ps, info);
            ::add_cpu_feature<cpu_features::avx2>(ps, info);
            ::add_cpu_feature<cpu_features::fdp_exception_only>(ps, info);
            ::add_cpu_feature<cpu_features::smep>(ps, info);
            ::add_cpu_feature<cpu_features::bmi2>(ps, info);
            ::add_cpu_feature<cpu_features::erms>(ps, info);
            ::add_cpu_feature<cpu_features::invpcid>(ps, info);
            ::add_cpu_feature<cpu_features::rtm>(ps, info);
            ::add_cpu_feature<cpu_features::intel_res_dir_monitoring>(ps, info);
            ::add_cpu_feature<cpu_features::adm_plat_qos_monitoring>(ps, info);
            ::add_cpu_feature<cpu_features::fcs_fds_deprecated>(ps, info);
            ::add_cpu_feature<cpu_features::mpx>(ps, info);
            ::add_cpu_feature<cpu_features::intel_res_dir_allocation>(ps, info);
            ::add_cpu_feature<cpu_features::amd_plat_qos_enforcement>(ps, info);
            ::add_cpu_feature<cpu_features::avx512_f>(ps, info); 
            ::add_cpu_feature<cpu_features::avx512_dq>(ps, info);
            ::add_cpu_feature<cpu_features::rdseed>(ps, info);
            ::add_cpu_feature<cpu_features::adx>(ps, info);
            ::add_cpu_feature<cpu_features::smap>(ps, info);
            ::add_cpu_feature<cpu_features::avx512_ifma>(ps, info);
            ::add_cpu_feature<cpu_features::pcommit>(ps, info);
            ::add_cpu_feature<cpu_features::clflushopt>(ps, info);
            ::add_cpu_feature<cpu_features::clwb>(ps, info);
            ::add_cpu_feature<cpu_features::pt>(ps, info);
            ::add_cpu_feature<cpu_features::avx512_pf>(ps, info);
            ::add_cpu_feature<cpu_features::avx512_er>(ps, info);
            ::add_cpu_feature<cpu_features::avx512_cd>(ps, info);
            ::add_cpu_feature<cpu_features::sha>(ps, info);
            ::add_cpu_feature<cpu_features::avx512_bw>(ps, info);
            ::add_cpu_feature<cpu_features::avx512_vl>(ps, info);

            // ECX
            ::add_cpu_feature<cpu_features::prefetchwt1>(ps, info);
            ::add_cpu_feature<cpu_features::avx512_vbmi>(ps, info);
            ::add_cpu_feature<cpu_features::umip>(ps, info);
            ::add_cpu_feature<cpu_features::pku>(ps, info);
            ::add_cpu_feature<cpu_features::ospke>(ps, info);
            ::add_cpu_feature<cpu_features::timed_wait_pause>(ps, info);
            ::add_cpu_feature<cpu_features::avx512_vmbi2>(ps, info);
            ::add_cpu_feature<cpu_features::control_flow_enforcement>(ps, info);
            ::add_cpu_feature<cpu_features::gfni>(ps, info);
            ::add_cpu_feature<cpu_features::vaes>(ps, info);
            ::add_cpu_feature<cpu_features::vpclmulqdq>(ps, info);
            ::add_cpu_feature<cpu_features::avx512_vnni>(ps, info);
            ::add_cpu_feature<cpu_features::avx512_bitalg>(ps, info);
            ::add_cpu_feature<cpu_features::tme_en>(ps, info);
            ::add_cpu_feature<cpu_features::avx512_vpopcntdq>(ps, info);
            ::add_cpu_feature<cpu_features::la57>(ps, info);
            ::add_cpu_feature<cpu_features::mawau>(ps, info);
            ::add_cpu_feature<cpu_features::read_processor_id>(ps, info);
            ::add_cpu_feature<cpu_features::key_locker>(ps, info);
            ::add_cpu_feature<cpu_features::bus_lock_detect>(ps, info);
            ::add_cpu_feature<cpu_features::cldemote>(ps, info);
            ::add_cpu_feature<cpu_features::mprr>(ps, info);
            ::add_cpu_feature<cpu_features::movdiri>(ps, info);
            ::add_cpu_feature<cpu_features::movdir64b>(ps, info);
            ::add_cpu_feature<cpu_features::enqcmd>(ps, info);
            ::add_cpu_feature<cpu_features::sgx_launch_config>(ps, info);
            ::add_cpu_feature<cpu_features::pks>(ps, info);

            // EDX
            ::add_cpu_feature<cpu_features::sgx_attestation>(ps, info);
            ::add_cpu_feature<cpu_features::avx512_4vnniw>(ps, info);
            ::add_cpu_feature<cpu_features::avx512_4fmaps>(ps, info);
            ::add_cpu_feature<cpu_features::fsrm>(ps, info);
            ::add_cpu_feature<cpu_features::uintr>(ps, info);
            ::add_cpu_feature<cpu_features::avx512_vp2intersect>(ps, info);
            ::add_cpu_feature<cpu_features::srbds_ctrl>(ps, info);
            ::add_cpu_feature<cpu_features::md_clear>(ps, info);
            ::add_cpu_feature<cpu_features::rtm_always_abort>(ps, info);
            ::add_cpu_feature<cpu_features::rtm_force_abort>(ps, info);
            ::add_cpu_feature<cpu_features::serialise>(ps, info);
            ::add_cpu_feature<cpu_features::hybrid>(ps, info);
            ::add_cpu_feature<cpu_features::tsxldtrk>(ps, info);
            ::add_cpu_feature<cpu_features::pconfig>(ps, info);
            ::add_cpu_feature<cpu_features::lbr>(ps, info);
            ::add_cpu_feature<cpu_features::cet_ibt>(ps, info);
            ::add_cpu_feature<cpu_features::amx_bf16>(ps, info);
            ::add_cpu_feature<cpu_features::avx512_fp16>(ps, info);
            ::add_cpu_feature<cpu_features::amx_tile>(ps, info);
            ::add_cpu_feature<cpu_features::amx_int8>(ps, info);
            ::add_cpu_feature<cpu_features::speculation_control>(ps, info);
            ::add_cpu_feature<cpu_features::stibp>(ps, info);
            ::add_cpu_feature<cpu_features::l1d_flush>(ps, info);
            ::add_cpu_feature<cpu_features::arch_capabilities>(ps, info);
            ::add_cpu_feature<cpu_features::core_capabilities>(ps, info);
            ::add_cpu_feature<cpu_features::ssbd>(ps, info);
        }

        if (get_cpu_info(info, 0x00000007, 0x00000001)) {
            // EAX
            ::add_cpu_feature<cpu_features::sha512>(ps, info);
            ::add_cpu_feature<cpu_features::sm3>(ps, info);
            ::add_cpu_feature<cpu_features::sm4>(ps, info);
            ::add_cpu_feature<cpu_features::rao_int>(ps, info);
            ::add_cpu_feature<cpu_features::avx_vnni>(ps, info);
            ::add_cpu_feature<cpu_features::avx512_bf16>(ps, info);
            ::add_cpu_feature<cpu_features::lass>(ps, info);
            ::add_cpu_feature<cpu_features::cmpccxadd>(ps, info);
            ::add_cpu_feature<cpu_features::arch_perf_monitoring>(ps, info);
            ::add_cpu_feature<cpu_features::fzrm>(ps, info);
            ::add_cpu_feature<cpu_features::fsrs>(ps, info);
            ::add_cpu_feature<cpu_features::rsrcs>(ps, info);
            ::add_cpu_feature<cpu_features::fred>(ps, info);
            ::add_cpu_feature<cpu_features::lkgs>(ps, info);
            ::add_cpu_feature<cpu_features::non_serialising_wrmsr>(ps, info);
            ::add_cpu_feature<cpu_features::nmi_source_reporting>(ps, info);
            ::add_cpu_feature<cpu_features::amx_fp16>(ps, info);
            ::add_cpu_feature<cpu_features::hreset>(ps, info);
            ::add_cpu_feature<cpu_features::avx_ifma>(ps, info);
            ::add_cpu_feature<cpu_features::linear_address_masking>(ps, info);
            ::add_cpu_feature<cpu_features::msr_list>(ps, info);
            ::add_cpu_feature<cpu_features::invd>(ps, info);
            ::add_cpu_feature<cpu_features::movrs>(ps, info);

            // EBX
            ::add_cpu_feature<cpu_features::ppin>(ps, info);
            ::add_cpu_feature<cpu_features::pbndkb>(ps, info);
            ::add_cpu_feature<cpu_features::cpuid_max_value>(ps, info);
            
            // ECX
            // EDX
        }

        //add_feature(cpu_features::topology_leaf_b());
        //add_feature(cpu_features::topology_extensions());

        ps.add<cpu::features>(property_set(std::move(props)));
    }

    return property_set(std::move(ps));
}


/*
 * LYRA_DETAIL_NAMESPACE::get_topology
 */
LYRA_NAMESPACE::property_set LYRA_NAMESPACE::cpu::get_topology(
        _In_ const collection_flags flags) {
    detail::property_set_impl ps;

#if defined(_WIN32) && (_WIN32_WINNT >= 0x0601)
    typedef SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX info_type;

    std::map<BYTE, std::vector<detail::property_set_impl>> caches;
    std::map<GROUP_AFFINITY, detail::property_set_impl> cores;
    std::map<GROUP_AFFINITY, DWORD> nodes;
    std::vector<std::set<GROUP_AFFINITY>> sockets;

    //// Enumerates the individual bits of the affinity mask 'g'.
    //const auto enum_cores = [](GROUP_AFFINITY g, auto callback) {
    //    constexpr auto affinity_bits = sizeof(KAFFINITY) * CHAR_BIT;
    //    constexpr auto affinity_bit = static_cast<KAFFINITY>(1);
    //    const auto mask = g.Mask;

    //    for (KAFFINITY i = 0; i < affinity_bits; ++i) {
    //        const auto a = affinity_bit << i;
    //        if ((mask & a) == a) {
    //            g.Mask = a;
    //            callback(g);
    //        }
    //    }
    //};

    const auto af_eq = [](const GROUP_AFFINITY& l, const GROUP_AFFINITY& r) {
        return ((l.Group == r.Group) && ((l.Mask & r.Mask) != 0));
    };

    detail::enumerate_cpu_info([&](const info_type& info) {
        switch (info.Relationship) {
            case RelationProcessorPackage:
                // This is the relation between the socket and the cores. We
                // remember which processor groups belong to which socket.
                sockets.emplace_back();
                for (WORD i = 0; i < info.Processor.GroupCount; ++i) {
                    sockets.back().emplace(info.Processor.GroupMask[i]);
                }
                break;

            case RelationProcessorCore:
                // This holds the detail information about the cores.
                assert(info.Processor.GroupCount == 1);
                cores[info.Processor.GroupMask[0]].add(u8"Affinity Group",
                    info.Processor.GroupMask[0].Group);
                cores[info.Processor.GroupMask[0]].add(u8"Affinity Mask",
                    info.Processor.GroupMask[0].Mask);
                cores[info.Processor.GroupMask[0]].add(u8"Hyperthreading",
                    (info.Processor.Flags & LTP_PC_SMT) != 0);
                cores[info.Processor.GroupMask[0]].add(u8"Efficiency Class",
                    info.Processor.EfficiencyClass);
                break;

            case RelationNumaNode:
                // This is a relation between a NUMA node and a core, so we
                // remember to which NUMA node the processor group belongs.
                nodes[info.NumaNode.GroupMask] = info.NumaNode.NodeNumber;
                for (WORD i = 1; i < info.NumaNode.GroupCount; ++i) {
                    nodes[info.NumaNode.GroupMasks[i]]
                        = info.NumaNode.NodeNumber;
                }
                break;

            case RelationCache: {
                // This is a relation between a cache and one or more cores.
                auto& c = caches[info.Cache.Level].emplace_back();
                c.add(u8"Associativity", info.Cache.Associativity);
                c.add(u8"Line Size", info.Cache.LineSize);
                c.add(u8"Size", info.Cache.CacheSize);

                switch (info.Cache.Type) {
                    case CacheUnified:
                        c.add(u8"Type", u8"Unified");
                        break;

                    case CacheInstruction:
                        c.add(u8"Type", u8"Instruction");
                        break;

                    case CacheData:
                        c.add(u8"Type", u8"Data");
                        break;

                    case CacheTrace:
                        c.add(u8"Type", u8"Trace");
                        break;

                    default:
                        c.add(u8"Type", info.Cache.Type);
                        break;
                }

                std::vector<detail::property_set_impl> affinity;
                auto& a = affinity.emplace_back();
                a.add(u8"Group", info.Cache.GroupMask.Group);
                a.add(u8"Mask", info.Cache.GroupMask.Mask);

                for (WORD i = 1; i < info.Cache.GroupCount; ++i) {
                    a.add(u8"Group", info.Cache.GroupMasks[i].Group);
                    a.add(u8"Mask", info.Cache.GroupMasks[i].Mask);
                }

                c.add(u8"Affinity", detail::make_property_sets(
                    std::move(affinity)));
                } break;

            default:
                // This relationship is not important for our purposes.
                break;
        }
    });

    // Establish the hierarcy between sockets, NUMA nodes and cores. It is not
    // possible for us to do that during enumeration as the information may
    // arrive in any order.
    std::vector<std::map<DWORD, std::vector<detail::property_set_impl>>> hps(
        sockets.size());
    for (auto& c : cores) {
        const auto nit = std::find_if(nodes.begin(),
            nodes.end(),
            [&c, af_eq](const auto& n) { return af_eq(c.first, n.first); });
        assert(nit != nodes.end());

        for (std::size_t s = 0; s < sockets.size(); ++s) {
            for (auto& g : sockets[s]) {
                if (af_eq(c.first, g)) {
                    hps[s][nit->second].push_back(std::move(c.second));
                }
            }
        }
    }

    // Convert the hierarchy into hierarchical property sets.
    std::vector<detail::property_set_impl> sps;
    sps.reserve(hps.size());
    for (auto& s : hps) {
        std::vector<detail::property_set_impl> nps;
        nps.reserve(s.size());

        auto& ss = sps.emplace_back();
        for (auto& n : s) {
            auto& nn = nps.emplace_back();
            nn.add(u8"Node Number", n.first);
            nn.add<core>(detail::make_property_sets(std::move(n.second)));
        }

        ss.add<numa_node>(detail::make_property_sets(std::move(nps)));
    }

    detail::checked_add<topology>(ps, flags, detail::make_property_sets(
        std::move(sps)));

    // Create the cache hierarchy and add it to the output.
    detail::property_set_impl cps;
    for (auto& c : caches) {
        cps.add(std::to_string(c.first), detail::make_property_sets(
            std::move(c.second)));
    }

    detail::checked_add<cache>(ps, flags, property_set(std::move(cps)));
#endif /* defined(_WIN32) && (_WIN32_WINNT >= 0x0601) */

    auto x = add_amd_topology(ps);

    return property_set(std::move(ps));
}
