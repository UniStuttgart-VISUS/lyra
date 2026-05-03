// <copyright file="guid.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2025 - 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

/*
 * src\guid.cpp
 *
 * Copyright (C) 2013 TheLib Team (http://www.thelib.org/license)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 * - Neither the name of TheLib, TheLib Team, nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THELIB TEAM AS IS AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL THELIB TEAM BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * GUID.cpp
 *
 * Copyright (C) 2006 - 2008 by Universitaet Stuttgart (VIS). 
 * Alle Rechte vorbehalten.
 */

#include "visus/lyra/guid.h"

#include <cassert>
#include <cerrno>
#include <memory>
#include <stdexcept>
#include <system_error>

#include "visus/lyra/convert_string.h"
#include "visus/lyra/on_exit.h"


/*
 * LYRA_NAMESPACE::guid::create
 */
LYRA_NAMESPACE::guid& LYRA_NAMESPACE::guid::create(
        _Out_ guid& retval, _In_ const bool no_mac) {
#if defined(_WIN32)
    RPC_STATUS status;

    if (no_mac) {
        status = ::UuidCreate(&retval._value);
    } else {
        status = ::UuidCreateSequential(&retval._value);
    }

    if ((status != RPC_S_OK) && (status != RPC_S_UUID_LOCAL_ONLY)) {
        throw std::system_error(status, std::system_category());
    }

#else /* defined(_WIN32) */
    if (no_mac) {
        ::uuid_generate_random(retval._value);
    } else {
        ::uuid_generate_time(retval._value);
    }
#endif /* defined(_WIN32) */

    return retval;
}


/*
 * LYRA_NAMESPACE::guid::parse
 */
LYRA_NAMESPACE::guid& LYRA_NAMESPACE::guid::parse(
        _Out_ guid& retval, _In_z_ const char *str) {
    if (str == nullptr) {
        throw std::invalid_argument("A valid GUID string must be provided.");
    }

#if defined(_WIN32)
    auto s = reinterpret_cast<RPC_CSTR>(const_cast<char *>(str));
    auto status = ::UuidFromStringA(s, &retval._value);
    if (status != RPC_S_OK) {
        throw std::system_error(status, std::system_category());
    }

#else /* defined(_WIN32) */
    if (::uuid_parse(str, retval._value) == -1) {
        throw std::system_error(errno, std::system_category());
    }
#endif /* defined(_WIN32) */

    return retval;
}


/*
 * LYRA_NAMESPACE::guid::parse
 */
LYRA_NAMESPACE::guid& LYRA_NAMESPACE::guid::parse(
        _Out_ guid& retval, _In_z_ const wchar_t *str) {
    if (str == nullptr) {
        throw std::invalid_argument("A valid GUID string must be provided.");
    }

#if defined(_WIN32)
    auto s = reinterpret_cast<RPC_WSTR>(const_cast<wchar_t *>(str));
    auto status = ::UuidFromStringW(s, &retval._value);
    if (status != RPC_S_OK) {
        throw std::system_error(status, std::system_category());
    }

#else /* defined(_WIN32) */
    auto s = convert_string<char>(str);
    parse(retval, s.c_str());
#endif /* defined(_WIN32) */

    return retval;
}


/*
 * LYRA_NAMESPACE::guid::zero
 */
const LYRA_NAMESPACE::guid LYRA_NAMESPACE::guid::zero;


/*
 * LYRA_NAMESPACE::guid::guid
 */
LYRA_NAMESPACE::guid::guid(void) noexcept {
    ::memset(&this->_value, 0, sizeof(this->_value));
}


/*
 * LYRA_NAMESPACE::guid::guid
 */
LYRA_NAMESPACE::guid::guid(_In_reads_(16) const std::uint8_t rhs[16]) {
    static_assert(sizeof(this->_value) == 16, "The native representation of "
        "a GUID is assumed to comprise 16 bytes.");

    if (rhs == nullptr) {
        throw std::invalid_argument("A valid pointer to 16 bytes of GUID data "
            "must be provided.");
    }

    auto dst = reinterpret_cast<std::uint8_t *>(std::addressof(this->_value));

    if (dst != rhs) {
        ::memcpy(dst, rhs, sizeof(this->_value));
    }
}


/*
 * LYRA_NAMESPACE::guid::guid
 */
LYRA_NAMESPACE::guid::guid(
        _In_ const std::uint8_t b1, _In_ const std::uint8_t b2,
        _In_ const std::uint8_t b3, _In_ const std::uint8_t b4,
        _In_ const std::uint8_t b5, _In_ const std::uint8_t b6,
        _In_ const std::uint8_t b7, _In_ const std::uint8_t b8,
        _In_ const std::uint8_t b9, _In_ const std::uint8_t b10,
        _In_ const std::uint8_t b11, _In_ const std::uint8_t b12,
        _In_ const std::uint8_t b13, _In_ const std::uint8_t b14,
        _In_ const std::uint8_t b15, _In_ const std::uint8_t b16) noexcept {
    auto dst = reinterpret_cast<std::uint8_t *>(std::addressof(this->_value));
#define __ASSIGN_BYTE(i) dst[i - 1] = b##i
    __ASSIGN_BYTE(1);
    __ASSIGN_BYTE(2);
    __ASSIGN_BYTE(3);
    __ASSIGN_BYTE(4);
    __ASSIGN_BYTE(5);
    __ASSIGN_BYTE(6);
    __ASSIGN_BYTE(7);
    __ASSIGN_BYTE(8);
    __ASSIGN_BYTE(9);
    __ASSIGN_BYTE(10);
    __ASSIGN_BYTE(11);
    __ASSIGN_BYTE(12);
    __ASSIGN_BYTE(13);
    __ASSIGN_BYTE(14);
    __ASSIGN_BYTE(15);
    __ASSIGN_BYTE(16);
#undef __ASSIGN_BYTE
}


/*
 * LYRA_NAMESPACE::guid::guid
 */
LYRA_NAMESPACE::guid::guid(_In_ const std::uint32_t i,
        _In_ const uint16_t s1, _In_ const uint16_t s2,
        _In_ const std::uint8_t b1, _In_ const std::uint8_t b2,
        _In_ const std::uint8_t b3, _In_ const std::uint8_t b4,
        _In_ const std::uint8_t b5, _In_ const std::uint8_t b6,
        _In_ const std::uint8_t b7, _In_ const std::uint8_t b8) noexcept {
    auto dst = reinterpret_cast<std::uint8_t *>(std::addressof(this->_value));
#define __ASSIGN_VALUE(v) ::memcpy(dst, &v, sizeof(v)); dst += sizeof(v)
    __ASSIGN_VALUE(i);
    __ASSIGN_VALUE(s1);
    __ASSIGN_VALUE(s2);
    __ASSIGN_VALUE(b1);
    __ASSIGN_VALUE(b2);
    __ASSIGN_VALUE(b3);
    __ASSIGN_VALUE(b4);
    __ASSIGN_VALUE(b5);
    __ASSIGN_VALUE(b6);
    __ASSIGN_VALUE(b7);
    __ASSIGN_VALUE(b8);
#undef __ASSIGN_VALUE
}


/*
 * LYRA_NAMESPACE::guid::guid
 */
LYRA_NAMESPACE::guid::guid(_In_ const std::uint32_t i,
        _In_ const std::uint16_t s1, _In_ const uint16_t s2,
        _In_ const uint16_t s3,
        _In_ const std::uint8_t b1, _In_ const std::uint8_t b2,
        _In_ const std::uint8_t b3, _In_ const std::uint8_t b4,
        _In_ const std::uint8_t b5, _In_ const std::uint8_t b6) noexcept {
    auto dst = reinterpret_cast<std::uint8_t *>(std::addressof(this->_value));
#define __ASSIGN_VALUE(v) ::memcpy(dst, &v, sizeof(v)); dst += sizeof(v)
    __ASSIGN_VALUE(i);
    __ASSIGN_VALUE(s1);
    __ASSIGN_VALUE(s2);
    __ASSIGN_VALUE(s3);
    __ASSIGN_VALUE(b1);
    __ASSIGN_VALUE(b2);
    __ASSIGN_VALUE(b3);
    __ASSIGN_VALUE(b4);
    __ASSIGN_VALUE(b5);
    __ASSIGN_VALUE(b6);
#undef __ASSIGN_VALUE
}


/*
 * LYRA_NAMESPACE::guid::guid
 */
LYRA_NAMESPACE::guid::guid(_In_ const std::uint32_t i,
        _In_ const std::uint16_t s1, const std::uint16_t s2,
        _In_reads_(8) const std::uint8_t b[8]) {
    if (b == nullptr) {
        throw std::invalid_argument("A valid pointer to the final eight bytes "
            "of the GUID must be provided.");
    }

    auto dst = reinterpret_cast<std::uint8_t *>(std::addressof(this->_value));
#define __ASSIGN_VALUE(v) ::memcpy(dst, &v, sizeof(v)); dst += sizeof(v)
    __ASSIGN_VALUE(i);
    __ASSIGN_VALUE(s1);
    __ASSIGN_VALUE(s2);
#undef __ASSIGN_VALUE
    ::memcpy(dst, b, 8);
}


/*
 * LYRA_NAMESPACE::guid::guid
 */
LYRA_NAMESPACE::guid::guid(_In_ const std::uint32_t i,
        _In_ const std::uint16_t s1, const std::uint16_t s2,
        _In_ const uint16_t s3,
        _In_reads_(6) const std::uint8_t b[6]) {
    if (b == nullptr) {
        throw std::invalid_argument("A valid pointer to the final six bytes "
            "of the GUID must be provided.");
    }

    auto dst = reinterpret_cast<std::uint8_t *>(std::addressof(this->_value));
#define __ASSIGN_VALUE(v) ::memcpy(dst, &v, sizeof(v)); dst += sizeof(v)
    __ASSIGN_VALUE(i);
    __ASSIGN_VALUE(s1);
    __ASSIGN_VALUE(s2);
    __ASSIGN_VALUE(s3);
#undef __ASSIGN_VALUE
    ::memcpy(dst, b, 6);
}

/*
 * LYRA_NAMESPACE::guid::clear
 */
void LYRA_NAMESPACE::guid::clear(void) noexcept {
    ::memset(&this->_value, 0, sizeof(this->_value));
}


/*
 * LYRA_NAMESPACE::guid::empty
 */
bool LYRA_NAMESPACE::guid::empty(void) const noexcept {
    return (*this == guid::zero);
}


/*
 * LYRA_NAMESPACE::guid::to_string
 */
std::size_t LYRA_NAMESPACE::guid::to_string(
        _Out_writes_opt_z_(cnt) char *dst,
        _In_ const std::size_t cnt) const {
#if defined(_WIN32)
    auto src = const_cast<::GUID *>(std::addressof(this->_value));

    RPC_CSTR tmp = nullptr;
    LYRA_ON_EXIT([&tmp](void) {
        if (tmp != nullptr) {
            ::RpcStringFreeA(&tmp);
        }
    });

    auto status = ::UuidToStringA(src, &tmp);
    if (status != RPC_S_OK) {
        throw std::system_error(status, std::system_category());
    }

    auto hack = reinterpret_cast<char *>(tmp);
    auto retval = ::strlen(hack);
    if ((dst != nullptr) && (cnt >= retval)) {
        ::strcpy_s(dst, cnt, hack);
    }

    return retval;

#else /* defined(_WIN32) */
    const auto retval = static_cast<std::size_t>(36 + 1);

    if ((dst != nullptr) && (cnt >= retval)) {
        ::uuid_unparse(this->_value, dst);
    }

    return retval;
#endif /* defined(_WIN32) */
}


#if defined(_WIN32)
/*
 * LYRA_NAMESPACE::guid::to_string
 */
std::size_t LYRA_NAMESPACE::guid::to_string(
        _Out_writes_opt_z_(cnt) wchar_t *dst,
        _In_ const std::size_t cnt) const {
    auto src = const_cast<::GUID *>(std::addressof(this->_value));

    RPC_WSTR tmp = nullptr;
    LYRA_ON_EXIT([&tmp](void) {
        if (tmp != nullptr) {
            ::RpcStringFreeW(&tmp);
        }
    });

    auto status = ::UuidToStringW(src, &tmp);
    if (status != RPC_S_OK) {
        throw std::system_error(status, std::system_category());
    }

    auto hack = reinterpret_cast<wchar_t *>(tmp);
    auto retval = ::wcslen(hack);
    if ((dst != nullptr) && (cnt >= retval)) {
        ::wcscpy_s(dst, cnt, hack);
    }

    return retval;
}
#endif /* defined(_WIN32) */


/*
 * LYRA_NAMESPACE::guid::operator =
 */
LYRA_NAMESPACE::guid& LYRA_NAMESPACE::guid::operator =(
        _In_ const guid& rhs) noexcept {
    if (this != std::addressof(rhs)) {
        ::memcpy(&this->_value, &rhs._value, sizeof(this->_value));
    }
    return *this;
}


/*
 * LYRA_NAMESPACE::guid::operator =
 */
LYRA_NAMESPACE::guid& LYRA_NAMESPACE::guid::operator =(
        _In_ const value_type& rhs) noexcept {
    auto dst = std::addressof(this->_value);
    auto src = std::addressof(rhs);

    if (src != dst) {
        ::memcpy(dst, src, sizeof(this->_value));
    }

    return *this;
}


/*
 * LYRA_NAMESPACE::guid::operator =
 */
LYRA_NAMESPACE::guid& LYRA_NAMESPACE::guid::operator =(
        _In_ const value_type *rhs) {
    if (rhs == nullptr) {
        throw std::invalid_argument("A valid pointer to a GUID must be "
            "provided as input.");
    }

    auto dst = std::addressof(this->_value);

    if (dst != rhs) {
        ::memcpy(dst, rhs, sizeof(this->_value));
    }

    return *this;
}


/*
 * LYRA_NAMESPACE::guid::operator ==
 */
bool LYRA_NAMESPACE::guid::operator ==(_In_ const guid& rhs) const noexcept {
    if (this == std::addressof(rhs)) {
        return true;
    }

#if defined(_WIN32)
    return (::InlineIsEqualGUID(this->_value, rhs._value) != 0);
#else /* defined(_WIN32) */
    return (::uuid_compare(this->_value, rhs._value) == 0);
#endif /* defined(_WIN32) */
}


/*
 * LYRA_NAMESPACE::guid::operator <
 */
bool LYRA_NAMESPACE::guid::operator <(_In_ const guid& rhs) const noexcept {
    return (::memcmp(std::addressof(this->_value),
        std::addressof(rhs._value),
        sizeof(this->_value)) < 0);
}


/*
 * LYRA_NAMESPACE::guid::operator <=
 */
bool LYRA_NAMESPACE::guid::operator <=(_In_ const guid& rhs) const noexcept {
    return (::memcmp(std::addressof(this->_value),
        std::addressof(rhs._value),
        sizeof(this->_value)) <= 0);
}
