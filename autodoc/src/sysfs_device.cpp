// <copyright file="sysfs_device.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "sysfs_device.h"

#include <fstream>
#include <regex>

#include "file.h"


/*
 * LYRA_DETAIL_NAMESPACE::sysfs_device::from_path
 */
std::vector<LYRA_DETAIL_NAMESPACE::sysfs_device>
LYRA_DETAIL_NAMESPACE::sysfs_device::from_path(_In_opt_z_ const char *path,
        _In_ const bool recursive) {
    std::set<sysfs_device> set;

    if (path == nullptr) {
        from_path(std::inserter(set, set.end()), devices_path, true);
    } else {
        from_path(std::inserter(set, set.end()), path, recursive);
    }

    std::vector<sysfs_device> retval;
    retval.reserve(set.size());
    std::copy(set.begin(), set.end(), std::back_inserter(retval));

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
                d = it->filename().string();
            }
        }
    };

    // Remember the path.
    this->_path = final_path(path.c_str());

    // Check whether we got the device via its class or bus directory, in which
    // case we can directly set the respective member variable.
    find_type(this->_bus, path, buses);
    find_type(this->_class, path, classes);

    // Check the subsystem symlink and resolve its final path. Check whether
    // this path contains the bus or the class.
    const auto subsystem = path / "subsystem";
    if (std::filesystem::is_symlink(subsystem)) {
        const auto target = final_path(subsystem.c_str());
        find_type(this->_bus, target, buses);
        find_type(this->_class, target, classes);
    }

    // Check whether there is file with the device ID and store it.
    try {
        const auto p = path / "device";
        std::ifstream stream(p);
        if (stream.is_open()) {
            stream >> this->_device;
        }
    } catch (const std::exception& ex) {
        LYRA_TRACE("Failed reading sysfs: %s", ex.what());
    }

    // Preserve the actual path to the driver node.
    {
        const auto p = path / "driver";
        if (std::filesystem::is_symlink(p)) {
            this->_driver = final_path(p.c_str());
        } else if (std::filesystem::exists(p)) {
            this->_driver = p.string();
        }
    }

    // Check whether there is a name file and if so, read its contents.
    try {
        const auto p = path / "name";
        std::ifstream stream(p);
        if (stream.is_open()) {
            stream >> this->_name;
        }
    } catch (const std::exception& ex) {
        LYRA_TRACE("Failed reading sysfs: %s", ex.what());
    }

    // Check whether there is a uevent file and if so, parse its key/value
    // pairs into a hash table.
    try {
        const auto p = path / "uevent";
        std::ifstream stream(p);
        if (stream.is_open()) {
            std::string line;
            std::smatch match;
            std::regex rx("([^=]+)\\s*=(.*)");

            while (std::getline(stream, line)) {
                if (std::regex_match(line, match, rx)) {
                    this->_uevent[match[1].str()] = match[2].str();
                }
            }
        }
    } catch (const std::exception& ex) {
        LYRA_TRACE("Failed reading sysfs: %s", ex.what());
    }

    // Check whether there is a vendor file and if so, read its contents.
    try {
        const auto p = path / "vendor";
        std::ifstream stream(p);
        if (stream.is_open()) {
            stream >> this->_vendor;
        }
    } catch (const std::exception& ex) {
        LYRA_TRACE("Failed reading sysfs: %s", ex.what());
}
}
