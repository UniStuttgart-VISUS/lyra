// <copyright file="affinity_mask.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "visus/autodoc/affinity_mask.h"

#include <algorithm>
#include <cassert>
#include <cerrno>
#include <memory>
#include <system_error>

#include "os_cpu_info.h"


/*
 * LYRA_NAMESPACE::affinity_mask::thread
 */
LYRA_NAMESPACE::affinity_mask LYRA_NAMESPACE::affinity_mask::thread(
        _In_ const thread_handle thread) {
    affinity_mask retval;

#if defined(_WIN32)
#if (_WIN32_WINNT >= 0x0601)
    if (!::GetThreadGroupAffinity(thread, &retval._mask)) {
        throw std::system_error(::GetLastError(), std::system_category());
    }
#else /* (_WIN32_WINNT >= 0x0601) */
    DWORD_PTR probe = 1;

    while (probe != 0) {
        const auto mask = ::SetThreadAffinityMask(thread, probe);

        if (mask != 0) {
            // If we were able to set the affinity mask, the return value was
            // the previous mask, which is what we want to know. Reset
            // the mask and exit the loop.
            ::SetThreadAffinityMask(thread, mask);
            retval._mask = mask;
            probe = 0;

        } else {
            const auto error = ::GetLastError();

            if (error == ERROR_INVALID_PARAMETER) {
                // Our probe was invalid as is included a CPU that is not
                // available to the process. Therefore, we try the next.
                probe <<= 1;

            } else {
                // This is a fatal error, we will not able to determine the
                // thread affinity.
                throw std::system_error(error, std::system_category());
            }
        }
    }
#endif /* (_WIN32_WINNT >= 0x0601) */
#else /* defined(_WIN32) */
    const auto cnt = detail::get_os_max_cpus();

    assert(retval._mask == nullptr);
    retval._mask = CPU_ALLOC(cnt);
    if (retval._mask == nullptr) {
        throw std::bad_alloc();
    }

    retval._size = CPU_ALLOC_SIZE(cnt);
    CPU_ZERO_S(retval._size, retval._mask);

    if (::sched_getaffinity(thread, retval._size, retval._mask) != 0) {
        throw std::system_error(errno, std::system_category());
    }
#endif /* defined(_WIN32) */

    return retval;
}


/*
 * LYRA_NAMESPACE::affinity_mask::thread
 */
LYRA_NAMESPACE::affinity_mask LYRA_NAMESPACE::affinity_mask::thread(void) {
#if defined(_WIN32)
    return thread(::GetCurrentThread());
#else /* defined(_WIN32) */
    return thread(::gettid());
#endif /* defined(_WIN32) */
}


/*
 * LYRA_NAMESPACE::affinity_mask::affinity_mask
 */
LYRA_NAMESPACE::affinity_mask::affinity_mask(void) noexcept
        : _mask { } {
#if !defined(_WIN32)
    assert(this->_mask == nullptr);
    this->_size = 0;
#endif /* !defined(_WIN32) */
}


/*
 * LYRA_NAMESPACE::affinity_mask::affinity_mask
 */
LYRA_NAMESPACE::affinity_mask::affinity_mask(
        _In_ mask_type mask,
        _In_ const std::size_t size) noexcept
        : _mask(mask) {
#if !defined(_WIN32)
    constexpr auto page = CPU_ALLOC_SIZE(1);
    const auto cpus = size / page + 1;
    this->_mask = CPU_ALLOC(cpus);
    if (this->_mask != nullptr) {
        this->_size = CPU_ALLOC_SIZE(cpus);
        assert(this->_size == size);
        CPU_ZERO_S(this->_size, this->_mask);
        CPU_OR_S(this->_size, this->_mask, this->_mask, mask);
    } else {
        this->_size = 0;
    }
#endif /* !defined(_WIN32) */
}


/*
 * LYRA_NAMESPACE::affinity_mask::affinity_mask
 */
LYRA_NAMESPACE::affinity_mask::affinity_mask(
        _In_reads_(cnt) const std::size_t *indices,
        _In_ const std::size_t cnt) noexcept
        : _mask { } {
#if defined(_WIN32) && (_WIN32_WINNT >= 0x0601)
    auto groups = detail::get_processor_groups();

    // Find the processor group based on the first index. All indices of the
    // mask must belong to the same group.
    std::size_t offset = 0;
    if (cnt > 0) {
        for (auto g = 0; g < groups.size(); ++g) {
            if (*indices - offset < groups[g].ActiveProcessorCount) {
                this->_mask.Group = static_cast<WORD>(g);
                break;
            }
            offset += groups[g].ActiveProcessorCount;
        }
    } /* if (cnt > 0) */

    // Construct the mask within the group.
    for (auto i = 0; i < cnt; ++i) {
        auto idx = indices[i] - offset;

        if (idx < groups[this->_mask.Group].ActiveProcessorCount) {
            const auto bit = static_cast<KAFFINITY>(1) << idx;
            this->_mask.Mask |= bit;
        }
    }

#elif defined(_WIN32)
    assert(this->_mask == 0);
    for (std::size_t i = 0; i < groups.size(); ++i) {
        this->_mask |= static_cast<mask_type>(1) << indices[i];
    }
#else /* defined(_WIN32) && (_WIN32_WINNT >= 0x0601) */
    const auto cpus = detail::get_os_max_cpus();
    this->_mask = CPU_ALLOC(cpus);
    if (this->_mask != nullptr) {
        this->_size = CPU_ALLOC_SIZE(cpus);
        CPU_ZERO_S(this->_size, this->_mask);
        for (std::size_t i = 0; i < cnt; ++i) {
            if (indices[i] < cpus) {
                CPU_SET_S(indices[i], this->_size, this->_mask);
            }
        }
    } else {
        this->_size = 0;
    }
#endif /* defined(_WIN32) && (_WIN32_WINNT >= 0x0601) */
}


/*
 * LYRA_NAMESPACE::affinity_mask::affinity_mask
 */
LYRA_NAMESPACE::affinity_mask::affinity_mask(
        _In_ const affinity_mask& other) noexcept
        : _mask(other._mask) {
#if !defined(_WIN32)
    this->_mask = nullptr;
    this->_size = 0;
    *this = other;
#endif /* !defined(_WIN32) */
}


/*
 * LYRA_NAMESPACE::affinity_mask::affinity_mask
 */
LYRA_NAMESPACE::affinity_mask::affinity_mask(
        _Inout_ affinity_mask&& other) noexcept
        : _mask(other._mask) {
#if !defined(_WIN32)
    other._mask = nullptr;
    this->_size = other._size;
    other._size = 0;
#endif /* !defined(_WIN32) */
}


/*
 * LYRA_NAMESPACE::affinity_mask::~affinity_mask
 */
LYRA_NAMESPACE::affinity_mask::~affinity_mask(void) noexcept {
#if !defined(_WIN32)
    if (this->_mask != nullptr) {
        CPU_FREE(this->_mask);
    }
#endif /* !defined(_WIN32) */
}


/*
 * LYRA_NAMESPACE::affinity_mask::clear
 */
void LYRA_NAMESPACE::affinity_mask::clear(void) noexcept {
#if defined(_WIN32)
    ::ZeroMemory(&this->_mask, sizeof(this->_mask));
#else /* defined(_WIN32) */
    if (this->_mask != nullptr) {
        CPU_FREE(this->_mask);
        this->_mask = nullptr;
    }
    this->_size = 0;
#endif /* defined(_WIN32) */
}


/*
 * LYRA_NAMESPACE::affinity_mask::test
 */
bool LYRA_NAMESPACE::affinity_mask::test(_In_ std::size_t idx) const noexcept {
#if defined(_WIN32) && (_WIN32_WINNT >= 0x0601)
    auto groups = detail::get_processor_groups();

    for (auto g = 0; (g < groups.size()) && (g <= this->_mask.Group); ++g) {
        if (idx < groups[g].ActiveProcessorCount) {
            assert(g == this->_mask.Group);
            const auto bit = static_cast<KAFFINITY>(1) << idx;
            return ((this->_mask.Mask & bit) != 0);
        }

        idx -= groups[g].ActiveProcessorCount;
    }

    return false;
#elif defined(_WIN32)
    const auto bit = static_cast<DWORD_PTR>(1) << idx;
    return ((this->_mask & bit) != 0);
#else /* defined(_WIN32) && (_WIN32_WINNT >= 0x0601) */
    return ((this->_mask != nullptr)
        && (CPU_ISSET_S(idx, this->_size, this->_mask) != 0));
#endif /* defined(_WIN32) && (_WIN32_WINNT >= 0x0601) */
}


/*
 * LYRA_NAMESPACE::affinity_mask::operator =
 */
LYRA_NAMESPACE::affinity_mask& LYRA_NAMESPACE::affinity_mask::operator =(
        _In_ const affinity_mask& rhs) noexcept {
    if (this != std::addressof(rhs)) {
#if defined(_WIN32)
        this->_mask = rhs._mask;
#else /* defined(_WIN32) */
        this->clear();

        if ((rhs._mask != nullptr) && (rhs._size > 0)) {
            constexpr auto page = CPU_ALLOC_SIZE(1);
            const auto cpus = rhs._size / page + 1;

            this->_mask = CPU_ALLOC(cpus);
            if (this->_mask != nullptr) {
                this->_size = CPU_ALLOC_SIZE(cpus);
                assert(this->_size == rhs._size);
                CPU_ZERO_S(this->_size, this->_mask);
                CPU_OR_S(this->_size, this->_mask, this->_mask, rhs._mask);
            } else {
                this->_size = 0;
            }
        }
#endif /* defined(_WIN32) */
    }

    return *this;
}


/*
 * LYRA_NAMESPACE::affinity_mask::operator =
 */
LYRA_NAMESPACE::affinity_mask& LYRA_NAMESPACE::affinity_mask::operator =(
        _Inout_ affinity_mask&& rhs) noexcept {
    if (this != std::addressof(rhs)) {
#if defined(_WIN32)
        this->_mask = rhs._mask;
#else /* defined(_WIN32) */
        this->clear();
        this->_mask = rhs._mask;
        rhs._mask = nullptr;
        this->_size = rhs._size;
        rhs._size = 0;
#endif /* defined(_WIN32) */
    }

    return *this;
}


/*
 * LYRA_NAMESPACE::affinity_mask::operator bool(
 */
LYRA_NAMESPACE::affinity_mask::operator bool(void) const noexcept {
#if defined(_WIN32)
    return (this->_mask.Mask != 0);
#else /* defined(_WIN32) */
    return ((this->_mask != nullptr)
        && (CPU_COUNT_S(this->_size, this->_mask) > 0));
#endif /* defined(_WIN32) */
}


/*
 * LYRA_NAMESPACE::affinity_mask::operator ==
 */
bool LYRA_NAMESPACE::affinity_mask::operator ==(
        _In_ const affinity_mask& rhs) const noexcept {
#if defined(_WIN32)
    return ((this->_mask.Group == rhs._mask.Group)
        && (this->_mask.Mask == rhs._mask.Mask));
#else /* defined(_WIN32) */
    if (this->_mask == rhs._mask) {
        // Trivial accept.
        assert(this->_size == rhs._size);
        return true;
    }

    if ((this->_mask == nullptr) || (rhs._mask == nullptr)) {
        // Trivial reject.
        return false;
    }

    if (this->_size != rhs._size) {
        // Trivial reject.
        return false;
    }

    return CPU_EQUAL_S(this->_size, this->_mask, rhs._mask);
#endif /* defined(_WIN32) */
}
