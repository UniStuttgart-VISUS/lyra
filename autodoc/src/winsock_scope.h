// <copyright file="winsock_scope.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_WINSOCK_SCOPE_H)
#define _LYRA_WINSOCK_SCOPE_H
#pragma once

#include <cinttypes>

#if defined(_WIN32)
#include <WinSock2.h>
#include <Windows.h>
#endif /* defined(_WIN32) */

#include "visus/autodoc/api.h"


LYRA_DETAIL_NAMESPACE_BEGIN

/// <summary>
/// A RAII scope for initialisation of Winsock.
/// </summary>
class LYRA_TEST_API winsock_scope final {

public:

    /// <summary>
    /// Initialises Winsock for the calling thread.
    /// </summary>
    /// <param name="version"></param>
    winsock_scope(_In_ const std::uint16_t version = 0x0202);

    /// <summary>
    /// Initialise from move.
    /// </summary>
    /// <param name="rhs"></param>
    winsock_scope(_Inout_ winsock_scope&& rhs) noexcept;

    /// <summary>
    /// Finalises the instance.
    /// </summary>
    ~winsock_scope(void) noexcept;

    /// <summary>
    /// Move assignment.
    /// </summary>
    /// <param name="rhs"></param>
    /// <returns></returns>
    winsock_scope& operator =(_Inout_ winsock_scope&& rhs) noexcept;

#if defined(_WIN32)
    /// <summary>
    /// Provides access to the <see cref="WSADATA" /> structure that was filled
    /// during initialisation.
    /// </summary>
    /// <returns></returns>
    inline operator const WSADATA&(void) const noexcept {
        return this->_data;
    }
#endif /* defined(_WIN32) */

private:

#if defined(_WIN32)
    WSADATA _data;
#endif /* defined(_WIN32) */
};

LYRA_DETAIL_NAMESPACE_END

#endif /* !defined(_LYRA_COM_SCOPE_H) */
