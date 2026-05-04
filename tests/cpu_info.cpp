// <copyright file="cpu_info.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include <gtest/gtest.h>

#include <vector>

#include "visus/autodoc/cpu.h"
#include "visus/autodoc/cpu_info.h"
#include "visus/autodoc/cpu_info_detector.h"
#include "visus/autodoc/simd_detector.h"


TEST(cpu_info, base) {
    std::vector<LYRA_NAMESPACE::cpu_info> infos(LYRA_NAMESPACE::get_cpu_info());
    EXPECT_GE(infos.size(), 1);
    EXPECT_EQ(LYRA_NAMESPACE::get_cpu_info(infos.data(), infos.size()), infos.size());
}


TEST(cpu_info, extended) {
    std::vector<LYRA_NAMESPACE::cpu_info> infos(LYRA_NAMESPACE::get_extended_cpu_info());
    EXPECT_GE(infos.size(), 1);
    EXPECT_EQ(LYRA_NAMESPACE::get_extended_cpu_info(infos.data(), infos.size()), infos.size());
}


TEST(cpu_info, simd) {
    EXPECT_TRUE(LYRA_NAMESPACE::simd_detector<LYRA_NAMESPACE::simd_instruction_set::none>());
    EXPECT_TRUE(LYRA_NAMESPACE::simd_detector<LYRA_NAMESPACE::simd_instruction_set::mmx>());
    EXPECT_TRUE(LYRA_NAMESPACE::simd_detector<LYRA_NAMESPACE::simd_instruction_set::sse>());
    EXPECT_TRUE(LYRA_NAMESPACE::simd_detector<LYRA_NAMESPACE::simd_instruction_set::sse2>());
    EXPECT_TRUE(LYRA_NAMESPACE::simd_detector<LYRA_NAMESPACE::simd_instruction_set::sse3>());
    EXPECT_TRUE(LYRA_NAMESPACE::simd_detector<LYRA_NAMESPACE::simd_instruction_set::ssse3>());
    EXPECT_TRUE(LYRA_NAMESPACE::simd_detector<LYRA_NAMESPACE::simd_instruction_set::sse4_1>());
}

TEST(cpu_info, properties) {
    auto p = LYRA_NAMESPACE::cpu::get_cpuid();
    EXPECT_FALSE(p.empty());
    EXPECT_TRUE(p.contains(LYRA_NAMESPACE::cpu::vendor::name));

    auto j = p.json();
    EXPECT_NE(j, nullptr);
}
