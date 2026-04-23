// <copyright file="setup_api.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2016 - 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "setup_api.h"


/*
 * LYRA_DETAIL_NAMESPACE::split_driver_version
 */
std::array<std::uint16_t, 4> LYRA_DETAIL_NAMESPACE::split_driver_version(
        _In_ decltype(SP_DRVINFO_DATA_W::DriverVersion) version) {
    auto revision = static_cast<std::uint16_t>(version & 0xFFFF);
    version >>= 16;
    auto patch = static_cast<std::uint16_t>(version & 0xFFFF);
    version >>= 16;
    auto minor = static_cast<std::uint16_t>(version & 0xFFFF);
    version >>= 16;
    auto major = static_cast<std::uint16_t>(version & 0xFFFF);
    version >>= 16;
    return { major, minor, patch, revision };
}
