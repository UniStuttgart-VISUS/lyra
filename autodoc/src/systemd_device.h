// <copyright file="systemd_device.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_SYSTEMD_DEVICE_H)
#define _LYRA_SYSTEMD_DEVICE_H
#pragma once

#include <cinttypes>
#include <string>
#include <vector>

#include "visus/autodoc/api.h"


LYRA_DETAIL_NAMESPACE_BEGIN

/// <summary>
/// A container for the device information obtained from via libsystemd.
/// </summary>
class LYRA_TEST_API systemd_device final {

public:

    /// <summary>
    /// Enumerates all devices known to libsystemd.
    /// </summary>
    /// <returns>A list of devices found using libsystemd. This list may be
    /// be empty if the system does not have the library.</returns>
    static std::vector<systemd_device> enumerate(void);

    /// <summary>
    /// Initialises a new instance.
    /// </summary>
    inline systemd_device(void) noexcept : _dev_num(0), _disk_seq(0),
        _ifindex(0) { }

    /// <summary>
    /// Gets the device node path of the device, provided the device has an
    /// associated device node.
    /// </summary>
    /// <returns></returns>
    inline const std::string dev_name(void) const noexcept {
        return this->_dev_name;
    }

    /// <summary>
    /// Returns the the device node major/minor if the device has an associated
    /// device node.
    /// </summary>
    /// <returns></returns>
    inline dev_t dev_num(void) const noexcept {
        return this->_dev_num;
    }

    /// <summary>
    /// Gets the sysfs path of the device.
    /// </summary>
    /// <returns></returns>
    inline const std::string& dev_path(void) const noexcept {
        return this->_dev_path;
    }

    /// <summary>
    /// Gets the type of the device if the subsystem manages multiple types of
    /// devices.
    /// </summary>
    /// <returns></returns>
    inline const std::string& dev_type(void) const noexcept {
        return this->_dev_type;
    }

    /// <summary>
    /// Gets the kernel disk sequence number of a block device.
    /// </summary>
    /// <returns></returns>
    inline std::uint64_t disk_seq(void) const noexcept {
        return this->_disk_seq;
    }

    /// <summary>
    /// Gets the kernel driver name attached to the device.
    /// </summary>
    /// <returns></returns>
    inline const std::string& driver(void) const noexcept {
        return this->_driver;
    }

    /// <summary>
    /// Gets the network interface index of the device if the device is a
    /// network interface device.
    /// </summary>
    /// <remarks>
    /// The &quot;lo&quot; interface typically has index 1.
    /// </remarks>
    /// <returns></returns>
    inline int ifindex(void) const noexcept {
        return this->_ifindex;
    }

    /// <summary>
    /// Gets the name of the device, either directly derived from systemd or
    /// from one of the device paths.
    /// </summary>
    /// <returns></returns>
    std::string name(void) const noexcept;

    /// <summary>
    /// Gets the kernel subsystem the device belongs to.
    /// </summary>
    /// <returns></returns>
    inline const std::string& subsystem(void) const noexcept {
        return this->_subsystem;
    }

    /// <summary>
    /// Gets the sysfs path of the device.
    /// </summary>
    /// <returns></returns>
    inline const std::string& sys_path(void) const noexcept {
        return this->_sys_path;
    }

    /// <summary>
    /// Gets the name of the sysfs entry of the device.
    /// </summary>
    /// <returns></returns>
    inline const std::string& sys_name(void) const noexcept {
        return this->_sys_name;
    }

    /// <summary>
    /// Gets the sysfs device number of the record, typically the numerical
    /// suffice of the name.
    /// </summary>
    /// <returns></returns>
    inline const std::string& sys_num(void) const noexcept {
        return this->_sys_num;
    }

private:

    std::string _dev_name;
    dev_t _dev_num;
    std::string _dev_path;
    std::string _dev_type;
    std::uint64_t _disk_seq;
    std::string _driver;
    int _ifindex;
    std::string _subsystem;
    std::string _sys_path;
    std::string _sys_name;
    std::string _sys_num;
};

LYRA_DETAIL_NAMESPACE_END

#endif /* !defined(_LYRA_SYSTEMD_DEVICE_H) */
