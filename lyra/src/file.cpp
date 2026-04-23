// <copyright file="file.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "file.h"

#include <algorithm>
#include <cerrno>
#include <system_error>

#if defined(_WIN32)
#include <wil/resource.h>
#else /* defined(_WIN32) */
#include <fcntl.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#endif /* defined(_WIN32) */

#include "visus/lyra/convert_string.h"


#if defined(_WIN32)
/*
 * LYRA_DETAIL_NAMESPACE::file_size
 */
std::size_t LYRA_DETAIL_NAMESPACE::file_size(_In_ HANDLE handle) {
    LARGE_INTEGER retval;
    if (!::GetFileSizeEx(handle, &retval)) {
        throw std::system_error(::GetLastError(), std::system_category());
    }
    return retval.QuadPart;
}
#endif /* defined (_WIN32) */


/*
 * LYRA_DETAIL_NAMESPACE::file_size
 */
std::size_t LYRA_DETAIL_NAMESPACE::file_size(_In_ int fd) {
    struct stat s;
    if (::fstat(fd, &s) == -1) {
#if defined(_WIN32)
        throw std::system_error(_doserrno, std::system_category());
#else /* defined(_WIN32) */
        throw std::system_error(errno, std::system_category());
#endif /* defined(_WIN32) */
    }
    return s.st_size;
}


/*
 * LYRA_DETAIL_NAMESPACE::file_size
 */
std::size_t LYRA_DETAIL_NAMESPACE::file_size(_In_z_ const char *path) {
#if defined(_WIN32)
    auto handle = open_read(path);
    return file_size(handle.get());
#else /* defined(_WIN32) */
    struct stat retval;
    if (::stat(path, &retval) == -1) {
        throw std::system_error(errno, std::system_category());
    }
    return retval.st_size;
#endif /* defined(_WIN32) */
}


/*
 * LYRA_DETAIL_NAMESPACE::file_size
 */
std::size_t LYRA_DETAIL_NAMESPACE::file_size(_In_z_ const wchar_t *path) {
#if defined(_WIN32)
    auto handle = open_read(path);
    return file_size(handle.get());
#else /* defined(_WIN32) */
    auto p = to_utf8(path);
    return file_size(p.c_str());
#endif /* defined(_WIN32) */
}


/*
 * LYRA_DETAIL_NAMESPACE::open_read
 */
LYRA_DETAIL_NAMESPACE::unique_file LYRA_DETAIL_NAMESPACE::open_read(
        _In_z_ const char *path) {
#if defined(_WIN32)
    unique_file retval(::CreateFileA(path, GENERIC_READ, FILE_SHARE_READ,
        0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0));
    if (!retval) {
        throw std::system_error(::GetLastError(), std::system_category());
    }
    return retval;
#else /* defined(_WIN32) */
    unique_file retval(::open(path, O_RDONLY));
    if (!retval) {
        throw std::system_error(errno, std::system_category());
    }
    return retval;
#endif /* defined(_WIN32) */
}

/*
 * LYRA_DETAIL_NAMESPACE::open_read
 */
LYRA_DETAIL_NAMESPACE::unique_file LYRA_DETAIL_NAMESPACE::open_read(
        _In_z_ const wchar_t *path) {
#if defined(_WIN32)
    wil::unique_hfile retval(::CreateFileW(path, GENERIC_READ, FILE_SHARE_READ,
        0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0));
    if (!retval) {
        throw std::system_error(::GetLastError(), std::system_category());
    }
    return retval;
#else /* defined(_WIN32) */
    auto p = to_utf8(path);
    return open_read(p.c_str());
#endif /* defined(_WIN32) */
}


/*
 * LYRA_DETAIL_NAMESPACE::read_all_bytes
 */
std::vector<std::uint8_t> LYRA_DETAIL_NAMESPACE::read_all_bytes(
        _In_ const unique_file& file) {
    std::vector<std::uint8_t> retval(file_size(file.get()));
    auto total = static_cast<std::size_t>(0);

#if defined(_WIN32)
    if (::SetFilePointer(file.get(), 0, nullptr, FILE_BEGIN)
            == INVALID_SET_FILE_POINTER) {
        throw std::system_error(::GetLastError(), std::system_category());
    }
#else /* defined(_WIN32) */
    if (::lseek(file.get(), 0, SEEK_SET) == -1) {
        throw std::system_error(errno, std::system_category());
    }
#endif /* defined(_WIN32) */

    while (true) {
        auto rem = retval.size() - total;
        if (rem == 0) {
            rem = retval.size() / 2;
            if (rem == 0) {
                rem = 8;
            }
            retval.resize(retval.size() + rem);
        }

        auto dst = retval.data() + total;

#if defined(_WIN32)
        DWORD read;
        if (!::ReadFile(file.get(), dst, static_cast<DWORD>(rem), &read,
                nullptr)) {
            throw std::system_error(::GetLastError(), std::system_category());
        }
#else /* defined(_WIN32) */
        auto read = ::read(file.get(), dst, rem);
        if (read == -1) {
            throw std::system_error(errno, std::system_category());
        }
#endif /* defined(_WIN32) */

        if (read == 0) {
            retval.resize(total);
            return retval;
        }

        total += read;
    }
}
