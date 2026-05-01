// <copyright file="processes.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "processes.h"

#include <memory>

#if defined(_WIN32)
#include <wil/result.h>
#else /* defined(_WIN32) */
#include <unistd.h>
#endif /* defined(_WIN32) */

#include "visus/lyra/convert_string.h"

#include "file.h"
#include "string_manipulation.h"


/*
 * LYRA_DETAIL_NAMESPACE::get_command_line
 */
std::string LYRA_DETAIL_NAMESPACE::get_command_line(void) {
#if defined(_WIN32)
    return to_utf8(::GetCommandLineW());
#else /* defined(_WIN32) */
    const auto retval = read_all_bytes("/proc/self/cmdline");
    return reinterpret_cast<const char *>(retval.data());
#endif /* defined(_WIN32) */
}


/*
 * LYRA_DETAIL_NAMESPACE::get_process_id
 */
LYRA_DETAIL_NAMESPACE::pid_type LYRA_DETAIL_NAMESPACE::get_process_id(
        void) noexcept {
#if defined(_WIN32)
    return ::GetCurrentProcessId();
#else /* defined(_WIN32) */
    return ::getpid();
#endif /* defined(_WIN32) */
}


/*
 * LYRA_DETAIL_NAMESPACE::get_executable_path
 */
std::string LYRA_DETAIL_NAMESPACE::get_executable_path(
        _In_ const pid_type pid) {
#if defined(_WIN32)
    wil::unique_handle handle(::OpenProcess(
        PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pid));
    if (!handle) {
        return "";
    }

    std::vector<wchar_t> path(MAX_PATH + 1);

    while (true) {
        auto l = static_cast<DWORD>(path.size());
        ::QueryFullProcessImageNameW(handle.get(), 0, path.data(), &l);
        switch (::GetLastError()) {
            case ERROR_SUCCESS:
                remove_trailing_nulls(path);
                return to_utf8(path.data(), path.size());

            case ERROR_INSUFFICIENT_BUFFER:
                path.resize(path.size() * 2);
                break;

            default:
                return "";
        }
    }

#else /* defined(_WIN32) */
    std::string path("/proc/");
    path += std::to_string(pid);
    path += "/exe";
    return final_path(path.c_str());
#endif /* defined(_WIN32) */
}


/*
 * LYRA_DETAIL_NAMESPACE::get_executable_path
 */
std::string LYRA_DETAIL_NAMESPACE::get_executable_path(void) {
#if defined(_WIN32)
    return get_module_path(NULL);
#else /* defined(_WIN32) */
    return final_path("/proc/self/exe");
#endif /* defined(_WIN32) */
}


#if defined(_WIN32)
/*
 * LYRA_DETAIL_NAMESPACE::get_process_image
 */
std::string LYRA_DETAIL_NAMESPACE::get_module_path(_In_opt_ HMODULE handle) {
    std::vector<wchar_t> path(MAX_PATH + 1);

    while (true) {
        const auto l = static_cast<DWORD>(path.size());
        ::GetModuleFileNameW(handle, std::addressof(path[0]), l);
        switch (::GetLastError()) {
            case ERROR_SUCCESS:
                remove_trailing_nulls(path);
                return to_utf8(path.data(), path.size());

            case ERROR_INSUFFICIENT_BUFFER:
                path.resize(path.size() * 2);
                break;

            default:
                return "";
        }
    }
}
#endif /* defined(_WIN32) */
