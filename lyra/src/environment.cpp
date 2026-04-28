// <copyright file="environment.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "visus/lyra/environment.h"

#include <cerrno>
#include <system_error>
#include <vector>

#if defined(_WIN32)
#include <Windows.h>
#include <wil/result.h>
#else /* defined(_WIN32) */
#include <unistd.h>
#endif /* defined(_WIN32) */

#include "visus/lyra/convert_string.h"
#include "visus/lyra/multi_sz.h"
#include "visus/lyra/trace.h"

#include "property_set_impl.h"


/// <summary>
/// Determines the current working directory.
/// </summary>
/// <returns>The current working directory.</returns>
static std::string get_current_directory(void) {
#if defined(_WIN32)
    std::vector<wchar_t> path(MAX_PATH);

    auto len = ::GetCurrentDirectoryW(static_cast<DWORD>(path.size()),
        path.data());
    THROW_LAST_ERROR_IF(len == 0);

    if (len > path.size()) {
        path.resize(len);
        len = ::GetCurrentDirectoryW(static_cast<DWORD>(path.size()),
            path.data());
        THROW_LAST_ERROR_IF(len == 0);
    }

    return LYRA_NAMESPACE::to_utf8(path.data(), len);
#else /* defined(_WIN32) */
    std::vector<char> path(pathconf(".", _PC_PATH_MAX));

    if (getcwd(path.data(), path.size()) == nullptr) {
        throw std::system_error(errno, std::generic_category());
    }

    return std::string(path.data());
#endif /* defined(_WIN32) */
}


/*
 * LYRA_NAMESPACE::environment::get
 */
LYRA_NAMESPACE::property_set LYRA_NAMESPACE::environment::get(void) {
    detail::property_set_impl ps;
    property_set retval;

    try {
        auto path = get_current_directory();
        ps.add<LYRA_NAMESPACE::environment::current_directory>(path.data());
    } catch (std::exception& ex) {
        LYRA_TRACE(_T("Failed to obtain the current directory: %s"), ex.what());
    }

    realise(retval, std::move(ps));
    return retval;
}


/*
 * LYRA_NAMESPACE::environment::get_variables
 */
LYRA_NAMESPACE::property_set LYRA_NAMESPACE::environment::get_variables(
        void) {
    throw "TODO";
}
