// <copyright file="to_string.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_TO_STRING_H)
#define _LYRA_TO_STRING_H
#pragma once

#include <string>
#include <type_traits>

#include "visus/autodoc/guid.h"
#include "visus/autodoc/multi_sz.h"
#include "visus/autodoc/property_set.h"
#include "visus/autodoc/timestamp.h"

#include "boolean.h"


LYRA_DETAIL_NAMESPACE_BEGIN

/// <summary>
/// Answer the string type of the STL-provided string conversion of
/// <typeparamref name="TType" />.
/// </summary>
template<class TType>
using std_string_type = decltype(std::to_string(std::declval<TType>()));

/// <summary>
/// Determines whether the STL provides a string conversion for
/// <typeparamref name="TType" />
/// </summary>
template<class TType, class = void>
struct has_std_to_string : std::false_type { };

/// <summary>
/// Specialisation for types having an STL-provided string conversion.
/// </summary>
/// <typeparam name="TType"></typeparam>
template<class TType>
struct has_std_to_string<TType, std::void_t<std_string_type<TType>>>
    : std::true_type { };

/// <summary>
/// Construct a string from the given character pointer, which may be
/// <see langword="nullptr" />, enclosing the result in quotes.
/// </summary>
LYRA_TEST_API std::string to_quoted_string(_In_opt_z_ const char *str);

/// <summary>
/// Convert a GUID to a string enclosed in quotes.
/// </summary>
inline std::string to_quoted_string(_In_ const guid& value) {
    auto retval = value.to_string<char>();
    retval.insert(retval.begin(), u8'\"');
    retval += u8'\"';
    return retval;
}

/// <summary>
/// Convert a <see cref="multi_sz" /> to a string, possibly joining multiple
/// strings into one using the specified separator. The whole result will be
/// enclosed in quotes.
/// </summary>
LYRA_TEST_API std::string to_quoted_string(_In_ const multi_sz& value,
    _In_ const char sep = u8',');

/// <summary>
/// Convert a <see cref="property_set" /> to a string. This is required for
/// visiting property sets, but does not do anything useful. The function does
/// not do anything useful, it just needs to exist for visiting property sets.
/// If a valid string representation is needed at some point, a meaningful
/// implementation can be added here.
/// </summary>
inline std::string to_quoted_string(const property_set&) {
    return u8"\"{property_set}\"";
}

/// <summary>
/// Convert a <see cref="timestamp" /> to a string.
/// </summary>
/// <remarks>
/// Timestamps are represented as numbers and never quoted.
/// </remarks>
inline std::string to_quoted_string(_In_ const timestamp value) {
    return std::to_string(value.value());
}

/// <summary>
/// Convert a value to a string using the STL-provided conversion.
/// </summary>
/// <remarks>
/// Numbers are never quoted.
/// </remarks>
template<class TType>
inline std::enable_if_t<has_std_to_string<TType>::value, std::string>
to_quoted_string(_In_ const TType& value) {
    return std::to_string(value);
}

/// <summary>
/// Construct a string from the given character pointer, which may be
/// <see langword="nullptr" />.
/// </summary>
LYRA_TEST_API std::string to_unquoted_string(_In_opt_z_ const char *str);

/// <summary>
/// Convert a GUID to a string.
/// </summary>
inline std::string to_unquoted_string(_In_ const guid& value) {
    return value.to_string<char>();
}

/// <summary>
/// Convert a <see cref="multi_sz" /> to a string, possibly joining multiple
/// strings into one using the specified separator.
/// </summary>
LYRA_TEST_API std::string to_unquoted_string(_In_ const multi_sz& value,
    _In_ const char sep = u8',');

/// <summary>
/// Convert a <see cref="property_set" /> to a string. This is required for
/// visiting property sets, but does not do anything useful. The function does
/// not do anything useful, it just needs to exist for visiting property sets.
/// If a valid string representation is needed at some point, a meaningful
/// implementation can be added here.
/// </summary>
inline std::string to_unquoted_string(const property_set&) {
    return u8"{property_set}";
}

/// <summary>
/// Convert a <see cref="timestamp" /> to a string.
/// </summary>
inline std::string to_unquoted_string(_In_ const timestamp value) {
    return std::to_string(value.value());
}

/// <summary>
/// Convert a value to a string using the STL-provided conversion.
/// </summary>
template<class TType>
inline std::enable_if_t<has_std_to_string<TType>::value, std::string>
to_unquoted_string(_In_ const TType& value) {
    return std::to_string(value);
}

/// <summary>
/// Converts the given value to a string, using one of
/// <see cref="to_quoted_string" /> or <see cref="to_unquoted_string" />
/// functions.
/// </summary>
/// <remarks>
/// Note that this indirection is required because directly using overloads
/// of <see cref="to_string" /> leads to ambiguities for pointer types in
/// MSVC.
/// </remarks>
template<class TType>
inline std::enable_if_t<!std::is_pointer_v<TType>, std::string> to_string(
        _In_ TType&& value, _In_ const bool quote) {
    return quote
        ? to_quoted_string(std::forward<TType>(value))
        : to_unquoted_string(std::forward<TType>(value));
}

/// <summary>
/// Converts the given character pointer to a string, using one of
/// <see cref="to_quoted_string" /> or <see cref="to_unquoted_string" />
/// functions.
/// </summary>
/// <remarks>
/// Note that this indirection is required because directly using overloads
/// of <see cref="to_string" /> leads to ambiguities for pointer types in
/// MSVC.
/// </remarks>
inline std::string to_string(
        _In_opt_z_ const char *value,
        _In_ const bool quote) {
    return quote ? to_quoted_string(value) : to_unquoted_string(value);
}

/// <summary>
/// Converts a pointer to a value to a string using whatever function is
/// provided for <see cref="TType" />.
/// </summary>
template<class TType>
std::enable_if_t<!std::is_same_v<std::decay_t<TType>, char>, std::string>
to_string(_In_opt_ const TType *value, _In_ const bool quote) {
    static_assert(!std::is_pointer_v<TType>, "Double pointers are not "
        "supported for conversion to string.");

    if (value == nullptr) {
        return "";
    }

    return to_string(*value, quote);
}

LYRA_DETAIL_NAMESPACE_END

#endif /* !defined(_LYRA_TO_STRING_H) */
