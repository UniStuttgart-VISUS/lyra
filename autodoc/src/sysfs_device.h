// <copyright file="sysfs_device.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_SYSFS_DEVICE_H)
#define _LYRA_SYSFS_DEVICE_H
#pragma once

#include <algorithm>
#include <cassert>
#include <filesystem>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

#include "visus/autodoc/trace.h"


LYRA_DETAIL_NAMESPACE_BEGIN

/// <summary>
/// A container for the device information obtained from sysfs on Linux.
/// </summary>
class LYRA_TEST_API sysfs_device final {

public:

    static std::vector<sysfs_device> from_path(_In_opt_z_ const char *path
        = nullptr, _In_ const bool recursive = false);

    static constexpr const char *block_path = "/sys/block";
    static constexpr const char *bus_path = "/sys/bus";
    static constexpr const char *classes_path = "/sys/class";
    static constexpr const char *devices_path = "/sys/devices";

    /// <summary>
    /// Gets the BUS type of the device.
    /// </summary>
    /// <returns></returns>
    inline const std::string& bus(void) const noexcept {
        return this->_bus;
    }

    /// <summary>
    /// Gets the device ID.
    /// </summary>
    /// <returns></returns>
    inline const std::string& device(void) const noexcept {
        return this->_device;
    }

    /// <summary>
    /// Gets the name of the device class.
    /// </summary>
    /// <returns></returns>
    inline const std::string& device_class(void) const noexcept {
        return this->_class;
    }

    /// <summary>
    /// Gets the path to the driver if known.
    /// </summary>
    /// <returns></returns>
    inline const std::string& driver(void) const noexcept {
        return this->_driver;
    }

    /// <summary>
    /// Gets the name of the device in the sys file system.
    /// </summary>
    /// <returns></returns>
    inline const std::string& name(void) const noexcept {
        return this->_name;
    }

    /// <summary>
    /// Gets the path by which the device was discovered in sysfs.
    /// </summary>
    /// <returns></returns>
    inline const std::string& path(void) const noexcept {
        return this->_path;
    }

    /// <summary>
    /// Gets the contents of the uevent file at the time the snapshot was taken.
    /// </summary>
    /// <returns></returns>
    const std::unordered_map<std::string, std::string>& uevent(
            void) const noexcept {
        return this->_uevent;
    }

    /// <summary>
    /// Gets the name of the vendor, or more likely, the PCI ID of the vendor.
    /// </summary>
    /// <returns></returns>
    inline const std::string& vendor(void) const noexcept {
        return this->_vendor;
    }

private:

    template<class TIterator> static std::size_t from_path(_In_ TIterator oit,
        _In_ const std::filesystem::path& path, _In_ const bool recursive);

    /// <summary>
    /// Initialises a new instance.
    /// </summary>
    /// <param name="path">The path to the device folder, which will be used to
    /// search for additional information.</param>
    sysfs_device(_In_ const std::filesystem::path& path);

    std::string _bus;
    std::string _class;
    std::string _device;
    std::string _driver;
    std::string _name;
    std::string _path;
    std::unordered_map<std::string, std::string> _uevent;
    std::string _vendor;
};

LYRA_DETAIL_NAMESPACE_END


/// <summary>
/// Orders a device based on its path, which should be a unique key.
/// </summary>
template<> struct std::less<LYRA_DETAIL_NAMESPACE::sysfs_device> final {
    typedef LYRA_DETAIL_NAMESPACE::sysfs_device value_type;

    inline bool operator ()(_In_ const value_type& lhs,
            _In_ const value_type& rhs) const noexcept {
        constexpr std::less<std::string> less;
        return less(lhs.path(), rhs.path());
    }
};

#include "sysfs_device.inl"

#endif /* !defined(_LYRA_SYSFS_DEVICE_H) */
