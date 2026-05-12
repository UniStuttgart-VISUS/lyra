// <copyright file="consolidated.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "visus/autodoc/consolidated.h"

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
#include "setup_api.h"


/*
 * LYRA_NAMESPACE::consolidated::get
 */
LYRA_NAMESPACE::property_set LYRA_NAMESPACE::consolidated::get(
        _In_ const collection_flags flags) {
    detail::property_set_impl ps;

    // First, add the timestamp.
    if (detail::check_flags<timestamp>(flags)) {
        const auto now = LYRA_NAMESPACE::timestamp::now();
        ps.add<timestamp>(now);
    }

    // Get all the actual content that can be used without modifications.
    if (detail::check_flags<application>(flags)) {
        ps.add<application>(LYRA_NAMESPACE::application::get(flags));
    }

    if (detail::check_flags<environment>(flags)) {
        ps.add<environment>(LYRA_NAMESPACE::environment::get(flags));
    }

    if (detail::check_flags<operating_system>(flags)) {
        ps.add<operating_system>(LYRA_NAMESPACE::operating_system::get(flags));
    }

    // Get all of the raw data and consolidate it into the final output.
    detail::property_set_impl cpu(LYRA_NAMESPACE::cpu::get(flags));
    detail::property_set_impl dev(LYRA_NAMESPACE::hardware::get(flags));
    detail::property_set_impl mem(LYRA_NAMESPACE::memory::get(flags));
    detail::property_set_impl mounts(LYRA_NAMESPACE::mounts::get(flags));
    detail::property_set_impl net(LYRA_NAMESPACE::network::get(flags));
    detail::property_set_impl smbios(LYRA_NAMESPACE::smbios::get(flags));

    // Merge the network shares into the network block.
    {
        constexpr auto dst = share::name;
        constexpr auto src = LYRA_NAMESPACE::mounts::network::name;
        net.values[dst] = std::move(mounts.values[src]);
        detail::checked_add<network>(ps, flags, property_set(std::move(net)));
    }

    // Merge the hardware into a single block.
    {
        detail::property_set_impl p;


#if defined(_WIN32)
        {
            guid g(GUID_DEVCLASS_PROCESSOR);
            auto s = dev.values.find(g.to_string<char>());
            if (s != dev.values.end()) {
                // The processor class contains a CPU entry for each core, so
                // we need to merge this with the data from CPUID.
            }
        }
#endif /* defined(_WIN32) */
    }

    return property_set(std::move(ps));
}
