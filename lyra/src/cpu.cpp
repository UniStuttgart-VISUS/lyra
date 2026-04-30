// <copyright file="cpu.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "visus/lyra/cpu.h"

#include <algorithm>
#include <array>
#include <cstring>
#include <iomanip>
#include <sstream>

#include "visus/lyra/cpu_info.h"
#include "visus/lyra/instruction_detectors.h"
#include "visus/lyra/multi_sz.h"
#include "visus/lyra/simd_detector.h"

#include "property_set_impl.h"
#include "is_sensitive.h"


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
 * LYRA_NAMESPACE::cpu::get_cpuid
 */
LYRA_NAMESPACE::property_set LYRA_NAMESPACE::cpu::get_cpuid(
        _In_ const collection_flags flags) {
    constexpr auto reg_size = sizeof(std::uint32_t);
    detail::property_set_impl ps;
    property_set retval;

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

        ps.add<cpu::cpuid>(detail::to_property_set(std::move(info_set)));
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
            insts.add<cpu::simd_instructions>(detail::to_property_set(
                std::move(simds)));
        }

        ps.add<cpu::instructions>(detail::to_property_set(std::move(insts)));
    }

    detail::realise(retval, std::move(ps));
    return retval;
}
