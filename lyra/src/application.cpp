// <copyright file="application.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "visus/lyra/application.h"

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
    detail::checked_add<process_id>(ps, flags, detail::get_process_id());

    return detail::to_property_set(std::move(ps));
}
