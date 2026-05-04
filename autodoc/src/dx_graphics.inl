// <copyright file="dx_graphics.inl" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>


/*
 * LYRA_DETAIL_NAMESPACE::get_dxgi_adapters
 */
template<class TIterator, class TPredicate>
HRESULT LYRA_DETAIL_NAMESPACE::get_dxgi_adapters(
        _In_ TIterator oit,
        _In_ const TPredicate predicate,
        _In_opt_ IDXGIFactory *factory) {
    wil::com_ptr_nothrow<IDXGIFactory> f(factory);
    auto hr = S_OK;

    if (!f && FAILED(hr = create_dxgi_factory(f))) {
        return hr;
    }

    wil::com_ptr_nothrow<IDXGIAdapter> adapter;
    for (UINT i = 0; SUCCEEDED(hr = f->EnumAdapters(i, adapter.put())); ++i) {
        if (predicate(adapter.get())) {
            *oit++ = adapter;
        }
    }

    if (hr == DXGI_ERROR_NOT_FOUND) {
        hr = S_OK;
    }

    return hr;
}


/*
 * LYRA_DETAIL_NAMESPACE::get_dxgi_adapters
 */
template<class TIterator, class TPredicate>
HRESULT LYRA_DETAIL_NAMESPACE::get_dxgi_adapters(_In_ TIterator oit,
        _In_ const TPredicate predicate,
        _In_opt_ IDXGIFactory1 *factory) {
    wil::com_ptr_nothrow<IDXGIFactory1> f(factory);
    auto hr = S_OK;

    if (!f && FAILED(hr = create_dxgi_factory(f))) {
        return hr;
    }

    wil::com_ptr_nothrow<IDXGIAdapter1> adapter;
    for (UINT i = 0; SUCCEEDED(hr = f->EnumAdapters1(i, adapter.put())); ++i) {
        if (predicate(adapter.get())) {
            *oit++ = adapter;
        }
    }

    if (hr == DXGI_ERROR_NOT_FOUND) {
        hr = S_OK;
    }

    return hr;
}


/*
 * LYRA_DETAIL_NAMESPACE::get_dxgi_adapter_descs
 */
template<class TInputIterator, class TOutputIterator>
HRESULT LYRA_DETAIL_NAMESPACE::get_dxgi_adapter_descs(
        _In_ TOutputIterator oit,
        _In_ const TInputIterator begin,
        _In_ const TInputIterator end) {
    typedef typename std::iterator_traits<TInputIterator>::value_type value_t;
    typedef typename value_t::element_type adapter_t;
    auto hr = S_OK;

    if constexpr (std::is_same_v<adapter_t, IDXGIAdapter>) {
        for (auto it = begin; it != end; ++it) {
            DXGI_ADAPTER_DESC desc;
            if (FAILED(hr = (**it).GetDesc(&desc))) {
                return hr;
            }
            *oit++ = desc;
        }

        return S_OK;

    } else if constexpr (std::is_same_v<adapter_t, IDXGIAdapter1>) {
        for (auto it = begin; it != end; ++it) {
            DXGI_ADAPTER_DESC1 desc;
            if (FAILED(hr = (**it).GetDesc1(&desc))) {
                return hr;
            }
            *oit++ = desc;
        }

        return S_OK;

    } else {
        LYRA_TRACE(_T("No overload for the given adapter type exists."));
        return E_NOTIMPL;
    }

}
