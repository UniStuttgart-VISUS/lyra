// <copyright file="win32_shares.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_WIN32_SHARES_H)
#define _LYRA_WIN32_SHARES_H
#pragma once

#if defined(_WIN32)
#include <cinttypes>
#include <cstdlib>
#include <type_traits>
#include <vector>

#include <Windows.h>
#include <winnetwk.h>
#include <lm.h>

#include <wil/resource.h>

#include "visus/autodoc/on_exit.h"
#include "visus/autodoc/trace.h"

#include "callable_traits.h"


LYRA_DETAIL_NAMESPACE_BEGIN

/// <summary>
/// Derives the level constant from a share info structure type.
/// </summary>
template<class TShareInfo> struct share_info_level;

template<> struct share_info_level<SHARE_INFO_0> final {
    static constexpr DWORD level = 0;
};

template<> struct share_info_level<SHARE_INFO_1> final {
    static constexpr DWORD level = 1;
};

template<> struct share_info_level<SHARE_INFO_2> final {
    static constexpr DWORD level = 2;
};

template<> struct share_info_level<SHARE_INFO_502> final {
    static constexpr DWORD level = 502;
};

template<> struct share_info_level<SHARE_INFO_503> final {
    static constexpr DWORD level = 503;
};

/// <summary>
/// Derives the level constant from a share info structure type.
/// </summary>
template<class TShareInfo>
constexpr auto share_info_level_v = share_info_level<TShareInfo>::level;


/// <summary>
/// Derives the level constant from a use info structure type.
/// </summary>
template<class TUseInfo> struct use_info_level;

template<> struct use_info_level<USE_INFO_0> final {
    static constexpr DWORD level = 0;
};

template<> struct use_info_level<USE_INFO_1> final {
    static constexpr DWORD level = 1;
};

template<> struct use_info_level<USE_INFO_2> final {
    static constexpr DWORD level = 2;
};

/// <summary>
/// Derives the level constant from a use info structure type.
/// </summary>
template<class TUseInfo>
constexpr auto use_info_level_v = use_info_level<TUseInfo>::level;


/// <summary>
/// Invokes <paramref name="callback"/> for each network resource selected by
/// the given parameters.
/// </summary>
/// <typeparam name="TCallback"></typeparam>
/// <param name="scope"></param>
/// <param name="type"></param>
/// <param name="usage"></param>
/// <param name="callback"></param>
/// <returns></returns>
template<class TCallback> std::size_t enumerate_resources(
    _In_ const DWORD scope,
    _In_ const DWORD type,
    _In_ const DWORD usage,
    _In_opt_ NETRESOURCEW *resource,
    _In_ TCallback callback);

/// <summary>
/// Invokes <paramref name="callback"/> for each network resource selected by
/// the given parameters.
/// </summary>
/// <typeparam name="TCallback"></typeparam>
/// <param name="scope"></param>
/// <param name="type"></param>
/// <param name="usage"></param>
/// <param name="callback"></param>
/// <returns></returns>
template<class TCallback> inline std::size_t enumerate_resources(
        _In_ const DWORD scope,
        _In_ const DWORD type,
        _In_ const DWORD usage,
        _In_ TCallback callback) {
    return enumerate_resources(scope, type, usage, nullptr, callback);
}

/// <summary>
/// Invokes <paramref name="callback"/> for each share on the specified
/// <paramref name="server"/>.
/// </summary>
/// <typeparam name="TCallback">The type of the callback to be invoked. This
/// callback must accept a reference to one of the structures a
/// <see cref=share_info_level" /> specialisation is in place. You cannot use
/// an <see langword="auto" /> argument here as the share info level is derived
/// from the type of the parameter. The callback type <i>can</i> return a
/// <see langword="bool" /> to indicate whether the enumeration should continue
/// or be aborted.</typeparam>
/// <param name="server">The name of the server to enumerate the shares on.
/// If <see langword="nullptr" />, the local machine will be used.</param>
/// <param name="callback">The callback to be invoked for each share. Note that
/// the data pass ised to the callback are only valid as long as the callback
/// is executing. Any string that is to be used later must be copied.</param>
/// <returns>The number of callback invocations.</returns>
template<class TCallback> std::size_t enumerate_shares(
    _In_opt_z_ const wchar_t *server, _In_ TCallback callback);

/// <summary>
/// Invokes <paramref name="callback"/> for each connection from the local
/// to a share. Note that the underling <see cref="NetUseEnum" /> API does not
/// enumerate DFS shares or WebDAV.
/// </summary>
/// <typeparam name="TCallback"></typeparam>
/// <param name="server"></param>
/// <param name="callback"></param>
/// <returns></returns>
template<class TCallback> std::size_t enumerate_use(_In_ TCallback callback);

LYRA_DETAIL_NAMESPACE_END

#include "win32_shares.inl"

#endif /* defined(_WIN32) */
#endif /* !defined(_LYRA_WIN32_SHARES_H) */
