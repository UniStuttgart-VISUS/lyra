// <copyright file="mounts.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_MOUNTS_H)
#define _LYRA_MOUNTS_H
#pragma once

#include "visus/autodoc/collection_flags.h"
#include "visus/autodoc/multi_sz.h"
#include "visus/autodoc/property_set.h"


LYRA_NAMESPACE_BEGIN

namespace mounts {

    /// <summary>
    /// Identifies a group of properties holding the mounts on local block
    /// devices.
    /// </summary>
    struct disks final {
        typedef property_set type;
        static constexpr auto name = u8"Disks";
    };

    /// <summary>
    /// Identifies a property that describes the index of a disk.
    /// </summary>
    struct disk_index final {
        typedef std::uint32_t type;
        static constexpr auto name = u8"Disk Index";
    };

    /// <summary>
    /// Identifies a property that describes the file system of a volume.
    /// </summary>
    struct file_system final {
        typedef const char *type;
        static constexpr auto name = u8"File System";
    };

    /// <summary>
    /// Identifies a property that describes the partition a volume belongs to.
    /// </summary>
    struct partition final {
        typedef property_set type;
        static constexpr auto name = u8"Partition";
    };

    /// <summary>
    /// Identifies a property that describes the path of a volume, device, etc.
    /// </summary>
    struct path final {
        typedef const char *type;
        static constexpr auto name = u8"Path";
    };

    /// <summary>
    /// Identifies a property holding the source of a mount.
    /// </summary>
    struct source final {
        typedef const char *type;
        static constexpr auto name = u8"Source";
    };

    /// <summary>
    /// Identifies a property holding the target of a mount (the mount point).
    /// </summary>
    struct target final {
        typedef multi_sz type;
        static constexpr auto name = u8"Target";
    };

    /// <summary>
    /// Gets all all devices we can identify on the current system.
    /// </summary>
    /// <param name="flags">Allows for customising the collection
    /// behaviour.</param>
    /// <returns>The hardware properties of the calling process.</returns>
    LYRA_API property_set get(_In_ const collection_flags flags
        = collection_flags::none);

} /* namespace mounts */

LYRA_NAMESPACE_END

#endif /* !defined(_LYRA_MOUNTS_H) */
