// <copyright file="unique_mount.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "unique_mount.h"

#include <cstring>
#include <filesystem>
#include <vector>

#include <sys/stat.h>

#include "visus/autodoc/trace.h"


/*
 * LYRA_DETAIL_NAMESPACE::unique_mount::unique_mount
 */
LYRA_DETAIL_NAMESPACE::unique_mount::unique_mount(
        _In_z_ const char *source,
        _In_z_ const char *target,
        _In_ const char *type,
        _In_ const unsigned long flags,
        _In_opt_ const void *data)
        : _delete(std::filesystem::create_directory(target)) {
    if (::mount(source, target, type, flags, data) == 0) {
        this->_path = target;
        LYRA_TRACE("Mounted \"%s\" on \"%s\".", source, this->_path.c_str());
    } else if (this->_delete) {
        LYRA_TRACE("mount failed with error 0x%x, deleting temporary directory "
            "\"%s\" prepared for the mount.", errno, target);
        ::rmdir(target);
    }
}


/*
 * LYRA_DETAIL_NAMESPACE::unique_mount::unique_mount
 */
LYRA_DETAIL_NAMESPACE::unique_mount::unique_mount(
        _In_z_ const char *source,
        _In_ const char *type,
        _In_ const unsigned long flags,
        _In_opt_ const void *data)
        : _delete(true) {
    constexpr auto tpl = "/tmp/sys-XXXXXX";
    std::vector<char> buffer(tpl, tpl + ::strlen(tpl) + 1);
    const auto path = ::mkdtemp(buffer.data());
    LYRA_TRACE("Creating temporary mount location \"%s\" for \"%s\".", path,
        source);

    if (path == nullptr) {
        LYRA_TRACE("Creating temporary directory for mount failed with error "
            "0x%x.", errno);
        this->_delete = false;

    } else if (::mount(source, path, type, flags, data) != 0) {
        LYRA_TRACE("mount failed with error 0x%x, deleting temporary directory "
            "\"%s\" prepared for the mount.", errno, path);
        ::rmdir(path);

    } else {
        this->_path = path;
        LYRA_TRACE("Mounted \"%s\" on \"%s\".", source, this->_path.c_str());
    }
}


/*
 * LYRA_DETAIL_NAMESPACE::unique_mount::unmount
 */
void LYRA_DETAIL_NAMESPACE::unique_mount::unmount(void) noexcept {
    if (!this->_path.empty()) {
        ::umount(this->_path.c_str());

        if (this->_delete) {
            LYRA_TRACE("Deleting temporary mount location \"%s\".",
                this->_path.c_str());
            ::rmdir(this->_path.c_str());
        }

        this->_path.clear();
    }
}


/*
 * LYRA_DETAIL_NAMESPACE::unique_mount::operator =
 */
LYRA_DETAIL_NAMESPACE::unique_mount&
LYRA_DETAIL_NAMESPACE::unique_mount::operator =(
        _Inout_ unique_mount&& rhs) noexcept {
    if (this != std::addressof(rhs)) {
        this->unmount();
        this->_delete = rhs._delete;
        rhs._delete = false;
        this->_path = std::move(rhs._path);
        rhs._path = "";
    }

    return *this;
}
