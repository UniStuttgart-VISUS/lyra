// <copyright file="boolean.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_BOOLEAN_H)
#define _LYRA_BOOLEAN_H
#pragma once

#include <memory>

#include "visus/lyra/api.h"


LYRA_DETAIL_NAMESPACE_BEGIN

/// <summary>
/// Stores a Boolean value while preventing the use of the specialised vector
/// implementation that relies on bitfields.
/// </summary>
class LYRA_TEST_API boolean final {

public:

    /// <summary>
    /// Initialises a new instance.
    /// </summary>
    /// <param name="value">The initial value.</param>
    inline boolean(_In_ const bool value = false) noexcept : _value(value) { }

    /// <summary>
    /// Exposes the value.
    /// </summary>
    /// <returns>The current value.</returns>
    inline operator bool(void) const noexcept {
        return this->_value;
    }

    /// <summary>
    /// Answer the address of the stored value.
    /// </summary>
    /// <returns>The address of the stored value.</returns>
    inline bool *operator &(void) noexcept {
        return std::addressof(this->_value);
    }

    /// <summary>
    /// Answer the address of the stored value.
    /// </summary>
    /// <returns>The address of the stored value.</returns>
    inline const bool *operator &(void) const noexcept {
        return std::addressof(this->_value);
    }

    /// <summary>
    /// Assigns a new value.
    /// </summary>
    /// <param name="value">The new value.</param>
    /// <returns><c>*<see langword="this" /></c>.</returns>
    inline boolean& operator =(_In_ const bool value) noexcept {
        this->_value = value;
        return *this;
    }

private:

    bool _value;
};

static_assert(sizeof(boolean) == sizeof(bool), "boolean must have not have any "
    "size overhead.");

LYRA_DETAIL_NAMESPACE_END

#endif /* !defined(_LYRA_BOOLEAN_H) */
