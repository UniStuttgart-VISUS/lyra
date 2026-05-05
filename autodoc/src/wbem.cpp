// <copyright file="wbem.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if defined(_WIN32)
#include <initguid.h>
#include "wbem.h"


/*
 * LYRA_DETAIL_NAMESPACE::initialise_wbem_security
 */
void LYRA_DETAIL_NAMESPACE::initialise_wbem_security(void) {
    THROW_IF_FAILED(::CoInitializeSecurity(nullptr, -1, nullptr, nullptr,
        RPC_C_AUTHN_LEVEL_DEFAULT, RPC_C_IMP_LEVEL_IMPERSONATE, nullptr,
        EOAC_NONE, nullptr));
}


/*
 * LYRA_DETAIL_NAMESPACE::make_wbem_locator
 */
wil::com_ptr<IWbemLocator> LYRA_DETAIL_NAMESPACE::make_wbem_locator(void) {
    wil::com_ptr<IWbemLocator> retval;
    THROW_IF_FAILED(::CoCreateInstance(::CLSID_WbemLocator, nullptr, CLSCTX_ALL,
        ::IID_IWbemLocator, retval.put_void()));
    return retval;
}


/*
 * LYRA_DETAIL_NAMESPACE::make_wbem_services
 */
wil::com_ptr<IWbemServices> LYRA_DETAIL_NAMESPACE::make_wbem_services(
        _In_z_ const wchar_t *root) {
    auto locator = make_wbem_locator();

    wil::com_ptr<IWbemServices> retval;
    THROW_IF_FAILED(locator->ConnectServer(const_cast<wchar_t *>(root), nullptr,
        nullptr, nullptr, 0, nullptr, nullptr, retval.put()));

    return retval;
}


/*
 * LYRA_DETAIL_NAMESPACE::query_wql
 */
wil::com_ptr<IEnumWbemClassObject> LYRA_DETAIL_NAMESPACE::query_wql(
        _In_ IWbemServices *services,
        _In_z_ const wchar_t *query,
        _In_ const WBEM_GENERIC_FLAG_TYPE flags) {
    THROW_HR_IF(E_POINTER, services == nullptr);
    wil::com_ptr<IEnumWbemClassObject> retval;
    THROW_IF_FAILED(services->ExecQuery(L"WQL", const_cast<wchar_t *>(query),
        flags, nullptr, retval.put()));
    return retval;
}
#endif /* defined(_WIN32) */
