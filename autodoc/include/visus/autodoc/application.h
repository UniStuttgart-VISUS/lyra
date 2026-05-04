// <copyright file="application.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_APPLICATION_H)
#define _LYRA_APPLICATION_H
#pragma once

#include "visus/autodoc/collection_flags.h"
#include "visus/autodoc/property_set.h"


LYRA_NAMESPACE_BEGIN

namespace application {

    /// <summary>
    /// Identifies the property holding the command line.
    /// </summary>
    struct command_line final {
        typedef const char *type;
        static constexpr auto name = u8"Command Line";
    };

    /// <summary>
    /// Identifies the property holding the path to the executable.
    /// </summary>
    struct executable final {
        typedef const char *type;
        static constexpr auto name = u8"Executable";
    };

    /// <summary>
    /// Identifies the property holding a hash of the executable.
    /// </summary>
    struct hash final {
        typedef const char *type;
        static constexpr auto name = u8"Hash";
    };

    /// <summary>
    /// Identifies the property holding all libraries loaded by the calling
    /// process at the time of data collection.
    /// </summary>
    struct libraries final {
        typedef property_set type;
        static constexpr auto is_variable = true;
        static constexpr auto name = u8"Libraries";
    };

    /// <summary>
    /// Identifies the property holding the process ID.
    /// </summary>
    struct process_id final {
        typedef std::uint64_t type;
        static constexpr auto name = u8"Process ID";
    };

    /// <summary>
    /// Identifies the property holding the size of an executable in bytes.
    /// </summary>
    struct size final {
        typedef std::uint64_t type;
        static constexpr auto name = u8"Size";
    };

    /// <summary>
    /// Gets all information we have about the application under test (the
    /// calling process).
    /// </summary>
    /// <param name="flags">Allows for customising the collection
    /// behaviour.</param>
    /// <returns>A property set describing the CPU.</returns>
    LYRA_API property_set get(_In_ const collection_flags flags
        = collection_flags::none);

} /* namespace application */

LYRA_NAMESPACE_END

#endif /* !defined(_LYRA_APPLICATION_H) */
