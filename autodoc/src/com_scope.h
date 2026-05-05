// <copyright file="com_scope.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_COM_SCOPE_H)
#define _LYRA_COM_SCOPE_H
#pragma once

#include <cinttypes>

#include "visus/autodoc/api.h"


LYRA_DETAIL_NAMESPACE_BEGIN

/// <summary>
/// A RAII scope for COM initialisation.
/// </summary>
class LYRA_TEST_API com_scope final {

public:

#if defined(_WIN32)
    typedef DWORD flags_type;
#else /* defined(_WIN32) */
    typedef std::uint32_t flags_type;
#endif /* defined(_WIN32) */

    /// <summary>
    /// Initialises COM for the current thread with the given flags.
    /// </summary>
    /// <param name="flags"></param>
    com_scope(_In_ const flags_type flags = 0);

    /// <summary>
    /// Initialise from move.
    /// </summary>
    /// <param name="rhs"></param>
    com_scope(_Inout_ com_scope&& rhs) noexcept : _disposed(rhs._disposed) {
        rhs._disposed = true;
    }

    /// <summary>
    /// Finalises the instance.
    /// </summary>
    ~com_scope(void) noexcept;

    /// <summary>
    /// Move assignment.
    /// </summary>
    /// <param name="rhs"></param>
    /// <returns></returns>
    com_scope& operator =(_Inout_ com_scope&& rhs) noexcept;

private:

    bool _disposed;
};

LYRA_DETAIL_NAMESPACE_END

#endif /* !defined(_LYRA_COM_SCOPE_H) */
