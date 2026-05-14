// <copyright file="cpu_info.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <vector>

#include "visus/autodoc/affinity_scope.h"
#include "visus/autodoc/cpu.h"
#include "visus/autodoc/cpu_features.h"
#include "visus/autodoc/cpu_info.h"
#include "visus/autodoc/cpu_info_detector.h"
#include "visus/autodoc/cpu_states.h"
#include "visus/autodoc/cpu_vendor.h"
#include "visus/autodoc/simd_detector.h"

#include "os_cpu_info.h"


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

TEST(cpu_info, features) {
    LYRA_NAMESPACE::affinity_scope(LYRA_NAMESPACE::affinity_mask(std::size_t(0)));
    {
        std::uint32_t value;
        EXPECT_NO_THROW(value = LYRA_NAMESPACE::cpu_features::stepping_id());
    }
    {
        std::uint32_t value;
        EXPECT_NO_THROW(value = LYRA_NAMESPACE::cpu_features::model());
    }
    {
        std::uint32_t value;
        EXPECT_NO_THROW(value = LYRA_NAMESPACE::cpu_features::family_id());
    }
    {
        std::uint32_t value;
        EXPECT_NO_THROW(value = LYRA_NAMESPACE::cpu_features::processor_type());
    }
    {
        std::uint32_t value;
        EXPECT_NO_THROW(value = LYRA_NAMESPACE::cpu_features::extended_model());
    }
    {
        std::uint32_t value;
        EXPECT_NO_THROW(value = LYRA_NAMESPACE::cpu_features::extended_family_id());
    }
    {
        std::uint32_t value;
        EXPECT_NO_THROW(value = LYRA_NAMESPACE::cpu_features::brand());
    }
    {
        std::uint32_t value;
        EXPECT_NO_THROW(value = LYRA_NAMESPACE::cpu_features::clflush_size());
    }
    {
        std::uint32_t value;
        EXPECT_NO_THROW(value = LYRA_NAMESPACE::cpu_features::max_cpu_id());
    }
    {
        std::uint32_t value;
        EXPECT_NO_THROW(value = LYRA_NAMESPACE::cpu_features::apic_id());
    }
    {
        std::uint32_t value;
        EXPECT_NO_THROW(value = LYRA_NAMESPACE::cpu_features::sse3());
    }
    {
        std::uint32_t value;
        EXPECT_NO_THROW(value = LYRA_NAMESPACE::cpu_features::pclmulqdq());
    }
}

TEST(cpu_info, get) {
    auto p = LYRA_NAMESPACE::cpu::get_cpuid();
    EXPECT_FALSE(p.empty());
    EXPECT_TRUE(p.contains(LYRA_NAMESPACE::cpu::vendor::name));

    auto j = p.json();
    EXPECT_NE(j, nullptr);
}

TEST(cpu_info, process_mask) {
    EXPECT_GT(LYRA_DETAIL_NAMESPACE::get_os_max_cpus(), 0);
    const auto affinity = LYRA_DETAIL_NAMESPACE::get_process_cpu_affinity();
    EXPECT_FALSE(affinity.empty());
    EXPECT_LE(affinity.size(), LYRA_DETAIL_NAMESPACE::get_os_max_cpus());
}

TEST(cpu_info, topology) {
#if defined(_WIN32) && (_WIN32_WINNT >= 0x0601)
    LYRA_DETAIL_NAMESPACE::enumerate_cpu_toplogy([](const auto& info) {
        EXPECT_TRUE((info.Relationship == RelationProcessorPackage)
            || (info.Relationship == RelationProcessorCore)
            || (info.Relationship == RelationNumaNode));
    });
#endif /* defined(_WIN32) && (_WIN32_WINNT >= 0x0601) */

    auto p = LYRA_NAMESPACE::cpu::get_topology();
    EXPECT_FALSE(p.empty());
}

TEST(cpu_info, vendor) {
    LYRA_NAMESPACE::cpu_info info;
    EXPECT_TRUE(LYRA_NAMESPACE::get_cpu_info(info, 0));

    bool value;
    EXPECT_NO_THROW(value = LYRA_NAMESPACE::cpu_vendor::amd::check(info));
    EXPECT_NO_THROW(value = LYRA_NAMESPACE::cpu_vendor::ao486::check(info));
    EXPECT_NO_THROW(value = LYRA_NAMESPACE::cpu_vendor::centaur::check(info));
    EXPECT_NO_THROW(value = LYRA_NAMESPACE::cpu_vendor::fx32::check(info));
    EXPECT_NO_THROW(value = LYRA_NAMESPACE::cpu_vendor::connectix::check(info));
    EXPECT_NO_THROW(value = LYRA_NAMESPACE::cpu_vendor::cyrix::check(info));
    EXPECT_NO_THROW(value = LYRA_NAMESPACE::cpu_vendor::elbrus::check(info));
    EXPECT_NO_THROW(value = LYRA_NAMESPACE::cpu_vendor::hygon::check(info));
    EXPECT_NO_THROW(value = LYRA_NAMESPACE::cpu_vendor::insignia::check(info));
    EXPECT_NO_THROW(value = LYRA_NAMESPACE::cpu_vendor::intel::check(info));
    EXPECT_NO_THROW(value = LYRA_NAMESPACE::cpu_vendor::neko_project::check(info));
    EXPECT_NO_THROW(value = LYRA_NAMESPACE::cpu_vendor::nexgen::check(info));
    EXPECT_NO_THROW(value = LYRA_NAMESPACE::cpu_vendor::nsc::check(info));
    EXPECT_NO_THROW(value = LYRA_NAMESPACE::cpu_vendor::power_vm::check(info));
    EXPECT_NO_THROW(value = LYRA_NAMESPACE::cpu_vendor::rdc::check(info));
    EXPECT_NO_THROW(value = LYRA_NAMESPACE::cpu_vendor::rise::check(info));
    EXPECT_NO_THROW(value = LYRA_NAMESPACE::cpu_vendor::sis::check(info));
    EXPECT_NO_THROW(value = LYRA_NAMESPACE::cpu_vendor::transmeta::check(info));
    EXPECT_NO_THROW(value = LYRA_NAMESPACE::cpu_vendor::via::check(info));
    EXPECT_NO_THROW(value = LYRA_NAMESPACE::cpu_vendor::virtual_pc::check(info));
    EXPECT_NO_THROW(value = LYRA_NAMESPACE::cpu_vendor::vortex::check(info));
    EXPECT_NO_THROW(value = LYRA_NAMESPACE::cpu_vendor::zhaoxin::check(info));
}

TEST(cpu_info, hypervisor) {
    LYRA_NAMESPACE::cpu_info info;
    LYRA_NAMESPACE::get_cpu_info(info, 0x40000000);

    bool value;
    EXPECT_NO_THROW(value = LYRA_NAMESPACE::hypervisor::acrn::check(info));
    EXPECT_NO_THROW(value = LYRA_NAMESPACE::hypervisor::bhyve::check(info));
    EXPECT_NO_THROW(value = LYRA_NAMESPACE::hypervisor::fex_emu::check(info));
    EXPECT_NO_THROW(value = LYRA_NAMESPACE::hypervisor::hyperv::check(info));
    EXPECT_NO_THROW(value = LYRA_NAMESPACE::hypervisor::lmhs::check(info));
    EXPECT_NO_THROW(value = LYRA_NAMESPACE::hypervisor::napoca::check(info));
    EXPECT_NO_THROW(value = LYRA_NAMESPACE::hypervisor::nvmm::check(info));
    EXPECT_NO_THROW(value = LYRA_NAMESPACE::hypervisor::open_bsd_vmm::check(info));
    EXPECT_NO_THROW(value = LYRA_NAMESPACE::hypervisor::parallels::check(info));
    EXPECT_NO_THROW(value = LYRA_NAMESPACE::hypervisor::qemu::check(info));
    EXPECT_NO_THROW(value = LYRA_NAMESPACE::hypervisor::siemens::check(info));
    EXPECT_NO_THROW(value = LYRA_NAMESPACE::hypervisor::trusty::check(info));
    EXPECT_NO_THROW(value = LYRA_NAMESPACE::hypervisor::unisys::check(info));
    EXPECT_NO_THROW(value = LYRA_NAMESPACE::hypervisor::virtual_box::check(info));
    EXPECT_NO_THROW(value = LYRA_NAMESPACE::hypervisor::vmware::check(info));
    EXPECT_NO_THROW(value = LYRA_NAMESPACE::hypervisor::xen::check(info));
}

TEST(cpu_info, affinity_mask) {
    {
        LYRA_NAMESPACE::affinity_mask m;
        EXPECT_FALSE(m);
        EXPECT_FALSE(m.test(1));
        EXPECT_TRUE(m == m);
        EXPECT_FALSE(m != m);
    }
    {
        std::size_t s;
        LYRA_NAMESPACE::affinity_mask m(&s, 0);
        EXPECT_FALSE(m);
        EXPECT_FALSE(m.test(1));
        EXPECT_TRUE(m == m);
        EXPECT_FALSE(m != m);
    }
    {
        LYRA_NAMESPACE::affinity_mask m(static_cast<std::size_t>(0));
        EXPECT_TRUE(m);
        EXPECT_TRUE(m.test(0));
        EXPECT_FALSE(m.test(1));
        EXPECT_TRUE(m == m);
        EXPECT_FALSE(m != m);
    }
    {
        std::size_t s[] = { 0, 2 };
        LYRA_NAMESPACE::affinity_mask m(s, 2);
        EXPECT_TRUE(m);
        EXPECT_TRUE(m.test(0));
        EXPECT_FALSE(m.test(1));
        EXPECT_TRUE(m.test(2));
        EXPECT_TRUE(m == m);
        EXPECT_FALSE(m != m);
    }
    {
        LYRA_NAMESPACE::affinity_mask n;
        LYRA_NAMESPACE::affinity_mask m(n);
        EXPECT_FALSE(m);
        EXPECT_FALSE(m.test(1));
        EXPECT_TRUE(m == m);
        EXPECT_FALSE(m != m);
        EXPECT_TRUE(n == m);
        EXPECT_FALSE(n != m);
    }
    {
        LYRA_NAMESPACE::affinity_mask n(static_cast<std::size_t>(0));
        LYRA_NAMESPACE::affinity_mask m(n);
        EXPECT_TRUE(m);
        EXPECT_TRUE(m.test(0));
        EXPECT_FALSE(m.test(1));
        EXPECT_TRUE(m == m);
        EXPECT_FALSE(m != m);
        EXPECT_TRUE(n == m);
        EXPECT_FALSE(n != m);
    }
    {
        auto current = LYRA_NAMESPACE::affinity_mask::thread();
        EXPECT_TRUE(current);
        LYRA_NAMESPACE::affinity_mask m;
        EXPECT_FALSE(m);
        EXPECT_FALSE(m == current);
        EXPECT_TRUE(m != current);
        m = current;
        EXPECT_TRUE(m);
        EXPECT_TRUE(m == current);
        EXPECT_FALSE(m != current);
        current = std::move(m);
        EXPECT_TRUE(current);
    }
}

TEST(cpu_info, affinity_scope) {
    const auto affinity = LYRA_DETAIL_NAMESPACE::get_thread_cpu_affinity();

    {
        LYRA_NAMESPACE::affinity_mask mask(static_cast<std::size_t>(0));
        LYRA_NAMESPACE::affinity_scope scope(mask);
        EXPECT_TRUE(static_cast<bool>(scope));

        const auto changed = LYRA_DETAIL_NAMESPACE::get_thread_cpu_affinity();
        EXPECT_FALSE(std::equal(affinity.begin(), affinity.end(), changed.begin(), changed.end()));
    }

    const auto reverted = LYRA_DETAIL_NAMESPACE::get_thread_cpu_affinity();
    EXPECT_TRUE(std::equal(affinity.begin(), affinity.end(), reverted.begin(), reverted.end()));
}
