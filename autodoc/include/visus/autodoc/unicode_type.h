// <copyright file="unicode_type.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_UNICODE_TYPE_H)
#define _LYRA_UNICODE_TYPE_H
#pragma once

#include <cuchar>

#include "visus/autodoc/api.h"


LYRA_NAMESPACE_BEGIN

#if (defined(_WIN32) && (NTDDI_VERSION < NTDDI_WIN10_RS2))
/// <summary>
/// The representation of a UTF-16 code unit.
/// </summary>
/// <remarks>
/// From https://unicode-org.github.io/icu/userguide/strings/: &quot;The Unicode
/// standard defines a default encoding based on 16-bit code units. This is
/// supported in ICU by the definition of the UChar to be an unsigned 16-bit
/// integer type. This is the base type for character arrays for strings in
/// ICU.&quot; This means that libicu is using UTF-16 like Win32 wherefore we
/// need an intermediate step via <c>char16_t</c> on Linux. This typedef ensures
/// that we can have the same interface declarations on all platforms.
/// </remarks>
typedef wchar_t unicode_type;

#else /* (defined(_WIN32) && (NTDDI_VERSION < NTDDI_WIN10_RS2)) */
/// <summary>
/// The representation of a UTF-16 code unit.
/// </summary>
/// <remarks>
/// From https://unicode-org.github.io/icu/userguide/strings/: &quot;The Unicode
/// standard defines a default encoding based on 16-bit code units. This is
/// supported in ICU by the definition of the UChar to be an unsigned 16-bit
/// integer type. This is the base type for character arrays for strings in
/// ICU.&quot; This means that libicu is using UTF-16 like Win32 wherefore we
/// need an intermediate step via <c>char16_t</c> on Linux. This typedef ensures
/// that we can have the same interface declarations on all platforms.
/// </remarks>
typedef char16_t unicode_type;

#endif /* (defined(_WIN32) && (NTDDI_VERSION < NTDDI_WIN10_RS2)) */

LYRA_NAMESPACE_END

#endif /* !defined(_LYRA_UNICODE_TYPE_H) */
