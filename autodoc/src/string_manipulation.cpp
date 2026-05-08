// <copyright file="string_manipulation.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "string_manipulation.h"

#include <cinttypes>


/*
 * LYRA_DETAIL_NAMESPACE::to_hex_string
 */
std::string LYRA_DETAIL_NAMESPACE::to_hex_string(
        _In_reads_bytes_(cnt) const void *data,
        _In_ std::size_t cnt) {
    constexpr const char *lut = "0123456789abcdef";
    if (data == nullptr) {
        return std::string();
    }

    std::string retval(cnt * 2, 0);

    auto cur = static_cast<const std::uint8_t *>(data);
    for (std::size_t i = 0; i < cnt; ++i, ++cur) {
        retval[2 * i + 0] = lut[(*cur >> 4)];
        retval[2 * i + 1] = lut[*cur & 0xF];
    }

    return retval;
}
