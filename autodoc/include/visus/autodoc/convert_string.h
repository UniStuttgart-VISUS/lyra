// <copyright file="convert_string.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_CONVERT_STRING_H)
#define _LYRA_CONVERT_STRING_H
#pragma once

#include <algorithm>
#include <cstdlib>
#include <string>

#include "visus/autodoc/result_type.h"
#include "visus/autodoc/unicode_type.h"


LYRA_NAMESPACE_BEGIN

/// <summary>
/// Converts a string from UTF-16 to UTF-8.
/// </summary>
/// <param name="dst"></param>
/// <param name="cnt_dst"></param>
/// <param name="src"></param>
/// <param name="cnt_src"></param>
/// <returns></returns>
result_type LYRA_API to_utf8(
    _Out_writes_to_opt_(cnt_dst, return) char *dst,
    _Inout_ std::size_t& cnt_dst,
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
inline result_type LYRA_API to_utf8(
        _Out_writes_to_opt_(cnt_dst, return) char *dst,
        _Inout_ std::size_t& cnt_dst,
        _In_reads_or_z_(cnt_src) const wchar_t *src,
        _In_ const int cnt_src = -1) {
    static_assert(sizeof(wchar_t) == sizeof(unicode_type),
        "wchar_t must be the same size as unicode_type");
    auto s = reinterpret_cast<const unicode_type *>(src);
    return to_utf8(dst, cnt_dst, s, cnt_src);
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
result_type LYRA_API to_utf8(
    _Out_writes_to_opt_(cnt_dst, return) char *dst,
    _Inout_ std::size_t& cnt_dst,
    _In_reads_or_z_(cnt_src) const char32_t *src,
    _In_ int cnt_src = -1);

#if !defined(_WIN32)
/// <summary>
/// Converts a string from UTF-32 to UTF-8.
/// </summary>
/// <param name="dst"></param>
/// <param name="cnt_dst"></param>
/// <param name="src"></param>
/// <param name="cnt_src"></param>
/// <returns></returns>
inline result_type LYRA_API to_utf8(
        _Out_writes_to_opt_(cnt_dst, return) char *dst,
        _In_ std::size_t& cnt_dst,
        _In_reads_or_z_(cnt_src) const wchar_t *src,
        _In_ const int cnt_src = -1) {
    static_assert(sizeof(wchar_t) == sizeof(char32_t),
        "wchar_t must be the same size as unicode_type");
    auto s = reinterpret_cast<const char32_t *>(src);
    return to_utf8(dst, cnt_dst, s, cnt_src);
}
#endif /* !defined(_WIN32) */

/// <summary>
/// Converts UTF-16 or UTF-32 strings to UTF-8.
/// </summary>
/// <typeparam name="TTraits"></typeparam>
/// <typeparam name="TAlloc"></typeparam>
/// <typeparam name="TChar"></typeparam>
/// <param name="dst"></param>
/// <param name="src"></param>
/// <param name="cnt_src"></param>
/// <returns></returns>
template<class TTraits, class TAlloc, class TChar>
result_type to_utf8(
        _Out_ std::basic_string<char, TTraits, TAlloc>& dst,
        _In_reads_or_z_(cnt_src) const TChar *src,
        _In_ int cnt_src = -1) {
    std::size_t cnt = 0;
    to_utf8(nullptr, cnt, src, cnt_src);
    dst.resize(cnt);
    const auto retval = to_utf8(dst.data(), cnt, src, cnt_src);
    const auto end = std::remove(dst.begin(), dst.end(), static_cast<char>(0));
    dst.erase(end, dst.end());
    return retval;
}

/// <summary>
/// Converts UTF-16 or UTF-32 strings to UTF-8.
/// </summary>
/// <typeparam name="TChar"></typeparam>
/// <param name="dst"></param>
/// <param name="src"></param>
/// <param name="cnt_src"></param>
/// <returns>The converted string, or an empty string on error.</returns>
template<class TChar> inline std::string to_utf8(
        _In_reads_or_z_(cnt_src) const TChar *src,
        _In_ int cnt_src = -1) {
    std::string retval;
    if (LYRA_FAILED(to_utf8(retval, src, cnt_src))) {
        retval.clear();
    }
    return retval;
}

/// <summary>
/// Converts UTF-16 or UTF-32 strings to UTF-8.
/// </summary>
/// <typeparam name="TChar"></typeparam>
/// <typeparam name="TTraits"></typeparam>
/// <typeparam name="TAlloc"></typeparam>
/// <param name="src"></param>
/// <returns></returns>
template<class TChar, class TTraits, class TAlloc> inline
std::enable_if_t<(sizeof(TChar) > 1), std::string> to_utf8(
        _In_ const std::basic_string<TChar, TTraits, TAlloc>& src) {
    return to_utf8(src.c_str(), static_cast<int>(src.size()));
}

LYRA_NAMESPACE_END

#endif /* !defined(_LYRA_CONVERT_STRING_H) */
