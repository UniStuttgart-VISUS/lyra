// <copyright file="environment.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_ENVIRONMENT_H)
#define _LYRA_ENVIRONMENT_H
#pragma once

#include "visus/lyra/property_set.h"


LYRA_NAMESPACE_BEGIN

namespace environment {

    /// <summary>
    /// Identifies the current working directory of the process.
    /// </summary>
    struct current_directory final {
        typedef const char *type;
        static constexpr auto name = u8"CurrentDirectory";
    };

    /// <summary>
    /// Identifies a property set containing all environment variables.
    /// </summary>
    struct variables final {
        typedef property_set type;
        static constexpr auto name = u8"Variables";
    };

    /// <summary>
    /// Gets all environment properties.
    /// </summary>
    /// <returns>The environment properties of the calling process.</returns>
    LYRA_API property_set get(void);

    /// <summary>
    /// Gets all environment variables for the calling process.
    /// </summary>
    /// <returns>A property set holding the environment variables.</returns>
    LYRA_API property_set get_variables(void);

} /* namespace environment */

LYRA_NAMESPACE_END

#endif /* !defined(_LYRA_ENVIRONMENT_H) */
