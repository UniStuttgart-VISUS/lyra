// <copyright file="affinity_scope.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_AFFINITY_SCOPE_H)
#define _LYRA_AFFINITY_SCOPE_H
#pragma once

#include "visus/autodoc/affinity_mask.h"


LYRA_NAMESPACE_BEGIN

/// <summary>
/// A RAII scope for changing the thread affinity.
/// </summary>
class LYRA_API affinity_scope final {

public:

    /// <summary>
    /// Changes the thread affinity.
    /// </summary>
    /// <param name="mask">The affinity mask to set.</param>
    explicit inline affinity_scope(_In_ const affinity_mask& mask) noexcept {
        this->set(mask);
    }

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
        return this->_mask;
    }

private:

    void set(_In_ const affinity_mask& mask) noexcept;

    affinity_mask _mask;

};

LYRA_NAMESPACE_END

#endif /* !defined(_LYRA_AFFINITY_SCOPE_H) */
