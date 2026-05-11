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


/*
 * LYRA_NAMESPACE::consolidated::get
 */
LYRA_NAMESPACE::property_set LYRA_NAMESPACE::consolidated::get(
        _In_ const collection_flags flags) {
    detail::property_set_impl ps;

    // First, add the timestamp.
    if (detail::check_sensitive<timestamp>(flags)) {
        const auto now = LYRA_NAMESPACE::timestamp::now();
        ps.add<timestamp>(now);
    }

    throw "TODO";

    return property_set(std::move(ps));
}
