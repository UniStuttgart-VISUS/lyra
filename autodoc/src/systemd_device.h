// <copyright file="systemd_device.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_SYSTEMD_DEVICE_H)
#define _LYRA_SYSTEMD_DEVICE_H
#pragma once

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

private:

    std::string _driver;
    std::string _name;
    std::string _path;
    std::string _subsystem;
    std::string _sys_path;
    std::string _type;
};

LYRA_DETAIL_NAMESPACE_END

#endif /* !defined(_LYRA_SYSTEMD_DEVICE_H) */
