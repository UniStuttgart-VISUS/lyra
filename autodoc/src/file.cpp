// <copyright file="file.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "file.h"

#include <algorithm>
#include <cerrno>
#include <climits>
#include <cstdlib>
#include <cstring>
#include <system_error>

#if defined(_WIN32)
#include <wil/resource.h>
#else /* defined(_WIN32) */
#include <fcntl.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#endif /* defined(_WIN32) */

#include "visus/autodoc/convert_string.h"
#include "visus/autodoc/hash.h"
#include "visus/autodoc/on_exit.h"
#include "visus/autodoc/version.h"

#include "property_set_impl.h"
#include "string_manipulation.h"


/*
 * LYRA_DETAIL_NAMESPACE::file_hash
 */
std::string LYRA_DETAIL_NAMESPACE::file_hash(_In_ const unique_file& file) {
    hash hash(sha256);
    // TODO: stream the contents instead of reading everything into ram.
    auto data = read_all_bytes(file);
    hash.append(data.data(), data.size());
    return to_hex_string(hash.finish(), hash.length());
}


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
 * LYRA_DETAIL_NAMESPACE::final_path
 */
std::string LYRA_DETAIL_NAMESPACE::final_path(_In_z_ const char *path) {
#if defined(_WIN32)
    auto handle = open_read(path);

    auto len = ::GetFinalPathNameByHandleW(handle.get(), nullptr, 0,
        VOLUME_NAME_NT);
    if (len > 0) {
        std::vector<wchar_t> p(len + 1);
        len = ::GetFinalPathNameByHandleW(handle.get(), p.data(),
            static_cast<DWORD>(p.size()), VOLUME_NAME_NT);
        return to_utf8(p.data(), len);

    } else {
        return (path != nullptr) ? path : "";
    }

#else /* defined(_WIN32) */
    auto p = ::realpath(path, nullptr);
    if (p != nullptr) {
        LYRA_ON_EXIT([p](void) { ::free(p); });
        std::string retval(p);
        return retval;
    } else {
        return (path != nullptr) ? path : "";
    }
#endif /* defined(_WIN32) */
}


#if defined(_WIN32)
/*
 * LYRA_DETAIL_NAMESPACE::get_file_time
 */
void LYRA_DETAIL_NAMESPACE::get_file_time(
        _Out_ timestamp& creation_time,
        _Out_ timestamp& last_access_time,
        _Out_ timestamp& last_write_time,
        _In_ HANDLE file) {
    FILETIME c, a, w;
    THROW_LAST_ERROR_IF(!::GetFileTime(file, &c, &a, &w));
    creation_time = timestamp::from_file_time(c);
    last_access_time = timestamp::from_file_time(a);
    last_write_time = timestamp::from_file_time(w);
}
#endif /* defined(_WIN32) */


/*
 * LYRA_DETAIL_NAMESPACE::get_file_time
 */
void LYRA_DETAIL_NAMESPACE::get_file_time(
        _Out_ timestamp& status_time,
        _Out_ timestamp& last_access_time,
        _Out_ timestamp& last_write_time,
        _In_ int file) {
    struct stat s;
    if (::fstat(file, &s) != -1) {
#if defined(_WIN32)
        throw std::system_error(_doserrno, std::system_category());
#else /* defined(_WIN32) */
        throw std::system_error(errno, std::system_category());
#endif /* defined(_WIN32) */
    }

    status_time = timestamp::from_time_t(s.st_ctime);
    last_access_time = timestamp::from_time_t(s.st_atime);
    last_write_time = timestamp::from_time_t(s.st_mtime);
}


#if defined(_WIN32)
/*
 * LYRA_DETAIL_NAMESPACE::final_path
 */
std::string LYRA_DETAIL_NAMESPACE::final_path(_In_z_ const wchar_t *path) {
    auto handle = open_read(path);

    auto len = ::GetFinalPathNameByHandleW(handle.get(), nullptr, 0,
        VOLUME_NAME_NT);
    if (len > 0) {
        std::vector<wchar_t> p(len + 1);
        len = ::GetFinalPathNameByHandleW(handle.get(), p.data(),
            static_cast<DWORD>(p.size()), VOLUME_NAME_NT);
        return to_utf8(p.data(), len);

    } else {
        return (path != nullptr) ? to_utf8(path) : "";
    }
}
#endif /* defined(_WIN32) */


/*
 * LYRA_DETAIL_NAMESPACE::get_file_version_info
 */
LYRA_NAMESPACE::property_set LYRA_DETAIL_NAMESPACE::get_file_version_info(
        _In_z_ const char *path) {
    detail::property_set_impl ps;

#if defined(_WIN32)
#define _LYRA_ADD_FLAG_STR(s, f) do {\
    if ((v & f) == f) {\
        if (!s.empty()) { s+= u8", "; }\
        s += #f;\
    }\
} while (false)
#define _LYRA_ADD_FLAG_PROP(n, s, v) do {\
    if (s.empty()) {\
        ps.add(n, v);\
    } else {\
        s += u8" (" + std::to_string(v) + u8")";\
        ps.add(n, s.c_str());\
    }\
} while (false);

    const auto size = ::GetFileVersionInfoSizeA(path, nullptr);
    if (size > 0) {
        std::vector<std::uint8_t> buffer(size);
        if (::GetFileVersionInfoA(path, 0, size, buffer.data())) {
            void *block;
            UINT length;

            // The fixed root block described at
            // https://learn.microsoft.com/en-us/windows/win32/api/verrsrc/ns-verrsrc-vs_fixedfileinfo
            if (::VerQueryValueA(buffer.data(), "\\", &block, &length)) {
                auto info = reinterpret_cast<VS_FIXEDFILEINFO *>(block);

                {
                    const auto major = info->dwFileVersionMS >> 16;
                    const auto minor = info->dwFileVersionMS & 0xFFFF;
                    const auto release = info->dwFileDateLS >> 16;
                    const auto build = info->dwFileDateLS & 0xFFFF;

                    ps.add("File Version", LYRA_NAMESPACE::version::make(
                        major, minor, release, build));
                }

                {
                    const auto major = info->dwProductVersionMS >> 16;
                    const auto minor = info->dwProductVersionMS & 0xFFFF;
                    const auto release = info->dwProductVersionLS >> 16;
                    const auto build = info->dwProductVersionLS & 0xFFFF;

                    ps.add("Product Version", LYRA_NAMESPACE::version::make(
                        major, minor, release, build));
                }

                {
                    const auto v = info->dwFileFlagsMask & info->dwFileFlags;
                    std::string s;
                    _LYRA_ADD_FLAG_STR(s, VS_FF_DEBUG);
                    _LYRA_ADD_FLAG_STR(s, VS_FF_INFOINFERRED);
                    _LYRA_ADD_FLAG_STR(s, VS_FF_PATCHED);
                    _LYRA_ADD_FLAG_STR(s, VS_FF_PRERELEASE);
                    _LYRA_ADD_FLAG_STR(s, VS_FF_PRIVATEBUILD);
                    _LYRA_ADD_FLAG_STR(s, VS_FF_SPECIALBUILD);
                    _LYRA_ADD_FLAG_PROP("File Flags", s, v);
                }

                {
                    const auto v = info->dwFileOS;
                    std::string s;
                    _LYRA_ADD_FLAG_STR(s, VOS_DOS);
                    _LYRA_ADD_FLAG_STR(s, VOS_NT);
                    _LYRA_ADD_FLAG_STR(s, VOS__WINDOWS16);
                    _LYRA_ADD_FLAG_STR(s, VOS__WINDOWS32);
                    _LYRA_ADD_FLAG_STR(s, VOS_OS216);
                    _LYRA_ADD_FLAG_STR(s, VOS_OS232);
                    _LYRA_ADD_FLAG_STR(s, VOS__PM16);
                    _LYRA_ADD_FLAG_STR(s, VOS__PM32);
                    _LYRA_ADD_FLAG_PROP("File Operating System", s, v);
                }

                {
                    const auto v = info->dwFileType;
                    std::string s;
                    _LYRA_ADD_FLAG_STR(s, VFT_APP);
                    _LYRA_ADD_FLAG_STR(s, VFT_DLL);
                    _LYRA_ADD_FLAG_STR(s, VFT_DRV);
                    _LYRA_ADD_FLAG_STR(s, VFT_FONT);
                    _LYRA_ADD_FLAG_STR(s, VFT_STATIC_LIB);
                    _LYRA_ADD_FLAG_STR(s, VFT_VXD);
                    _LYRA_ADD_FLAG_PROP("File Type", s, v);
                }

                {
                    const auto v = info->dwFileSubtype;
                    std::string s;

                    switch (info->dwFileType) {
                        case VFT_DRV:
                            _LYRA_ADD_FLAG_STR(s, VFT2_DRV_COMM);
                            _LYRA_ADD_FLAG_STR(s, VFT2_DRV_DISPLAY);
                            _LYRA_ADD_FLAG_STR(s, VFT2_DRV_INSTALLABLE);
                            _LYRA_ADD_FLAG_STR(s, VFT2_DRV_KEYBOARD);
                            _LYRA_ADD_FLAG_STR(s, VFT2_DRV_LANGUAGE);
                            _LYRA_ADD_FLAG_STR(s, VFT2_DRV_MOUSE);
                            _LYRA_ADD_FLAG_STR(s, VFT2_DRV_NETWORK);
                            _LYRA_ADD_FLAG_STR(s, VFT2_DRV_PRINTER);
                            _LYRA_ADD_FLAG_STR(s, VFT2_DRV_SOUND);
                            _LYRA_ADD_FLAG_STR(s, VFT2_DRV_SYSTEM);
                            _LYRA_ADD_FLAG_STR(s, VFT2_DRV_VERSIONED_PRINTER);
                            break;

                        case VFT_FONT:
                            _LYRA_ADD_FLAG_STR(s, VFT2_FONT_RASTER);
                            _LYRA_ADD_FLAG_STR(s, VFT2_FONT_TRUETYPE);
                            _LYRA_ADD_FLAG_STR(s, VFT2_FONT_VECTOR);
                            break;
                    }

                    _LYRA_ADD_FLAG_PROP("File Sub-Type", s, v);
                }

                {
                    LARGE_INTEGER v;
                    v.HighPart = info->dwFileDateMS;
                    v.LowPart = info->dwFileDateLS;
                    const auto t = LYRA_NAMESPACE::timestamp::from_file_time(v);
                    ps.add("File Date", t);
                }
            } /* if (::VerQueryValueA(buffer.data(), "\\", &block, &length)) */
        } /* if (::GetFileVersionInfoA(path, 0, size, buffer.data())) */
    } /* if (size > 0) */

#undef _LYRA_ADD_FLAG_PROP
#undef _LYRA_ADD_FLAG_STR
#endif /* defined(_WIN32) */

    return property_set(std::move(ps));
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
