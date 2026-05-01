// <copyright file="string_manipulation.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "string_manipulation.h"

#include <cinttypes>
#include <iomanip>
#include <sstream>


/*
 * LYRA_DETAIL_NAMESPACE::to_hex_string
 */
std::string LYRA_DETAIL_NAMESPACE::to_hex_string(
        _In_reads_bytes_(cnt) const void *data,
        _In_ std::size_t cnt) {
    if (data == nullptr) {
        return std::string();
    }

    std::stringstream ss;
    ss << std::hex;

    auto cur = static_cast<const std::uint8_t *>(data);

    for (std::size_t i = 0; i < cnt; ++i, ++cur) {
        ss << std::setw(2) << std::setfill('0') << static_cast<int>(*cur);
    }

    return ss.str();
}
