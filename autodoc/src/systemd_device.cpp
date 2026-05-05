// <copyright file="systemd_device.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "systemd_device.h"

#include <filesystem>
#include <memory>

#if defined(LYRA_USE_SYSTEMD)
#include <systemd/sd-device.h>
#endif /* LYRA_USE_SYSTEMD */


/*
 * LYRA_DETAIL_NAMESPACE::systemd_device::enumerate
 */
std::vector<LYRA_DETAIL_NAMESPACE::systemd_device>
LYRA_DETAIL_NAMESPACE::systemd_device::enumerate(void) {
    std::vector<systemd_device> retval;

#if defined(LYRA_USE_SYSTEMD)
    typedef sd_device_enumerator enum_type;
    typedef decltype(&::sd_device_enumerator_unref) del_type;
    std::unique_ptr<enum_type, del_type> enumerator(nullptr,
        &::sd_device_enumerator_unref);

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

        //{
        //    const char *value;
        //    if (::sd_device_get_device_id(d, &value) >= 0) {
        //        device._dev_id = value;
        //    }
        //}

        {
            const char *value;
            if (::sd_device_get_devname(d, &value) >= 0) {
                device._dev_name = value;
            }
        }

        {
            const char *value;
            if (::sd_device_get_devpath(d, &value) >= 0) {
                device._dev_path = value;
            }
        }

        if (::sd_device_get_devnum(d, &device._dev_num) < 0) {
            device._dev_num = 0;
        }

        if (::sd_device_get_diskseq(d, &device._disk_seq) < 0) {
            device._disk_seq = 0;
        }

        {
            const char *value;
            if (::sd_device_get_driver(d, &value) >= 0) {
                device._driver = value;
            }
        }

        //{
        //    const char *value;
        //    if (::sd_device_get_driver_subsystem(d, &value) >= 0) {
        //        device._driver_subsystem = value;
        //    }
        //}

        if (::sd_device_get_ifindex(d, &device._ifindex) < 0) {
            device._ifindex = 0;
        }

        {
            const char *value;
            if (::sd_device_get_devtype(d, &value) >= 0) {
                device._dev_type = value;
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
            if (::sd_device_get_sysname(d, &value) >= 0) {
                device._sys_name = value;
            }
        }

        {
            const char *value;
            if (::sd_device_get_sysnum(d, &value) >= 0) {
                device._sys_num = value;
            }
        }

        {
            const char *value;
            if (::sd_device_get_syspath(d, &value) >= 0) {
                device._sys_path = value;
            }
        }
    } /* for (auto d =  ... */
#endif /* LYRA_USE_SYSTEMD */

    return retval;
}


/*
 * LYRA_DETAIL_NAMESPACE::systemd_device::name
 */
std::string LYRA_DETAIL_NAMESPACE::systemd_device::name(void) const noexcept {
    if (!this->_dev_name.empty()) {
        return this->_dev_name;

    } else if (!this->_sys_name.empty()) {
        return this->_sys_name;

    } else if (!this->_dev_path.empty()) {
        std::filesystem::path p(this->_dev_path);
        return p.filename().string();

    } else {
        std::filesystem::path p(this->_sys_path);
        return p.filename().string();
    }
}
