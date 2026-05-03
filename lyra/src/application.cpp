// <copyright file="application.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "visus/lyra/application.h"

#include <algorithm>

#include "visus/lyra/trace.h"

#include "file.h"
#include "property_set_impl.h"
#include "processes.h"


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

    detail::checked_add<process_id>(ps, flags, detail::get_process_id());

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
