// <copyright file="cpu.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_CPU_H)
#define _LYRA_CPU_H
#pragma once

#include "visus/autodoc/collection_flags.h"
#include "visus/autodoc/property_set.h"


LYRA_NAMESPACE_BEGIN

namespace cpu {

    /// <summary>
    /// Identifies the processor brand string as reported by the CPUID
    /// instruction.
    /// </summary>
    struct brand_string final {
        typedef const char *type;
        static constexpr auto is_immutable = true;
        static constexpr auto name = u8"Brand String";
    };

    /// <summary>
    /// Identifies a property that holds (hierarchically organised) information
    /// about the cache.
    /// </summary>
    struct cache final {
        typedef property_set type;
        static constexpr auto is_immutable = true;
        static constexpr auto name = u8"Cache";
    };

    /// <summary>
    /// Identifies a property that holds (hierarchically organised) information
    /// about a single CPU core
    /// </summary>
    struct core final {
        typedef property_set type;
        static constexpr auto name = u8"Core";
    };

    /// <summary>
    /// Identifies a property set that holds all information that is available
    /// via a CPUID query.
    /// </summary>
    struct cpuid final {
        typedef property_set type;
        static constexpr auto is_immutable = true;
        static constexpr auto is_sensitive = true;
        static constexpr auto name = u8"CPUID";
    };

    /// <summary>
    /// Identifies a property set that holds information about specific features
    /// the CPU might support.
    /// </summary>
    struct features final {
        typedef property_set type;
        static constexpr auto is_immutable = true;
        static constexpr auto is_uncommon = false;
        static constexpr auto name = u8"Features";
    };

    /// <summary>
    /// Identifies a property that holds (hierarchically organised) information
    /// about a NUMA node.
    /// </summary>
    struct numa_node final {
        typedef property_set type;
        static constexpr auto name = u8"NUMA Node";
    };

    /// <summary>
    /// Identifies a property holding the ordinal number of a CPU socket, core
    /// or NUMA node. All of these three entities have this property.
    /// </summary>
    struct number final {
        typedef std::uint32_t type;
        static constexpr auto name = u8"Number";
    };

    /// <summary>
    /// Identifies a property set that holds information about specific features
    /// the CPU might support as reported by the operating system.
    /// </summary>
    struct os_features final {
        typedef property_set type;
        static constexpr auto is_immutable = true;
        static constexpr auto is_uncommon = false;
        static constexpr auto name = u8"OS-Reported Features";
    };

    /// <summary>
    /// Identifies a property set that holds information about the SIMD
    /// instruction sets that are available.
    /// </summary>
    struct simd_instructions final {
        typedef property_set type;
        static constexpr auto is_immutable = true;
        static constexpr auto name = u8"SIMD";
    };

    /// <summary>
    /// Identifies a property that holds (hierarchically organised) information
    /// about the CPU in a specific socket.
    /// </summary>
    struct socket final {
        typedef property_set type;
        static constexpr auto name = u8"Socket";
    };

    /// <summary>
    /// Identifies a property that holds the hierarchically organised CPU
    /// topology of the system.
    /// </summary>
    struct topology final {
        typedef property_set type;
        static constexpr auto is_immutable = true;
        static constexpr auto name = u8"Topology";
    };

    /// <summary>
    /// Identifies the vendor of a CPU as reported by the CPUID instruction.
    /// </summary>
    struct vendor final {
        typedef const char *type;
        static constexpr auto is_immutable = true;
        static constexpr auto name = u8"Vendor";
    };

    /// <summary>
    /// Gets all information that is available about the CPU.
    /// </summary>
    /// <param name="flags">Allows for customising the collection
    /// behaviour.</param>
    LYRA_API property_set get(_In_ const collection_flags flags
        = collection_flags::none);

    /// <summary>
    /// Gets the information that is available via the CPUID instruction.
    /// </summary>
    /// <param name="flags">Allows for customising the collection
    /// behaviour.</param>
    /// <returns>A property set describing the CPU.</returns>
    LYRA_API property_set get_cpuid(_In_ const collection_flags flags
        = collection_flags::none);

    /// <summary>
    /// Gets a description of the CPU topology of the system.
    /// </summary>
    /// <param name="flags"></param>
    /// <returns></returns>
    LYRA_API property_set get_topology(_In_ const collection_flags flags
        = collection_flags::none);

} /* namespace cpu */

LYRA_NAMESPACE_END

#endif /* !defined(_LYRA_CPU_H) */
