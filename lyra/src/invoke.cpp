// <copyright file="invoke.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "invoke.h"

#include <array>
#include <cerrno>
#include <cstdio>
#include <memory>
#include <system_error>

#include "visus/lyra/convert_string.h"


#if defined(_WIN32)
typedef std::unique_ptr<FILE, decltype(&::_pclose)> unique_pipe;
#else /* defined(_WIN32) */
typedef std::unique_ptr<FILE, decltype(&::pclose)> unique_pipe;
#endif /* defined(_WIN32) */


/*
 * LYRA_DETAIL_NAMESPACE::invoke
 */
std::string LYRA_DETAIL_NAMESPACE::invoke(_In_z_ const char *cmd) {
    std::array<char, 512> buffer;
    std::string retval;

#if defined(_WIN32)
    unique_pipe f(::_popen(cmd, "r"), ::_pclose);
    if (f == nullptr) {
        throw std::system_error(_doserrno, std::system_category());
    }
#else /* defined(_WIN32) */
    unique_pipe f(::popen(cmd, "r"), ::pclose);
    if (f == nullptr) {
        throw std::system_error(errno, std::system_category());
    }
#endif /* defined(_WIN32) */

    while (!::feof(f.get())) {
        if (::fgets(buffer.data(), static_cast<int>(buffer.size()), f.get())
                != nullptr) {
            retval += buffer.data();
        }
    }

    return retval;
}


/*
 * LYRA_DETAIL_NAMESPACE::invoke
 */
std::string LYRA_DETAIL_NAMESPACE::invoke(_In_z_ const wchar_t *cmd) {
#if defined(_WIN32)
    std::array<wchar_t, 512> buffer;
    std::string retval;

    unique_pipe f(::_wpopen(cmd, L"r"), ::_pclose);
    if (f == nullptr) {
        throw std::system_error(_doserrno, std::system_category());
    }

    while (!::feof(f.get())) {
        if (::fgetws(buffer.data(), static_cast<int>(buffer.size()), f.get())
                != nullptr) {
            retval += to_utf8(buffer.data());
        }
    }

    return retval;
#else /* defined(_WIN32) */
    const auto c = to_utf8(cmd);
    return invoke(c.c_str());
#endif /* defined(_WIN32) */
}
