// <copyright file="dx_graphics.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "dx_graphics.h"

#include <algorithm>

#if defined(_WIN32)
/*
 * LYRA_DETAIL_NAMESPACE::get_dxgi_adapters
 */
std::vector<wil::com_ptr_nothrow<IDXGIAdapter>>
LYRA_DETAIL_NAMESPACE::get_dxgi_adapters(_In_ IDXGIFactory *factory) {
    std::vector<wil::com_ptr_nothrow<IDXGIAdapter>> retval;
    get_dxgi_adapters(std::back_inserter(retval), [](auto) { return true; },
        factory);
    return retval;
}


/*
 * LYRA_DETAIL_NAMESPACE::get_dxgi_adapters
 */
std::vector<wil::com_ptr_nothrow<IDXGIAdapter1>>
LYRA_DETAIL_NAMESPACE::get_dxgi_adapters(_In_ IDXGIFactory1 *factory) {
    std::vector<wil::com_ptr_nothrow<IDXGIAdapter1>> retval;
    get_dxgi_adapters(std::back_inserter(retval), [](auto) { return true; },
        factory);
    return retval;
}


/*
 * LYRA_DETAIL_NAMESPACE::get_dxgi_adapter_descs
 */
std::vector<DXGI_ADAPTER_DESC> LYRA_DETAIL_NAMESPACE::get_dxgi_adapter_descs(
        _In_opt_ IDXGIFactory *factory) {
    auto adapters = get_dxgi_adapters(factory);

    std::vector<DXGI_ADAPTER_DESC> retval;
    retval.reserve(adapters.size());
    get_dxgi_adapter_descs(std::back_inserter(retval),
        adapters.begin(),
        adapters.end());

    return retval;
}


/*
 * LYRA_DETAIL_NAMESPACE::get_dxgi_adapter_descs
 */
std::vector<DXGI_ADAPTER_DESC1> LYRA_DETAIL_NAMESPACE::get_dxgi_adapter_descs(
    _In_opt_ IDXGIFactory1 *factory) {
    auto adapters = get_dxgi_adapters(factory);

    std::vector<DXGI_ADAPTER_DESC1> retval;
    retval.reserve(adapters.size());
    get_dxgi_adapter_descs(std::back_inserter(retval),
        adapters.begin(),
        adapters.end());

    return retval;
}
#endif /* defined(_WIN32) */
