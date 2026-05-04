// <copyright file="systemd_device.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "systemd_device.h"

#include <memory>

#include <systemd/sd-device.h>


/*
 * LYRA_DETAIL_NAMESPACE::systemd_device::enumerate
 */
std::vector<LYRA_DETAIL_NAMESPACE::systemd_device>
LYRA_DETAIL_NAMESPACE::systemd_device::enumerate(void) {
    typedef sd_device_enumerator enum_type;
    typedef decltype(&::sd_device_enumerator_unref) del_type;
    std::unique_ptr<enum_type, del_type> enumerator(nullptr,
        &::sd_device_enumerator_unref);
    std::vector<systemd_device> retval;

    // Create a device enumerator and hand it over to the unique pointer if
    // successful.
    {
        sd_device_enumerator *e;
        if (::sd_device_enumerator_new(&e) == 0) {
            enumerator.reset(e);
        }
    }

    // Create descriptor objects for all devices.
    for (auto d = ::sd_device_enumerator_get_device_first(enumerator.get());
            d != nullptr;
            d = ::sd_device_enumerator_get_device_next(enumerator.get())) {
        retval.emplace_back();
        auto& device = retval.back();

        {
            const char *value;
            if (::sd_device_get_driver(d, &value) >= 0) {
                device._driver = value;
            }
        }

        {
            const char *value;
            if (::sd_device_get_devname(d, &value) >= 0) {
                device._name = value;
            }
        }

        {
            const char *value;
            if (::sd_device_get_devpath(d, &value) >= 0) {
                device._path = value;
            }
        }

        {
            const char *value;
            if (::sd_device_get_subsystem(d, &value) >= 0) {
                device._subsystem = value;
            }
        }

        {
            const char *value;
            if (::sd_device_get_syspath(d, &value) >= 0) {
                device._sys_path = value;
            }
        }

        {
            const char *value;
            if (::sd_device_get_devtype(d, &value) >= 0) {
                device._type = value;
            }
        }
    } /* for (auto d =  ... */

    return retval;
}
