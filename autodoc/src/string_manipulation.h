// <copyright file="string_manipulation.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_STRING_MANIPULATION_H)
#define _LYRA_STRING_MANIPULATION_H
#pragma once

#include <algorithm>
#include <cassert>
#include <cinttypes>
#include <iterator>
#include <string>
#include <vector>

#include "visus/autodoc/api.h"


LYRA_DETAIL_NAMESPACE_BEGIN

/// <summary>
/// Answer whether <paramref name="value" /> is contained in
/// <paramref name="container" />.
/// </summary>
/// <typeparam name="TContainer"></typeparam>
/// <typeparam name="TValue"></typeparam>
/// <param name="container"></param>
/// <param name="value"></param>
/// <returns></returns>
template<class TContainer, class TValue> inline bool contains(
        _In_ const TContainer& container,
        _In_ const TValue& value) {
    const auto end = container.end();
    return (std::find(container.begin(), end, value) != container.end());
}

/// <summary>
/// Answer whether the given null-terminated string is empty, i.e. either a
/// <see langword="nullptr" /> or a string with zero length.
/// </summary>
template<class TChar>
inline constexpr bool empty_string(_In_z_ const TChar *str) noexcept {
    return ((str == nullptr) || (*str == 0));
}

/// <summary>
/// Answer whether the given string is empty.
/// </summary>
template<class TChar, class TTraits, class TAlloc> inline bool empty_string(
        _In_ const std::basic_string<TChar, TTraits, TAlloc>& str) noexcept {
    return str.empty();
}

/// <summary>
/// Makes sure that the given string is null-terminated.
/// </summary>
/// <typeparam name="TChar"></typeparam>
/// <param name="str"></param>
/// <returns></returns>
template<class TChar>
std::vector<TChar>& null_terminate(_In_ std::vector<TChar>& str);

/// <summary>
/// Returns an iterator to the last non-zero character at the end of the
/// given range.
/// </summary>
/// <typeparam name="TIterator"></typeparam>
/// <param name="begin"></param>
/// <param name="end"></param>
/// <returns></returns>
template<class TIterator> TIterator remove_trailing_nulls(
    _In_ const TIterator begin, _In_ const TIterator end);

/// <summary>
/// Erases all trailing nulls from the given string.
/// </summary>
/// <typeparam name="TChar"></typeparam>
/// <typeparam name="TTraits"></typeparam>
/// <typeparam name="TAllocator"></typeparam>
/// <param name="str"></param>
/// <returns></returns>
template<class TChar, class TTraits, class TAllocator>
std::basic_string<TChar, TTraits, TAllocator>& remove_trailing_nulls(
        _In_ std::basic_string<TChar, TTraits, TAllocator>& str) {
    const auto end = remove_trailing_nulls(str.begin(), str.end());
    str.erase(end, str.end());
    return str;
}

/// <summary>
/// Erases all trailing nulls from the given vector.
/// </summary>
/// <typeparam name="TChar"></typeparam>
/// <param name="str"></param>
/// <returns></returns>
template<class TChar>
std::vector<TChar>& remove_trailing_nulls(_In_ std::vector<TChar>& str) {
    const auto end = remove_trailing_nulls(str.begin(), str.end());
    str.erase(end, str.end());
    return str;
}

/// <summary>
/// Replaces all elements in the range [<paramref name="begin" />,
/// <paramref name="end" />[ that match the given <paramref name="predicate" />
/// with the given specified <paramref name="value" />.
/// </summary>
/// <typeparam name="TIterator"></typeparam>
/// <typeparam name="TPredicate"></typeparam>
/// <param name="begin"></param>
/// <param name="end"></param>
/// <param name="predicate"></param>
/// <param name="value"></param>
/// <returns>The number of replaced elements.</returns>
template<class TIterator, class TPredicate> std::size_t replace_if(
    _In_ const TIterator begin,
    _In_ const TIterator end,
    _In_ const TPredicate predicate,
    _In_ const typename std::iterator_traits<TIterator>::value_type& value);

/// <summary>
/// Converts a string of bytes to a string of hexadecimal characters
/// representing the individual bytes.
/// </summary>
/// <param name="data"></param>
/// <param name="cnt"></param>
/// <returns></returns>
LYRA_TEST_API std::string to_hex_string(_In_reads_bytes_(cnt) const void *data,
    _In_ std::size_t cnt);

/// <summary>
/// Returns the new begin after trimming all elements from the begin of the
/// range [<paramref name="begin" />, <paramref name="end" />[ that satisfy the
/// given <paramref name="predicate" />.
/// </summary>
/// <typeparam name="TIterator"></typeparam>
/// <typeparam name="TPredicate"></typeparam>
/// <param name="begin"></param>
/// <param name="end"></param>
/// <param name="pred"></param>
/// <returns></returns>
template<class TIterator, class TPredicate> inline TIterator trim_left_if(
        _In_ const TIterator begin,
        _In_ const TIterator end,
        _In_ TPredicate&& predicate) {
    return std::find_if_not(begin, end, std::forward<TPredicate>(predicate));
}

/// <summary>
/// Returns the new begin of the given null-terminated string after trimming all
/// characters from the beginning that satisfy the given
/// <paramref name="predicate" />.
/// </summary>
/// <typeparam name="TChar"></typeparam>
/// <typeparam name="TPredicate"></typeparam>
/// <param name="str"></param>
/// <param name="predicate"></param>
/// <returns></returns>
template<class TChar, class TPredicate> inline TChar *trim_left_if(
        _In_z_ TChar *str,
        _In_ TPredicate&& predicate) {
    assert(str != nullptr);
    const auto len = std::char_traits<TChar>::length(str);
    return trim_left_if(str, str + len, std::forward<TPredicate>(predicate));
}

/// <summary>
/// Removes all elements from the begin of the given
/// <paramref name="container" /> that satisfy the given
/// <paramref name="predicate" />.
/// </summary>
/// <typeparam name="TContainer"></typeparam>
/// <typeparam name="TPredicate"></typeparam>
/// <param name="container"></param>
/// <param name="predicate"></param>
/// <returns></returns>
template<class TContainer, class TPredicate>
std::enable_if_t<!std::is_pointer_v<TContainer>, TContainer&> trim_left_if(
        _Inout_ TContainer& container,
        _In_ TPredicate&& predicate) {
    const auto b = trim_left_if(container.begin(), container.end(),
        std::forward<TPredicate>(predicate));
    container.erase(container.begin(), b);
    return container;
}

/// <summary>
/// Returns the new begin after trimming all elements from the begin of the
/// range [<paramref name="begin" />, <paramref name="end" />[ that are in
/// <paramref name="values" />.
/// </summary>
/// <typeparam name="TIterator"></typeparam>
/// <typeparam name="TValues"></typeparam>
/// <param name="begin"></param>
/// <param name="end"></param>
/// <param name=""></param>
/// <returns></returns>
template<class TIterator, class TValues> inline TIterator trim_left(
        _In_ const TIterator begin,
        _In_ const TIterator end,
        _In_ const TValues& values) {
    const auto p = [&values](const auto& v) { return contains(values, v); };
    return trim_left_if(begin, end, p);
}

/// <summary>
/// Returns the new begin of the given null-terminated string after trimming all
/// characters from the beginning that are in <paramref name="values" />.
/// </summary>
/// <typeparam name="TChar"></typeparam>
/// <typeparam name="TPredicate"></typeparam>
/// <param name="str"></param>
/// <param name="predicate"></param>
/// <returns></returns>
template<class TChar, class TValues> inline TChar *trim_left(
        _In_z_ TChar *str,
        _In_ TValues&& values) {
    assert(str != nullptr);
    const auto len = std::char_traits<TChar>::length(str);
    return trim_left(str, str + len, std::forward<TValues>(values));
}

/// <summary>
/// Removes all elements from the begin of the given
/// <paramref name="container" /> that are in <paramref name="values" />.
/// </summary>
/// <typeparam name="TContainer"></typeparam>
/// <typeparam name="TValues"></typeparam>
/// <param name="container"></param>
/// <param name="predicate"></param>
/// <returns></returns>
template<class TContainer, class TValues>
std::enable_if_t<!std::is_pointer_v<TContainer>, TContainer&> trim_left(
        _Inout_ TContainer& container,
        _In_ const TValues& values) {
    const auto p = [&values](const auto& v) { return contains(values, v); };
    return trim_left_if(container, p);
}

/// <summary>
/// Returns the new end after trimming all elements from the end of the
/// range [<paramref name="begin" />, <paramref name="end" />[ that satisfy the
/// given <paramref name="predicate" />.
/// </summary>
/// <typeparam name="TIterator">An iterator (typically over characters).
/// </typeparam>
/// <typeparam name="TPredicate">A predicate to check a single character
/// against.</typeparam>
/// <param name="begin">The begin of the range to be trimmed.</param>
/// <param name="end">The end of the range to be trimmed.</param>
/// <param name="predicate">The predicate to check against.</param>
/// <returns>The new end of the range.</returns>
template<class TIterator, class TPredicate> inline TIterator trim_right_if(
        _In_ const TIterator begin,
        _In_ const TIterator end,
        _In_ TPredicate&& predicate) {
    const auto b = std::make_reverse_iterator(end);
    const auto e = std::make_reverse_iterator(begin);
    return std::find_if_not(b, e, std::forward<TPredicate>(predicate)).base();
}

/// <summary>
/// Trims the end of the given null-terminated string by replacing the first
/// character that does satisfy the <paramref name="predicate" /> with a null
/// character.
/// </summary>
/// <typeparam name="TChar">A (non-constant) character type.</typeparam>
/// <typeparam name="TPredicate"></typeparam>
/// <param name="str"></param>
/// <param name="predicate"></param>
/// <returns></returns>
template<class TChar, class TPredicate> inline void trim_right_if(
        _In_z_ TChar *str,
        _In_ TPredicate&& predicate) {
    assert(str != nullptr);
    const auto len = std::char_traits<TChar>::length(str);
    const auto end = trim_right_if(str, str + len,
        std::forward<TPredicate>(predicate));
    *end = static_cast<TChar>(0);
}

/// <summary>
/// Removes all elements from the end of the given
/// <paramref name="container" /> that satisfy the given
/// <paramref name="predicate" />.
/// </summary>
/// <typeparam name="TContainer"></typeparam>
/// <typeparam name="TPredicate"></typeparam>
/// <param name="container"></param>
/// <param name="predicate"></param>
/// <returns></returns>
template<class TContainer, class TPredicate> 
std::enable_if_t<!std::is_pointer_v<TContainer>, TContainer&> trim_right_if(
        _Inout_ TContainer& container,
        _In_ TPredicate&& predicate) {
    const auto e = trim_right_if(container.begin(), container.end(),
        std::forward<TPredicate>(predicate));
    container.erase(e, container.end());
    return container;
}

/// <summary>
/// Returns the new end after trimming all elements from the end of the
/// range [<paramref name="begin" />, <paramref name="end" />[ that are in
/// <paramref name="values" />.
/// </summary>
/// <typeparam name="TIterator"></typeparam>
/// <param name="begin"></param>
/// <param name="end"></param>
/// <param name=""></param>
/// <returns></returns>
template<class TIterator, class TValues> inline TIterator trim_right(
        _In_ const TIterator& begin,
        _In_ const TIterator& end,
        _In_ const TValues& values) {
    const auto p = [&values](const auto& v) { return contains(values, v); };
    return trim_right_if(begin, end, p);
}

/// <summary>
/// Trims the end of the given null-terminated string by replacing the first
/// character not in <paramref name="values" /> with a null character.
/// </summary>
/// <typeparam name="TChar">A (non-constant) character type.</typeparam>
/// <typeparam name="TPredicate"></typeparam>
/// <param name="str"></param>
/// <param name="predicate"></param>
/// <returns></returns>
template<class TChar, class TValues> inline void trim_right(
        _In_z_ TChar *str,
        _In_ const TValues& values) {
    const auto p = [&values](const auto& v) { return contains(values, v); };
    return trim_right_if(str, p);
}

/// <summary>
/// Removes all elements from the end of the given
/// <paramref name="container" /> that are in <paramref name="values" />.
/// </summary>
/// <typeparam name="TContainer"></typeparam>
/// <typeparam name="TValues"></typeparam>
/// <param name="container"></param>
/// <param name="predicate"></param>
/// <returns></returns>
template<class TContainer, class TValues>
std::enable_if_t<!std::is_pointer_v<TContainer>, TContainer&> trim_right(
        _Inout_ TContainer& container,
        _In_ const TValues& values) {
    const auto p = [&values](const auto& v) { return contains(values, v); };
    return trim_right_if(container, p);
}

/// <summary>
/// Removes all elements from the begin and the end of the given
/// <paramref name="container" /> that satisfy the given
/// <paramref name="predicate" />.
/// </summary>
/// <typeparam name="TContainer"></typeparam>
/// <typeparam name="TPredicate"></typeparam>
/// <param name="container"></param>
/// <param name="predicate"></param>
/// <returns></returns>
template<class TContainer, class TPredicate>
std::enable_if_t<!std::is_pointer_v<TContainer>, TContainer&> trim_if(
        _Inout_ TContainer& container,
        _In_ const TPredicate predicate) {
    const auto b = trim_left_if(container.begin(), container.end(), predicate);
    container.erase(container.begin(), b);
    const auto e = trim_right_if(container.begin(), container.end(), predicate);
    container.erase(e, container.end());
    return container;
}

/// <summary>
/// Terminates the given null-terminated string at the end by replacing the first
/// character that does not satisfy the <paramref name="predicate" /> with a null
/// character and returns the new begin of the string at the first character that
/// does not satisfy the <paramref name="predicate" /> at the beginning.
/// </summary>
/// <typeparam name="TChar"></typeparam>
/// <typeparam name="TPredicate"></typeparam>
/// <param name="str"></param>
/// <param name="predicate"></param>
/// <returns></returns>
template<class TChar, class TPredicate> inline TChar *trim_if(
        _In_z_ TChar *str,
        _In_ const TPredicate predicate) {
    trim_right_if(str, predicate);
    return trim_left_if(str, predicate);
}

/// <summary>
/// Removes all elements from the begin and the end of the given
/// <paramref name="container" /> that are in <paramref name="values" />.
/// </summary>
/// <typeparam name="TContainer"></typeparam>
/// <typeparam name="TPredicate"></typeparam>
/// <param name="container"></param>
/// <param name="predicate"></param>
/// <returns></returns>
template<class TContainer, class TValues>
std::enable_if_t<!std::is_pointer_v<TContainer>, TContainer&> trim(
        _Inout_ TContainer& container,
        _In_ const TValues& values) {
    const auto p = [&values](const auto& v) { return contains(values, v); };
    const auto b = trim_left_if(container.begin(), container.end(), p);
    container.erase(container.begin(), b);
    const auto e = trim_right_if(container.begin(), container.end(), p);
    container.erase(e, container.end());
    return container;
}

/// <summary>
/// Terminates the given null-terminated string at the end by replacing the first
/// character that is not in <paramref name="values" /> with a null character and
/// returns the new begin of the string at the first character that is not in
/// <paramref name="values" /> at the beginning.
/// </summary>
/// <typeparam name="TChar"></typeparam>
/// <typeparam name="TPredicate"></typeparam>
/// <param name="str"></param>
/// <param name="predicate"></param>
/// <returns></returns>
template<class TChar, class TValues> inline TChar *trim(
        _In_z_ TChar *str,
        _In_ const TValues& values) {
    trim_right(str, values);
    return trim_left(str, values);
}

LYRA_DETAIL_NAMESPACE_END

#include "string_manipulation.inl"

#endif /* !defined(_LYRA_STRING_MANIPULATION_H) */
