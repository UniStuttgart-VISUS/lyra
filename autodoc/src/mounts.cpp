// <copyright file="mounts.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "visus/autodoc/mounts.h"

#include <unordered_map>
#include <vector>

#include <wil/resource.h>

#include "visus/autodoc/convert_string.h"

#include "property_set_impl.h"
#include "win32_disks.h"
#include "win32_shares.h"


/*
 * LYRA_NAMESPACE::mounts::get
 */
LYRA_NAMESPACE::property_set LYRA_NAMESPACE::mounts::get(
        _In_ const collection_flags flags) {
    detail::property_set_impl ps;

#if defined(_WIN32)
    // Get the mount points of all local disks.
    try {
        std::vector<property_set> pss;
        const auto disk_infos = detail::get_disks();
        const auto volumes = detail::get_volumes();

        for (auto& v : volumes) {
            detail::property_set_impl ps;
            detail::checked_add<source>(ps, flags, to_utf8(v.path()));

            multi_sz targets;
            for (auto t : v.mount_points()) {
                targets.add(to_utf8(t));
            }
            detail::checked_add<target>(ps, flags, std::move(targets));

            detail::checked_add<file_system>(ps, flags,
                to_utf8(v.file_system()));
            detail::checked_add("Name", ps, flags,
                to_utf8(v.name()));
            detail::checked_add("Volume Serial", ps, flags,
                v.serial());

            std::vector<property_set> extents;
            for (auto& e : v.extents()) {
                detail::property_set_impl ps;
                detail::checked_add<disk_index>(ps, flags,
                    e.DiskNumber);
                detail::checked_add<offset>(ps, flags,
                    e.StartingOffset.QuadPart);
                detail::checked_add<length>(ps, flags,
                    e.ExtentLength.QuadPart);
                extents.emplace_back(std::move(ps));
            }
            detail::checked_add("Extents", ps, flags, std::move(extents));

            std::vector<property_set> vol_disks;
            for (auto& d : disk_infos) {
                if (v.on_disk(d)) {
                    detail::property_set_impl ps;
                    detail::checked_add<device_object>(ps, flags,
                        to_utf8(d.device()));
                    detail::checked_add<path>(ps, flags,
                        to_utf8(d.path()));
                    detail::checked_add<disk_index>(ps, flags,
                        d.number().DeviceNumber);
                    //detail::checked_add("Disk Geometry", ps, flags,
                    //    d.geometry());
                    //detail::checked_add("Disk Layout", ps, flags,
                    //    d.layout());
                    vol_disks.emplace_back(std::move(ps));
                }
            }
            detail::checked_add<disks>(ps, flags, std::move(vol_disks));

            pss.push_back(property_set(std::move(ps)));
        }

        ps.add<disks>(std::move(pss));
    } catch (const std::exception& ex) {
        LYRA_TRACE("Failed to get disk information: %s", ex.what());
    }

    try {
        std::vector<property_set> pss;

        detail::enumerate_resources(RESOURCE_CONNECTED,
            RESOURCETYPE_DISK,
            RESOURCEUSAGE_CONTAINER | RESOURCEUSAGE_ATTACHED,
                [&pss, flags](const NETRESOURCEW& r) {
            if (r.dwUsage != 0) {
                detail::property_set_impl ps;
                detail::checked_add<source>(ps, flags,
                    to_utf8(r.lpRemoteName));
                detail::checked_add<target>(ps, flags,
                    multi_sz::for_string(to_utf8(r.lpLocalName)));
                detail::checked_add("Comment", ps, flags,
                    to_utf8(r.lpComment));
                detail::checked_add("Provider", ps, flags,
                    to_utf8(r.lpProvider));
                pss.push_back(property_set(std::move(ps)));
            }
        });

        ps.add<network>(std::move(pss));
    } catch (const std::exception& ex) {
        LYRA_TRACE("Failed to get share information: %s", ex.what());
    }

#else /* !defined(_WIN32) */
#endif /* defined(_WIN32) */

    return property_set(std::move(ps));
}
