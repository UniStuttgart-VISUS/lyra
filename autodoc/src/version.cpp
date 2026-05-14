// <copyright file="version.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "visus/autodoc/version.h"

#include <regex>

#include "equals.h"
#include "property_set_impl.h"


/*
 * LYRA_NAMESPACE::version::make
 */
LYRA_NAMESPACE::property_set LYRA_NAMESPACE::version::make(
        _In_ const std::uint32_t major,
        _In_ const std::uint32_t minor,
        _In_ const std::uint32_t release,
        _In_ const std::uint32_t build,
        _In_opt_z_ const char *prerelease) {
    detail::property_set_impl ps;

    ps.add<LYRA_NAMESPACE::version::major>(major);
    ps.add<LYRA_NAMESPACE::version::minor>(minor);
    ps.add<LYRA_NAMESPACE::version::release>(release);
    ps.add<LYRA_NAMESPACE::version::build>(build);
    ps.add(LYRA_NAMESPACE::version::prerelease::name, prerelease);

    return property_set(std::move(ps));
}


/*
 * LYRA_NAMESPACE::version::make
 */
LYRA_NAMESPACE::property_set LYRA_NAMESPACE::version::make(
        _In_ const std::uint32_t major,
        _In_ const std::uint32_t minor) {
    detail::property_set_impl ps;

    ps.add<LYRA_NAMESPACE::version::major>(major);
    ps.add<LYRA_NAMESPACE::version::minor>(minor);

    return property_set(std::move(ps));
}


/*
 * LYRA_NAMESPACE::version::make
 */
LYRA_NAMESPACE::property_set LYRA_NAMESPACE::version::make(
        _In_ const std::uint32_t major,
        _In_ const std::uint32_t minor,
        _In_z_ const char *date,
        _In_z_ const char *time) {
    const std::regex rx_date("([^\\s]+)\\s*(\\d+)\\s*(\\d+)");
    const std::regex rx_time("(\\d+):(\\d+):(\\d+)");

    std::cmatch match;
    detail::property_set_impl ps;

    ps.add<LYRA_NAMESPACE::version::major>(major);
    ps.add<LYRA_NAMESPACE::version::minor>(minor);

    if ((date != nullptr) && std::regex_match(date, match, rx_date)) {
        const auto y = std::stoi(match[3]);
        const auto d = std::stoi(match[2]);
        const auto m = [&match](const std::string& s) {
            if (detail::iequals(s, u8"jan")) return 1;
            if (detail::iequals(s, u8"feb")) return 2;
            if (detail::iequals(s, u8"mar")) return 3;
            if (detail::iequals(s, u8"apr")) return 4;
            if (detail::iequals(s, u8"may")) return 5;
            if (detail::iequals(s, u8"jun")) return 6;
            if (detail::iequals(s, u8"jul")) return 7;
            if (detail::iequals(s, u8"aug")) return 8;
            if (detail::iequals(s, u8"sep")) return 9;
            if (detail::iequals(s, u8"oct")) return 10;
            if (detail::iequals(s, u8"nov")) return 11;
            if (detail::iequals(s, u8"dec")) return 12;
            return 0;
        }(match[1]);
        ps.add<LYRA_NAMESPACE::version::release>(y * 10000 + m * 100 + d);
    }

    if ((time != nullptr) && std::regex_match(time, match, rx_time)) {
        const auto h = std::stoi(match[1]);
        const auto m = std::stoi(match[2]);
        const auto s = std::stoi(match[3]);
        ps.add<LYRA_NAMESPACE::version::build>(h * 10000 + m * 100 + s);
    }

    return property_set(std::move(ps));
}
