// <copyright file="os_cpu_info.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "os_cpu_info.h"

#if !defined(_WIN32)
#include <sched.h>
#include <unistd.h>
#endif /* !defined(_WIN32) */

#include "visus/autodoc/on_exit.h"

#include "contains.h"



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
    {
        DWORD_PTR mask;
        if (::GetProcessAffinityMask(::GetCurrentProcess(), &mask, nullptr)) {
            const auto limit = (std::min)(get_os_max_cpus(),
                static_cast<std::size_t>(sizeof(DWORD_PTR) * CHAR_BIT));
            retval.resize(limit);

            for (std::size_t i = 0; i < limit; ++i) {
                const auto bit = static_cast<DWORD_PTR>(1) << i;
                retval[i] = (mask & bit) != 0;
            }
        }
    }

#else /* defined(_WIN32) */
    const auto cnt = get_os_max_cpus();

    auto cpus = CPU_ALLOC(cnt);
    if (cpus != nullptr) {
        LYRA_ON_EXIT([cpus](void) { CPU_FREE(cpus); });
        const auto size = CPU_ALLOC_SIZE(cnt);
        CPU_ZERO_S(size, cpus);

        if (::sched_getaffinity(0, size, cpus) == 0) {
            for (std::size_t i = 0; i < cnt; ++i) {
                retval.push_back(CPU_ISSET(i, cpus) != 0);
            }
        }
    }
#endif /* defined(_WIN32) */

    return retval;
}


#if defined(_WIN32) && (_WIN32_WINNT >= 0x0601)
/*
 * LYRA_DETAIL_NAMESPACE::get_logical_processor_info
 */
SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX *
LYRA_DETAIL_NAMESPACE::get_logical_processor_info(
        _Out_ std::vector<std::uint8_t>& buffer,
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
