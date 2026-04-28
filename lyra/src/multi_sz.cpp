// <copyright file="multi_sz.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
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

#include "visus/lyra/multi_sz.h"


/*
 * LYRA_NAMESPACE::multi_sz::iterator::operator =
 */
typename LYRA_NAMESPACE::multi_sz::iterator&
LYRA_NAMESPACE::multi_sz::iterator::operator =(_In_ const iterator& rhs) {
    if (this != std::addressof(rhs)) {
        this->_current = rhs._current;
    }

    return *this;
}


/*
 * LYRA_NAMESPACE::multi_sz::iterator::operator ++
 */
typename LYRA_NAMESPACE::multi_sz::iterator&
LYRA_NAMESPACE::multi_sz::iterator::operator ++(void) {
    if (this->_current != nullptr) {
        assert(*this->_current != 0);
        while (*this->_current++ != 0);

        // Note: The loop above always increments the pointer, i.e. if we
        // detect a zero here, it is the second in a row. Hence, we set
        // 'current' to nullptr to signal that the end was reached.
        // Further note that we cannot use the second zero as end marker,
        // because an empty multi-sz is represented by nullptr and we must
        // recognise its begin as its end.
        if (*this->_current == 0) {
            this->_current = nullptr;
        }
    }

    return *this;
}


/*
 * LYRA_NAMESPACE::multi_sz::multi_sz
 */
LYRA_NAMESPACE::multi_sz::multi_sz(
        _In_opt_z_ const value_type *value)
        : _value(nullptr) {
    if ((value != nullptr) && (*value != 0)) {
        const auto cnt = detail::multi_sz_size(value);
        assert(cnt > 2);
        this->_value = new value_type[cnt];
        ::memcpy(this->_value, value, cnt * sizeof(value_type));
    }
}


/*
 * LYRA_NAMESPACE::multi_sz::multi_sz
 */
LYRA_NAMESPACE::multi_sz::multi_sz(
        _In_reads_(cnt) const value_type **strings,
        _In_ const std::size_t cnt)
    : _value(detail::multi_sz_add<char>(nullptr, strings, strings + cnt)) { }


/*
 * LYRA_NAMESPACE::multi_sz::add
 */
LYRA_NAMESPACE::multi_sz& LYRA_NAMESPACE::multi_sz::add(
        _In_opt_z_ const value_type *str) {
    auto value = detail::multi_sz_add(this->_value, str);
    this->clear();
    this->_value = value;
    return *this;
}


/*
 * LYRA_NAMESPACE::multi_sz::clear
 */
void LYRA_NAMESPACE::multi_sz::clear(void) noexcept {
    static std::allocator<char> allocator;
    allocator.deallocate(this->_value, detail::multi_sz_size(this->_value));
    this->_value = nullptr;
}

///*
// * LYRA_NAMESPACE::multi_sz::insert
// */
//template<class TChar, class TTraits> 
//LYRA_NAMESPACE::multi_sz&
//LYRA_NAMESPACE::multi_sz::insert(
//        _In_ const size_type idx,
//        _In_opt_z_ const value_type *str) {
//    const auto cnt_cur = multi_sz::length(this->_value);
//
//    if (idx >= multi_sz::count(this->_value)) {
//        this->add(str);
//
//    } else if ((str != nullptr) && (*str != 0)) {
//        const auto cnt_str = traits_type::length(str) + 1;
//        const auto offset = this->at(idx) - this->_value;
//        assert(cnt_cur > 0);
//        assert(cnt_str > 0);
//
//        auto value = new value_type[cnt_cur + cnt_str];
//        ::memcpy(value,
//            this->_value,
//            offset * sizeof(value_type));
//        ::memcpy(value + offset,
//            str,
//            cnt_str * sizeof(value_type));
//        ::memcpy(value + offset + cnt_str,
//            this->_value + offset,
//            (cnt_cur - offset) * sizeof(value_type));
//        assert(value[cnt_cur + cnt_str - 1] == 0);
//        assert(value[cnt_cur + cnt_str - 2] == 0);
//
//        this->clear();
//        this->_value = value;
//    }
//
//    return *this;
//}
//
//
///*
// * LYRA_NAMESPACE::multi_sz::remove
// */
//template<class TChar, class TTraits>
//LYRA_NAMESPACE::multi_sz&
//LYRA_NAMESPACE::multi_sz::remove(
//        _In_opt_z_ const value_type *str) {
//    if ((str != nullptr) && (*str != 0)) {
//        this->remove_if([str](const value_type *s) {
//            assert(s != nullptr);
//            auto l = str;
//            auto r = s;
//
//            while ((*l != 0) && (*l == *r)) {
//                ++l;
//                ++r;
//            }
//
//            return (*l == *r);
//        });
//    }
//
//    return *this;
//}
//
//
///*
// * LYRA_NAMESPACE::multi_sz::remove_if
// */
//template<class TChar, class TTraits>
//template<class TPredicate>
//LYRA_NAMESPACE::multi_sz&
//LYRA_NAMESPACE::multi_sz::remove_if(
//        _In_ const TPredicate predicate) {
//    if (this->_value != nullptr) {
//        auto cnt_rem = static_cast<std::size_t>(0);
//        std::vector<const value_type *> rems;
//
//        // Find the matches to be removed.
//        {
//            auto cursor = this->_value;
//
//            while (*cursor != 0) {
//                if (predicate(cursor)) {
//                    // Remember the match and consume it.
//                    rems.push_back(cursor);
//
//                    while (*cursor++ != 0) {
//                        ++cnt_rem;
//                    }
//
//                    ++cnt_rem;  // Terminator must be removed as well.
//
//                } else {
//                    // Consume the non-match.
//                    while (*cursor++ != 0);
//                }
//            }
//        }
//
//        // Reallocate and copy the non-removed elements.
//        if (cnt_rem > 0) {
//            const auto cnt_cur = multi_sz::size(this->_value);
//
//            if (cnt_rem + 1 < cnt_cur) {
//                const auto cnt_new = cnt_cur - cnt_rem;
//                auto rem = rems.begin();
//                auto src = this->_value;
//                auto dst = new value_type[cnt_new];
//                auto value = dst;
//
//                while (*src != 0) {
//                    if ((rem == rems.end()) || (src != *rem)) {
//                        while ((*dst++ = *src++) != 0);
//                    } else {
//                        while (*src++ != 0);
//                        ++rem;
//                    }
//                }
//
//                *dst = 0;   // Add double terminator at end.
//
//                delete[] this->_value;
//                this->_value = value;
//                assert(this->_value[cnt_new - 1] == 0);
//                assert(this->_value[cnt_new - 2] == 0);
//
//            } else {
//                // Everything matched, so clear the whole data.
//                this->clear();
//            }
//        }
//
//    } /* if (this->_value != nullptr) */
//
//    return *this;
//}


/*
 * LYRA_NAMESPACE::multi_sz::operator =
 */
LYRA_NAMESPACE::multi_sz& LYRA_NAMESPACE::multi_sz::operator =(
        _In_ const multi_sz& rhs) {
    if (this != std::addressof(rhs)) {
        this->clear();
        assert(this->_value == nullptr);
        this->_value = detail::multi_sz_concat(this->_value, rhs._value);
    }

    return *this;
}


/*
 * LYRA_NAMESPACE::multi_sz::operator =
 */
LYRA_NAMESPACE::multi_sz& LYRA_NAMESPACE::multi_sz::operator =(
        _Inout_ multi_sz&& rhs) noexcept {
    if (this != std::addressof(rhs)) {
        this->_value = rhs._value;
        rhs._value = nullptr;
    }

    return *this;
}
