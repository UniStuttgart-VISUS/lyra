// <copyright file="win32_shares.inl" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>


/*
 * LYRA_DETAIL_NAMESPACE::enumerate_shares
 */
template<class TCallback>
std::size_t LYRA_DETAIL_NAMESPACE::enumerate_shares(
        _In_opt_z_ const wchar_t *server,
        _In_ TCallback callback) {
    SHARE_INFO_2 *buffer = nullptr;
    DWORD read = 0;
    DWORD resume = 0;
    std::size_t retval = 0;
    NET_API_STATUS status = ERROR_SUCCESS;
    DWORD total = 0;

    do {
        status = ::NetShareEnum(const_cast<wchar_t *>(server),
            2,
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
            if (!cb(buffer[i])) {
                return retval;
            }
        }
    } while ((status == ERROR_MORE_DATA));
}
