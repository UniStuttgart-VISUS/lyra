// <copyright file="processes.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "processes.h"

#include <memory>

#if defined(_WIN32)
#include <Psapi.h>
#include <TlHelp32.h>

#include <wil/resource.h>
#include <wil/result.h>
#else /* defined(_WIN32) */
#include <dlfcn.h>
#include <link.h>
#include <unistd.h>
#endif /* defined(_WIN32) */

#include "visus/lyra/convert_string.h"
#include "visus/lyra/on_exit.h"
#include "visus/lyra/trace.h"

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


/*
 * LYRA_DETAIL_NAMESPACE::get_loaded_library_paths
 */
std::vector<std::string> LYRA_DETAIL_NAMESPACE::get_loaded_library_paths(void) {
    std::vector<std::string> retval;

#if defined(_WIN32)
    MODULEENTRY32W module { };
    module.dwSize = sizeof(module);

    wil::unique_hfile snapshot(::CreateToolhelp32Snapshot(TH32CS_SNAPMODULE,
        ::GetCurrentProcessId()));
    if (!snapshot) {
        LYRA_TRACE(_T("CreateToolhelp32Snapshot failed: 0x%x"),
            ::GetLastError());
        return retval;
    }

    if (!::Module32FirstW(snapshot.get(), &module)) {
        LYRA_TRACE(_T("Module32First failed: 0x%x"), ::GetLastError());
        return retval;
    }

    do {
        retval.push_back(to_utf8(module.szExePath));
    } while (::Module32NextW(snapshot.get(), &module));

#else /* defined(_WIN32) */
    link_map *map = nullptr;

    auto handle = ::dlopen(nullptr, RTLD_NOW);
    if (handle == nullptr) {
        LYRA_TRACE("dlopen failed: %s", ::dlerror());
        return retval;
    }
    LYRA_ON_EXIT([handle] { ::dlclose(handle); });

    if (::dlinfo(handle, RTLD_DI_LINKMAP, &map) == -1) {
        LYRA_TRACE("RTLD_DI_LINKMAP failed: %s\n", ::dlerror());
        return retval;
    }

    while (map != nullptr) {
        if ((map->l_name != nullptr) && (*map->l_name != 0)) {
            retval.push_back(map->l_name);
        }

        map = map->l_next;
    }

#endif /* defined(_WIN32) */

    return retval;
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
