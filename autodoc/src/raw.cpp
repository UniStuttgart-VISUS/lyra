// <copyright file="raw.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "visus/autodoc/raw.h"

#include "visus/autodoc/application.h"
#include "visus/autodoc/cpu.h"
#include "visus/autodoc/environment.h"
#include "visus/autodoc/graphics.h"
#include "visus/autodoc/hardware.h"
#include "visus/autodoc/memory.h"
#include "visus/autodoc/network.h"
#include "visus/autodoc/mounts.h"
#include "visus/autodoc/operating_system.h"
#include "visus/autodoc/smbios.h"
#include "visus/autodoc/timestamp.h"

#include "property_set_impl.h"


/*
 * LYRA_NAMESPACE::raw::get
 */
LYRA_NAMESPACE::property_set LYRA_NAMESPACE::raw::get(
        _In_ const collection_flags flags) {
    detail::property_set_impl ps;

    // First, add the timestamp.
    if (detail::check_flags<timestamp>(flags)) {
        const auto now = LYRA_NAMESPACE::timestamp::now();
        ps.add<timestamp>(now);
    }

    // Add all the actual information.
    if (detail::check_flags<application>(flags)) {
        ps.add<application>(LYRA_NAMESPACE::application::get(flags));
    }

    if (detail::check_flags<cpu>(flags)) {
        ps.add<cpu>(LYRA_NAMESPACE::cpu::get(flags));
    }

    if (detail::check_flags<environment>(flags)) {
        ps.add<environment>(LYRA_NAMESPACE::environment::get(flags));
    }

    if (detail::check_flags<graphics>(flags)) {
        ps.merge(LYRA_NAMESPACE::graphics::get(flags));
    }

    if (detail::check_flags<hardware>(flags)) {
        ps.add<hardware>(LYRA_NAMESPACE::hardware::get(flags));
    }

    if (detail::check_flags<memory>(flags)) {
        ps.add<memory>(LYRA_NAMESPACE::memory::get(flags));
    }

    if (detail::check_flags<mounts>(flags)) {
        ps.add<mounts>(LYRA_NAMESPACE::mounts::get(flags));
    }

    if (detail::check_flags<network>(flags)) {
        ps.add<network>(LYRA_NAMESPACE::network::get(flags));
    }

    if (detail::check_flags<operating_system>(flags)) {
        ps.add<operating_system>(LYRA_NAMESPACE::operating_system::get(
            flags));
    }

    if (detail::check_flags<smbios>(flags)) {
        ps.add<smbios>(LYRA_NAMESPACE::smbios::get(flags));
    }

    return property_set(std::move(ps));
}
