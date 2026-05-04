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

    static std::vector<systemd_device> enumerate(void);

    /// <summary>
    /// Initialises a new instance.
    /// </summary>
    inline systemd_device(void) noexcept : _dev_num(0), _disk_seq(0),
        _ifindex(0) { }

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
