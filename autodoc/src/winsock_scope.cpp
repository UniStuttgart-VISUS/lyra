// <copyright file="winsock_scope.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "winsock_scope.h"

#include <cstring>
#include <memory>

#if defined(_WIN32)
#include <wil/result.h>
#endif /* defined(_WIN32) */


/*
 * LYRA_DETAIL_NAMESPACE::winsock_scope::winsock_scope
 */
LYRA_DETAIL_NAMESPACE::winsock_scope::winsock_scope(
        _In_ const std::uint16_t version) {
#if defined(_WIN32)
    THROW_IF_WIN32_ERROR(::WSAStartup(version, &this->_data));
#endif /* defined(_WIN32) */
}

/*
 * LYRA_DETAIL_NAMESPACE::winsock_scope::winsock_scope
 */
LYRA_DETAIL_NAMESPACE::winsock_scope::winsock_scope(
        _Inout_ winsock_scope&& rhs) noexcept {
#if defined(_WIN32)
    ::memcpy(&this->_data, &rhs._data, sizeof(this->_data));
    ::ZeroMemory(&rhs._data, sizeof(rhs._data));
#endif /* defined(_WIN32) */
}


/*
 * LYRA_DETAIL_NAMESPACE::winsock_scope::~winsock_scope
 */
LYRA_DETAIL_NAMESPACE::winsock_scope::~winsock_scope(void) noexcept {
#if defined(_WIN32)
    if (this->_data.wVersion != 0) {
        ::WSACleanup();
    }
#endif /* defined(_WIN32) */
}


/*
 * LYRA_DETAIL_NAMESPACE::winsock_scope::operator =
 */
LYRA_DETAIL_NAMESPACE::winsock_scope& LYRA_DETAIL_NAMESPACE::winsock_scope::operator =(
        _Inout_ winsock_scope&& rhs) noexcept {
    if (this != std::addressof(rhs)) {
#if defined(_WIN32)
        if (this->_data.wVersion != 0) {
            ::WSACleanup();
        }

        ::memcpy(&this->_data, &rhs._data, sizeof(this->_data));
        ::ZeroMemory(&rhs._data, sizeof(rhs._data));
#endif /* defined(_WIN32) */
    }

    return *this;
}
