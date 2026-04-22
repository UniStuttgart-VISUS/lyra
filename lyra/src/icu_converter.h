// <copyright file="icu_converter.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_ICU_CONVERTER_H)
#define _LYRA_ICU_CONVERTER_H
#pragma once

#if defined(_WIN32) && (NTDDI_VERSION >= NTDDI_WIN10_RS2)
#include <icu.h>
#elif defined(_WIN32)
#include <icucommon.h>
#include <icui18n.h>
#else /* !defined(_WIN32) */
#include <unicode/ucnv.h>
#include <unicode/unistr.h>
#include <unicode/utypes.h>
#endif /* !defined(_WIN32) */

#include "visus/lyra/api.h"


LYRA_DETAIL_NAMESPACE_BEGIN

/// <summary>
/// A RAII wrapper for performing conversions between UTF-16 and UTF-8.
/// </summary>
class icu_converter final {

public:

    /// <summary>
    /// Initialises a new instance.
    /// </summary>
    /// <param name="encoding">The name of the encoding to convert from and to.
    /// </param>
    explicit icu_converter(_In_opt_z_ const char *encoding);

    icu_converter(const icu_converter&) = delete;

    ~icu_converter(void);

    icu_converter& operator =(const icu_converter&) = delete;

    inline operator UConverter*(void) const noexcept {
        return this->_converter;
    }

    inline operator bool(void) const noexcept {
        return (this->_converter != nullptr);
    }

private:

    UConverter *_converter;

};

LYRA_DETAIL_NAMESPACE_END

#endif /* !defined(_LYRA_ICU_CONVERTER_H) */
