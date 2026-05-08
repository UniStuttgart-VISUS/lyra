// <copyright file="win32_shares.inl" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>


/*
 * LYRA_DETAIL_NAMESPACE::enumerate_resources
 */
template<class TCallback>
std::size_t LYRA_DETAIL_NAMESPACE::enumerate_resources(
        _In_ const DWORD scope,
        _In_ const DWORD type,
        _In_ const DWORD usage,
        _In_opt_ NETRESOURCEW *resource,
        _In_ TCallback callback) {
    constexpr auto abortable = !std::is_same_v<void,
        std::invoke_result_t<TCallback, NETRESOURCEW>>;
    std::vector<NETRESOURCEW> buffer(16);
    DWORD cnt;
    auto cnt_buffer = static_cast<DWORD>(buffer.size() * sizeof(NETRESOURCEW));
    HANDLE handle;
    std::size_t retval = 0;

    auto status = ::WNetOpenEnumW(scope, type, usage, resource, &handle);
    if (status != NO_ERROR) {
        LYRA_TRACE(_T("WNetOpenEnumW failed with error %u"), status);
        return 0;
    }
    LYRA_ON_EXIT([&handle](void) { ::WNetCloseEnum(handle); });

    do {
        status = ::WNetEnumResourceW(handle, &cnt, buffer.data(), &cnt_buffer);

        switch (status) {
            case NO_ERROR:
                // We got at least a batch of data to process.
                for (DWORD i = 0; i < cnt; ++i) {
                    ++retval;

                    if constexpr (abortable) {
                        if (!callback(buffer[i])) {
                            return retval;
                        }
                    } else {
                        callback(buffer[i]);
                    }

                    if ((buffer[i].dwUsage & RESOURCEUSAGE_CONTAINER) != 0) {
                        LYRA_TRACE(L"Recursive enumeration of container %s.",
                            buffer[i].lpRemoteName);
                        retval += enumerate_resources(scope, type, usage,
                            std::addressof(buffer[i]), callback);
                    }
                }
                break;

            case ERROR_MORE_DATA:
                // We need a larger buffer, so resize it.
                buffer.resize(cnt_buffer / sizeof(NETRESOURCEW));
                break;

            default:
                // This is a fatal error.
                LYRA_TRACE(_T("WNetEnumResourceW failed with error %u"), status);
                return retval;
        }
    } while (status != ERROR_NO_MORE_ITEMS);

    return retval;
}


/*
 * LYRA_DETAIL_NAMESPACE::enumerate_shares
 */
template<class TCallback>
std::size_t LYRA_DETAIL_NAMESPACE::enumerate_shares(
        _In_opt_z_ const wchar_t *server,
        _In_ TCallback callback) {
    typedef callable_traits<TCallback> cb_traits;
    typedef std::decay_t<cb_traits::argument<0>> info_type;
    constexpr auto abortable = !std::is_same_v<void, cb_traits::return_type>;
    info_type *buffer = nullptr;
    DWORD read = 0;
    DWORD resume = 0;
    std::size_t retval = 0;
    NET_API_STATUS status = ERROR_SUCCESS;
    DWORD total = 0;

    do {
        status = ::NetShareEnum(const_cast<wchar_t *>(server),
            share_info_level_v<info_type>,
            reinterpret_cast<LPBYTE *>(&buffer),
            MAX_PREFERRED_LENGTH,
            &read,
            &total,
            &resume);
        switch (status) {
            case ERROR_SUCCESS:
            case ERROR_MORE_DATA:
                break;
            default:
                LYRA_TRACE(_T("NetShareEnum failed with error %u"), status);
                return retval;
        }
        LYRA_ON_EXIT([&buffer](void) { ::NetApiBufferFree(buffer); });

        for (DWORD i = 0; i < read; ++i) {
            ++retval;
            if constexpr (abortable) {
                if (!callback(buffer[i])) {
                    return retval;
                }
            } else {
                callback(buffer[i]);
            }
        }
    } while ((status == ERROR_MORE_DATA));

    return retval;
}


/*
 * LYRA_DETAIL_NAMESPACE::enumerate_use
 */
template<class TCallback>
std::size_t LYRA_DETAIL_NAMESPACE::enumerate_use(_In_ TCallback callback) {
    typedef callable_traits<TCallback> cb_traits;
    typedef std::decay_t<cb_traits::argument<0>> info_type;
    constexpr auto abortable = !std::is_same_v<void, cb_traits::return_type>;
    info_type *buffer = nullptr;
    DWORD read = 0;
    DWORD resume = 0;
    std::size_t retval = 0;
    NET_API_STATUS status = ERROR_SUCCESS;
    DWORD total = 0;

    do {
        // Note: as per https://learn.microsoft.com/en-us/windows/win32/api/lmuse/nf-lmuse-netuseenum
        // only the local server is supported anyway, so we pass nullptr here.
        status = ::NetUseEnum(nullptr,
            use_info_level_v<info_type>,
            reinterpret_cast<LPBYTE *>(&buffer),
            MAX_PREFERRED_LENGTH,
            &read,
            &total,
            &resume);
        switch (status) {
            case ERROR_SUCCESS:
            case ERROR_MORE_DATA:
                break;
            default:
                LYRA_TRACE(_T("NetUseEnum failed with error %u"), status);
                return retval;
        }
        LYRA_ON_EXIT([&buffer](void) { ::NetApiBufferFree(buffer); });

        for (DWORD i = 0; i < read; ++i) {
            ++retval;
            if constexpr (abortable) {
                if (!callback(buffer[i])) {
                    return retval;
                }
            } else {
                callback(buffer[i]);
            }
        }
    } while ((status == ERROR_MORE_DATA));

    return retval;
}

