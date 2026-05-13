// <copyright file="affinity_scope.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_AFFINITY_SCOPE_H)
#define _LYRA_AFFINITY_SCOPE_H
#pragma once

#include <cstdlib>
#include <type_traits>

#if defined(_WIN32)
#include <Windows.h>
#else /* defined(_WIN32) */
#include <sched.h>
#include <unistd.h>
#endif /* defined(_WIN32) */

#include "visus/autodoc/api.h"


LYRA_DETAIL_NAMESPACE_BEGIN

/// <summary>
/// A RAII scope for changing the thread affinity.
/// </summary>
class LYRA_TEST_API affinity_scope final {

public:

#if defined(_WIN32)
#if (_WIN32_WINNT >= 0x0601)
    typedef GROUP_AFFINITY& mask_type;
#else /* (_WIN32_WINNT >= 0x0601) */
    typedef DWORD_PTR mask_type;
#endif /* (_WIN32_WINNT >= 0x0601) */
#else /* defined(_WIN32) */
    typedef cpu_set_t *mask_type;
#endif /* defined(_WIN32) */

    /// <summary>
    /// Changes the thread affinity.
    /// </summary>
    /// <param name="mask">The affinity mask to set.</param>
    /// <param name="size">The size of the mask. This parameter is irrelevant
    /// on Windows.</param>
    affinity_scope(_In_ const mask_type mask,
        _In_ const std::size_t size = 0) noexcept;

    affinity_scope(const affinity_scope& rhs) = delete;

    /// <summary>
    /// Finalises the instance.
    /// </affinity_scope>
    ~affinity_scope(void) noexcept;

    affinity_scope& operator =(const affinity_scope& rhs) = delete;

    /// <summary>
    /// Answer whether the scope is valid, i.e. whether the thread affinity has
    /// been set to the mask provided to the constructor.
    /// </summary>
    /// <returns></returns>
    inline operator bool(void) const noexcept {
        return this->_valid;
    }

private:

    void set(_In_ const mask_type mask, _In_ const std::size_t size) noexcept;

    std::decay_t<mask_type> _mask;
#if !defined(_WIN32)
    std::size_t _size;
#endif /* !defined(_WIN32) */
    bool _valid;

};

LYRA_DETAIL_NAMESPACE_END

#endif /* !defined(_LYRA_AFFINITY_SCOPE_H) */
