// <copyright file="dxgi.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_DX_GRAPHICS_H)
#define _LYRA_DX_GRAPHICS_H
#pragma once

#if defined(_WIN32)

#include <iterator>
#include <type_traits>
#include <vector>

#include <dxgi1_2.h>

#include <wil/com.h>

#include "visus/autodoc/trace.h"


LYRA_DETAIL_NAMESPACE_BEGIN

/// <summary>
/// Creates a DXGI factor.
/// </summary>
/// <param name="retval">The variable to receive the output.</param>
/// <returns><c>S_OK</c> in case of success, an error code otherwise.</returns>
inline HRESULT create_dxgi_factory(
        _Out_ wil::com_ptr_nothrow<IDXGIFactory>& retval) {
    return ::CreateDXGIFactory(::IID_IDXGIFactory, retval.put_void());
}

/// <summary>
/// Creates a DXGI factor.
/// </summary>
/// <param name="retval">The variable to receive the output.</param>
/// <returns><c>S_OK</c> in case of success, an error code otherwise.</returns>
inline HRESULT create_dxgi_factory(
        _Out_ wil::com_ptr_nothrow<IDXGIFactory1>& retval) {
    return ::CreateDXGIFactory1(::IID_IDXGIFactory1, retval.put_void());
}

/// <summary>
/// Gets all available DXGI adapters on the system and returns the ones that
/// satisfy the given predicate.
/// </summary>
/// <typeparam name="TIterator"></typeparam>
/// <typeparam name="TPredicate"></typeparam>
/// <param name="oit"></param>
/// <param name="predicate"></param>
/// <param name="factory"></param>
/// <returns></returns>
template<class TIterator, class TPredicate>
HRESULT get_dxgi_adapters(_In_ TIterator oit, _In_ const TPredicate predicate,
    _In_opt_ IDXGIFactory *factory);

/// <summary>
/// Gets all available DXGI 1.1 adapters on the system and returns the ones that
/// satisfy the given predicate.
/// </summary>
/// <typeparam name="TIterator"></typeparam>
/// <typeparam name="TPredicate"></typeparam>
/// <param name="oit"></param>
/// <param name="predicate"></param>
/// <param name="factory"></param>
/// <returns></returns>
template<class TIterator, class TPredicate>
HRESULT get_dxgi_adapters(_In_ TIterator oit, _In_ const TPredicate predicate,
    _In_opt_ IDXGIFactory1 *factory = nullptr);

/// <summary>
/// Gets the available DXGI adapters on the system.
/// </summary>
/// <param name="factory">The factory used to enumerate DXGI adapters. If
/// this parameter is <see langword="nullptr" />, a temporary factory will be
/// created to enumerate the adapters.</param>
/// <returns>A vector of COM pointers to the available DXGI adapters.</returns>
std::vector<wil::com_ptr_nothrow<IDXGIAdapter>> LYRA_TEST_API get_dxgi_adapters(
    _In_opt_ IDXGIFactory *factory);

/// <summary>
/// Gets the available DXGI adapters on the system.
/// </summary>
/// <param name="factory">The factory used to enumerate DXGI adapters. If
/// this parameter is <see langword="nullptr" />, a temporary factory will be
/// created to enumerate the adapters.</param>
/// <returns>A vector of COM pointers to the available DXGI adapters.</returns>
std::vector<wil::com_ptr_nothrow<IDXGIAdapter1>> LYRA_TEST_API get_dxgi_adapters(
    _In_opt_ IDXGIFactory1 *factory = nullptr);

/// <summary>
/// Gets the descriptions of all the given DXGI adapters.
/// </summary>
template<class TInputIterator, class TOutputIterator>
HRESULT get_dxgi_adapter_descs(_In_ TOutputIterator oit,
    _In_ const TInputIterator begin,
    _In_ const TInputIterator end);

/// <summary>
/// Gets the descriptions of all DXGI adapters in the system
/// </summary>
/// <param name="factory"></param>
/// <returns></returns>
std::vector<DXGI_ADAPTER_DESC> LYRA_TEST_API get_dxgi_adapter_descs(
    _In_opt_ IDXGIFactory *factory);

/// <summary>
/// Gets the descriptions of all DXGI adapters in the system
/// </summary>
/// <param name="factory"></param>
/// <returns></returns>
std::vector<DXGI_ADAPTER_DESC1> LYRA_TEST_API get_dxgi_adapter_descs(
    _In_opt_ IDXGIFactory1 *factory = nullptr);

LYRA_DETAIL_NAMESPACE_END

#include "dx_graphics.inl"

#endif /* defined(_WIN32) */
#endif /* !defined(_LYRA_DX_GRAPHICS_H) */
