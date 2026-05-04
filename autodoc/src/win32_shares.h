// <copyright file="win32_shares.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_WIN32_SHARES_H)
#define _LYRA_WIN32_SHARES_H
#pragma once

#if defined(_WIN32)
#include <cstdlib>
#include <type_traits>

#include <Windows.h>
#include <lm.h>

#include <wil/resource.h>

#include "visus/autodoc/on_exit.h"
#include "visus/autodoc/trace.h"


LYRA_DETAIL_NAMESPACE_BEGIN

/// <summary>
/// Derives the level constant from a share info structure type.
/// </summary>
template<class TShareInfo, bool Invoke = false> struct share_info_level;

template<> struct share_info_level<SHARE_INFO_0, true> final {
    static constexpr DWORD level = 0;
};

template<> struct share_info_level<SHARE_INFO_1, true> final {
    static constexpr DWORD level = 1;
};

template<> struct share_info_level<SHARE_INFO_2, true> final {
    static constexpr DWORD level = 2;
};

template<> struct share_info_level<SHARE_INFO_502, true> final {
    static constexpr DWORD level = 502;
};

template<> struct share_info_level<SHARE_INFO_503, true> final {
    static constexpr DWORD level = 503;
};




/// <summary>
/// Derives the level constant from a share info structure type.
/// </summary>
template<class TShareInfo>
constexpr auto share_info_level_v = share_info_level<TShareInfo>::level;


/// <summary>
/// Invokes <paramref name="callback"/> for each share on the specified
/// <paramref name="server"/>.
/// </summary>
/// <typeparam name="TCallback">The type of the callback to be invoked. This
/// callback must accept a reference to one of the structures a
/// <see cref=share_info_level" /> specialisation is in place.</typeparam>
/// <param name="server">The name of the server to enumerate the shares on.
/// If <see langword="nullptr" />, the local machine will be used.</param>
/// <param name="callback">The callback to be invoked for each share. Note that
/// the data pass ised to the callback are only valid as long as the callback
/// is executing. Any string that is to be used later must be copied.</param>
/// <returns>The number of callback invocations.</returns>
template<class TCallback> std::size_t enumerate_shares(
    _In_opt_z_ const wchar_t *server, _In_ TCallback callback);


LYRA_DETAIL_NAMESPACE_END

#include "win32_shares.inl"

#endif /* defined(_WIN32) */
#endif /* !defined(_LYRA_WIN32_SHARES_H) */
