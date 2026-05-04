// <copyright file="sysfs_device.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "sysfs_device.h"


/*
 * LYRA_DETAIL_NAMESPACE::sysfs_device::from_path
 */
std::vector<LYRA_DETAIL_NAMESPACE::sysfs_device>
LYRA_DETAIL_NAMESPACE::sysfs_device::from_path(_In_opt_z_ const char *path) {
    std::vector<sysfs_device> retval;

    if (path == nullptr) {
        std::set<sysfs_device> set;
        from_path(std::inserter(set, set.end()), devices_path);
        from_path(std::inserter(set, set.end()), block_path);
        from_container(std::inserter(set, set.end()), classes_path);
        from_container(std::inserter(set, set.end()), bus_path);
        retval.reserve(set.size());
        std::copy(set.begin(), set.end(), std::back_inserter(retval));

    } else {
        from_path(std::back_inserter(retval), path);
    }

    return retval;
}


/*
 * LYRA_DETAIL_NAMESPACE::sysfs_device::sysfs_device
 */
LYRA_DETAIL_NAMESPACE::sysfs_device::sysfs_device(
        _In_ const std::filesystem::path& path)
        : _name(path.filename().string()) {
    assert(path.is_absolute());
    assert(path.has_parent_path());
    static const std::filesystem::path buses(bus_path);
    static const std::filesystem::path classes(classes_path);
    static const auto find_type = [](
            std::string& d,
            const std::filesystem::path& p,
            const std::filesystem::path& r) {
        if (d.empty()) {
            auto it = std::search(p.begin(), p.end(), r.begin(), r.end());
            if (it == p.begin()) {
                std::advance(it, std::distance(r.begin(), r.end()));
                d = it->filename();
            }
        }
    };

    // Check whether we got the device via its class or bus directory, in which
    // case we can directly set the respective member variable.
    find_type(this->_bus, path, buses);
    find_type(this->_class, path, classes);

    const auto subsystem = path / "subsystem";
    if (std::filesystem::is_symlink(subsystem)) {
        const auto target = std::filesystem::read_symlink(subsystem);
        find_type(this->_bus, target, buses);
        find_type(this->_class, target, classes);
    }
}
