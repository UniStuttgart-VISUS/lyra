// <copyright file="wbem.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_WBEM_H)
#define _LYRA_WBEM_H
#pragma once

#include <wil/com.h>
#include <wil/result.h>

#include <WbemIdl.h>

#include "visus/autodoc/trace.h"

#include "com_scope.h"


#if defined(_WIN32)

LYRA_DETAIL_NAMESPACE_BEGIN

/// <summary>
/// Initialise COM security for using WBEM. The caller must hold a
/// <see cref="com_scope" /> for the calling thread.
/// </summary>
/// <exception cref="wil::ResultException">If the operation fails.</exception>
LYRA_TEST_API void initialise_wbem_security(void);

/// <summary>
/// Creates a new WBEM locator object, which is the entry point to everything
/// WBEM.
/// </summary>
/// <returns>A COM pointer to the newly created WBEM locator object.</returns>
/// <exception cref="wil::ResultException">If the operation fails.</exception>
LYRA_TEST_API wil::com_ptr<IWbemLocator> make_wbem_locator(void);

/// <summary>
/// Creates a new WBEM locator object and connects to the specified namespace.
/// </summary>
/// <param name="root"></param>
/// <returns></returns>
LYRA_TEST_API wil::com_ptr<IWbemServices> make_wbem_services(
    _In_z_ const wchar_t *root);

/// <summary>
/// Execute the specified WQL query on the given WBEM services and return an
/// enumerator for the results.
/// </summary>
/// <param name="services"></param>
/// <param name="query"></param>
/// <returns></returns>
LYRA_TEST_API wil::com_ptr<IEnumWbemClassObject> query_wql(
    _In_ IWbemServices *services,
    _In_z_ const wchar_t *query,
    _In_ const WBEM_GENERIC_FLAG_TYPE flags = WBEM_RETURN_WHEN_COMPLETE);

LYRA_DETAIL_NAMESPACE_END

#include "wbem.inl"

#endif /* defined(_WIN32) */
#endif /* !defined(_LYRA_WBEM_H) */
