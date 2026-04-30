// <copyright file="memory.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_MEMORY_H)
#define _LYRA_MEMORY_H
#pragma once

#include "visus/lyra/collection_flags.h"
#include "visus/lyra/property_set.h"


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
        static constexpr auto name = u8"Available Physical Memory";
    };

    /// <summary>
    /// Identifies the amount of unreserved and uncommitted memory currently in
    /// the user-mode portion of the virtual address space of the calling 
    /// process.
    /// </summary>
    struct available_virtual_memory final {
        typedef std::uint64_t type;
        static constexpr auto name = u8"Available Virtual Memory";
    };

    /// <summary>
    /// Identifies the property holding the memory status from Win32 API.
    /// </summary>
    struct memory_status final {
        typedef property_set type;
        static constexpr auto name = u8"Memory Status";
    };

    /// <summary>
    /// Identifies a property holding the page size in bytes.
    /// </summary>
    struct page_size final {
        typedef std::uint64_t type;
        static constexpr auto name = u8"Page Size";
    };

    /// <summary>
    /// Identifies a property holding the number of pages.
    /// </summary>
    struct pages final {
        typedef std::uint64_t type;
        static constexpr auto name = u8"Pages";
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
    /// Identifies a property holding the total physical memory.
    /// </summary>
    struct total_physical_memory final {
        typedef std::uint64_t type;
        static constexpr auto name = u8"Total Physical Memory";
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
