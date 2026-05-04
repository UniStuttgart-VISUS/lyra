// <copyright file="autodoc.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_AUTODOC_H)
#define _LYRA_AUTODOC_H
#pragma once

#include "visus/autodoc/collection_flags.h"


#if defined(__cplusplus)
LYRA_NAMESPACE_BEGIN

/// <summary>
/// A RAII helper for collecting and persisting system information on
/// construction and destruction, respectively.
/// </summary>
class LYRA_API autodoc final {

public:

    autodoc(const autodoc&) = delete;

    autodoc& operator =(const autodoc&) = delete;

private:

    collection_flags _flags;
};

LYRA_NAMESPACE_END
#endif /* defined(__cplusplus) */


/// <summary>
/// Collects the raw data from the system and writes it to the specified file.
/// </summary>
/// <param name="path"></param>
/// <param name="flags"></param>
/// <returns>Zero in case of success, an error code otherwise.</returns>
extern "C" int LYRA_API autodoc_write_raw_a(
    _In_z_ const char *path,
    _In_ const LYRA_NAMESPACE::collection_flags flags);

#if defined(_WIN32)
extern "C" int LYRA_API autodoc_write_raw_w(
    _In_z_ const wchar_t *path,
    _In_ const LYRA_NAMESPACE::collection_flags flags);
#endif /* defined(_WIN32) */


#if defined(_WIN32) && (defined(UNICODE) || defined(_UNICODE))
#define autodoc_write_raw autodoc_write_raw_w
#else /* defined(_WIN32) && (defined(UNICODE) || defined(_UNICODE)) */
#define autodoc_write_raw autodoc_write_raw_a
#endif /* defined(_WIN32) && (defined(UNICODE) || defined(_UNICODE)) */

#endif /* !defined(_LYRA_AUTODOC_H) */
