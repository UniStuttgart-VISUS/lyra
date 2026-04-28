// <copyright file="multi_sz.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2013 - 2026 Visualisierungsinstitut der Universität Stuttgart.
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

#if !defined(_LYRA_MULTI_SZ_H)
#define _LYRA_MULTI_SZ_H
#pragma once

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <iterator>
#include <memory>
#include <numeric>
#include <string>
#include <vector>

#include "visus/lyra/api.h"


LYRA_DETAIL_NAMESPACE_BEGIN

/// <summary>
/// Allocate a new multi-sz concatenating <paramref name="rhs" /> at the end
/// of the <paramref name="lhs" />.
/// </summary>
/// <remarks>
/// <para>This function returns memory that the caller must deallocate using the
/// appropriate allocator. Avoid using it whenever possible.</para>
/// </remarks>
/// <typeparam name="TChar">The character type of the strings in the multi-sz.
/// </typeparam>
/// <typeparam name="TAlloc">The allocator type to be used for the returned
/// buffer. The default is <see cref="std::allocator" />.</typeparam>
/// <param name="lhs">The multi-sz to append to. It is safe to pass
/// <see langword="nullptr" />. A copy of <paramref name="rhs" /> as multi-sz,
/// i.e. with two zeros at the end, will be returned in this case, provided it
/// is non-<see langword="nullptr" />. The caller remains owner of the memory.
/// </param>
/// <param name="rhs">The string to be appended. This must not be a multi-sz!
/// If it is, only the first element will be appended. It is safe to pass
/// <see langword="nullptr" />. A copy of <paramref name="lhs" /> will be
/// returned in this case, provided it is non-<see langword="nullptr" />. The
/// caller remains owner of the memory.</param>
/// <param name="allocator">The allocator to be used for the returned buffer.
/// </param>
/// <returns>A new multi-sz holding all concatenated values. This buffer must
/// be feed using <paramref name="allocator" />.</returns>
template<class TChar, class TAlloc = std::allocator<TChar>>
_Ret_maybenull_z_ TChar *multi_sz_add(
    _In_opt_z_ const TChar *lhs,
    _In_opt_z_ const TChar *rhs,
    _In_ TAlloc allocator = TAlloc());

/// <summary>
/// Allocate a new multi-sz concatenating the existing <paramref name="lhs" />
/// with all of the strings in the range <paramref name="begin" /> to
/// <paramref name="end" />.
/// </summary>
/// <remarks>
/// <para>This function returns memory that the caller must deallocate using the
/// appropriate allocator. Avoid using it whenever possible.</para>
/// <para>This function allocates the full range of the result at once, i.e. it
/// is more efficient than concatenating strings one after another.</para>
/// </remarks>
/// <typeparam name="TChar">The character type of the strings in the multi-sz.
/// </typeparam>
/// <typeparam name="TAlloc">The allocator type to be used for the returned
/// buffer. The default is <see cref="std::allocator" />.</typeparam>
template<class TChar, class TIterator, class TAlloc = std::allocator<TChar>>
_Ret_maybenull_z_ TChar *multi_sz_add(
        _In_opt_z_ const TChar *lhs,
        _In_ TIterator begin,
        _In_ TIterator end,
        _In_ TAlloc allocator = TAlloc());

/// <summary>
/// Answer a pointer to the <paramref name="idx" />th string in the given
/// multi-sz.
/// </summary>
/// <remarks>
/// Note that is is indispensable that <paramref name="multi_sz" /> is
/// terminated using <i>two</c> zeros. It is safe to pass
/// <see langword="nullptr" />, though.
/// </remarks>
/// <param name="multi_sz">A set of zero-terminated strings, terminated with
/// <i>two</c> zeros at the end of the set. The caller remains owner of the
/// memory.</param>
/// <param name="idx">The zero-based index of the string to retrieve.
/// </param>
/// <returns>A pointer to the <paramref name="idx" />th string or
/// <see langword="nullptr" /> if that string does not exist.</returns>
template<class TChar>
_Ret_maybenull_z_ const TChar *multi_sz_at(_In_opt_z_ const TChar *multi_sz,
    _In_ const std::size_t idx) noexcept;

/// <summary>
/// Concatenate two multi-sz strings.
/// </summary>
/// <remarks>
/// Note that is is indispensable that <paramref name="lhs" /> and
/// <pararmef name="rhs" /> are both terminated using <i>two</c> zeros. It is 
/// safe to pass <see langword="nullptr" /> for any of them, though.
/// </remarks>
/// <typeparam name="TChar"></typeparam>
/// <typeparam name="TAlloc"></typeparam>
/// <param name="lhs"></param>
/// <param name="rhs"></param>
/// <param name="allocator"></param>
/// <returns></returns>
template<class TChar, class TAlloc = std::allocator<TChar>>
_Ret_maybenull_z_ TChar *multi_sz_concat(
    _In_opt_z_ const TChar *lhs,
    _In_opt_z_ const TChar *rhs,
    _In_ TAlloc allocator = TAlloc());

/// <summary>
/// Copy the strings to an output iterator.
/// </summary>
/// <typeparam name="TIterator">An output iterator receiving the strings.
/// </typeparam>
/// <param name="multi_sz">A set of zero-terminated strings, terminated with
/// <i>two</c> zeros at the end of the set. The caller remains owner of the
/// memory.</param>
/// <param name="oit">The output iterator receiving the data.</param>
/// <returns>The output iterator past the last written element.</returns>
template<class TChar, class TIterator>
static TIterator multi_sz_copy(_In_ TIterator oit,
    _In_opt_z_ const TChar *multi_sz);

/// <summary>
///  Answer the number of entries in the set of zero-terminated strings
/// <paramref name="multi_sz" />.
/// </summary>
/// <remarks>
/// Note that is is indispensable that <paramref name="multi_sz" /> is
/// terminated using <i>two</c> zeros. It is safe to pass
/// <see langword="nullptr" />, though.
/// </remarks>
/// <param name="multi_sz">A set of zero-terminated strings, terminated with
/// <i>two</c> zeros at the end of the set. The caller remains owner of the
/// memory.</param>
/// <returns>The number of strings in the set.</returns>
template<class TChar>
std::size_t multi_sz_count(_In_opt_z_ const TChar *multi_sz) noexcept;

/// <summary>
/// Test whether <paramref name="lhs" /> and <paramref name="rhs" /> are two
/// multi-sz strings with the same content.
/// </summary>
/// <typeparam name="TChar"></typeparam>
/// <param name="lhs"></param>
/// <param name="rhs"></param>
/// <returns></returns>
template<class TChar> bool multi_sz_equals(_In_opt_z_ const TChar *lhs,
    _In_opt_z_ const TChar *rhs) noexcept;

/// <summary>
/// Answer the length of the set of zero-terminated strings
/// <paramref name="multi_sz" /> in number of characters.
/// </summary>
/// <remarks>
/// Note that is is indispensable that <paramref name="multi_sz" /> is
/// terminated using <i>two</c> zeros. It is safe to pass
/// <see langword="nullptr" />, though.
/// </remarks>
/// <param name="multi_sz">A set of zero-terminated strings, terminated with
/// <i>two</c> zeros at the end of the set. The caller remains owner of the
/// memory.</param>
/// <returns>The number of characters that are required to store the
/// <paramref name="multi_sz" />.</returns>
template<class TChar>
std::size_t multi_sz_size(_In_opt_z_ const TChar *multi_sz) noexcept;

/// <summary>
/// Answer the required buffer size, including the terminating null, for the
/// given null-terminated string.
/// </summary>
/// <typeparam name="TChar">The character type of the string.</typeparam>
/// <param name="sz">A null-terminated string. The caller remains owner of the
/// memory. It is safe to pass <see langword="nullptr" />.</param>
/// <returns>The number of characters that are required to store the string,
/// including the terminating null.</returns>
template<class TChar> std::size_t sz_size(_In_opt_z_ const TChar *sz) noexcept {
    typedef std::char_traits<TChar> traits_type;
    return (sz != nullptr) ? traits_type::length(sz) + 1 : 0;
}

/// <summary>
/// Answer the required buffer size, including the terminating null, for the
/// given STL string.
/// </summary>
/// <typeparam name="TChar">The character type of the string.</typeparam>
/// <typeparam name="TTraits">The character traits type of the string.
/// </typeparam>
/// typeparam name="TAlloc">The allocator type of the string.</typeparam>
/// <param name="sz">A string.</param>
/// <returns>The number of characters that are required to store the string,
/// including the terminating null.</returns>
template<class TChar, class TTraits, class TAlloc> inline std::size_t sz_size(
        _In_ const std::basic_string<TChar, TTraits, TAlloc>& sz) noexcept {
    return sz.size() + 1;
}

LYRA_DETAIL_NAMESPACE_END


LYRA_NAMESPACE_BEGIN

/// <summary>
/// Represents a set of zero-terminated strings terminated by an additional zero,
/// which is commonly used in the Windows registry to store arrays of strings.
/// </summary>
class LYRA_API multi_sz final {

public:

    /// <summary>
    /// An iterator over the strings in a <see cref="multi_sz" />.
    /// </summary>
    class iterator final {

    public:

        /// <summary>
        /// The iterator category.
        /// </summary>
        typedef std::forward_iterator_tag iterator_category;

        /// <summary>
        /// The type being iterated.
        /// </summary>
        typedef const char *value_type;

        /// <summary>
        /// The type expressing distances between iterators.
        /// </summary>
        typedef std::ptrdiff_t difference_type;

        /// <summary>
        /// A pointer to the iterated values.
        /// </summary>
        typedef const char *pointer;

        /// <summary>
        /// A reference to the iterated values.
        /// </summary>
        typedef const char *reference;

        /// <summary>
        /// Clone <paramref name="rhs" />.
        /// </summary>
        /// <param name="rhs">The object to be cloned.</param>
        inline iterator(_In_ const iterator& rhs) {
            *this = rhs;
        }

        /// <summary>
        /// Assignment.
        /// </summary>
        /// <param name="rhs">The right-hand side operand.</param>
        /// <returns><c>*<see langword="this" />.</c>.</returns>
        iterator& operator =(_In_ const iterator& rhs);

        /// <summary>
        /// Test for equality.
        /// </summary>
        /// <param name="rhs">The right-hand side operand.</param>
        /// <returns><c>true</c> if this iterator and <paramref name="rhs" />
        /// are equal, <c>false</c> otherwise.</returns>
        inline bool operator ==(_In_ const iterator& rhs) noexcept {
            return (this->_current == rhs._current);
        }

        /// <summary>
        /// Test for inequality.
        /// </summary>
        /// <param name="rhs">The right-hand side operand.</param>
        /// <returns><c>true</c> if this iterator and <paramref name="rhs" />
        /// are not equal, <c>false</c> otherwise.</returns>
        inline bool operator !=(_In_ const iterator& rhs) noexcept {
            return (this->_current != rhs._current);
        }

        /// <summary>
        /// Dereference the iterator.
        /// </summary>
        /// <returns>The current string.</returns>
        inline reference operator *(void) const {
            return this->_current;
        }

        /// <summary>
        /// Provide access to the contained string.
        /// </summary>
        /// <returns>The current string.</returns>
        inline pointer operator ->(void) const {
            return this->_current;
        }

        /// <summary>
        /// Prefix increment.
        /// </summary>
        /// <returns><c>*<see langword="this" />.</c>.</returns>
        iterator& operator ++(void);

        /// <summary>
        /// Postfix increment.
        /// </summary>
        /// <returns>The previous iterator position.</returns>
        inline iterator operator ++(int) {
            iterator retval(*this);
            ++(*this);
            return retval;
        }

    private:

        inline iterator(_In_opt_z_ value_type current) : _current(current) { }

        value_type _current;

        friend class multi_sz;
    };

    /// <summary>
    /// The type used for a character.
    /// </summary>
    typedef char value_type;

    /// <summary>
    /// Creates a new instance for a single element.
    /// </summary>
    /// <param name="value">The initial string to be added to a new multi-sz
    /// string.</param>
    /// <returns>A new multi-sz instance holding the given string.</returns>
    static inline multi_sz for_string(_In_z_ const value_type *value) {
        multi_sz retval;
        retval.add(value);
        return retval;
    }

    /// <summary>
    /// Creates a new instance for a single element.
    /// </summary>
    /// <param name="value">The initial string to be added to a new multi-sz
    /// string.</param>
    /// <returns>A new multi-sz instance holding the given string.</returns>
    template<class TTraits, class TAlloc>
    static inline multi_sz for_string(
            _In_ const std::basic_string<value_type, TTraits, TAlloc>& value) {
        multi_sz retval;
        retval.add(value.c_str());
        return retval;
    }

    /// <summary>
    /// Initialises a new instance.
    /// </summary>
    /// <param name="value">A set of zero-terminated strings, terminated with
    /// <i>two</c> zeros at the end of the set. The caller remains owner of the
    /// memory.</param>
    explicit multi_sz(_In_opt_z_ const value_type *value = nullptr);

    /// <summary>
    /// Clone <paramref name="rhs" />.
    /// </summary>
    /// <param name="rhs">The object to be clones.</param>
    inline multi_sz(_In_ const multi_sz& rhs) : _value(nullptr) {
        *this = rhs;
    }

    /// <summary>
    /// Move <paramref name="rhs" />.
    /// </summary>
    /// <param name="rhs">The object to be moved.</param>
    inline multi_sz(_Inout_ multi_sz&& rhs) noexcept : _value(rhs._value) {
        rhs._value = nullptr;
    }

    /// <summary>
    /// Initialises a new instance from an array of strings.
    /// </summary>
    /// <param name="strings">An array of <parmaref name="cnt" /> strings.
    /// </param>
    /// <param name="cnt"></param>
    multi_sz(_In_reads_(cnt) const value_type **strings,
        _In_ const std::size_t cnt);

    /// <summary>
    /// Finalises the instance.
    /// </summary>
    inline ~multi_sz(void) noexcept {
        this->clear();
    }

    /// <summary>
    /// Append <paramref name="str" /> at the end of the multi-sz.
    /// </summary>
    /// <param name="str">The string to be appended. It is safe to pass
    /// <see langword="nullptr" /> (nothing will happen in this case). Nothing
    /// will happen if <paramref name="str" /> is an empty string either. The
    /// caller remains owner of the memory.</param>
    /// <returns><c>*<see langword="this" />.</c>.</returns>
    multi_sz& add(_In_opt_z_ const value_type *str);

    /// <summary>
    /// Answer the raw data of the <paramref name="idx" />th string in the
    /// multi-sz or <c>nullptr</c> if no such element exists.
    /// </summary>
    /// <param name="idx">The zero-based index of the string to retrieve.
    /// </param>
    /// <returns>A pointer to the <paramref name="idx" />th string or
    /// <see langword="nullptr" /> if that string does not exist.</returns>
    inline _Ret_maybenull_z_ const value_type *at(
            _In_ const std::size_t idx) const noexcept {
        return detail::multi_sz_at(this->_value, idx);
    }

    /// <summary>
    /// Gets an iterator over the strings in the multi-sz.
    /// </summary>
    /// <returns>An iterator for the begin of the strings.</returns>
    inline iterator begin(void) const noexcept {
        return iterator(this->_value);
    }

    /// <summary>
    /// Clear all contents.
    /// </summary>
    void clear(void) noexcept;

    /// <summary>
    /// Answer the number of strings in the multi-sz.
    /// </summary>
    /// <returns>The number of strings stored in the object.</returns>
    inline std::size_t count(void) const noexcept {
        return detail::multi_sz_count(this->_value);
    }

    /// <summary>
    /// Answer the raw data pointer.
    /// </summary>
    /// <remarks>
    /// Note, that is unsafe to operate with this pointer and that the pointer
    /// might become invalid as the multi-sz is manipulated using other methods.
    /// <returns>The raw data pointer.</returns>
    inline _Ret_maybenull_z_ const value_type *data(void) const noexcept {
        return this->_value;
    }

    /// <summary>
    /// Answer whether there is not string in the multi-sz.
    /// </summary>
    /// <returns><see langword="true" /> if the multi-sz holds no data,
    /// <see langword="false" /> otherwise.</returns>
    inline bool empty(void) const noexcept {
        return (this->_value == nullptr) || (*this->_value == 0);
    }

    /// <summary>
    /// Gets an iterator for the end of the multi-sz.
    /// </summary>
    /// <returns>The end of the strings in the multi-sz.</returns>
    inline iterator end(void) const noexcept {
        return iterator(nullptr);
    }

    ///// <summary>
    ///// Insert <paramref name="str" /> at the <paramref name="idx" />th
    ///// position.
    ///// </summary>
    ///// <remarks>
    ///// All elements behind <paramref name="idx" /> are shifted one element
    ///// right. <paramref name="idx" /> must be a valid index in the multi-sz.
    ///// Otherwise, the method behaves like <see cref="add" />.
    ///// <param name="idx">The zero-based position where to insert the new
    ///// string.</param>
    ///// <param name="str">The string to be appended. It is safe to pass
    ///// <c>nullptr</c> (nothing will happen in this case). Nothing will happen
    ///// if <paramref name="str" /> is an empty string either. The caller remains
    ///// owner of the memory.</param>
    ///// <returns><c>*<see langword="this" />.</c>.</returns>
    //multi_sz& insert(_In_ const std::size_t idx,
    //    _In_opt_z_ const value_type *str);

    /// <summary>
    /// Answer the size of the multi-sz including all terminating zeros.
    /// </summary>
    /// <returns>The number of characters in the multi-sz.</returns>
    inline std::size_t length(void) const noexcept {
        return detail::multi_sz_size(this->_value);
    }

    /// <summary>
    /// Append <paramref name="str" /> at the end of the multi-sz.
    /// </summary>
    /// <param name="str">The string to be appended. It is safe to pass
    /// <c>nullptr</c> (nothing will happen in this case). Nothing will happen
    /// if <paramref name="str" /> is an empty string either. The caller remains
    /// owner of the memory.</param>
    /// <returns><c>*<see langword="this" />.</c>.</returns>
    inline multi_sz& push_back(_In_opt_z_ const value_type *str) {
        return this->add(str);
    }

    ///// <summary>
    ///// Remove all occurrences of <paramref name="str" /> in the set.
    ///// </summary>
    ///// <param name="str">The string to be removed. It is safe to pass
    ///// <c>nullptr</c>. The caller remains owner of the memory.</param>
    ///// <returns><c>*<see langword="this" />.</c>.</returns>
    //multi_sz& remove(_In_opt_z_ const value_type *str);

    ///// <summary>
    ///// Remove all strings that match the given <paramref name="predicate" />.
    ///// </summary>
    ///// <typeparam name="TPredicate">The type of the predicate, which must
    ///// accept a <c>const value_type *</c> and yield a <c>bool</c>.</typeparam>
    ///// <param name="predicate">The predicate selecting the strings to be
    ///// removed.</param>
    ///// <returns><c>*<see langword="this" />.</c>.</returns>
    //template<class TPredicate>
    //multi_sz& remove_if(_In_ const TPredicate predicate);

    /// <summary>
    /// Answer the size of the multi-sz including all terminating zeros.
    /// </summary>
    /// <returns>The number of characters in the multi-sz.</returns>
    inline std::size_t size(void) const noexcept {
        return detail::multi_sz_size(this->_value);
    }

    /// <summary>
    /// Assignment.
    /// </summary>
    /// <param name="rhs">The right-hand side operand.</param>
    /// <returns><c>*<see langword="this" />.</c>.</returns>
    multi_sz& operator =(_In_ const multi_sz& rhs);

    /// <summary>
    /// Move assignment.
    /// </summary>
    /// <param name="rhs">The right-hand side operand.</param>
    /// <returns><c>*<see langword="this" />.</c>.</returns>
    multi_sz& operator =(_Inout_ multi_sz&& rhs) noexcept;

    /// <summary>
    /// Test for equality.
    /// </summary>
    /// <param name="rhs">The right-hand side operand.</param>
    /// <returns><see langword="true" /> if <paramref name="rhs" /> contains the
    /// same strings in the same order as this object, <see langword="false" />
    /// otherwise.</returns>
    inline bool operator ==(_In_ const multi_sz& rhs) const noexcept {
        return detail::multi_sz_equals(this->_value, rhs._value);
    }

    /// <summary>
    /// Test for inequality.
    /// </summary>
    /// <param name="rhs">The right-hand side operand.</param>
    /// <returns><see langword="true" /> if <paramref name="rhs" /> does not
    /// contain the same strings in the same order as this object,
    /// <see langword="false" /> otherwise.</returns>
    inline bool operator !=(_In_ const multi_sz& rhs) const noexcept {
        return !(*this == rhs);
    }

    /// <summary>
    /// Append <paramref name="str" /> at the end of the multi-sz.
    /// </summary>
    /// <param name="str">The string to be appended. It is safe to pass
    /// <c>nullptr</c> (nothing will happen in this case). Nothing will happen
    /// if <paramref name="str" /> is an empty string either. The caller remains
    /// owner of the memory.</param>
    /// <returns><c>*<see langword="this" />.</c>.</returns>
    inline multi_sz& operator +=(_In_opt_z_ const value_type *rhs) {
        this->add(rhs);
        return *this;
    }

    /// <summary>
    /// Answer the raw data of the <paramref name="idx" />th string in the
    /// multi-sz or <c>nullptr</c> if no such element exists.
    /// <param name="idx">The zero-based index of the string to retrieve.
    /// </param>
    /// <returns>A pointer to the <paramref name="idx" />th string or
    /// <see langword="nullptr" /> if that string does not exist.</returns>
    inline _Ret_maybenull_z_ const value_type *operator [](
            _In_ const std::size_t idx) const noexcept {
        return detail::multi_sz_at(this->_value, idx);
    }

    /// <summary>
    /// Convert the multi-sz to a vector of strings.
    /// </summary>
    /// <returns>A vector containing all strings in the multi-sz.</returns>
    inline operator std::vector<std::string>(void) const {
        std::vector<std::string> retval;
        detail::multi_sz_copy(std::back_inserter(retval), this->_value);
        return retval;
    }

private:

    value_type *_value;
};

LYRA_NAMESPACE_END

#include "visus/lyra/multi_sz.inl"

#endif /* !defined(_LYRA_MULTI_SZ_H) */
