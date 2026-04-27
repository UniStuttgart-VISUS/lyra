// <copyright file="version.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "visus/lyra/version.h"

#include "property_set_impl.h"


/*
 * LYRA_NAMESPACE::version::make
 */
LYRA_NAMESPACE::property_set LYRA_NAMESPACE::version::make(
        _In_ const std::uint32_t major,
        _In_ const std::uint32_t minor,
        _In_ const std::uint32_t patch,
        _In_opt_z_ const char *prerelease) {
    detail::property_set_impl ps;
    property_set retval;

    ps.add<LYRA_NAMESPACE::version::major>(major);
    ps.add<LYRA_NAMESPACE::version::minor>(minor);
    ps.add<LYRA_NAMESPACE::version::patch>(patch);
    ps.add(LYRA_NAMESPACE::version::prerelease::name, prerelease);

    realise(retval, std::move(ps));
    return retval;
}


/*
 * LYRA_NAMESPACE::version::make
 */
LYRA_NAMESPACE::property_set LYRA_NAMESPACE::version::make(
        _In_ const std::uint32_t major,
        _In_ const std::uint32_t minor) {
    detail::property_set_impl ps;
    property_set retval;

    ps.add<LYRA_NAMESPACE::version::major>(major);
    ps.add<LYRA_NAMESPACE::version::minor>(minor);

    realise(retval, std::move(ps));
    return retval;
}
