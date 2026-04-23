// <copyright file="operating_system.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_OPERATING_SYSTEM_H)
#define _LYRA_OPERATING_SYSTEM_H
#pragma once

#include "visus/lyra/property_set.h"


LYRA_NAMESPACE_BEGIN

namespace operating_system {

    struct family final {
        typedef std::string type;
        static constexpr auto name = u8"family";
    };

    struct version final {
        typedef property_set type;
        static constexpr auto name = u8"version";
    };

} /* namespace operating_system */

LYRA_NAMESPACE_END

#endif /* !defined(_LYRA_OPERATING_SYSTEM_H) */
