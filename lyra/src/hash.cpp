// <copyright file="hash.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "visus/lyra/hash.h"

#include <cassert>

#if defined(LYRA_USE_OPENSSL)
#include <openssl/evp.h>
#endif /* defined(LYRA_USE_OPENSSL) */

#include "visus/lyra/dispatch_list.h"
#include "visus/lyra/trace.h"


#if defined(_WIN32)
/*
 * LYRA_NAMESPACE::hash::hash
 */
LYRA_NAMESPACE::hash::hash(_In_ const hash_algorithm algorithm)
        : _algorithm(NULL), _hash(NULL), _object(nullptr), _buffer(nullptr),
        _length(0) {
    assert(algorithm != nullptr);
    {
        const auto status = ::BCryptOpenAlgorithmProvider(
            &this->_algorithm, algorithm, nullptr, 0);
        LYRA_TRACE(_T("BCryptOpenAlgorithmProvider returned 0x%x."),
            status);
    }

    // Determine how big the hash object is.
    DWORD cnt_object = 0;
    {
        DWORD cnt_data = 0;
        const auto status = ::BCryptGetProperty(this->_algorithm,
            BCRYPT_OBJECT_LENGTH, reinterpret_cast<PBYTE>(&cnt_object),
            sizeof(DWORD), &cnt_data, 0);
        LYRA_TRACE(_T("BCryptGetProperty returned 0x%x."), status);
    }

    // Allocate space for the hash object.
    this->_object = new BYTE[cnt_object];

    // Determine how big the hash itself is.
    {
        DWORD cnt_hash = 0;
        DWORD cnt_data = 0;
        const auto status = ::BCryptGetProperty(this->_algorithm,
            BCRYPT_HASH_LENGTH, reinterpret_cast<PBYTE>(&cnt_hash),
            sizeof(DWORD), &cnt_data, 0);
        LYRA_TRACE(_T("BCryptGetProperty returned 0x%x."), status);
        this->_length = cnt_hash;
    }

    // Allocate space for the hash.
    this->_buffer = new std::uint8_t[this->_length];
    {
        const auto status = ::BCryptCreateHash(this->_algorithm, &this->_hash,
            this->_object, cnt_object, nullptr, 0, 0);
        LYRA_TRACE(_T("BCryptCreateHash returned 0x%x."), status);
    }
}

#else
/*
 * LYRA_NAMESPACE::hash::hash
 */
LYRA_NAMESPACE::hash::hash(_In_ const hash_algorithm algorithm)
        : _context(nullptr), _buffer(nullptr) {
#if defined(LYRA_USE_OPENSSL)
    auto context = ::EVP_MD_CTX_create();
    if (context != nullptr) {
        this->_context = context;
        const EVP_MD *type = nullptr;

        switch (algorithm) {
            case hash_algorithm::sha256:
                type = ::EVP_sha256();
                break;

            case hash_algorithm::sha512:
                type = ::EVP_sha512();
                break;

            default:
                assert(false);
                this->clear();
                break;
        }

        if ((type != nullptr) && !::EVP_DigestInit(context, type)) {
            this->clear();
        }

        if (context != nullptr) {
            assert(type != nullptr);
            this->_length = static_cast<std::size_t>(::EVP_MD_size(type));
            this->_buffer = new std::uint8_t[this->_length];
        }
    }
#endif /* defined(LYRA_USE_OPENSSL) */
}
#endif /* defined(_WIN32) */


/*
 * LYRA_NAMESPACE::hash::~hash
 */
LYRA_NAMESPACE::hash::~hash(void) noexcept {
    this->clear();
    delete[] this->_buffer;
}


/*
 * LYRA_NAMESPACE::hash::append
 */
bool LYRA_NAMESPACE::hash::append(_In_reads_(cnt) const void *data,
        _In_ std::size_t cnt) {
    if (data == nullptr) {
        return false;
    }

#if defined(_WIN32)
    const auto status = ::BCryptHashData(
        this->_hash,
        static_cast<PBYTE>(const_cast<void *>(data)),
        static_cast<ULONG>(cnt),
        0);
    return (status >= 0);
#elif defined(LYRA_USE_OPENSSL)
    assert(this->_context != nullptr);
    auto context = static_cast<EVP_MD_CTX *>(this->_context);
    return (::EVP_DigestUpdate(context, data, cnt) != 0);

#else /* defined(_WIN32) */
    return false;
#endif /* defined(_WIN32) */
}


/*
 * LYRA_NAMESPACE::hash::finish
 */
_Ret_maybenull_ const std::uint8_t *LYRA_NAMESPACE::hash::finish(
        void) noexcept {
#if defined(_WIN32)
    const auto status = ::BCryptFinishHash(this->_hash, this->_buffer,
        static_cast<ULONG>(this->_length), 0);
    this->clear();
    return (status >= 0) ? this->_buffer : nullptr;
#elif defined(LYRA_USE_OPENSSL)
    assert(this->_context != nullptr);
    auto context = static_cast<EVP_MD_CTX *>(this->_context);
    const auto status = ::EVP_DigestFinal(context, this->_buffer, nullptr);
    this->clear();
    return (status != 0) ? this->_buffer : nullptr;

#else /* defined(_WIN32) */
    return nullptr;
#endif /* defined(_WIN32) */
}


/*
 * LYRA_NAMESPACE::hash::clear
 */
void LYRA_NAMESPACE::hash::clear(void) noexcept {
#if defined(_WIN32)
    if (this->_hash != NULL) {
        ::BCryptDestroyHash(this->_hash);
        this->_hash = NULL;
    }
    if (this->_object != nullptr) {
        ::SecureZeroMemory(this->_object, sizeof(this->_object));
        delete[] this->_object;
        this->_object = nullptr;
    }
    if (this->_algorithm != NULL) {
        ::BCryptCloseAlgorithmProvider(this->_algorithm, 0);
        this->_algorithm = NULL;
    }
#elif defined(LYRA_USE_OPENSSL)
    if (this->_context != nullptr) {
        auto context = static_cast<EVP_MD_CTX *>(this->_context);
        ::EVP_MD_CTX_destroy(context);
        this->_context = nullptr;
    }
#endif /* defined(_WIN32) */
}
