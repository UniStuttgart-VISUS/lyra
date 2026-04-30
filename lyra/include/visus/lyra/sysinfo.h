// <copyright file="sysinfo.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_SYSINFO_H)
#define _LYRA_SYSINFO_H
#pragma once

#include "visus/lyra/collection_flags.h"
#include "visus/lyra/property_set.h"


LYRA_NAMESPACE_BEGIN

namespace sysinfo {

    /// <summary>
    /// Retrieves all property sets we can get in their raw form, i.e. without
    /// removing duplicate information that can be obtained in multiple ways.
    /// </summary>
    /// <param name="flags"></param>
    /// <returns></returns>
    property_set LYRA_API raw(_In_ const collection_flags flags
        = collection_flags::none);

} /* namespace sysinfo */

LYRA_NAMESPACE_END

#endif /* !defined(_LYRA_SYSINFO_H) */
