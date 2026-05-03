// <copyright file="environment.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_ENVIRONMENT_H)
#define _LYRA_ENVIRONMENT_H
#pragma once

#include "visus/lyra/collection_flags.h"
#include "visus/lyra/property_set.h"


LYRA_NAMESPACE_BEGIN

namespace environment {

    /// <summary>
    /// Identifies the current working directory of the process.
    /// </summary>
    struct current_directory final {
        typedef const char *type;
        static constexpr auto is_variable = true;
        static constexpr auto name = u8"Current Directory";
    };

    /// <summary>
    /// Identifies a property set containing all environment variables.
    /// </summary>
    struct variables final {
        typedef property_set type;
        static constexpr auto is_sensitive = true;
        static constexpr auto is_variable = true;
        static constexpr auto name = u8"Variables";
    };

    /// <summary>
    /// Gets all environment properties.
    /// </summary>
    /// <param name="flags">Allows for customising the collection
    /// behaviour.</param>
    /// <returns>The environment properties of the calling process.</returns>
    LYRA_API property_set get(_In_ const collection_flags flags
        = collection_flags::none);

    /// <summary>
    /// Gets all environment variables for the calling process.
    /// </summary>
    /// <param name="flags">Allows for customising the collection
    /// behaviour. If <see cref="collection_flags::no_sensitive"/> is set, the
    /// result will be empty as any environment variable might potentially be
    /// sensitive, for instance if used for credential.</param>
    /// <returns>A property set holding the environment variables.</returns>
    LYRA_API property_set get_variables(_In_ const collection_flags flags
        = collection_flags::none);

} /* namespace environment */

LYRA_NAMESPACE_END

#endif /* !defined(_LYRA_ENVIRONMENT_H) */
