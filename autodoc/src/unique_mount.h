// <copyright file="unique_mount.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_UNIQUE_MOUNT_H)
#define _LYRA_UNIQUE_MOUNT_H
#pragma once

#if !defined(_WIN32)
#include <cassert>
#include <string>

#include <unistd.h>

#include <sys/types.h>
#include <sys/mount.h>

#include "visus/autodoc/api.h"


LYRA_DETAIL_NAMESPACE_BEGIN

/// <summary>
/// RAII wrapper for temporary mounting a filesystem on Linux.
/// </summary>
class LYRA_TEST_API unique_mount final {

public:

    /// <summary>
    /// Initialises a new instance.
    /// </summary>
    inline unique_mount(void) noexcept : _delete(false) { }

    /// <summary>
    /// Initialises a new instance.
    /// </summary>
    /// <param name="source"></param>
    /// <param name="target"></param>
    /// <param name="type"></param>
    /// <param name="flags"></param>
    /// <param name="data"></param>
    unique_mount(_In_z_ const char *source, _In_z_ const char *target,
        _In_ const char *type, _In_ const unsigned long flags = 0,
        _In_opt_ const void *data = nullptr);

    /// <summary>
    /// Initialises a new instance.
    /// </summary>
    /// <param name="source"></param>
    /// <param name="type"></param>
    /// <param name="flags"></param>
    /// <param name="data"></param>
    unique_mount(_In_z_ const char *source, _In_ const char *type,
        _In_ const unsigned long flags = 0,
        _In_opt_ const void *data = nullptr);

    /// <summary>
    /// Initialise from move.
    /// </summary>
    /// <param name="other">The object to be moved.</param>
    inline unique_mount(_Inout_ unique_mount&& other) noexcept {
        assert(this->_path.empty());
        *this = std::move(other);
    }

    /// <summary>
    /// Finalises the instance.
    /// </summary>
    inline ~unique_mount(void) noexcept {
        this->unmount();
    }

    /// <summary>
    /// Move assignment.
    /// </summary>
    /// <param name="rhs"></param>
    /// <returns></returns>
    unique_mount& operator =(_Inout_ unique_mount&& rhs) noexcept;

    /// <summary>
    /// Unmounts the filesystem if the mount is valid.
    /// </summary>
    void unmount(void) noexcept;

    /// <summary>
    /// Indicates whether the instance represents a valid mount.
    /// </summary>
    /// <returns><see langword="true" /> if the instance represents a valid
    /// mount, <see langword="false" /> otherwise.</returns>
    inline operator bool(void) const noexcept {
        return !this->_path.empty();
    }

    /// <summary>
    /// Converts the instance to its mount point.
    /// </summary>
    /// <returns>The mount point used by this mount.</returns>
    inline operator const std::string&(void) const noexcept {
        return this->_path;
    }

private:

    bool _delete;
    std::string _path;
};

LYRA_DETAIL_NAMESPACE_END

#endif /* !defined(_WIN32) */
#endif /* !defined(_LYRA_UNIQUE_MOUNT_H) */
