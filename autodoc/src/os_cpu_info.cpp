// <copyright file="os_cpu_info.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "os_cpu_info.h"

#include <climits>

#if defined(_WIN32)
#include <Windows.h>
#else /* !defined(_WIN32) */
#include <sched.h>
#include <unistd.h>
#endif /* !defined(_WIN32) */

#include "visus/autodoc/on_exit.h"

#include "contains.h"
#include "property_set_impl.h"


/// <summary>
/// Convert an integral mask to a vector of Boolean values where each elements
/// indicates whether a specific bit is set in the mask.
/// </summary>
template<class TType> static std::vector<bool> to_booleans(
        _In_ const TType mask, _In_ std::size_t limit) {
    constexpr auto length = sizeof(TType) * CHAR_BIT;
    if (limit > length) {
        limit = length;
    }

    std::vector<bool> retval(limit);
    for (std::size_t i = 0; i < limit; ++i) {
        const auto bit = static_cast<TType>(1) << i;
        retval[i] = (mask & bit) != 0;
    }

    return retval;
}


/*
 * LYRA_DETAIL_NAMESPACE::get_process_cpu_affinity
 */
std::vector<bool> LYRA_DETAIL_NAMESPACE::get_process_cpu_affinity(void) {
    std::vector<bool> retval;

#if defined(_WIN32)
#if (_WIN32_WINNT >= 0x0601)
    auto cnt_groups = ::GetActiveProcessorGroupCount();

    if (cnt_groups > 0) {
        // We need to know (i) which processor groups the current process is allowed to
        // run on and (ii) how many processors are active in this group.
        std::vector<std::uint8_t> buffer;
        std::vector<USHORT> groups(cnt_groups);
        auto info = get_logical_processor_info(buffer, RelationGroup);

        if ((info != nullptr) && (info->Relationship == RelationGroup)
                && ::GetProcessGroupAffinity(::GetCurrentProcess(), &cnt_groups,
                groups.data())) {
            for (std::size_t i = 0; i < cnt_groups; ++i) {
                const auto& g = info->Group.GroupInfo[i];

                if (contains(groups, static_cast<USHORT>(i))) {
                    // Check the processors of the group against the affinity mask.
                    for (auto j = 0; j < g.ActiveProcessorCount; ++j) {
                        const auto bit = static_cast<KAFFINITY>(1) << j;
                        retval.push_back((g.ActiveProcessorMask & bit) != 0);
                    }

                } else {
                    // All processors in this group are unused/unavailable.
                    for (auto j = 0; j < g.ActiveProcessorCount; ++j) {
                        retval.push_back(false);
                    }
                }
            }

            return retval;
        }
    } /* if (cnt_groups > 0) */

#endif /* (_WIN32_WINNT >= 0x0601) */
    if (retval.empty()){
        DWORD_PTR mask, dummy;
        if (::GetProcessAffinityMask(::GetCurrentProcess(), &mask, &dummy)) {
            retval = to_booleans(mask, get_os_max_cpus());
        }
    }

#else /* defined(_WIN32) */
    return get_thread_cpu_affinity(0);
#endif /* defined(_WIN32) */

    return retval;
}


#if defined(_WIN32)
/*
 * LYRA_DETAIL_NAMESPACE::get_processor_features
 */
LYRA_NAMESPACE::property_set LYRA_DETAIL_NAMESPACE::get_processor_features(
        void) {
    property_set_impl ps;

#define _LYRA_ADD_PROF_FEATURE(feature) do {\
    ps.add(#feature, ::IsProcessorFeaturePresent(feature) != FALSE);\
} while (false)

    _LYRA_ADD_PROF_FEATURE(PF_ARM_64BIT_LOADSTORE_ATOMIC);
    _LYRA_ADD_PROF_FEATURE(PF_ARM_DIVIDE_INSTRUCTION_AVAILABLE);
    _LYRA_ADD_PROF_FEATURE(PF_ARM_EXTERNAL_CACHE_AVAILABLE);
    _LYRA_ADD_PROF_FEATURE(PF_ARM_FMAC_INSTRUCTIONS_AVAILABLE);
    _LYRA_ADD_PROF_FEATURE(PF_ARM_VFP_32_REGISTERS_AVAILABLE);
    _LYRA_ADD_PROF_FEATURE(PF_3DNOW_INSTRUCTIONS_AVAILABLE);
    _LYRA_ADD_PROF_FEATURE(PF_CHANNELS_ENABLED);
    _LYRA_ADD_PROF_FEATURE(PF_COMPARE_EXCHANGE_DOUBLE);
    _LYRA_ADD_PROF_FEATURE(PF_COMPARE_EXCHANGE128);
    _LYRA_ADD_PROF_FEATURE(PF_COMPARE64_EXCHANGE128);
    _LYRA_ADD_PROF_FEATURE(PF_FASTFAIL_AVAILABLE);
    _LYRA_ADD_PROF_FEATURE(PF_FLOATING_POINT_EMULATED);
    _LYRA_ADD_PROF_FEATURE(PF_FLOATING_POINT_PRECISION_ERRATA);
    _LYRA_ADD_PROF_FEATURE(PF_MMX_INSTRUCTIONS_AVAILABLE);
    _LYRA_ADD_PROF_FEATURE(PF_NX_ENABLED);
    _LYRA_ADD_PROF_FEATURE(PF_PAE_ENABLED);
    _LYRA_ADD_PROF_FEATURE(PF_RDTSC_INSTRUCTION_AVAILABLE);
    _LYRA_ADD_PROF_FEATURE(PF_RDWRFSGSBASE_AVAILABLE);
    _LYRA_ADD_PROF_FEATURE(PF_SECOND_LEVEL_ADDRESS_TRANSLATION);
    _LYRA_ADD_PROF_FEATURE(PF_SSE3_INSTRUCTIONS_AVAILABLE);
    _LYRA_ADD_PROF_FEATURE(PF_SSSE3_INSTRUCTIONS_AVAILABLE);
    _LYRA_ADD_PROF_FEATURE(PF_SSE4_1_INSTRUCTIONS_AVAILABLE);
    _LYRA_ADD_PROF_FEATURE(PF_SSE4_2_INSTRUCTIONS_AVAILABLE);
    _LYRA_ADD_PROF_FEATURE(PF_AVX_INSTRUCTIONS_AVAILABLE);
    _LYRA_ADD_PROF_FEATURE(PF_AVX2_INSTRUCTIONS_AVAILABLE);
    _LYRA_ADD_PROF_FEATURE(PF_AVX512F_INSTRUCTIONS_AVAILABLE);
    _LYRA_ADD_PROF_FEATURE(PF_VIRT_FIRMWARE_ENABLED);
    _LYRA_ADD_PROF_FEATURE(PF_XMMI_INSTRUCTIONS_AVAILABLE);
    _LYRA_ADD_PROF_FEATURE(PF_XMMI64_INSTRUCTIONS_AVAILABLE);
    _LYRA_ADD_PROF_FEATURE(PF_XSAVE_ENABLED);
    _LYRA_ADD_PROF_FEATURE(PF_ARM_V8_INSTRUCTIONS_AVAILABLE);
    _LYRA_ADD_PROF_FEATURE(PF_ARM_V8_CRYPTO_INSTRUCTIONS_AVAILABLE);
    _LYRA_ADD_PROF_FEATURE(PF_ARM_V8_CRC32_INSTRUCTIONS_AVAILABLE);
    _LYRA_ADD_PROF_FEATURE(PF_ARM_V81_ATOMIC_INSTRUCTIONS_AVAILABLE);
    _LYRA_ADD_PROF_FEATURE(PF_ARM_V82_DP_INSTRUCTIONS_AVAILABLE);
    _LYRA_ADD_PROF_FEATURE(PF_ARM_V83_JSCVT_INSTRUCTIONS_AVAILABLE);
    _LYRA_ADD_PROF_FEATURE(PF_ARM_V83_LRCPC_INSTRUCTIONS_AVAILABLE);
    _LYRA_ADD_PROF_FEATURE(PF_ARM_SVE_INSTRUCTIONS_AVAILABLE);
    _LYRA_ADD_PROF_FEATURE(PF_ARM_SVE2_INSTRUCTIONS_AVAILABLE);
    _LYRA_ADD_PROF_FEATURE(PF_ARM_SVE2_1_INSTRUCTIONS_AVAILABLE);
    _LYRA_ADD_PROF_FEATURE(PF_ARM_SVE_AES_INSTRUCTIONS_AVAILABLE);
    _LYRA_ADD_PROF_FEATURE(PF_ARM_SVE_PMULL128_INSTRUCTIONS_AVAILABLE);
    _LYRA_ADD_PROF_FEATURE(PF_ARM_SVE_BITPERM_INSTRUCTIONS_AVAILABLE);
    _LYRA_ADD_PROF_FEATURE(PF_ARM_SVE_BF16_INSTRUCTIONS_AVAILABLE);
    _LYRA_ADD_PROF_FEATURE(PF_ARM_SVE_EBF16_INSTRUCTIONS_AVAILABLE);
    _LYRA_ADD_PROF_FEATURE(PF_ARM_SVE_B16B16_INSTRUCTIONS_AVAILABLE);
    _LYRA_ADD_PROF_FEATURE(PF_ARM_SVE_SHA3_INSTRUCTIONS_AVAILABLE);
    _LYRA_ADD_PROF_FEATURE(PF_ARM_SVE_SM4_INSTRUCTIONS_AVAILABLE);
    _LYRA_ADD_PROF_FEATURE(PF_ARM_SVE_I8MM_INSTRUCTIONS_AVAILABLE);
    _LYRA_ADD_PROF_FEATURE(PF_ARM_SVE_F32MM_INSTRUCTIONS_AVAILABLE);
    _LYRA_ADD_PROF_FEATURE(PF_ARM_SVE_F64MM_INSTRUCTIONS_AVAILABLE);
    _LYRA_ADD_PROF_FEATURE(PF_BMI2_INSTRUCTIONS_AVAILABLE);
    _LYRA_ADD_PROF_FEATURE(PF_MOVDIR64B_INSTRUCTION_AVAILABLE);
    _LYRA_ADD_PROF_FEATURE(PF_ARM_LSE2_AVAILABLE);
    _LYRA_ADD_PROF_FEATURE(PF_ARM_SHA3_INSTRUCTIONS_AVAILABLE);
    _LYRA_ADD_PROF_FEATURE(PF_ARM_SHA512_INSTRUCTIONS_AVAILABLE);
    _LYRA_ADD_PROF_FEATURE(PF_ARM_V82_I8MM_INSTRUCTIONS_AVAILABLE);
    _LYRA_ADD_PROF_FEATURE(PF_ARM_V82_FP16_INSTRUCTIONS_AVAILABLE);
    _LYRA_ADD_PROF_FEATURE(PF_ARM_V86_BF16_INSTRUCTIONS_AVAILABLE);
    _LYRA_ADD_PROF_FEATURE(PF_ARM_V86_EBF16_INSTRUCTIONS_AVAILABLE);
    _LYRA_ADD_PROF_FEATURE(PF_ARM_SME_INSTRUCTIONS_AVAILABLE);
    _LYRA_ADD_PROF_FEATURE(PF_ARM_SME2_INSTRUCTIONS_AVAILABLE);
    _LYRA_ADD_PROF_FEATURE(PF_ARM_SME2_1_INSTRUCTIONS_AVAILABLE);
    _LYRA_ADD_PROF_FEATURE(PF_ARM_SME2_2_INSTRUCTIONS_AVAILABLE);
    _LYRA_ADD_PROF_FEATURE(PF_ARM_SME_AES_INSTRUCTIONS_AVAILABLE);
    _LYRA_ADD_PROF_FEATURE(PF_ARM_SME_SBITPERM_INSTRUCTIONS_AVAILABLE);
    _LYRA_ADD_PROF_FEATURE(PF_ARM_SME_SF8MM4_INSTRUCTIONS_AVAILABLE);
    _LYRA_ADD_PROF_FEATURE(PF_ARM_SME_SF8MM8_INSTRUCTIONS_AVAILABLE);
    _LYRA_ADD_PROF_FEATURE(PF_ARM_SME_SF8DP2_INSTRUCTIONS_AVAILABLE);
    _LYRA_ADD_PROF_FEATURE(PF_ARM_SME_SF8DP4_INSTRUCTIONS_AVAILABLE);
    _LYRA_ADD_PROF_FEATURE(PF_ARM_SME_SF8FMA_INSTRUCTIONS_AVAILABLE);
    _LYRA_ADD_PROF_FEATURE(PF_ARM_SME_F8F32_INSTRUCTIONS_AVAILABLE);
    _LYRA_ADD_PROF_FEATURE(PF_ARM_SME_F8F16_INSTRUCTIONS_AVAILABLE);
    _LYRA_ADD_PROF_FEATURE(PF_ARM_SME_F16F16_INSTRUCTIONS_AVAILABLE);
    _LYRA_ADD_PROF_FEATURE(PF_ARM_SME_B16B16_INSTRUCTIONS_AVAILABLE);
    _LYRA_ADD_PROF_FEATURE(PF_ARM_SME_F64F64_INSTRUCTIONS_AVAILABLE);
    _LYRA_ADD_PROF_FEATURE(PF_ARM_SME_I16I64_INSTRUCTIONS_AVAILABLE);
    _LYRA_ADD_PROF_FEATURE(PF_ARM_SME_LUTv2_INSTRUCTIONS_AVAILABLE);
    _LYRA_ADD_PROF_FEATURE(PF_ARM_SME_FA64_INSTRUCTIONS_AVAILABLE);
    //_LYRA_ADD_PROF_FEATURE(PF_UMONITOR_INSTRUCTION_AVAILABLE);

#undef _LYRA_ADD_PROF_FEATURE

    return property_set(std::move(ps));
}
#endif /* defined(_WIN32) */


/*
 * LYRA_DETAIL_NAMESPACE::get_thread_cpu_affinity
 */
std::vector<bool> LYRA_DETAIL_NAMESPACE::get_thread_cpu_affinity(
        _In_ const affinity_mask::thread_handle thread) {
    std::vector<bool> retval;

#if defined(_WIN32) && (_WIN32_WINNT >= 0x0601)
    auto cnt_groups = ::GetActiveProcessorGroupCount();

    if (cnt_groups > 0) {
        GROUP_AFFINITY affinity;
        std::vector<std::uint8_t> buffer;
        auto info = get_logical_processor_info(buffer, RelationGroup);

        if ((info != nullptr) && (info->Relationship == RelationGroup)
                && ::GetThreadGroupAffinity(thread, &affinity)) {
            for (std::size_t i = 0; i < cnt_groups; ++i) {
                if (i == affinity.Group) {
                    // Convert the mask to Booleans.
                    auto& g = info->Group.GroupInfo[i];
                    for (auto j = 0; j < g.ActiveProcessorCount; ++j) {
                        const auto bit = static_cast<KAFFINITY>(1) << j;
                        retval.push_back((affinity.Mask & bit) != 0);
                    }
                } else {
                    // The thread is not running in this group.
                    const auto& g = info->Group.GroupInfo[i];
                    for (auto j = 0; j < g.ActiveProcessorCount; ++j) {
                        retval.push_back(false);
                    }
                }
            }

            return retval;
        }
    } /* if (cnt_groups > 0) */
#elif defined(_WIN32)
    try {
        const auto mask = affinity_mask::thread(thread);
        const auto bits = static_cast<DWORD_PTR>(mask);
        retval = to_booleans(bits, get_os_max_cpus());
    } catch (...) { /* Ignore this and leave result empty. */ }
#else /* defined(_WIN32) && (_WIN32_WINNT >= 0x0601) */
    try {
        const auto cnt = get_os_max_cpus();
        const auto mask = affinity_mask::thread(thread);

        for (std::size_t i = 0; i < cnt; ++i) {
            retval.push_back(CPU_ISSET(i, mask.get()) != 0);
        }

    } catch (...) { /* Ignore this and leave result empty. */ }
#endif /* defined(_WIN32) && (_WIN32_WINNT >= 0x0601) */

    return retval;
}


/*
 * LYRA_DETAIL_NAMESPACE::get_thread_cpu_affinity
 */
std::vector<bool> LYRA_DETAIL_NAMESPACE::get_thread_cpu_affinity(void) {
#if defined(_WIN32)
    return get_thread_cpu_affinity(::GetCurrentThread());
#else /* defined(_WIN32) */
    return get_thread_cpu_affinity(::gettid());
#endif /* defined(_WIN32) */
}


#if defined(_WIN32) && (_WIN32_WINNT >= 0x0601)
/*
 * LYRA_DETAIL_NAMESPACE::get_logical_processor_info
 */
SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX *
LYRA_DETAIL_NAMESPACE::get_logical_processor_info(
        _Inout_ std::vector<std::uint8_t>& buffer,
        _In_ const LOGICAL_PROCESSOR_RELATIONSHIP relationship) {
    if (buffer.empty()) {
        buffer.resize(sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX));
    }

    auto retval = reinterpret_cast<SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX *>(
        buffer.data());
    auto size = static_cast<DWORD>(buffer.size());

    while (true) {
        if (::GetLogicalProcessorInformationEx(relationship, retval, &size)) {
            return retval;
        }

        const auto error = ::GetLastError();
        if (error != ERROR_INSUFFICIENT_BUFFER) {
            ::SetLastError(error);
            return nullptr;
        }

        buffer.resize(size);
        retval = reinterpret_cast<SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX *>(
            buffer.data());
    }
}
#endif /* defined(_WIN32) && (_WIN32_WINNT >= 0x0601) */


/*
 * LYRA_DETAIL_NAMESPACE::get_os_max_cpus
 */
std::size_t LYRA_DETAIL_NAMESPACE::get_os_max_cpus(void) {
#if defined(_WIN32)
#if (_WIN32_WINNT >= 0x0601)
    const auto groups = ::GetActiveProcessorGroupCount();

    if (groups > 0){
        std::vector<std::uint8_t> buffer(groups
            * sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX));
        auto info = get_logical_processor_info(buffer, RelationGroup);

        std::size_t retval = 0;
        if ((info != nullptr) && (info->Relationship == RelationGroup)) {
            for (std::size_t i = 0; i < groups; ++i) {
                retval += info->Group.GroupInfo[i].ActiveProcessorCount;
            }

            return retval;
        }
    }
#endif /* (_WIN32_WINNT >= 0x0601) */

    {
        SYSTEM_INFO info;
        ::GetSystemInfo(&info);
        return info.dwNumberOfProcessors;
    }

#else /* defined(_WIN32) */
    return static_cast<std::size_t>(::sysconf(_SC_NPROCESSORS_CONF));
#endif /* defined(_WIN32) */
}


#if defined(_WIN32) && (_WIN32_WINNT >= 0x0601)
/*
 * LYRA_DETAIL_NAMESPACE::get_processor_groups
 */
std::vector<PROCESSOR_GROUP_INFO> LYRA_DETAIL_NAMESPACE::get_processor_groups(
        void) {
    const auto groups = ::GetActiveProcessorGroupCount();
    std::vector<std::uint8_t> buffer(groups * sizeof(PROCESSOR_GROUP_INFO));
    std::vector<PROCESSOR_GROUP_INFO> retval;

    auto info = get_logical_processor_info(buffer, RelationGroup);
    if (info != nullptr) {
        retval.reserve(groups);
        std::copy_n(info->Group.GroupInfo,
            info->Group.ActiveGroupCount,
            std::back_inserter(retval));
    }

    return retval;
}
#endif /* defined(_WIN32) && (_WIN32_WINNT >= 0x0601) */
