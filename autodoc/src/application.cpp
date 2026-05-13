// <copyright file="application.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "visus/autodoc/application.h"

#include <algorithm>

#include "visus/autodoc/environment.h"
#include "visus/autodoc/trace.h"

#include "boolean.h"
#include "file.h"
#include "git_environment.h"
#include "os_cpu_info.h"
#include "property_set_impl.h"
#include "processes.h"


/// <summary>
/// Adds the file timestamps to the property set.
/// </summary>
static bool add_timestamps(_Inout_ LYRA_DETAIL_NAMESPACE::property_set_impl& ps,
        _In_ const LYRA_NAMESPACE::collection_flags flags,
        _In_ const std::string& path) {
    using namespace LYRA_NAMESPACE;

    try {
        auto handle = detail::open_read(path.c_str());

        timestamp a, o, w;
        LYRA_DETAIL_NAMESPACE::get_file_time(a, o, w, handle.get());

        detail::checked_add<application::access_time>(ps, flags, a);
        detail::checked_add<application::write_time>(ps, flags, w);
#if defined(_WIN32)
        detail::checked_add<application::create_time>(ps, flags, o);
#else /* defined(_WIN32) */
        detail::checked_add<application::change_time>(ps, flags, o);
#endif /* defined(_WIN32) */

        return true;
    } catch (const std::exception& ex) {
        LYRA_TRACE(_T("Failed to obtain timestamps for %s: %s"), path.c_str(),
            ex.what());
        return false;
    }
}


/*
 * LYRA_NAMESPACE::application::get
 */
LYRA_NAMESPACE::property_set LYRA_NAMESPACE::application::get(
    _In_ const collection_flags flags) {
    detail::property_set_impl ps;

    const auto cmd_line = detail::get_command_line();
    detail::checked_add<command_line>(ps, flags, cmd_line.c_str());
    const auto exe = detail::get_executable_path();
    detail::checked_add<executable>(ps, flags, exe.c_str());

    try {
        const auto exe_hash = detail::file_hash(exe.c_str());
        detail::checked_add<hash>(ps, flags, exe_hash.c_str());
    } catch (const std::exception& ex) {
        LYRA_TRACE(_T("Failed to get executable hash: %s"), ex.what());
    }

    add_timestamps(ps, flags, exe);
    detail::checked_add<version_info>(ps, flags, detail::get_file_version_info(
        exe.c_str()));

    {
        detail::git_environment git(exe);
        if (git) {
            detail::checked_add<environment::git>(ps, flags, git);
        }
    }

    detail::checked_add<process_id>(ps, flags,
        detail::get_process_id());

    {
        const auto affinity = detail::get_process_cpu_affinity();
        std::string value(affinity.size(), '-');
        for (std::size_t i = 0; i < affinity.size(); ++i) {
            if (affinity[i]) {
                value[i] = '*';
            }
        }
        detail::checked_add<cpu_affinity>(ps, flags, value);
    }

    try {
        detail::checked_add<size>(ps, flags, detail::file_size(exe.c_str()));
    } catch (const std::exception& ex) {
        LYRA_TRACE(_T("Failed to get executable size: %s"), ex.what());
    }

    if (detail::check_sensitive<libraries>(flags)) {
        const auto dlls = detail::get_loaded_library_paths();
        std::vector<property_set> pss;
        pss.reserve(dlls.size());

        std::transform(dlls.begin(), dlls.end(), std::back_inserter(pss),
            [flags](const auto& d) {
                detail::property_set_impl ps;
                detail::checked_add<executable>(ps, flags, d.c_str());
                add_timestamps(ps, flags, d);
                detail::checked_add<version_info>(ps, flags,
                    detail::get_file_version_info(d.c_str()));

                try {
                    const auto dll_hash = detail::file_hash(d.c_str());
                    detail::checked_add<hash>(ps, flags, dll_hash.c_str());
                } catch (const std::exception& ex) {
                    LYRA_TRACE(_T("Failed to get library hash: %s"), ex.what());
                }

                try {
                    detail::checked_add<size>(ps, flags, detail::file_size(
                        d.c_str()));
                } catch (const std::exception& ex) {
                    LYRA_TRACE(_T("Failed to get library size: %s"), ex.what());
                }

                return property_set(std::move(ps));
            });

        ps.add<libraries>(std::move(pss));
    }

    return property_set(std::move(ps));
}
