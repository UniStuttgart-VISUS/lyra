// <copyright file="unique_fd.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_UNIQUE_FD_H)
#define _LYRA_UNIQUE_FD_H
#pragma once

#if defined(_WIN32)
#include <io.h>
#include <Windows.h>
#else /* defined(_WIN32) */
#include <fcntl.h>
#include <unistd.h>
#endif /* defined(_WIN32) */

#include "visus/lyra/api.h"


LYRA_DETAIL_NAMESPACE_BEGIN

/// <summary>
/// RAII wrapper for POSIX file descriptors.
/// </summary>
class LYRA_TEST_API unique_fd final {

public:

    /// <summary>
    /// Initialises a new instance.
    /// </summary>
    /// <param name="fd"></param>
    inline unique_fd(_In_ const int fd = -1) noexcept : _fd(fd) { }

    /// <summary>
    /// Initialise from move.
    /// </summary>
    /// <param name="other">The object to be moved.</param>
    inline unique_fd(_Inout_ unique_fd&& other) noexcept : _fd(other._fd) {
        other._fd = -1;
    }

    /// <summary>
    /// Finalises the instance.
    /// </summary>
    inline ~unique_fd(void) {
        this->reset();
    }

    /// <summary>
    /// Gets the enclosed file descriptor.
    /// </summary>
    /// <returns></returns>
    inline int get(void) const noexcept {
        return this->_fd;
    }

    /// <summary>
    /// Releases any existing descriptor and takes ownership of the given one.
    /// </summary>
    /// <param name="fd"></param>
    void reset(_In_ const int fd = -1) noexcept;

    /// <summary>
    /// Move assignment.
    /// </summary>
    /// <param name="rhs"></param>
    /// <returns></returns>
    unique_fd& operator =(_Inout_ unique_fd&& rhs) noexcept;

    /// <summary>
    /// Indicates whether the instance currently owns a valid file descriptor.
    /// </summary>
    /// <returns><see langword="true" /> if the instance owns a valid file
    /// descriptor, <see langword="false" /> otherwise.</returns>
    inline operator bool(void) const noexcept {
        return (this->_fd != -1);
    }

private:

    int _fd;
};

LYRA_DETAIL_NAMESPACE_END

#endif /* !defined(_LYRA_UNIQUE_FD_H) */
