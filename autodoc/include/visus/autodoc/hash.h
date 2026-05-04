// <copyright file="hash.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_HASH_H)
#define _LYRA_HASH_H
#pragma once

#include <cinttypes>
#include <cstdlib>

#if defined(_WIN32)
#include <Windows.h>
#include <bcrypt.h>
#endif /* defined(_WIN32) */

#include "visus/autodoc/api.h"


LYRA_NAMESPACE_BEGIN

#if defined(_WIN32)
/// <summary>
/// The type used to identify a hash algorithm.
/// </summary>
typedef const wchar_t *hash_algorithm;

constexpr auto sha256 = BCRYPT_SHA256_ALGORITHM;
constexpr auto sha512 = BCRYPT_SHA512_ALGORITHM;

#else /* !defined(_WIN32) */
/// <summary>
/// Defines possible hash algorithms to be used with the <see cref="hash" />
/// class.
/// </summary>
enum hash_algorithm {
    sha256,
    sha512
};
#endif /* defined(_WIN32) */


/// <summary>
/// A utility class for hashing data.
/// </summary>
class LYRA_API hash final {

public:

    /// <summary>
    /// Initialises a new instance.
    /// </summary>
    /// <param name="algorithm">The hash algorithm to use.</param>
    explicit hash(_In_ const hash_algorithm algorithm);

    hash(const hash&) = delete;

    /// <summary>
    /// Finalises the instance.
    /// </summary>
    ~hash(void) noexcept;

    /// <summary>
    /// Appends <paramref name="cnt" /> bytes of data to the hash.
    /// </summary>
    /// <param name="data"></param>
    /// <param name="cnt"></param>
    /// <returns><see langword="true" /> if the data was successfully appended,
    /// <see langword="false" /> otherwise.</returns>
    bool append(_In_reads_(cnt) const void *data, _In_ std::size_t cnt);

    /// <summary>
    /// Finishes the hash and returns the pointer to the result. No further data
    /// can be appended to the hash after finishing it.
    /// </summary>
    /// <returns>A pointer to the hash or <see langword="nullptr" /> if the
    /// operation failed.</returns>
    _Ret_maybenull_ const std::uint8_t *finish(void) noexcept;

    /// <summary>
    /// Answer the length of the hash in bytes.
    /// </summary>
    /// <returns></returns>
    inline std::size_t length(void) const noexcept {
        return this->_length;
    }

    hash& operator =(const hash&) = delete;

private:

    /// <summary>
    /// Clears all internal state except for the result.
    /// </summary>
    void clear(void) noexcept;

#if defined(_WIN32)
    BCRYPT_ALG_HANDLE _algorithm;
    BCRYPT_HASH_HANDLE _hash;
    PBYTE _object;
#else /* !defined(_WIN32) */
    void *_context;
#endif /* defined(_WIN32) */

    std::uint8_t *_buffer;
    std::size_t _length;
};

LYRA_NAMESPACE_END

#endif /* !defined(_LYRA_HASH_H) */
