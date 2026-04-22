// <copyright file="convert_string.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_CONVERT_STRING_H)
#define _LYRA_CONVERT_STRING_H
#pragma once

#include <cstdlib>

#include "visus/lyra/unicode_type.h"


LYRA_NAMESPACE_BEGIN

/// <summary>
/// Converts a string from UTF-16 to UTF-8.
/// </summary>
/// <param name="dst"></param>
/// <param name="cnt_dst"></param>
/// <param name="src"></param>
/// <param name="cnt_src"></param>
/// <returns></returns>
std::size_t LYRA_API to_utf8(
    _Out_writes_to_opt_(cnt_dst, return) char *dst,
    _In_ std::size_t cnt_dst,
    _In_reads_or_z_(cnt_src) const unicode_type *src,
    _In_ int cnt_src = -1);


#if (defined(_WIN32) && (NTDDI_VERSION >= NTDDI_WIN10_RS2))

/// <summary>
/// Converts a string from UTF-16 to UTF-8.
/// </summary>
/// <param name="dst"></param>
/// <param name="cnt_dst"></param>
/// <param name="src"></param>
/// <param name="cnt_src"></param>
/// <returns></returns>
inline std::size_t LYRA_API to_utf8(
        _Out_writes_to_opt_(cnt_dst, return) char *dst,
        _In_ const std::size_t cnt_dst,
        _In_reads_or_z_(cnt_src) const wchar_t *src,
        _In_ const int cnt_src = -1) {
    static_assert(sizeof(wchar_t) == sizeof(unicode_type),
        "wchar_t must be the same size as unicode_type");
    return to_utf8(
        dst, cnt_dst,
        reinterpret_cast<const unicode_type *>(src), cnt_src);
}

#endif /* (defined(_WIN32) && (NTDDI_VERSION >= NTDDI_WIN10_RS2)) */


/// <summary>
/// Converts a string from UTF-32 to UTF-8.
/// </summary>
/// <param name="dst"></param>
/// <param name="cnt_dst"></param>
/// <param name="src"></param>
/// <param name="cnt_src"></param>
/// <returns></returns>
std::size_t LYRA_API to_utf8(
    _Out_writes_to_opt_(cnt_dst, return) char *dst,
    _In_ std::size_t cnt_dst,
    _In_reads_or_z_(cnt_src) const char32_t *src,
    _In_ const int cnt_src = -1);


#if !defined(_WIN32)

/// <summary>
/// Converts a string from UTF-32 to UTF-8.
/// </summary>
/// <param name="dst"></param>
/// <param name="cnt_dst"></param>
/// <param name="src"></param>
/// <param name="cnt_src"></param>
/// <returns></returns>
inline std::size_t LYRA_API to_utf8(
        _Out_writes_to_opt_(cnt_dst, return) char *dst,
        _In_ const std::size_t cnt_dst,
        _In_reads_or_z_(cnt_src) const wchar_t *src,
        _In_ const int cnt_src = -1) {
    static_assert(sizeof(wchar_t) == sizeof(char32_t),
        "wchar_t must be the same size as unicode_type");
    return to_utf8(
        dst, cnt_dst,
        reinterpret_cast<const char32_t *>(src), cnt_src);
}

#endif /* !defined(_WIN32) */

LYRA_NAMESPACE_END

#endif /* !defined(_LYRA_CONVERT_STRING_H) */
