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
    if (detail::check_sensitive<timestamp>(flags)) {
        const auto now = LYRA_NAMESPACE::timestamp::now();
        ps.add<timestamp>(now.value());
    }

    // Add all the actual information.
    if (detail::check_sensitive<application>(flags)) {
        ps.add<application>(LYRA_NAMESPACE::application::get(flags));
    }

    if (detail::check_sensitive<cpu::cpuid>(flags)) {
        ps.add<cpu::cpuid>(cpu::get_cpuid(flags));
    }

    if (detail::check_sensitive<environment>(flags)) {
        ps.add<environment>(LYRA_NAMESPACE::environment::get(flags));
    }

    if (detail::check_sensitive<hardware>(flags)) {
        ps.add<hardware>(LYRA_NAMESPACE::hardware::get(flags));
    }

    if (detail::check_sensitive<memory>(flags)) {
        ps.add<memory>(LYRA_NAMESPACE::memory::get(flags));
    }

    if (detail::check_sensitive<operating_system>(flags)) {
        ps.add<operating_system>(LYRA_NAMESPACE::operating_system::get(
            flags));
    }

    if (detail::check_sensitive<graphics::dxgi_adapter>(flags)) {
        ps.merge(graphics::get_dxgi_adapters(flags));
    }

    if (detail::check_sensitive<smbios>(flags)) {
        ps.add<smbios>(LYRA_NAMESPACE::smbios::get(flags));
    }

    return property_set(std::move(ps));
}
