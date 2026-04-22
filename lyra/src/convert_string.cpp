// <copyright file="convert_string.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "visus/lyra/convert_string.h"

#include <array>
#include <cassert>
#include <limits>

#include "visus/lyra/endian.h"

#include "icu_converter.h"
#include "results.h"


/*
 * LYRA_NAMESPACE::to_utf8
 */
LYRA_NAMESPACE::result_type LYRA_NAMESPACE::to_utf8(
        _Out_writes_to_opt_(cnt_dst, return) char *dst,
        _Inout_ std::size_t& cnt_dst,
        _In_reads_or_z_(cnt_src) const unicode_type *src,
        _In_ int cnt_src) {
    thread_local detail::icu_converter conv("UTF-8");
    if (!conv) {
        return detail::results::invalid_state;
    }

    if (dst == nullptr) {
        // Make sure that the output buffer size is zero if the output buffer is
        // invalid, such that we get the required buffer size.
        cnt_dst = 0;
    }
    if (src == nullptr) {
        // Likewise, if the source is invalid, bail out directly.
        return detail::results::invalid_argument;
    }

    if ((cnt_src < 0) && (src != nullptr)) {
        // User requested us to measure the input, do so now.
        cnt_src = 1;
        for (auto s = src; *s != 0; ++s, ++cnt_src);
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

    auto required = (cur_dst - dst);

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
            required += cur_dst - buffer.data();
        } while (status == U_BUFFER_OVERFLOW_ERROR);
    }

    // If we are still in an error state here, something other than too little
    // output storage must have gone wrong.
    if (U_FAILURE(status)) {
        return detail::results::catastrophic_failure;
    }

    // If the output buffer was too small, report the required size.
    if (required > cnt_dst) {
        cnt_dst = required;
        return detail::results::insufficient_buffer;
    }

    return detail::results::success;
}


/*
 * LYRA_NAMESPACE::to_utf8
 */
LYRA_NAMESPACE::result_type LYRA_NAMESPACE::to_utf8(
        _Out_writes_to_opt_(cnt_dst, return) char *dst,
        _Inout_ std::size_t& cnt_dst,
        _In_reads_or_z_(cnt_src) const char32_t *src,
        _In_ int cnt_src) {
    if (dst == nullptr) {
        // Make sure that the output buffer size is zero if the output
        // buffer is invalid, such that we get the required buffer size.
        cnt_dst = 0;
    }
    if (src == nullptr) {
        // Likewise, if the source is invalid, bail out directly.
        return detail::results::invalid_argument;
    }

    if ((cnt_src < 0) && (src != nullptr)) {
        // User requested us to measure the input, do so now.
        cnt_src = 1;
        for (auto s = src; *s != 0; ++s, ++cnt_src);
        cnt_src *= sizeof(char32_t);
    }
    assert(src != nullptr);
    assert(cnt_src > 0);
    assert(cnt_dst <= (std::numeric_limits<std::int32_t>::max)());
    assert(cnt_src <= (std::numeric_limits<std::int32_t>::max)());

    auto status = U_ZERO_ERROR;
    cnt_dst = ::ucnv_convert(
        "UTF-8",
        (endian::system == endian::big) ? "UTF-32-BE" : "UTF-32-LE",
        reinterpret_cast<char *>(dst),
        static_cast<std::int32_t>(cnt_dst),
        reinterpret_cast<const char *>(src),
        static_cast<std::int32_t>(cnt_src),
        &status);

    switch (status) {
        case U_ZERO_ERROR:
            return detail::results::success;

        case U_BUFFER_OVERFLOW_ERROR:
            return detail::results::insufficient_buffer;

        default:
            return U_FAILURE(status)
                ? detail::results::catastrophic_failure
                : detail::results::success;
    }
}
