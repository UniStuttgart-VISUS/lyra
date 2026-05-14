// <copyright file="affinity_scope.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "visus/autodoc/affinity_scope.h"

#include <cassert>

#include <os_cpu_info.h>

#include "visus/autodoc/on_exit.h"
#include "visus/autodoc/trace.h"


/*
 * LYRA_NAMESPACE::affinity_scope::~affinity_scope
 */
LYRA_NAMESPACE::affinity_scope::~affinity_scope(void) noexcept {
    if (*this) {
        this->set(this->_mask);
    }
}


/*
 * LYRA_NAMESPACE::affinity_scope::set
 */
void LYRA_NAMESPACE::affinity_scope::set(
        _In_ const affinity_mask& mask) noexcept {
#if defined(_WIN32) && (_WIN32_WINNT >= 0x0601)
    GROUP_AFFINITY prev;

    if (::SetThreadGroupAffinity(::GetCurrentThread(), mask, &prev)) {
        this->_mask = affinity_mask(prev);
    } else {
        LYRA_TRACE(_T("Failed to set thread group affinity: 0x%x"),
            ::GetLastError());
        this->_mask.clear();
    }

#elif defined(_WIN32)
    const auto prev = ::SetThreadAffinityMask(::GetCurrentThread(), mask);
    if (prev != 0) {
        this->_mask = affinity_mask(prev);
    } else {
        LYRA_TRACE(_T("Failed to set thread affinity mask: 0x%x"),
            ::GetLastError());
        this->_mask.clear();
    }

#else /* defined(_WIN32) && (_WIN32_WINNT >= 0x0601) */
    try {
        this->_mask = affinity_mask::thread();
        assert(this->_mask);
    } catch (...) {
        LYRA_TRACE(_T("Failed to get current thread affinity mask."));
        this->_mask.clear();
    }

    if (this->_mask) {
        if (::sched_setaffinity(::gettid(), mask.size(), mask.get()) != 0) {
            LYRA_TRACE(_T("Failed to set thread affinity mask: 0x%x"), errno);
            this->_mask.clear();
        }
    }
#endif /* defined(_WIN32) && (_WIN32_WINNT >= 0x0601) */
}
