// <copyright file="hardware.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "visus/autodoc/hardware.h"

#include <unordered_map>

#include "visus/autodoc/convert_string.h"
#include "visus/autodoc/guid.h"
#include "visus/autodoc/multi_sz.h"
#include "visus/autodoc/timestamp.h"
#include "visus/autodoc/trace.h"

#include "setup_api.h"
#include "property_set_impl.h"


#if defined(_WIN32)
/// <summary>
/// Tries to extract the given property and adds it to <see cref="ps" />.
/// </summary>
template<class TProp> bool try_add_multi_sz_prop(
        _Inout_ LYRA_DETAIL_NAMESPACE::property_set_impl& ps,
        _In_ const LYRA_NAMESPACE::collection_flags flags,
        _In_ HDEVINFO handle,
        _In_ SP_DEVINFO_DATA& data,
        _In_ const DWORD prop) {
    try {
        const auto value = LYRA_DETAIL_NAMESPACE::get_device_registry_property(
            handle, data, prop);
        if (!value.empty()) {
            const auto v = reinterpret_cast<const wchar_t*>(value.data());
            std::vector<const wchar_t *> w;
            LYRA_DETAIL_NAMESPACE::multi_sz_copy(std::back_inserter(w), v);
            LYRA_NAMESPACE::multi_sz m;
            // TODO: this could be optimise to reduce reallocations.
            for (auto s : w) {
                auto t = LYRA_NAMESPACE::to_utf8(s);
                m.add(t.c_str());
            }
            LYRA_DETAIL_NAMESPACE::checked_add<TProp>(ps, flags, std::move(m));
            return true;
        }

    } catch (const std::exception& ex) {
        LYRA_TRACE("Failed to retrieve device registry property 0x%x: %s",
            prop, ex.what());
    }

    return false;
}

/// <summary>
/// Tries to extract the given property and adds it to <see cref="ps" />.
/// </summary>
template<class TProp> bool try_add_prop(
        _Inout_ LYRA_DETAIL_NAMESPACE::property_set_impl& ps,
        _In_ const LYRA_NAMESPACE::collection_flags flags,
        _In_ HDEVINFO handle,
        _In_ SP_DEVINFO_DATA& data,
        _In_ const DWORD prop) {
    try {
        const auto value = LYRA_DETAIL_NAMESPACE::get_device_registry_property(
            handle, data, prop);
        if (value.size() == sizeof(typename TProp::type)) {
            LYRA_DETAIL_NAMESPACE::checked_add<TProp>(ps, flags,
                *reinterpret_cast<const typename TProp::type *>(value.data()));
            return true;
        }
    } catch (const std::exception& ex) {
        LYRA_TRACE("Failed to retrieve device registry property 0x%x: %s",
            prop, ex.what());
    }

    return false;
}

/// <summary>
/// Tries to extract the given property and adds it to <see cref="ps" />.
/// </summary>
template<class TProp> bool try_add_string_prop(
        _Inout_ LYRA_DETAIL_NAMESPACE::property_set_impl& ps,
        _In_ const LYRA_NAMESPACE::collection_flags flags,
        _In_ HDEVINFO handle,
        _In_ SP_DEVINFO_DATA& data,
        _In_ const DWORD prop) {
    try {
        const auto value = LYRA_DETAIL_NAMESPACE::get_device_registry_property(
            handle, data, prop);
        if (!value.empty()) {
            const auto v = reinterpret_cast<const wchar_t*>(value.data());
            const auto u = LYRA_NAMESPACE::to_utf8(v);
            LYRA_DETAIL_NAMESPACE::checked_add<TProp>(ps, flags, u.c_str());
            return true;
        }

    } catch (const std::exception& ex) {
        LYRA_TRACE("Failed to retrieve device registry property 0x%x: %s",
            prop, ex.what());
    }

    return false;
}

/// <summary>
/// Tries to add information about the driver of the given device to
/// <see cref="ps" />.
/// </summary>
static void try_add_driver(
        _Inout_ LYRA_DETAIL_NAMESPACE::property_set_impl& ps,
        _In_ const LYRA_NAMESPACE::collection_flags flags,
        _In_ HDEVINFO handle,
        _In_ SP_DEVINFO_DATA& data) {
    using namespace LYRA_NAMESPACE;
    try {
        detail::add_device_install_flags(handle, &data, 0,
            DI_FLAGSEX_INSTALLEDDRIVER);
        detail::enum_driver_info(handle,
            &data,
            SPDIT_COMPATDRIVER,
            [&](HDEVINFO h, PSP_DEVINFO_DATA d, SP_DRVINFO_DATA_W& drv) {
                using namespace LYRA_NAMESPACE;
                using detail::checked_add;

                detail::property_set_impl dps;
                const auto desc = to_utf8(drv.Description);
                checked_add<hardware::description>(dps, flags,desc.c_str());
                const auto mfg = to_utf8(drv.MfgName);
                checked_add<hardware::manufacturer>(dps, flags, mfg.c_str());
                const auto prov = to_utf8(drv.ProviderName);
                checked_add<hardware::provider>(dps, flags, prov.c_str());
                const auto type = static_cast<std::uint32_t>(drv.DriverType);
                checked_add<hardware::driver_type>(dps, flags, type);
                const auto drv_date = timestamp::from_file_time(drv.DriverDate);
                checked_add<hardware::date>(dps, flags, drv_date);
                const auto ver = std::to_string(drv.DriverVersion);
                checked_add<hardware::version>(dps, flags, ver.c_str());

                auto props = property_set(std::move(dps));
                checked_add<hardware::driver>(ps, flags, std::move(props));
                return false;
            });
    } catch (const std::exception& ex) {
        LYRA_TRACE("Failed to retrieve device driver: %s", ex.what());
    }
}


/// <summary>
/// Tries retrieving the class GUID of the given device.
/// </summary>
static LYRA_NAMESPACE::guid try_get_clsid(_In_ HDEVINFO handle,
        _In_ SP_DEVINFO_DATA& data) {
    LYRA_NAMESPACE::guid retval;
    try {
        const auto value = LYRA_DETAIL_NAMESPACE::get_device_registry_property(
            handle, data, SPDRP_CLASSGUID);
        if (!value.empty()) {
            retval = LYRA_NAMESPACE::guid(value.data());
        }
    } catch (const std::exception& ex) {
        LYRA_TRACE("Failed to retrieve device class: %s", ex.what());
    }

    return retval;
}
#endif /* defined(_WIN32) */


/*
 * LYRA_NAMESPACE::hardware::get
 */
LYRA_NAMESPACE::property_set LYRA_NAMESPACE::hardware::get(
        _In_ const collection_flags flags) {
    detail::property_set_impl ps;

#if defined(_WIN32)
    try {
        std::unordered_map<guid, std::vector<property_set>> classes;

        detail::enum_class_devices(nullptr,
                [&classes, flags](HDEVINFO handle, SP_DEVINFO_DATA& data) {
            const auto clsid = try_get_clsid(handle, data);

            detail::property_set_impl dps;
            ::try_add_string_prop<description>(dps, flags, handle, data,
                SPDRP_DEVICEDESC);
            ::try_add_string_prop<device_class>(dps, flags, handle, data,
                SPDRP_CLASS);
            ::try_add_prop<device_type>(dps, flags, handle, data,
                SPDRP_DEVTYPE);
            ::try_add_driver(dps, flags, handle, data);
            ::try_add_string_prop<friendly_name>(dps, flags, handle, data,
                SPDRP_FRIENDLYNAME);
            ::try_add_string_prop<location>(dps, flags, handle, data,
                SPDRP_LOCATION_INFORMATION);
            ::try_add_string_prop<manufacturer>(dps, flags, handle, data,
                SPDRP_MFG);
            ::try_add_multi_sz_prop<hardware_id>(dps, flags, handle, data,
                SPDRP_HARDWAREID);
            classes[clsid].emplace_back(std::move(dps));

            return true;
        }, DIGCF_ALLCLASSES | DIGCF_PRESENT | DIGCF_PROFILE);

        for (auto& c : classes) {
            const auto clsid = c.first.to_string<char>();
            ps.add(clsid.c_str(), std::move(c.second));
        }
    } catch (const std::exception& ex) {
        LYRA_TRACE("Failed to enumerate devices: %s", ex.what());
    }
#endif /* defined(_WIN32)*/

    return property_set(std::move(ps));
}
