// <copyright file="graphics.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "visus/lyra/graphics.h"

#include "dx_graphics.h"
#include "property_set_impl.h"
#include "is_sensitive.h"


/*
 * LYRA_NAMESPACE::graphics::get_dxgi_adapters
 */
LYRA_NAMESPACE::property_set LYRA_NAMESPACE::graphics::get_dxgi_adapters(
        _In_ const collection_flags flags) {
    property_set retval;
#if defined(_WIN32)
    static const auto make_luid = [](const LUID& luid) {
        detail::property_set_impl ps;
        ps.add(u8"LowPart", static_cast<std::uint32_t>(luid.LowPart));
        ps.add(u8"HighPart", luid.HighPart);
        return detail::to_property_set(std::move(ps));
    };

    std::vector<property_set> pss;

    auto descs1 = detail::get_dxgi_adapter_descs();
    for (const auto& d : descs1) {
        detail::property_set_impl ps;
        ps.add(u8"Description", d.Description);
        ps.add(u8"VendorId", d.VendorId);
        ps.add(u8"DeviceId", d.DeviceId);
        ps.add(u8"SubSysId", d.SubSysId);
        ps.add(u8"Revision", d.Revision);
        ps.add(u8"DedicatedVideoMemory", d.DedicatedVideoMemory);
        ps.add(u8"DedicatedSystemMemory", d.DedicatedSystemMemory);
        ps.add(u8"SharedSystemMemory", d.SharedSystemMemory);
        ps.add(u8"AdapterLuid", make_luid(d.AdapterLuid));
        ps.add(u8"Flags", d.Flags);
        pss.push_back(detail::to_property_set(std::move(ps)));
    }

    if (descs1.empty()) {
        LYRA_TRACE(_T("Falling back to DXGI 1.0."));
        IDXGIFactory1 *factory = nullptr;
        auto descs = detail::get_dxgi_adapters(factory);

        for (const auto& d : descs1) {
            detail::property_set_impl ps;
            ps.add(u8"Description", d.Description);
            ps.add(u8"VendorId", d.VendorId);
            ps.add(u8"DeviceId", d.DeviceId);
            ps.add(u8"SubSysId", d.SubSysId);
            ps.add(u8"Revision", d.Revision);
            ps.add(u8"DedicatedVideoMemory", d.DedicatedVideoMemory);
            ps.add(u8"DedicatedSystemMemory", d.DedicatedSystemMemory);
            ps.add(u8"SharedSystemMemory", d.SharedSystemMemory);
            ps.add(u8"AdapterLuid", make_luid(d.AdapterLuid));
            pss.push_back(detail::to_property_set(std::move(ps)));
        }
    }

    {
        detail::property_set_impl ps;
        ps.add<dxgi_adapter>(std::move(pss));
        detail::realise(retval, std::move(ps));
    }
#endif /* defined(_WIN32) */

    return retval;
}
