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
#include <vector>

#include "visus/autodoc/trace.h"


LYRA_DETAIL_NAMESPACE_BEGIN

/// <summary>
/// A container for the device information obtained from sysfs on Linux.
/// </summary>
class LYRA_TEST_API sysfs_device final {

public:

    static std::vector<sysfs_device> from_path(_In_opt_z_ const char *path
        = nullptr);

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
    /// Gets the name of the device class.
    /// </summary>
    /// <returns></returns>
    inline const std::string& device_class(void) const noexcept {
        return this->_class;
    }

    /// <summary>
    /// Gets the name of the device in the sys file system.
    /// </summary>
    /// <returns></returns>
    inline const std::string& name(void) const noexcept {
        return this->_name;
    }

    /// <summary>
    /// Gets the name of the vendor, or more likely, the PCI ID of the vendor.
    /// </summary>
    /// <returns></returns>
    inline const std::string& vendor(void) const noexcept {
        return this->_vendor;
    }

private:

    template<class TIterator > static std::size_t from_container(
        _In_ TIterator oit, _In_ const std::filesystem::path& path);

    template<class TIterator> static std::size_t from_path(_In_ TIterator oit,
        _In_ const std::filesystem::path& path);

    /// <summary>
    /// Initialises a new instance.
    /// </summary>
    /// <param name="path">The path to the device folder, which will be used to
    /// search for additional information.</param>
    sysfs_device(_In_ const std::filesystem::path& path);

    std::string _bus;
    std::string _class;
    std::string _name;
    std::string _vendor;
};

LYRA_DETAIL_NAMESPACE_END


/// <summary>
/// Orders a device based on its name.
/// </summary>
template<> struct std::less<LYRA_DETAIL_NAMESPACE::sysfs_device> final {
    typedef LYRA_DETAIL_NAMESPACE::sysfs_device value_type;

    inline bool operator ()(_In_ const value_type& lhs,
            _In_ const value_type& rhs) const noexcept {
        constexpr std::less<std::string> less;
        return less(lhs.name(), rhs.name());
    }
};

#include "sysfs_device.inl"

#endif /* !defined(_LYRA_SYSFS_DEVICE_H) */
