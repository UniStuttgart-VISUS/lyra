// <copyright file="api.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_API_H)
#define _LYRA_API_H
#pragma once


/// <summary>
/// A helper for implementing <see cref="LYRA_CONCAT" />.
/// </summary>
#define _LYRA_CONCAT(l, r) l ##r


/// <summary>
/// Concatenates two preprocessor symbols.
/// </summary>
/// <param name="l">The left-hand side operand.</param>
/// <param name="r">The right-hand side operand.</param>
#define LYRA_CONCAT(l, r) _LYRA_CONCAT(l, r)


/******************************************************************************\
 * Update the ABI version (i.e. if something in the interface changes) here:  *
\******************************************************************************/
#define LYRA_ABI_MAJOR 0
#define LYRA_ABI_MINOR 1


/// <summary>
/// Creates the full ABI versison string.
/// </summary>
/// <param name="mj">The major version of the ABI.</param>
/// <param name="mn">The minor version of the ABI.</param>
#define LYRA_ABI_VERSION(mj, mn) LYRA_CONCAT(mj, LYRA_CONCAT(_, mn))


#if defined(__cplusplus)
/// <summary>
/// Creates the name of the namespace for the specified ABI version.
/// </summary>
/// <param name="mj">The major version of the ABI.</param>
/// <param name="mn">The minor version of the ABI.</param>
#define LYRA_ABI_NAMESPACE(mj, mn) LYRA_CONCAT(v, LYRA_ABI_VERSION(mj, mn))


/// <summary>
/// Creates the name of the current ABI namespace.
/// </summary>
/// <remarks>
/// Use this macro when defining implementations in cpp or inline files.
/// </remarks>
#define LYRA_NAMESPACE visus::lyra::\
LYRA_ABI_NAMESPACE(LYRA_ABI_MAJOR, LYRA_ABI_MINOR)


/// <summary>
/// Creates the name of the namespace for the implementation details.
/// </summary>
/// <remarks>
/// Use this macro when defining implementations in cpp or inline files.
/// </remarks>
#define LYRA_DETAIL_NAMESPACE LYRA_NAMESPACE::detail


/// <summary>
/// Helper for opening the namespace <paramref name="n" />.
/// </summary>
#define _LYRA_NAMESPACE_BEGIN(n) namespace n {


/// <summary>
/// Marks the begin of the current ABI namespace.
/// </summary>
#define LYRA_NAMESPACE_BEGIN \
_LYRA_NAMESPACE_BEGIN(visus)\
_LYRA_NAMESPACE_BEGIN(lyra)\
inline _LYRA_NAMESPACE_BEGIN(LYRA_ABI_NAMESPACE(LYRA_ABI_MAJOR, LYRA_ABI_MINOR))

/// <summary>
/// Marks the end of the current ABI namespace.
/// </summary>
#define LYRA_NAMESPACE_END } } }

/// <summary>
/// Marks the begin of the namespace for implementation details.
/// </summary>
#define LYRA_DETAIL_NAMESPACE_BEGIN \
LYRA_NAMESPACE_BEGIN _LYRA_NAMESPACE_BEGIN(detail)

/// <summary>
/// Marks the end of the namespace for implementation details.
/// </summary>
#define LYRA_DETAIL_NAMESPACE_END } LYRA_NAMESPACE_END

#else /* defined(__cplusplus) */

#define LYRA_NAMESPACE
#define LYRA_NAMESPACE_BEGIN
#define LYRA_NAMESPACE_END

#endif /* defined(__cplusplus) */


/// <summary>
/// Helper for defining an enum class in C++ and a regular enum in C in the
/// public API.
/// </summary>
#if defined(__cplusplus)
#define LYRA_ENUM_CLASS class
#else /* defined(__cplusplus) */
#define LYRA_ENUM_CLASS
#endif /* defined(__cplusplus) */


#if defined(_WIN32)

#if defined(LYRA_EXPORTS)
#define LYRA_API __declspec(dllexport)

// If we are building the tests, also export private APIs for testing.
#if defined(LYRA_EXPORT_TESTS)
#define LYRA_TEST_API __declspec(dllexport)
#else /* defined(LYRA_EXPORT_TESTS) */
#define LYRA_TEST_API
#endif /* defined(LYRA_EXPORT_TESTS) */

#else /* defined(LYRA_EXPORTS) */
#define LYRA_API __declspec(dllimport)

// If we are building the tests, import private APIs as well.
#if defined(LYRA_EXPORT_TESTS)
#define LYRA_TEST_API __declspec(dllimport)
#else /* defined(LYRA_EXPORT_TESTS) */
#define LYRA_TEST_API
#endif /* defined(LYRA_EXPORT_TESTS) */

#endif /* defined(LYRA_EXPORTS) */

#else /* defined(_WIN32) */
#define LYRA_API
#define LYRA_TEST_API

#endif /* defined(_WIN32) */

#if defined(_WIN32)
#include <sal.h>
#include <Windows.h>

#else /* defined(_WIN32) */
#include "visus/autodoc/nosal.h"
#endif /* defined(_WIN32) */

#endif /* !defined(_LYRA_API_H) */
