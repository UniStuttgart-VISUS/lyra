// <copyright file="memory.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "visus/lyra/memory.h"

#if defined(_WIN32)
#include <Windows.h>
#else /* !defined(_WIN32) */
#include <unistd.h>
#endif /* !defined(_WIN32) */

#include "property_set_impl.h"


/*
 * LYRA_NAMESPACE::memory::get
 */
LYRA_NAMESPACE::property_set LYRA_NAMESPACE::memory::get(
        _In_ const collection_flags flags) {
    detail::property_set_impl ps;

#if defined(_WIN32)
    ps.add<memory_status>(get_memory_status(flags));
#endif /* defined(_WIN32) */

#if !defined(_WIN32)
    ps.add<sysconf>(get_sysconf(flags));
#endif /* !defined(_WIN32) */

    return detail::to_property_set(std::move(ps));
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

    return detail::to_property_set(std::move(ps));

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

    detail::checked_add<pages>(ps, flags, cnt);
    detail::checked_add<page_size>(ps, flags, size);
    detail::checked_add<total_physical_memory>(ps, flags, cnt * size);
#endif /* !defined(_WIN32) */

    return detail::to_property_set(std::move(ps));
}
