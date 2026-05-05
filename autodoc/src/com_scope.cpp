// <copyright file="com_scope.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "com_scope.h"

#include <memory>

#if defined(_WIN32)
#include <objbase.h>

#include <wil/com.h>
#endif /* defined(_WIN32) */


/*
 * LYRA_DETAIL_NAMESPACE::com_scope::com_scope
 */
LYRA_DETAIL_NAMESPACE::com_scope::com_scope(_In_ const flags_type flags) {
#if defined(_WIN32)
    THROW_IF_FAILED(::CoInitializeEx(nullptr, flags));
#endif /* defined(_WIN32) */
}


/*
 * LYRA_DETAIL_NAMESPACE::com_scope::~com_scope
 */
LYRA_DETAIL_NAMESPACE::com_scope::~com_scope(void) noexcept {
#if defined(_WIN32)
    if (!this->_disposed) {
        ::CoUninitialize();
    }
#endif /* defined(_WIN32) */
}


/*
 * LYRA_DETAIL_NAMESPACE::com_scope::operator =
 */
LYRA_DETAIL_NAMESPACE::com_scope& LYRA_DETAIL_NAMESPACE::com_scope::operator =(
        _Inout_ com_scope&& rhs) noexcept {
    if (this != std::addressof(rhs)) {
#if defined(_WIN32)
        if (!this->_disposed) {
            ::CoUninitialize();
        }
#endif /* defined(_WIN32) */

        this->_disposed = rhs._disposed;
        rhs._disposed = true;
    }

    return *this;
}
