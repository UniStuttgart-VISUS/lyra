// <copyright file="autodoc.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_AUTODOC_H)
#define _LYRA_AUTODOC_H
#pragma once

#include "visus/autodoc/collection_flags.h"


LYRA_NAMESPACE_BEGIN

#if defined(__cplusplus)
/// <summary>
/// A RAII helper for collecting and persisting system information on
/// construction and destruction, respectively.
/// </summary>
class LYRA_API autodoc final {

public:

    /// <summary>
    /// Initialises a new instance.
    /// </summary>
    /// <param name="path"></param>
    /// <param name="flags"></param>
    explicit autodoc(_In_opt_z_ const char *path = nullptr,
        _In_ const collection_flags flags = collection_flags::no_sensitive);

    /// <summary>
    /// Initialises a new instance.
    /// </summary>
    /// <param name="path"></param>
    /// <param name="enter_flags"></param>
    /// <param name="exit_flags"></param>
    autodoc(_In_opt_z_ const char *path,
        _In_ const collection_flags enter_flags,
        _In_ const collection_flags exit_flags);

    /// <summary>
    /// Initialises a new instance.
    /// </summary>
    /// <param name="path"></param>
    /// <param name="flags"></param>
    explicit autodoc(_In_opt_z_ const wchar_t *path,
        _In_ const collection_flags flags = collection_flags::no_sensitive);

    /// <summary>
    /// Initialises a new instance.
    /// </summary>
    /// <param name="path"></param>
    /// <param name="enter_flags"></param>
    /// <param name="exit_flags"></param>
    autodoc(_In_opt_z_ const wchar_t *path,
        _In_ const collection_flags enter_flags,
        _In_ const collection_flags exit_flags);

    autodoc(const autodoc&) = delete;

    /// <summary>
    /// Finalises the instance.
    /// </summary>
    ~autodoc(void) noexcept;

    autodoc& operator =(const autodoc&) = delete;

private:

    struct collect *_collect;
};

#endif /* defined(__cplusplus) */

LYRA_NAMESPACE_END


/// <summary>
/// Collects the raw data from the system and writes it to the specified file.
/// </summary>
/// <param name="path">The path to the file where the data should be stored.
/// </param>
/// <param name="flags">The flags specifying which data to collect.</param>
/// <returns>Zero in case of success, an error code otherwise.</returns>
#if defined(__cplusplus)
extern "C" int LYRA_API autodoc_write_raw_a(
    _In_z_ const char *path,
    _In_ const LYRA_NAMESPACE::collection_flags flags);
#else /* defined(__cplusplus) */
int LYRA_API autodoc_write_raw_a(
    _In_z_ const char *path,
    _In_ const enum collection_flags flags);
#endif /* defined(__cplusplus) */

/// <summary>
/// Collects the raw data from the system and writes it to the specified file.
/// </summary>
/// <param name="path">The path to the file where the data should be stored.
/// </param>
/// <param name="flags">The flags specifying which data to collect.</param>
/// <returns>Zero in case of success, an error code otherwise.</returns>
#if defined(__cplusplus)
extern "C" int LYRA_API autodoc_write_raw_w(
    _In_z_ const wchar_t *path,
    _In_ const LYRA_NAMESPACE::collection_flags flags);
#else /* defined(__cplusplus) */
int LYRA_API autodoc_write_raw_w(
    _In_z_ const wchar_t *path,
    _In_ const enum collection_flags flags);
#endif /* defined(__cplusplus) */

#if defined(_WIN32) && (defined(UNICODE) || defined(_UNICODE))
#define autodoc_write_raw autodoc_write_raw_w
#else /* defined(_WIN32) && (defined(UNICODE) || defined(_UNICODE)) */
#define autodoc_write_raw autodoc_write_raw_a
#endif /* defined(_WIN32) && (defined(UNICODE) || defined(_UNICODE)) */

#endif /* !defined(_LYRA_AUTODOC_H) */
