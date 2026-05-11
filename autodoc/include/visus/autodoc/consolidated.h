// <copyright file="consolidated.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_CONSOLIDATED_H)
#define _LYRA_CONSOLIDATED_H
#pragma once

#include "visus/autodoc/raw.h"


LYRA_NAMESPACE_BEGIN

namespace consolidated {

    /// <summary>
    /// Identifies a property holding the timestamp when the information was
    /// collected.
    /// </summary>
    typedef raw::timestamp timestamp;

    /// <summary>
    /// Gets the consolidated system information, which is the same as the raw
    /// data after removing duplicates. This function tries to find the most
    /// data about the system it can, but removes the noise from the raw data.
    /// </summary>
    /// <param name="flags">Allows for customising the collection
    /// behaviour.</param>
    LYRA_API property_set get(_In_ const collection_flags flags
        = collection_flags::none);

} /* namespace consolidated */

LYRA_NAMESPACE_END

#endif /* !defined(_LYRA_CONSOLIDATED_H) */
