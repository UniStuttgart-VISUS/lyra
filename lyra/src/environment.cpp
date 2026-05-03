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


#if !defined(_WIN32)
/// <summary>
/// This should give us access to the environment on Linux according to
/// https://stackoverflow.com/questions/2085302/printing-all-environment-variables-in-c-c.
/// </summary>
extern char **environ;
#endif /* defined(_WIN32) */


/*
 * LYRA_NAMESPACE::environment::get
 */
LYRA_NAMESPACE::property_set LYRA_NAMESPACE::environment::get(
        _In_ const collection_flags flags) {
    typedef LYRA_NAMESPACE::environment::current_directory cwd;
    typedef LYRA_NAMESPACE::environment::variables vars;

    detail::property_set_impl ps;

    try {
        auto path = get_current_directory();
        detail::checked_add<cwd>(ps, flags, path.data());
    } catch (std::exception& ex) {
        LYRA_TRACE(_T("Failed to obtain the current directory: %s"), ex.what());
    }

    detail::checked_add<vars>(ps, flags, get_variables(flags));

    return property_set(std::move(ps));
}


/*
 * LYRA_NAMESPACE::environment::get_variables
 */
LYRA_NAMESPACE::property_set LYRA_NAMESPACE::environment::get_variables(
        _In_ const collection_flags flags) {
    if (detail::check_sensitive<variables>(flags)) {
        detail::property_set_impl ps;

#if defined(_WIN32)
        auto strings = ::GetEnvironmentStringsW();
        if (strings != nullptr) {
            std::vector<std::wstring> pairs;
            detail::multi_sz_copy(std::back_inserter(pairs), strings);

            for (auto& p : pairs) {
                auto sep = p.find(L'=');
                if (sep != std::wstring::npos) {
                    auto name = p.substr(0, sep);
                    if (name.empty()) {
                        continue;
                    }

                    auto value = p.substr(sep + 1);
                    ps.add(to_utf8(name), to_utf8(value));
                }
            }
        }

#else /* defined(_WIN32) */
        for (auto p = environ; *p != nullptr; ++p) {
            std::string pair(*p);

            auto sep = pair.find('=');
            if (sep != std::string::npos) {
                auto name = pair.substr(0, sep);
                if (name.empty()) {
                    continue;
                }

                auto value = pair.substr(sep + 1);
                ps.add(std::move(name), std::move(value));
            }
        }

#endif /* defined(_WIN32) */

        return property_set(std::move(ps));
    }

    return property_set();
}
