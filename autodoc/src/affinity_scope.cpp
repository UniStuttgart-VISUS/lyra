// <copyright file="affinity_scope.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "affinity_scope.h"

#include <cassert>

#include <os_cpu_info.h>

#include "visus/autodoc/on_exit.h"
#include "visus/autodoc/trace.h"


/*
 * LYRA_DETAIL_NAMESPACE::affinity_scope::affinity_scope
 */
LYRA_DETAIL_NAMESPACE::affinity_scope::affinity_scope(
        _In_ const mask_type mask,
        _In_ const std::size_t size) noexcept
        : _valid(false) {
#if !defined(_WIN32)
    this->_mask = nullptr;
    this->_size = 0;
#endif /* !defined(_WIN32) */
    this->set(mask, size);
}


/*
 * LYRA_DETAIL_NAMESPACE::affinity_scope::~affinity_scope
 */
LYRA_DETAIL_NAMESPACE::affinity_scope::~affinity_scope(void) noexcept {
    if (this->_valid) {
#if defined(_WIN32)
        this->set(this->_mask, 0);
#else /* defined(_WIN32) */
        this->set(this->_mask, this->_size);
#endif /* defined(_WIN32) */
        assert(this->_valid);
    }

#if !defined(_WIN32)
    if (this->_mask != nullptr) {
        CPU_FREE(this->_mask);
    }
#endif /* !defined(_WIN32) */
}


/*
 * LYRA_DETAIL_NAMESPACE::affinity_scope::set
 */
void LYRA_DETAIL_NAMESPACE::affinity_scope::set(
        _In_ const mask_type mask,
        _In_ const std::size_t size) noexcept {
#if defined(_WIN32)
#if (_WIN32_WINNT >= 0x0601)
    this->_valid = (::SetThreadGroupAffinity(::GetCurrentThread(), &mask,
        &this->_mask) != FALSE);
    if (!this->_valid) {
        LYRA_TRACE(_T("Failed to set thread group affinity: 0x%x"),
            ::GetLastError());
    }

#else /* (_WIN32_WINNT >= 0x0601) */
    this->_mask = ::SetThreadAffinityMask(::GetCurrentThread(), mask);
    this->_valid = (this->_mask != 0);
#endif /* (_WIN32_WINNT >= 0x0601) */

#else /* defined(_WIN32) */
    // Clear any previous allocation. The reason why we preserve the pointer
    // until we leave the scope is that the destructor calls us with the old
    // mask stored in the instance. If we free that right away, the call to
    // sched_setaffinity at the end will use invalid memory.
    auto prev_mask = this->_mask;
    LYRA_ON_EXIT([prev_mask](void) {
        if (prev_mask != nullptr) {
            CPU_FREE(prev_mask);
        }
    });

    // Allocate space to preserve the old mask.
    const auto cnt = get_os_max_cpus();
    this->_mask = CPU_ALLOC(cnt);
    this->_valid = (this->_mask != nullptr);

    // Preserve the old mask.
    if (this->_valid) {
        this->_size = CPU_ALLOC_SIZE(cnt);
        CPU_ZERO_S(this->_size, this->_mask);
        this->_valid = (::sched_getaffinity(::gettid(), this->_size,
            this->_mask) == 0);
    }

    // Set the new mask, but only if we were able to preserve the old one.
    if (this->_valid) {
        this->_valid = (::sched_setaffinity(::gettid(), size, mask) == 0);
    }
#endif /* defined(_WIN32) */
}
