// <copyright file="convert_string.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "visus/lyra/convert_string.h"

#include <array>
#include <cassert>
#include <limits>

#include "utf8_converter.h"


/*
 * LYRA_NAMESPACE::to_utf8
 */
std::size_t LYRA_NAMESPACE::to_utf8(
        _Out_writes_to_opt_(cnt_dst, return) char *dst,
        _In_ std::size_t cnt_dst,
        _In_reads_or_z_(cnt_src) const unicode_type *src,
        _In_ int cnt_src) {
    thread_local detail::utf8_converter conv;
    if (!conv) {
        return 0;
    }

    if (dst == nullptr) {
        // Make sure that the output buffer size is zero if the output buffer is
        // invalid, such that we get the required buffer size.
        cnt_dst = 0;
    }
    if (src == nullptr) {
        // Likewise, if the source is invalid, bail out directly.
        return 0;
    }

    if ((cnt_src < 0) && (src != nullptr)) {
        // User requested us to measure the input, do so now.
        cnt_src = 0;
        for (auto s = src; *s != 0; ++s) {
            ++cnt_src;
        }
    }
    assert(src != nullptr);
    assert(cnt_src > 0);

    // Perform a stream conversion, possibly into an intermediate buffer just
    // for measuring.
    auto cur_dst = dst;
    auto dst_end = dst + cnt_dst;
    auto cur_src = src;
    auto src_end = cur_src + cnt_src;

    auto status = U_ZERO_ERROR;
    ::ucnv_fromUnicode(conv,
        &cur_dst, dst_end,
        &cur_src, src_end,
        nullptr, true, &status);

    auto retval = (cur_dst - dst);

    if (status == U_BUFFER_OVERFLOW_ERROR) {
        // If the output buffer was too small, create an intermediate one and
        // measure how much we need. ICU typically uses 1024 for this case, so
        // we do so, too.
        std::array<char, 1024> buffer;
        dst_end = buffer.data() + buffer.size();

        do {
            cur_dst = buffer.data();
            status = U_ZERO_ERROR;
            ::ucnv_fromUnicode(conv,
                &cur_dst, dst_end,
                &cur_src, src_end,
                nullptr, true, &status);
            retval += cur_dst - buffer.data();
        } while (status == U_BUFFER_OVERFLOW_ERROR);
    }

    // If we are still in an error state here, something other than too little
    // output storage must have gone wrong.
    if (U_FAILURE(status)) {
        return 0;
    }

    return retval;
}


/*
 * LYRA_NAMESPACE::to_utf8
 */
std::size_t LYRA_NAMESPACE::to_utf8(
        _Out_writes_to_opt_(cnt_dst, return) char *dst,
        _In_ std::size_t cnt_dst,
        _In_reads_or_z_(cnt_src) const char32_t *src,
        _In_ const int cnt_src) {
    if (dst == nullptr) {
        // Make sure that the output buffer size is zero if the output
        // buffer is invalid, such that we get the required buffer size.
        cnt_dst = 0;
    }
    if (src == nullptr) {
        // Likewise, if the source is invalid, bail out directly.
        return 0;
    }

    assert(src != nullptr);
    assert(cnt_dst <= (std::numeric_limits<std::int32_t>::max)());
    assert(cnt_src <= (std::numeric_limits<std::int32_t>::max)());
    auto status = U_ZERO_ERROR;

    auto retval = ::ucnv_convert("UTF-8", "UTF-32",
        reinterpret_cast<char *>(dst), static_cast<std::int32_t>(cnt_dst),
        reinterpret_cast<const char *>(src), static_cast<int32_t>(cnt_src),
        &status);

    return ((status != U_BUFFER_OVERFLOW_ERROR) && (cnt_dst > 0)) ? 0 : retval;
}
