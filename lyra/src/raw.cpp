// <copyright file="raw.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "visus/lyra/raw.h"

#include "visus/lyra/cpu.h"
#include "visus/lyra/environment.h"
#include "visus/lyra/graphics.h"
#include "visus/lyra/operating_system.h"
#include "visus/lyra/smbios.h"
#include "visus/lyra/timestamp.h"

#include "property_set_impl.h"
#include "is_sensitive.h"


/*
 * LYRA_NAMESPACE::raw::get
 */
LYRA_NAMESPACE::property_set LYRA_NAMESPACE::raw::get(
        _In_ const collection_flags flags) {
    detail::property_set_impl ps;

    // First, add the timestamp.
    if (detail::check_sensitive<timestamp>(flags)) {
        const auto now = LYRA_NAMESPACE::timestamp::now();
        ps.add<timestamp>(now.value());
    }

    // Add all the actual information.
    if (detail::check_sensitive<cpu::cpuid>(flags)) {
        ps.add<cpu::cpuid>(cpu::get_cpuid(flags));
    }

    if (detail::check_sensitive<environment>(flags)) {
        ps.add<environment>(LYRA_NAMESPACE::environment::get(flags));
    }

    if (detail::check_sensitive<operating_system>(flags)) {
        ps.add<operating_system>(LYRA_NAMESPACE::operating_system::get(
            flags));
    }

    if (detail::check_sensitive<graphics::dxgi_adapter>(flags)) {
        detail::merge(ps, graphics::get_dxgi_adapters(flags));
    }

    if (detail::check_sensitive<smbios>(flags)) {
        ps.add<smbios>(LYRA_NAMESPACE::smbios::get(flags));
    }

    return detail::to_property_set(std::move(ps));
}
