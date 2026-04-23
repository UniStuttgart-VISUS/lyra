// <copyright file="file.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_FILE_H)
#define _LYRA_FILE_H
#pragma once

#include <cinttypes>
#include <memory>
#include <string>
#include <vector>

#if defined(_WIN32)
#include <wil/resource.h>
#endif /* defined(_WIN32) */

#include "visus/lyra/api.h"

#include "unique_fd.h"


LYRA_DETAIL_NAMESPACE_BEGIN

#if defined(_WIN32)
/// <summary>
/// A RAII wrapper for file handles.
/// </summary>
typedef wil::unique_hfile unique_file;
#else /* defined(_WIN32) */
/// <summary>
/// A RAII wrapper for file descriptors.
/// </summary>
typedef unique_fd unique_file;
#endif /* defined(_WIN32) */


#if defined(_WIN32)
/// <summary>
/// Answer the size of the given file, in bytes.
/// </summary>
/// <param name="handle"></param>
/// <returns></returns>
/// <exception cref="std::system_error"></exception>
std::size_t LYRA_API file_size(_In_ HANDLE handle);
#endif /* defined (_WIN32) */

/// <summary>
/// Answer the size of the given file, in bytes.
/// </summary>
/// <param name="fd"></param>
/// <returns></returns>
/// <exception cref="std::system_error"></exception>
std::size_t LYRA_API file_size(_In_ int fd);

/// <summary>
/// Answer the size of the given file, in bytes.
/// </summary>
/// <param name="path"></param>
/// <returns></returns>
/// <exception cref="std::system_error"></exception>
std::size_t LYRA_API file_size(_In_z_ const char *path);

/// <summary>
/// Answer the size of the given file, in bytes.
/// </summary>
/// <param name="path"></param>
/// <returns></returns>
/// <exception cref="std::system_error"></exception>
std::size_t LYRA_API file_size(_In_z_ const wchar_t *path);

/// <summary>
/// Opens an existing file for reading.
/// </summary>
/// <param name="path"></param>
/// <returns></returns>
unique_file LYRA_API open_read(_In_z_ const char *path);

/// <summary>
/// Opens an existing file for reading.
/// </summary>
/// <param name="path"></param>
/// <returns></returns>
unique_file LYRA_API open_read(_In_z_ const wchar_t *path);

/// <summary>
/// Read all contents of a binary file.
/// </summary>
std::vector<std::uint8_t> LYRA_API read_all_bytes(_In_ const unique_file& file);

//#if defined(_WIN32)
///// <summary>
///// Return the requested registry value below <paramref name="key" /> to
///// <paramref name="dst" /> and its type to <see cref="type" />.
///// </summary>
//LSTATUS read_reg_value(std::vector<std::uint8_t>& dst, DWORD& type,
//    HKEY key, const char *name);
//#endif /* defined (_WIN32) */

LYRA_DETAIL_NAMESPACE_END

#endif /* !defined(_LYRA_FILE_H) */
