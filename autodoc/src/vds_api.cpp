// <copyright file="vds_api.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if defined(_WIN32)
#include <initguid.h>
#include "vds_api.h"


/*
 * LYRA_DETAIL_NAMESPACE::load_vds_service
 */
wil::com_ptr<IVdsService> LYRA_DETAIL_NAMESPACE::load_vds_service(
        _In_opt_z_ const wchar_t *machine_name) {
    LYRA_TRACE(_T("Creating VDS service loader..."));
    wil::com_ptr<IVdsServiceLoader> loader;
    THROW_IF_FAILED(::CoCreateInstance(
        CLSID_VdsLoader,
        nullptr,
        CLSCTX_ALL,
        IID_IVdsServiceLoader,
        loader.put_void()));

    LYRA_TRACE(_T("Loading VDS..."));
    wil::com_ptr<IVdsService> retval;
    THROW_IF_FAILED(loader->LoadService(const_cast<wchar_t *>(machine_name),
        retval.put()));
    assert(retval);

    LYRA_TRACE(_T("Waiting for VDS service to be ready..."));
    THROW_IF_FAILED(retval->WaitForServiceReady());

    LYRA_TRACE(_T("VDS is ready."));
    return retval;
}
#endif /* defined(_WIN32) */
