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

#include "visus/autodoc/cpu_info.h"
#include "visus/autodoc/instruction_detectors.h"
#include "visus/autodoc/multi_sz.h"
#include "visus/autodoc/simd_detector.h"

#include "os_cpu_info.h"
#include "property_set_impl.h"


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

        if (detail::check_sensitive<cpu::brand_string>(flags)
                && (i == brand_comps)) {
            ps.add<cpu::brand_string>(brand.data());
        }
    }

    if (detail::check_sensitive<cpu::cpuid>(flags)) {
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

    if (detail::check_sensitive<cpu::instructions>(flags)) {
        detail::property_set_impl insts;

        insts.add(u8"POPCNT", instruction_detectors::popcnt());
        insts.add(u8"XGETBV", instruction_detectors::xgetbv());

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
            insts.add<cpu::simd_instructions>(property_set(
                std::move(simds)));
        }

        ps.add<cpu::instructions>(property_set(std::move(insts)));
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

    return property_set(std::move(ps));
}
