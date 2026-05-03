// <copyright file="hardware.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "visus/lyra/hardware.h"

#include <unordered_map>

#include "visus/lyra/convert_string.h"
#include "visus/lyra/guid.h"
#include "visus/lyra/trace.h"

#include "setup_api.h"
#include "property_set_impl.h"


#if defined(_WIN32)
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
        LYRA_TRACE(_T("Failed to retrieve device registry property 0x%x: %s"),
            prop, ex.what());
    }

    return false;
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
                //try {
                //    const auto value = LYRA_DETAIL_NAMESPACE::get_device_registry_property(
                //        handle, data, prop);
                //    if (!value.empty()) {
                //        const auto v = reinterpret_cast<const wchar_t*>(value.data());
                //        const auto u = LYRA_NAMESPACE::to_utf8(v);
                //        LYRA_DETAIL_NAMESPACE::checked_add<TProp>(ps, flags, u.c_str());
                //        return true;
                //    }

                //} catch (const std::exception& ex) {
                //    LYRA_TRACE(_T("Failed to retrieve device registry property 0x%x: %s"),
                //        prop, ex.what());
                //}


                detail::property_set_impl dps;
                ::try_add_string_prop<device_class>(dps, flags, handle, data,
                    SPDRP_CLASS);
                ::try_add_string_prop<friendly_name>(dps, flags, handle, data,
                    SPDRP_FRIENDLYNAME);

                //{
                //    const auto v = try_get_prop(SPDRP_DEVICEDESC);
                //    if (!v.empty()) {
                //    };
                //}
                //

                //const auto desc = try_get_prop(SPDRP_DEVICEDESC);


                const auto desc = detail::get_device_registry_property(
                    handle, data, SPDRP_DEVICEDESC);

                const auto hwid = detail::get_device_registry_property(
                    handle, data, SPDRP_HARDWAREID);

                detail::add_device_install_flags(handle, &data, 0,
                DI_FLAGSEX_INSTALLEDDRIVER);
                detail::enum_driver_info(handle, &data, SPDIT_COMPATDRIVER,
                    [&dps, flags](HDEVINFO h, PSP_DEVINFO_DATA d,
                            SP_DRVINFO_DATA_W& drv) {
                        return false;
                    });

                return true;
            }, DIGCF_ALLCLASSES | DIGCF_PRESENT | DIGCF_PROFILE);
    } catch (const std::exception& ex) {
        LYRA_TRACE("Failed to enumerate devices: %s", ex.what());
    }

#endif /* defined(_WIN32)*/

    return property_set(std::move(ps));
}
