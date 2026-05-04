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

#include "visus/autodoc/api.h"

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

/// <summary>
/// Computes a hash of a file's contents.
/// </summary>
/// <param name="file"></param>
/// <returns>The hexadecimal string representation of the hash.
/// </returns>
LYRA_TEST_API std::string file_hash(_In_ const unique_file& file);

/// <summary>
/// Computes a hash of a file's contents.
/// </summary>
/// <typeparam name="TChar"></typeparam>
/// <param name="path"></param>
/// <returns>The hexadecimal string representation of the hash.
/// </returns>
template<class TChar> std::string file_hash(_In_z_ const TChar *path);

#if defined(_WIN32)
/// <summary>
/// Answer the size of the given file, in bytes.
/// </summary>
/// <param name="handle"></param>
/// <returns></returns>
/// <exception cref="std::system_error"></exception>
LYRA_TEST_API std::size_t file_size(_In_ HANDLE handle);
#endif /* defined (_WIN32) */

/// <summary>
/// Answer the size of the given file, in bytes.
/// </summary>
/// <param name="fd"></param>
/// <returns></returns>
/// <exception cref="std::system_error"></exception>
LYRA_TEST_API std::size_t file_size(_In_ int fd);

/// <summary>
/// Answer the size of the given file, in bytes.
/// </summary>
/// <param name="path"></param>
/// <returns></returns>
/// <exception cref="std::system_error"></exception>
LYRA_TEST_API std::size_t file_size(_In_z_ const char *path);

/// <summary>
/// Answer the size of the given file, in bytes.
/// </summary>
/// <param name="path"></param>
/// <returns></returns>
/// <exception cref="std::system_error"></exception>
LYRA_TEST_API std::size_t file_size(_In_z_ const wchar_t *path);

/// <summary>
/// Resolves any links in the given <paramref name="path" /> and returns
/// the final path.
/// </summary>
/// <param name="path"></param>
/// <returns></returns>
LYRA_TEST_API std::string final_path(_In_z_ const char *path);

/// <summary>
/// Opens an existing file for reading.
/// </summary>
/// <param name="path"></param>
/// <returns></returns>
LYRA_TEST_API unique_file open_read(_In_z_ const char *path);

/// <summary>
/// Opens an existing file for reading.
/// </summary>
/// <param name="path"></param>
/// <returns></returns>
LYRA_TEST_API unique_file open_read(_In_z_ const wchar_t *path);

/// <summary>
/// Read all contents of a binary file.
/// </summary>
/// <param name="file"></param>
/// <returns></returns>
LYRA_TEST_API std::vector<std::uint8_t> read_all_bytes(
    _In_ const unique_file& file);

/// <summary>
/// Read all contents of a binary file.
/// </summary>
/// <param name="path"></param>
/// <returns></returns>
inline std::vector<std::uint8_t> read_all_bytes(_In_z_ const char *path) {
    return read_all_bytes(open_read(path));
}

/// <summary>
/// Read all contents of a binary file.
/// </summary>
/// <param name="path"></param>
/// <returns></returns>
inline  std::vector<std::uint8_t> read_all_bytes(_In_z_ const wchar_t *path) {
    return read_all_bytes(open_read(path));
}

//#if defined(_WIN32)
///// <summary>
///// Return the requested registry value below <paramref name="key" /> to
///// <paramref name="dst" /> and its type to <see cref="type" />.
///// </summary>
//LSTATUS read_reg_value(std::vector<std::uint8_t>& dst, DWORD& type,
//    HKEY key, const char *name);
//#endif /* defined (_WIN32) */

LYRA_DETAIL_NAMESPACE_END

#include "file.inl"

#endif /* !defined(_LYRA_FILE_H) */
