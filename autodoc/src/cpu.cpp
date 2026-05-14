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

        props.add(u8"Stepping", cpu_features::stepping_id());
        props.add(u8"Model", (cpu_features::extended_model() << 4)
            | cpu_features::model());
        props.add(u8"Family", cpu_features::extended_family_id()
            + cpu_features::family_id());
        props.add(u8"Processor Type", cpu_features::processor_type());
        props.add(u8"Brand ID", cpu_features::brand());
        props.add(u8"Cache Line Flush Size", cpu_features::clflush_size() * 8);
        props.add(u8"Maximum CPUs per package", cpu_features::max_cpu_id());
        props.add(u8"APIC ID", cpu_features::apic_id());
        props.add(u8"PCLMULQDQ", cpu_features::pclmulqdq());
        props.add(u8"64-bit Debug Store", cpu_features::dtes64());
        props.add(u8"MONITOR/MWAIT", cpu_features::monitor());
        props.add(u8"CPL-Qualified Debug Store (ds-cpl)",
            cpu_features::ds_cpl());
        props.add(u8"Virtual Machine Extensions (vmx)", cpu_features::vmx());
        props.add(u8"Safer Mode Extensions (smx)", cpu_features::smx());
        props.add(u8"Enhanced Intel SpeedStep Technology (est)",
            cpu_features::est());
        props.add(u8"Thermal Monitor 2 (tm2)", cpu_features::tm2());
        props.add(u8"L1 context ID (cnxt-id)", cpu_features::cnxt_id());
        props.add(u8"Silicon Debug interface (sdbg)", cpu_features::sdbg());
        props.add(u8"Fused multiply add (fma)", cpu_features::fma());
        props.add(u8"CMPXCHG16B", cpu_features::cx16());
        props.add(u8"Task Priority Messages (xtpr)", cpu_features::xtpr());
        props.add(u8"Performance and Debug Capability (pdcm)",
            cpu_features::pdcm());
        props.add(u8"Process Context Identifiers (pcid)", cpu_features::pcid());
        props.add(u8"Direct Cache Access (dca)", cpu_features::dca());
        props.add(u8"Enhanced APIC (x2apic)", cpu_features::x2apic());
        props.add(u8"MOVBE", cpu_features::movbe());
        props.add(u8"POPCNT", cpu_features::popcnt());
        props.add(u8"TSC Deadline (tsc-deadline)",
            cpu_features::tsc_deadline());
        props.add(u8"AES (aes-ni)", cpu_features::aes());
        props.add(u8"Extensible Processor Save/Restore (xsave)",
            cpu_features::xsave());
        props.add(u8"Extensible Processor Save/Restore enabled (osxsave)",
            cpu_features::osxsave());
        props.add(u8"FP16 Conversion (f16c)", cpu_features::f16c());
        props.add(u8"On-Chip Random Number Generator (rdrnd)",
            cpu_features::rdrnd());
        props.add(u8"Hypervisor Present (hypervisor)",
            cpu_features::hypervisor());
        props.add(u8"On-board x87 Floating-Point Unit (fpu)",
            cpu_features::fpu());
        props.add(u8"Virtual 8086 Mode Extensions (vme)", cpu_features::vme());
        props.add(u8"Debugging Extensions (de)", cpu_features::de());
        props.add(u8"Large Pages (pse)", cpu_features::pse());
        props.add(u8"Time Stamp Counter and RDSTC (tsc)", cpu_features::tsc());
        props.add(u8"Machine-Specific Registers (msr)", cpu_features::msr());
        props.add(u8"Physical Address Extension (pae)", cpu_features::pae());
        props.add(u8"Machine Check Exception (mce)", cpu_features::mce());
        props.add(u8"CMPXCHG8B", cpu_features::cx8());
        props.add(u8"APIC", cpu_features::apic());
        props.add(u8"Fast System Call Instructions (sep)", cpu_features::sep());
        props.add(u8"Memory Type Range Registers (mtrr)", cpu_features::mtrr());
        props.add(u8"Page Global Enable (pge)", cpu_features::pge());
        props.add(u8"Machine Check Architecture (mca)", cpu_features::mca());
        props.add(u8"Conditional Move Instructions (cmov)",
            cpu_features::cmov());
        props.add(u8"Page Attribute Table (pat)", cpu_features::pat());
        props.add(u8"36-bit Page Size Extensions (pse-36)",
            cpu_features::pse36());
        props.add(u8"Processor Serial Number (psn)", cpu_features::psn());
        props.add(u8"CLFLUSH", cpu_features::clfsh());
        props.add(u8"No-Execute Bit (nx)", cpu_features::nx());
        props.add(u8"Debug Store (ds)", cpu_features::ds());
        props.add(u8"Advanced Configuration and Power Interface (acpi)",
            cpu_features::acpi());
        props.add(u8"FXSAVE/FXSTOR", cpu_features::fxsr());
        props.add(u8"CPU Cache implements self-snoop (ss)", cpu_features::ss());
        props.add(u8"Maximum APIC IDs Reserved Field (htt)",
            cpu_features::htt());
        props.add(u8"Thermal Monitor Automatically Limits Temperature (tm)",
            cpu_features::tm());
        props.add(u8"Itanium Processor (ia64)", cpu_features::ia64());
        props.add(u8"Pending Break Enable (pbe)", cpu_features::pbe());

        props.add(u8"Topology Leaf B", cpu_features::topology_leaf_b());
        props.add(u8"Topology Extensions", cpu_features::topology_extensions());

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
