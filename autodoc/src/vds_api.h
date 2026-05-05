// <copyright file="vds_api.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_VDS_API_H)
#define _LYRA_VDS_API_H
#pragma once

#if defined(_WIN32)
#include <cassert>

#include <comdef.h>
#include <tchar.h>
#include <Windows.h>
#include <vds.h>

#include <wil/com.h>
#include <wil/resource.h>
#include <wil/result.h>

#include "visus/autodoc/dispatch_list.h"
#include "visus/autodoc/trace.h"


LYRA_DETAIL_NAMESPACE_BEGIN

/// <summary>
/// Invoke <paramref name="callback"/> for each VDS service provider returned by
/// <see cref="IVdsService::QueryProviders"/>.
/// </summary>
/// <typeparam name="TCallback"></typeparam>
/// <param name="vds"></param>
/// <param name="callback"></param>
template<class TCallback> void foreach_vds_service_provider(
    _In_ IVdsService *vds, _In_ TCallback callback);

/// <summary>
/// Creates a VDS service loader, loads the service and waits for it to be
/// ready. The caller must run as administrator for this function to succeed.
/// </summary>
/// <param name="machine_name"></param>
/// <returns></returns>
LYRA_TEST_API wil::com_ptr<IVdsService> load_vds_service(
    _In_opt_z_ const wchar_t *machine_name = nullptr);

LYRA_DETAIL_NAMESPACE_END

#include "vds_api.inl"

#endif /* defined(_WIN32) */
#endif /* !defined(_LYRA_VDS_API_H) */
