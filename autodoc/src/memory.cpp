// <copyright file="memory.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "visus/autodoc/memory.h"

#include <fstream>
#include <regex>

#if defined(_WIN32)
#include <Windows.h>
#include <Psapi.h>
#else /* !defined(_WIN32) */
#include <unistd.h>
#endif /* !defined(_WIN32) */

#include "equals.h"
#include "property_set_impl.h"


/*
 * LYRA_NAMESPACE::memory::get
 */
LYRA_NAMESPACE::property_set LYRA_NAMESPACE::memory::get(
        _In_ const collection_flags flags) {
    detail::property_set_impl ps;

#if defined(_WIN32)
    ps.add<memory_status>(get_memory_status(flags));
    ps.add<performance_info>(get_performance_info(flags));
#else /* !defined(_WIN32) */
    ps.add<meminfo>(get_meminfo(flags));
#endif /* defined(_WIN32) */

#if !defined(_WIN32)
    ps.add<sysconf>(get_sysconf(flags));
#endif /* !defined(_WIN32) */

    return property_set(std::move(ps));
}


/*
 * LYRA_NAMESPACE::memory::get_meminfo
 */
LYRA_NAMESPACE::property_set LYRA_NAMESPACE::memory::get_meminfo(
        _In_ const collection_flags flags) {
    detail::property_set_impl ps;

#if !defined(_WIN32)
    std::ifstream stream("/proc/meminfo");

    if (stream.is_open()) {
        std::string line;
        std::smatch match;
        std::regex rx("([^:]+)\\s*:\\s*(\\d+)\\s*(.*)");

        while (std::getline(stream, line)) {
            if (std::regex_match(line, match, rx)) {
                auto value = std::stoull(match[2].str());
                if (detail::iequals(match[3].str(), "kB")) {
                    value *= 1000;
                } else if (detail::iequals(match[3].str(), "MB")) {
                    value *= 1000 * 1000;
                } else if (detail::iequals(match[3].str(), "GB")) {
                    value *= 1000 * 1000 * 1000;
                }

                ps.add(match[1].str(), value);
            }
        }
    }
#endif /* defined(_WIN32) */

    return property_set(std::move(ps));
}


/*
 * LYRA_NAMESPACE::memory::get_memory_status
 */
LYRA_NAMESPACE::property_set LYRA_NAMESPACE::memory::get_memory_status(
        _In_ const collection_flags flags) {
    detail::property_set_impl ps;

#if defined(_WIN32)
    MEMORYSTATUSEX status;
    status.dwLength = sizeof(status);

    if (::GlobalMemoryStatusEx(&status)) {
        detail::checked_add(u8"Memory Load", ps, flags, 
            static_cast<std::uint32_t>(status.dwMemoryLoad));
        detail::checked_add<total_physical_memory>(ps, flags,
            status.ullTotalPhys);
        detail::checked_add<available_physical_memory>(ps, flags,
            status.ullAvailPhys);
        detail::checked_add(u8"Total Page File", ps, flags,
            status.ullTotalPageFile);
        detail::checked_add(u8"Available Page File", ps, flags,
            status.ullAvailPageFile);
        detail::checked_add<total_virtual_memory>(ps, flags,
            status.ullTotalVirtual);
        detail::checked_add<available_virtual_memory>(ps, flags,
            status.ullAvailVirtual);
    }
#endif /* defined(_WIN32) */

    return property_set(std::move(ps));
}


/*
 * LYRA_NAMESPACE::memory::get_performance_info
 */
LYRA_NAMESPACE::property_set LYRA_NAMESPACE::memory::get_performance_info(
        _In_ const collection_flags flags) {
    detail::property_set_impl ps;

#if defined(_WIN32)
    PERFORMANCE_INFORMATION info;
    info.cb = sizeof(info);

    if (::GetPerformanceInfo(&info, sizeof(info))) {
        detail::checked_add<total_committed_pages>(ps, flags,
            info.CommitTotal);
        detail::checked_add<total_committed_memory>(ps, flags,
            info.CommitTotal * info.PageSize);
        detail::checked_add<committed_pages_limit>(ps, flags,
            info.CommitLimit);
        detail::checked_add<committed_memory_limit>(ps, flags,
            info.CommitLimit * info.PageSize);
        detail::checked_add(u8"Peak Committed Pages", ps, flags,
            info.CommitPeak);
        detail::checked_add(u8"Peak Committed Memory", ps, flags,
            info.CommitPeak * info.PageSize);
        detail::checked_add<total_physical_pages>(ps, flags,
            info.PhysicalTotal);
        detail::checked_add<total_physical_memory>(ps, flags,
            info.PhysicalTotal * info.PageSize);
        detail::checked_add<available_physical_pages>(ps, flags,
            info.PhysicalAvailable);
        detail::checked_add<available_physical_memory>(ps, flags,
            info.PhysicalAvailable * info.PageSize);
        detail::checked_add<system_cache_pages>(ps, flags,
            info.SystemCache);
        detail::checked_add<total_kernel_pages>(ps, flags,
            info.KernelTotal);
        detail::checked_add<paged_kernel_pages>(ps, flags,
            info.KernelTotal * info.PageSize);
        detail::checked_add<non_paged_kernel_pages>(ps, flags,
            info.KernelNonpaged);
        detail::checked_add<page_size>(ps, flags, info.PageSize);
    }
#endif /* defined(_WIN32) */

    return property_set(std::move(ps));
}


/*
 * LYRA_NAMESPACE::memory::get_sysconf
 */
LYRA_NAMESPACE::property_set LYRA_NAMESPACE::memory::get_sysconf(
        _In_ const collection_flags flags) {
    detail::property_set_impl ps;

#if !defined(_WIN32)
    const auto cnt = static_cast<std::uint64_t>(::sysconf(_SC_PHYS_PAGES));
    const auto size = static_cast<std::uint64_t>(::sysconf(_SC_PAGESIZE));

    detail::checked_add<total_physical_pages>(ps, flags, cnt);
    detail::checked_add<page_size>(ps, flags, size);
    detail::checked_add<total_physical_memory>(ps, flags, cnt * size);
#endif /* !defined(_WIN32) */

    return property_set(std::move(ps));
}
