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


/// <summary>
/// Tests whether <typeparamref name="I" /> is supported and adds the result to
/// <paramref name="ps" />.
/// </summary>
template<LYRA_NAMESPACE::simd_instruction_set I>
inline void add_simd(_Inout_ LYRA_DETAIL_NAMESPACE::property_set_impl& ps,
        _In_z_ const char *name) {
    ps.add(name, LYRA_NAMESPACE::simd_detector<I>());
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
        detail::property_set_impl props;

        const auto add_feature = [&props](const auto& feature) {
            typedef std::decay_t<decltype(feature)> feature_type;
            props.add<feature_type>(feature);
        };

        add_feature(cpu_features::stepping_id());
        props.add(u8"Model", (cpu_features::extended_model() << 4)
            | cpu_features::model());
        props.add(u8"Family", cpu_features::extended_family_id()
            + cpu_features::family_id());
        add_feature(cpu_features::processor_type());
        add_feature(cpu_features::brand());
        add_feature(cpu_features::clflush_size());
        add_feature(cpu_features::max_cpu_id());
        add_feature(cpu_features::apic_id());
        //add_feature(cpu_features::sse3());
        add_feature(cpu_features::pclmulqdq());
        add_feature(cpu_features::dtes64());
        add_feature(cpu_features::monitor());
        add_feature(cpu_features::ds_cpl());
        add_feature(cpu_features::vmx());
        add_feature(cpu_features::smx());
        add_feature(cpu_features::est());
        add_feature(cpu_features::tm2());
        //add_feature(cpu_features::ssse3());
        add_feature(cpu_features::cnxt_id());
        add_feature(cpu_features::sdbg());
        add_feature(cpu_features::fma());
        add_feature(cpu_features::cx16());
        add_feature(cpu_features::xtpr());
        add_feature(cpu_features::pdcm());
        add_feature(cpu_features::pcid());
        add_feature(cpu_features::dca());
        //add_feature(cpu_features::sse4_1());
        //add_feature(cpu_features::sse4_2());
        add_feature(cpu_features::x2apic());
        add_feature(cpu_features::movbe());
        add_feature(cpu_features::popcnt());
        add_feature(cpu_features::tsc_deadline());
        add_feature(cpu_features::aes());
        add_feature(cpu_features::xsave());
        add_feature(cpu_features::osxsave());
        add_feature(cpu_features::f16c());
        add_feature(cpu_features::rdrnd());
        add_feature(cpu_features::hypervisor());
        add_feature(cpu_features::fpu());
        add_feature(cpu_features::vme());
        add_feature(cpu_features::de());
        add_feature(cpu_features::pse());
        add_feature(cpu_features::tsc());
        add_feature(cpu_features::msr());
        add_feature(cpu_features::pae());
        add_feature(cpu_features::mce());
        add_feature(cpu_features::cx8());
        add_feature(cpu_features::apic());
        add_feature(cpu_features::sep());
        add_feature(cpu_features::mtrr());
        add_feature(cpu_features::pge());
        add_feature(cpu_features::mca());
        add_feature(cpu_features::cmov());
        add_feature(cpu_features::pse36());
        add_feature(cpu_features::psn());
        add_feature(cpu_features::clfsh());
        add_feature(cpu_features::nx());
        add_feature(cpu_features::ds());
        add_feature(cpu_features::acpi());
        //add_feature(cpu_features::mmx());
        add_feature(cpu_features::fxsr());
        //add_feature(cpu_features::sse());
        //add_feature(cpu_features::sse2());
        add_feature(cpu_features::ss());
        add_feature(cpu_features::htt());
        add_feature(cpu_features::ia64());
        add_feature(cpu_features::pbe());
        add_feature(cpu_features::topology_leaf_b());
        add_feature(cpu_features::topology_extensions());
        {
            const cpu_features::emx emx;
            add_feature(emx);
            if (emx) {
                add_feature(cpu_features::min_monitor_size());
                add_feature(cpu_features::max_monitor_size());
                add_feature(cpu_features::ibe());
                add_feature(cpu_features::monitorless_mwait());
                add_feature(cpu_features::c0_sub_states());
                add_feature(cpu_features::c1_sub_states());
                add_feature(cpu_features::c2_sub_states());
                add_feature(cpu_features::c3_sub_states());
                add_feature(cpu_features::c4_sub_states());
                add_feature(cpu_features::c5_sub_states());
                add_feature(cpu_features::c6_sub_states());
                add_feature(cpu_features::c7_sub_states());
            }
        }
        add_feature(cpu_features::dts());
        add_feature(cpu_features::turbo_boost());
        add_feature(cpu_features::arat());
        add_feature(cpu_features::pln());
        add_feature(cpu_features::ecmd());
        add_feature(cpu_features::ptm());
        add_feature(cpu_features::hwp());
        add_feature(cpu_features::hwp_notification());
        add_feature(cpu_features::hwp_activity_window());
        add_feature(cpu_features::hwp_epp());
        add_feature(cpu_features::hwp_pkg());
        add_feature(cpu_features::hdc());
        add_feature(cpu_features::turbo_boost_max());
        add_feature(cpu_features::hwp_capabilities());
        add_feature(cpu_features::hwp_peci_override());
        add_feature(cpu_features::flexible_hwp());
        add_feature(cpu_features::hwp_request_fast_access());
        add_feature(cpu_features::hw_feedback_interface());
        add_feature(cpu_features::hwp_request_ignore_idle());
        add_feature(cpu_features::hwp_control_msr());
        add_feature(cpu_features::thread_director());
        // TODO
        add_feature(cpu_features::sha512());
        add_feature(cpu_features::sm3());
        add_feature(cpu_features::sm4());
        add_feature(cpu_features::rao_int());
        //add_feature(cpu_features::avx_vnni());
        //add_feature(cpu_features::avx512_bf16());
        add_feature(cpu_features::lass());
        add_feature(cpu_features::cmpccxadd());
        add_feature(cpu_features::architectural_performance_monitoring());
        add_feature(cpu_features::fzrm());
        add_feature(cpu_features::fsrs());
        add_feature(cpu_features::rsrcs());
        add_feature(cpu_features::fred());
        add_feature(cpu_features::lkgs());
        add_feature(cpu_features::non_serialising_wrmsr());
        add_feature(cpu_features::nmi_source_reporting());
        add_feature(cpu_features::amx_fp16());
        add_feature(cpu_features::hreset());
        //add_feature(cpu_features::avx_ifma());
        add_feature(cpu_features::linear_address_masking());
        add_feature(cpu_features::msr_list());
        add_feature(cpu_features::invd());
        add_feature(cpu_features::movrs());

        if (detail::check_sensitive<cpu::simd_instructions>(flags)) {
            detail::property_set_impl simds;
            ::add_simd<simd_instruction_set::mmx>(simds, u8"MMX");
            ::add_simd<simd_instruction_set::sse>(simds, u8"SSE");
            ::add_simd<simd_instruction_set::sse2>(simds, u8"SSE 2");
            ::add_simd<simd_instruction_set::sse3>(simds, u8"SSE 3");
            ::add_simd<simd_instruction_set::ssse3>(simds, u8"SSSE 3");
            ::add_simd<simd_instruction_set::sse4_1>(simds, u8"SSE 4.1");
            ::add_simd<simd_instruction_set::sse4_2>(simds, u8"SSE 4.2");
            ::add_simd<simd_instruction_set::avx>(simds, u8"AVX");
            ::add_simd<simd_instruction_set::avxvnni>(simds, u8"AVX VNNI");
            ::add_simd<simd_instruction_set::avxifma>(simds, u8"AVX IFMA");
            ::add_simd<simd_instruction_set::avx2>(simds, u8"AVX 2");
            ::add_simd<simd_instruction_set::avx512>(simds,
                u8"AVX 512 Foundation");
            ::add_simd<simd_instruction_set::avx512pf>(simds,
                u8"AVX 512 Prefetch");
            ::add_simd<simd_instruction_set::avx512dq>(simds,
                u8"AVX 512 Vector Double Word and Quad Word");
            ::add_simd<simd_instruction_set::avx512bw>(simds,
                u8"AVX 512 Vector Byte and Word");
            ::add_simd<simd_instruction_set::avx512vl>(simds,
                u8"AVX 512 Vector Length");
            ::add_simd<simd_instruction_set::avx512cd>(simds,
                u8"AVX 512 Conflict Detection");
            ::add_simd<simd_instruction_set::avx512ifma>(simds,
                u8"AVX 512 Integer Fused Multiply Add");
            ::add_simd<simd_instruction_set::avx512er>(simds,
                u8"AVX 512 Exponential and Reciprocal");
            ::add_simd<simd_instruction_set::avx5124fmaps>(simds, u8"AVX 512 "
                u8"Vector Fused Multiply Accumulation Packed Single Precision");
            ::add_simd<simd_instruction_set::avx5124vnniw>(simds, u8"AVX 512 "
                u8"Vector Neural Network Instructions Word Variable Precision");
            ::add_simd<simd_instruction_set::avx512vpopcntdq>(simds,
                u8"AVX 512 Vector POPCOUNT Double Word and Quad Word");
            ::add_simd<simd_instruction_set::avx512vnni>(simds,
                u8"AVX 512 Vector Neural Network Instructions");
            ::add_simd<simd_instruction_set::avx512bitalg>(simds,
                u8"AVX 512 Bit Algorithms");
            //::add_simd<simd_instruction_set::avx512gfni>(simds,
            //    u8"AVX 512 Galois Field New Instructions");
            //::add_simd<simd_instruction_set::avx512vpclmulqdq>(simds,
            //    u8"AVX 512 ");
            //::add_simd<simd_instruction_set::avx512vaes>(simds,
            //    u8"AVX 512 ");
            ::add_simd<simd_instruction_set::avx512bf16>(simds,
                u8"AVX 512 Instructions for bfloat16 Numbers");
            props.add<cpu::simd_instructions>(property_set(
                std::move(simds)));
        }

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
