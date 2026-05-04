// <copyright file="memory.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_MEMORY_H)
#define _LYRA_MEMORY_H
#pragma once

#include "visus/autodoc/collection_flags.h"
#include "visus/autodoc/property_set.h"


LYRA_NAMESPACE_BEGIN

namespace memory {

    /// <summary>
    /// Identifies a property holding the available physical memory.
    /// </summary>
    /// <remarks>
    /// On Windows, this is the amount of physical memory that can be
    /// immediately reused without having to write its contents to disk
    /// first. It is the sum of the size of the standby, free, and zero lists.
    /// </remarks>
    struct available_physical_memory final {
        typedef std::uint64_t type;
        static constexpr auto is_variable = true;
        static constexpr auto name = u8"Available Physical Memory";
    };

    /// <summary>
    /// Identifies a property holding the available physical pages.
    /// </summary>
    struct available_physical_pages final {
        typedef std::uint64_t type;
        static constexpr auto is_variable = true;
        static constexpr auto name = u8"Available Physical Pages";
    };

    /// <summary>
    /// Identifies the amount of unreserved and uncommitted memory currently in
    /// the user-mode portion of the virtual address space of the calling 
    /// process.
    /// </summary>
    struct available_virtual_memory final {
        typedef std::uint64_t type;
        static constexpr auto is_variable = true;
        static constexpr auto name = u8"Available Virtual Memory";
    };

    /// <summary>
    /// Identifies a property holding the maximum number of pages that can be
    /// committed without extending the page file.
    /// </summary>
    struct committed_pages_limit final {
        typedef std::uint64_t type;
        static constexpr auto is_variable = true;
        static constexpr auto name = u8"Committed Pages Limit";
    };

    /// <summary>
    /// Identifies a property holding the maximum amount of memory in bytes that
    /// can be committed without extending the page file.
    /// </summary>
    struct committed_memory_limit final {
        typedef std::uint64_t type;
        static constexpr auto is_variable = true;
        static constexpr auto name = u8"Committed Memory Limit";
    };

    /// <summary>
    /// Identifies the property holding the memory status from Win32 API.
    /// </summary>
    struct memory_status final {
        typedef property_set type;
        static constexpr auto is_variable = true;
        static constexpr auto name = u8"Memory Status";
    };

    /// <summary>
    /// Identifies a property holding the number of pages currently used by
    /// the kernel in non-paged memory.
    /// </summary>
    struct non_paged_kernel_pages final {
        typedef std::uint64_t type;
        static constexpr auto is_variable = true;
        static constexpr auto name = u8"Non-Paged Kernel Pages";
    };

    /// <summary>
    /// Identifies a property holding the number of pages currently used by
    /// the kernel in paged memory.
    /// </summary>
    struct paged_kernel_pages final {
        typedef std::uint64_t type;
        static constexpr auto is_variable = true;
        static constexpr auto name = u8"Paged Kernel Pages";
    };

    /// <summary>
    /// Identifies a property holding the page size in bytes.
    /// </summary>
    struct page_size final {
        typedef std::uint64_t type;
        static constexpr auto is_variable = true;
        static constexpr auto name = u8"Page Size";
    };

    /// <summary>
    /// Identifies a property holding the memory information obtained from the
    /// performance information API on Windows.
    /// </summary>
    struct performance_info final {
        typedef property_set type;
        static constexpr auto is_variable = true;
        static constexpr auto name = u8"Performance Info";
    };

    /// <summary>
    /// Identifies a property holding the memory information from sysconf on
    /// POSIX systems.
    /// </summary>
    struct sysconf final {
        typedef property_set type;
        static constexpr auto name = u8"Sysconf";
    };

    /// <summary>
    /// Identifies a property holding the number of pages used for system cache,
    /// which is the standby list plus the system working set on Windows.
    /// </summary>
    struct system_cache_pages final {
        typedef std::uint64_t type;
        static constexpr auto is_variable = true;
        static constexpr auto name = u8"System Cache Pages";
    };

    /// <summary>
    /// Identifies a property holding the total amount of committed memory.
    /// </summary>
    struct total_committed_memory final {
        typedef std::uint64_t type;
        static constexpr auto is_variable = true;
        static constexpr auto name = u8"Total Committed Memory";
    };

    /// <summary>
    /// Identifies a property holding the number of pages committed by the
    /// system.
    /// </summary>
    /// <remarks>
    /// On windows, committing pages using <see cref="VirtualAlloc" /> with
    /// <c>MEM_COMMIT</c> increases this value immediately. However, the
    /// pyhsical memory is not changed until the pages are accessed.
    /// </remarks>
    struct total_committed_pages final {
        typedef std::uint64_t type;
        static constexpr auto is_variable = true;
        static constexpr auto name = u8"Total Committed Pages";
    };

    /// <summary>
    /// The total number of pages currently used by the kernel.
    /// </summary>
    struct total_kernel_pages final {
        typedef std::uint64_t type;
        static constexpr auto is_variable = true;
        static constexpr auto name = u8"Total Kernel Pages";
    };

    /// <summary>
    /// Identifies a property holding the total physical memory.
    /// </summary>
    struct total_physical_memory final {
        typedef std::uint64_t type;
        static constexpr auto name = u8"Total Physical Memory";
    };

    /// <summary>
    /// Identifies a property holding the total number of physical memory pages.
    /// </summary>
    struct total_physical_pages final {
        typedef std::uint64_t type;
        static constexpr auto name = u8"Total Physical Pages";
    };

    /// <summary>
    /// Identifies a property holding the total virtual memory.
    /// </summary>
    struct total_virtual_memory final {
        typedef std::uint64_t type;
        static constexpr auto name = u8"Total Virtual Memory";
    };

    /// <summary>
    /// Gets everything we know about the memory.
    /// </summary>
    /// <param name="flags">Allows for customising the collection
    /// behaviour.</param>
    /// <returns>A property set describing the RAM.</returns>
    LYRA_API property_set get(_In_ const collection_flags flags
        = collection_flags::none);

    /// <summary>
    /// Gets the current memory status via a Win32 API call.
    /// </summary>
    /// <param name="flags">Allows for customising the collection
    /// behaviour.</param>
    /// <returns>A property set describing the memory as reported by Win32 API.
    /// </returns>
    LYRA_API property_set get_memory_status(_In_ const collection_flags flags
        = collection_flags::none);

    /// <summary>
    /// Gets the current memory information via the performance information API
    /// on Windows.
    /// </summary>
    /// <param name="flags">Allows for customising the collection
    /// behaviour.</param>
    /// <returns>A property set describing the memory as reported by Win32 API.
    /// </returns>
    LYRA_API property_set get_performance_info(_In_ const collection_flags flags
        = collection_flags::none);

    /// <summary>
    /// Gets the page size and number of pages from sysconf.
    /// </summary>
    /// <param name="flags">Allows for customising the collection
    /// behaviour.</param>
    /// <returns>A property set describing the memory as reported by sysconf.
    /// </returns>
    LYRA_API property_set get_sysconf(_In_ const collection_flags flags
        = collection_flags::none);

} /* namespace memory */

LYRA_NAMESPACE_END

#endif /* !defined(_LYRA_CPU_H) */
