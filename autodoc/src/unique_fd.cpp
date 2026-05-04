// <copyright file="unique_fd.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "unique_fd.h"

#include <memory>


/*
 * LYRA_DETAIL_NAMESPACE::unique_fd::reset
 */
void LYRA_DETAIL_NAMESPACE::unique_fd::reset(_In_ const int fd) noexcept {
    if (*this) {
#if defined(_WIN32)
        ::_close(this->_fd);
#else /* defined(_WIN32) */
        ::close(this->_fd);
#endif /* defined(_WIN32) */
    }

    this->_fd = fd;
}


/*
 * LYRA_DETAIL_NAMESPACE::unique_fd::operator =
 */
LYRA_DETAIL_NAMESPACE::unique_fd&
LYRA_DETAIL_NAMESPACE::unique_fd::operator =(_Inout_ unique_fd&& rhs) noexcept {
    if (this != std::addressof(rhs)) {
        this->reset(rhs._fd);
        rhs._fd = -1;
    }

    return *this;
}
