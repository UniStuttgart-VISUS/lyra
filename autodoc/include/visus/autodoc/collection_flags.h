// <copyright file="collection_flags.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_COLLECTION_FLAGS_H)
#define _LYRA_COLLECTION_FLAGS_H
#pragma once

#if defined(__cplusplus)
#include <cinttypes>
#include <type_traits>
#else /* defined(__cplusplus) */
#include <stdint.h>
#endif /* defined(__cplusplus) */

#include "visus/autodoc/api.h"


LYRA_NAMESPACE_BEGIN

#if defined(__cplusplus)
#define _LYRA_COLLECTION_FLAGS(f) f
#else /* defined(__cplusplus) */
#define _LYRA_COLLECTION_FLAGS(f) collection_flags_##f
#endif /* defined(__cplusplus) */

/// <summary>
/// Allows for customising how the library collects information.
/// </summary>
enum LYRA_ENUM_CLASS collection_flags {

    /// <summary>
    /// Specifies the default collection behaviour.
    /// </summary>
    _LYRA_COLLECTION_FLAGS(none) = 0,

    /// <summary>
    /// Instructs the library not to include any sensitive information.
    /// </summary>
    _LYRA_COLLECTION_FLAGS(no_sensitive) = 0x00000001,

    /// <summary>
    /// Instructs the library to remove all information that is not explicitly
    /// declared via public property descriptors. These typically represent the
    /// smallest common denominator, but for example, the OS version on Windows
    /// contains additional information not available on Linux and vice versa.
    /// Such information will be removed if this flag is set,
    /// </summary>
    _LYRA_COLLECTION_FLAGS(no_undeclared) = 0x00000002,

    /// <summary>
    /// Instructs the library to only collect the properties that are variable
    /// over time.
    /// </summary>
    _LYRA_COLLECTION_FLAGS(no_immutable) = 0x00000004,
};

#undef _LYRA_COLLECTION_FLAGS


#if defined(__cplusplus)
/// <summary>
/// Answer whether all of <paramref name="flag" /> are set in
/// <paramref name="flags" />.
/// </summary>
/// <param name="flags">The flags to be tested.</param>
/// <param name="flag">The flags that must be fully contained in
/// <paramref name="flags" />.</param>
/// <returns><see langword="true" /> if all bits of <paramref name="flag" />
/// are set in <paramref name="flags"/>, <see langword="false" /> otherwise.
/// </returns>
inline constexpr bool has_flag(_In_ const collection_flags flags,
        _In_ const collection_flags flag) noexcept {
    typedef std::underlying_type_t<collection_flags> int_type;
    const auto fs = static_cast<int_type>(flags);
    const auto f = static_cast<int_type>(flag);
    return (fs & f) == f;
}
#endif /* defined(__cplusplus) */

LYRA_NAMESPACE_END


#if defined(__cplusplus)
/// <summary>
/// Combines two <see cref="collection_flags" /> values into one.
/// </summary>
/// <param name="lhs">The left-hand-side operand.</param>
/// <param name="rhs">The right-hand-side operand.</param>
/// <returns>The bitwise or'ed combination of the two operands.</returns>
inline constexpr LYRA_NAMESPACE::collection_flags operator |(
        _In_ const LYRA_NAMESPACE::collection_flags lhs,
        _In_ const LYRA_NAMESPACE::collection_flags rhs) noexcept {
    typedef std::underlying_type_t<LYRA_NAMESPACE::collection_flags> int_type;
    const auto l = static_cast<int_type>(lhs);
    const auto r = static_cast<int_type>(rhs);
    return static_cast<LYRA_NAMESPACE::collection_flags>(l | r);
}
#endif /* defined(__cplusplus) */

#endif /* !defined(_LYRA_COLLECTION_FLAGS_H) */
