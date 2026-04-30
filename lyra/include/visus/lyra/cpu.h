// <copyright file="cpu.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_CPU_H)
#define _LYRA_CPU_H
#pragma once

#include "visus/lyra/collection_flags.h"
#include "visus/lyra/property_set.h"


LYRA_NAMESPACE_BEGIN

namespace cpu {

    /// <summary>
    /// Identifies the processor brand string as reported by the CPUID
    /// instruction.
    /// </summary>
    struct brand_string final {
        typedef const char *type;
        static constexpr auto name = u8"Brand String";
    };

    /// <summary>
    /// Identifies a property set that holds all information that is available
    /// via a CPUID query.
    /// </summary>
    struct cpuid final {
        typedef property_set type;
        static constexpr auto is_sensitive = true;
        static constexpr auto name = u8"CPUID";
    };

    /// <summary>
    /// Identifies a property set that holds information about specific
    /// instruction sets the CPU might support.
    /// </summary>
    struct instructions final {
        typedef property_set type;
        static constexpr auto name = u8"Instructions";
    };

    /// <summary>
    /// Identifies a property set that holds information about the SIMD
    /// instruction sets that are available.
    /// </summary>
    struct simd_instructions final {
        typedef property_set type;
        static constexpr auto name = u8"SIMD";
    };

    /// <summary>
    /// Identifies the vendor of a CPU as reported by the CPUID instruction.
    /// </summary>
    struct vendor final {
        typedef const char *type;
        static constexpr auto name = u8"Vendor";
    };

    /// <summary>
    /// Gets the information that is available via the CPUID instruction.
    /// </summary>
    /// <param name="flags">Allows for customising the collection
    /// behaviour.</param>
    /// <returns>A property set describing the CPU.</returns>
    LYRA_API property_set get_cpuid(_In_ const collection_flags flags
        = collection_flags::none);

} /* namespace cpu */

LYRA_NAMESPACE_END

#endif /* !defined(_LYRA_CPU_H) */
