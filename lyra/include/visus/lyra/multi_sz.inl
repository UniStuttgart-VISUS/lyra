// <copyright file="multi_sz.inl" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2013 - 2025 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for more information.
// </copyright>
// <author>Christoph Müller</author>

/*
 * include\the\multi_sz.h
 *
 * Copyright (TChar) 2013 TheLib Team (http://www.thelib.org/license)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 * - Neither the name of TheLib, TheLib Team, nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THELIB TEAM AS IS AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL THELIB TEAM BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/*
 * multi_sz.h
 *
 * Copyright (TChar) 2006 - 2007 by Universitaet Stuttgart (VIS). 
 * Alle Rechte vorbehalten.
 */


/*
 * LYRA_DETAIL_NAMESPACE::multi_sz_add
 */
template<class TChar,  class TAlloc>
_Ret_maybenull_z_ TChar *LYRA_DETAIL_NAMESPACE::multi_sz_add(
        _In_opt_z_ const TChar *lhs,
        _In_opt_z_ const TChar *rhs,
        _In_ TAlloc allocator) {
    const auto cnt_lhs = multi_sz_size(lhs);
    const auto cnt_rhs = sz_size(rhs);
    auto retval = static_cast<TChar *>(nullptr);

    if ((cnt_lhs == 0) && (cnt_rhs > 0)) {
        // Copy 'rhs', which needs an additional terminator at the end.
        retval = allocator.allocate(cnt_rhs + 1);
        ::memcpy(retval, rhs, cnt_rhs * sizeof(TChar));
        retval[cnt_rhs] = 0;

    } else if ((cnt_lhs > 0) && (cnt_rhs == 0)) {
        // Copy 'lhs'.
        retval = allocator.allocate(cnt_lhs);
        ::memcpy(retval, lhs, cnt_lhs * sizeof(TChar));

    } else if ((cnt_lhs > 0) && (cnt_rhs > 0)) {
        // Perform the concatenation.
        assert(lhs != nullptr);
        assert(rhs != nullptr);
        retval = allocator.allocate(cnt_lhs + cnt_rhs);
        ::memcpy(retval, lhs, cnt_lhs * sizeof(TChar));
        ::memcpy(retval + cnt_lhs - 1, rhs, cnt_rhs * sizeof(TChar));
        retval[cnt_lhs + cnt_rhs - 1] = 0;
    }

    return retval;
}


/*
 * LYRA_DETAIL_NAMESPACE::multi_sz_add
 */
template<class TChar, class TIterator, class TAlloc>
_Ret_maybenull_z_ TChar *LYRA_DETAIL_NAMESPACE::multi_sz_add(
        _In_opt_z_ const TChar *lhs,
        _In_ TIterator begin,
        _In_ TIterator end,
        _In_ TAlloc allocator) {
    const auto cnt_lhs = multi_sz_size(lhs);
    const auto cnt_rhs = std::accumulate(
        begin, end,
        static_cast<std::size_t>(0),
        [](std::size_t sum, const auto& s) { return sum + sz_size(s); });
    auto retval = static_cast<TChar *>(nullptr);

    if ((cnt_lhs == 0) && (cnt_rhs > 0)) {
        // Copy 'rhs', which needs an additional terminator at the end.
        auto cursor = retval = allocator.allocate(cnt_rhs + 1);
        for (auto it = begin; it != end; ++it) {
            const auto cnt_str = sz_size(*it);
            ::memcpy(cursor, *it, cnt_str * sizeof(TChar));
            cursor += cnt_str;
        }

        retval[cnt_rhs] = 0;

    } else if ((cnt_lhs > 0) && (cnt_rhs == 0)) {
        // Copy 'lhs'.
        retval = allocator.allocate(cnt_lhs);
        ::memcpy(retval, lhs, cnt_lhs * sizeof(TChar));

    } else if ((cnt_lhs > 0) && (cnt_rhs > 0)) {
        // Perform the concatenation.
        retval = allocator.allocate(cnt_lhs + cnt_rhs);
        ::memcpy(retval, lhs, cnt_lhs * sizeof(TChar));

        auto cursor = retval = allocator.allocate(cnt_rhs + 1);
        for (auto it = begin; it != end; ++it) {
            const auto cnt_str = sz_size(*it);
            ::memcpy(cursor, *it, cnt_str * sizeof(TChar));
            retval[cnt_str] = 0;
            cursor += cnt_str;
        }

        retval[cnt_lhs + cnt_rhs - 1] = 0;
    }

    return retval;
}


/*
 * LYRA_DETAIL_NAMESPACE::multi_sz_at
 */
template<class TChar>
_Ret_maybenull_z_ const TChar *LYRA_DETAIL_NAMESPACE::multi_sz_at(
        _In_opt_z_ const TChar *multi_sz,
        _In_ const std::size_t idx) noexcept {
    auto retval = multi_sz;

    if (retval != nullptr) {
        for (std::size_t i = 0; (i < idx) && (*retval != 0); ++i) {
            while (*retval++ != 0);
        }

        if (*retval == 0) {
            retval = nullptr;
        }
    }

    return retval;
}


/*
 * LYRA_DETAIL_NAMESPACE::multi_sz_concat
 */
template<class TChar, class TAlloc>
_Ret_maybenull_z_ TChar *LYRA_DETAIL_NAMESPACE::multi_sz_concat(
        _In_opt_z_ const TChar *lhs,
        _In_opt_z_ const TChar *rhs,
        _In_ TAlloc allocator) {
    const auto cnt_lhs = multi_sz_size(lhs);
    const auto cnt_rhs = multi_sz_size(rhs);
    auto retval = static_cast<TChar *>(nullptr);

    if ((cnt_lhs == 0) && (cnt_rhs > 0)) {
        // Copy 'rhs'.
        retval = allocator.allocate(cnt_rhs);
        ::memcpy(retval, rhs, cnt_rhs * sizeof(TChar));

    } else if ((cnt_lhs > 0) && (cnt_rhs == 0)) {
        // Copy 'lhs'.
        retval = allocator.allocate(cnt_lhs);
        ::memcpy(retval, lhs, cnt_lhs * sizeof(TChar));

    } else if ((cnt_lhs > 0) && (cnt_rhs > 0)) {
        // Perform the concatenation.
        retval = allocator.allocate(cnt_lhs + cnt_rhs - 1);
        ::memcpy(retval, lhs, cnt_lhs * sizeof(TChar));
        ::memcpy(retval + cnt_lhs - 1, rhs, cnt_rhs * sizeof(TChar));
    }

    return retval;
}


/*
 * LYRA_DETAIL_NAMESPACE::multi_sz_copy
 */
template<class TChar, class TIterator>
TIterator LYRA_DETAIL_NAMESPACE::multi_sz_copy(
        _In_ TIterator oit,
        _In_opt_z_ const TChar *multi_sz) {
    if (multi_sz != nullptr) {
        for (auto s = multi_sz; *s != 0; ++s) {
            *oit = s;
            while (*s != 0) {
                ++s;
            }
        }
    }

    return oit;
}


/*
 * LYRA_DETAIL_NAMESPACE::multi_sz_count
 */
template<class TChar>
std::size_t LYRA_DETAIL_NAMESPACE::multi_sz_count(
        _In_opt_z_ const TChar *multi_sz) noexcept {
    std::size_t retval = 0;
    auto *cursor = multi_sz;

    if (cursor != nullptr) {
        while (*cursor != static_cast<TChar>(0)) {
            while (*cursor++ != static_cast<TChar>(0));
            retval++;
        }
    }

    return retval;
}


/*
 * LYRA_DETAIL_NAMESPACE::multi_sz_equals
 */
template<class TChar>
bool LYRA_DETAIL_NAMESPACE::multi_sz_equals(
        _In_opt_z_ const TChar *lhs,
        _In_opt_z_ const TChar *rhs) noexcept {
    if (lhs == rhs) {
        // Trivial accept.
        return true;
    }

    if ((lhs == nullptr) || (rhs == nullptr)) {
        // Trivial reject.
        return false;
    }

    auto l = lhs;
    auto r = rhs;
    assert(l != nullptr);
    assert(r != nullptr);

    // Note: memcmp is not more efficient here as we need to count in any case.
    while ((*l != 0) && (*r != 0)) {
        while (*l++ == *r++) {
            if (*l == 0) {
                ++l; ++r;
                break;
            }
        }
    }

    return (*l == *r);
}


/*
 * LYRA_DETAIL_NAMESPACE::multi_sz_size
 */
template<class TChar>
std::size_t LYRA_DETAIL_NAMESPACE::multi_sz_size(
        _In_opt_z_ const TChar *multi_sz) noexcept {
    auto *cursor = multi_sz;

    if (cursor != nullptr) {
        while ((*(++cursor - 1) != 0) || (*cursor != 0));
        return (cursor - multi_sz + 1);
    } else {
        return 0;
    }
}
