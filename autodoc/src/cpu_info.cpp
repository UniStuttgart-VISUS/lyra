// <copyright file="cpu_info.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2025 - 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "visus/autodoc/cpu_info.h"

#include <cassert>
#include <cerrno>
#include <cstring>
#include <system_error>

#if defined(_WIN32)
#include <intrin.h>

#if defined(_M_ARM)
#include <armintr.h>
#include <arm_neon.h>
#endif /* defined(_M_ARM) */

#if defined (_M_ARM64)
#include <arm64intr.h>
#include <arm64_neon.h>
#endif /* defined(_M_ARM64) */

#else /* defined(_WIN32) */
#include <cpuid.h>
#endif /* defined(_WIN32) */

#include "visus/autodoc/trace.h"


// List of derived from
// https://stackoverflow.com/questions/152016/detecting-cpu-architecture-compile-time
#if defined(__x86_64__) || defined(__x86_64) || defined(__amd64__)\
    || defined(__amd64) || defined(_M_X64) || defined(_M_AMD64)\
    || defined(i386) || defined(__i386) || defined(__i386__)\
    || defined(__i486__) || defined(__i586__) || defined(__i686__)\
    || defined(__IA32__)|| defined(_M_I86) || defined(_M_IX86)\
    || defined(__X86__) || defined(_X86_) || defined(__THW_INTEL__)\
    || defined(__I86__) || defined(__INTEL__) || defined(__386)
#define _LYRA_X86
#endif /* defined(__x86_64__) || ... */


#if !defined(_WIN32)
/// <summary>
/// Port of Windows's __cpuidex function for Linux.
/// </summary>
static void __cpuidex(_Out_writes_(4) int retval[4],
        _In_ const int leaf, _In_ const int subleaf) noexcept {
    if (subleaf != 0) {
#if defined(__i386__) && defined(__PIC__)
        __asm__ volatile(
            "xchg %%ebx, %%edi\n\t"
            "cpuid\n\t"
            "xchg %%ebx, %%edi\n\t"
            : "=a" (retval[0]),
              "=D" (retval[1]),
              "=c" (retval[2]),
              "=d" (retval[3])
            : "a" (leaf), "c" (subleaf));
#else /* defined(__i386__) && defined(__PIC__) */
        __asm__ volatile(
            "cpuid\n\t"
            : "=a" (retval[0]),
              "=b" (retval[1]),
              "=c" (retval[2]),
              "=d" (retval[3])
            : "a" (leaf), "c" (subleaf));
#endif /* defined(__i386__) && defined(__PIC__) */
    } else if (!::__get_cpuid(leaf,
            reinterpret_cast<unsigned int *>(retval + 0),
            reinterpret_cast<unsigned int *>(retval + 1),
            reinterpret_cast<unsigned int *>(retval + 2),
            reinterpret_cast<unsigned int *>(retval + 3))) {
        LYRA_TRACE("CPUID 0x%x failed.", leaf);
        ::memset(retval, 0, 4 * sizeof(*retval));
    }
}
#endif /* !defined(_WIN32) */



/*
 * LYRA_DETAIL_NAMESPACE::get_cpu_info
 */
std::size_t LYRA_DETAIL_NAMESPACE::get_cpu_info(
        _Out_writes_opt_(cnt) cpu_info *dst,
        _In_ std::size_t cnt,
        _In_ const std::uint32_t base,
        _In_ const std::uint32_t subleaf) {
    assert((base == 0) || (base == 0x80000000));
    cpu_info info;

    if (dst == nullptr) {
        cnt = 0;
    }

#if defined(_LYRA_X86)
#if defined(_WIN32)
    // Determine the number of functions available as described on
    // https://learn.microsoft.com/de-de/cpp/intrinsics/cpuid-cpuidex?view=msvc-170
    ::__cpuid(info.values, base);
#else /* defined(_WIN32) */
    // Determine the number of functions available as described on
    // https://stackoverflow.com/questions/14266772/how-do-i-call-cpuid-in-linux
    info.registers.eax = ::__get_cpuid_max(base, nullptr);
#endif /* defined(_WIN32) */

    // EAX now holds the number of queries we can make.
    const auto retval = static_cast<std::uint32_t>(info.registers.eax);
    assert(retval > base);

    // Make all queries that are available and for which we have storage.
    for (std::uint32_t i = base, j = 0; (i < retval) && (j < cnt); ++i, ++j) {
        auto& d = dst[j];
        ::__cpuidex(reinterpret_cast<int *>(d.values), i, subleaf);
    }

    return (retval - base);

#else /* defined(_LYRA_X86) */
    return 0;
#endif /* defined(_LYRA_X86) */
}


/*
 * LYRA_NAMESPACE::get_cpu_info
 */
std::size_t LYRA_NAMESPACE::get_cpu_info(
        _Out_writes_opt_(cnt) cpu_info *dst,
        _In_ const std::size_t cnt) {
    return detail::get_cpu_info(dst, cnt, 0);
}


/*
 * LYRA_NAMESPACE::get_cpu_info
 */
bool LYRA_API LYRA_NAMESPACE::get_cpu_info(
        _Out_ cpu_info& info,
        _In_ const std::uint32_t leaf,
        _In_ const std::uint32_t subleaf) {
#if defined(_LYRA_X86)
    constexpr auto threshold = 0x80000000;
    const auto base = (leaf >= threshold) ? threshold : 0;
    const auto available = detail::get_cpu_info(nullptr, 0, base);
    auto retval = (leaf < base + available);

    if (retval) {
        ::__cpuidex(reinterpret_cast<int *>(info.values), leaf, subleaf);
    }

    return retval;
#else /* defined(_LYRA_X86) */
    return false;
#endif /* defined(_LYRA_X86) */
}


/*
 * LYRA_NAMESPACE::get_extended_cpu_info
 */
std::size_t LYRA_NAMESPACE::get_extended_cpu_info(
        _Out_writes_opt_(cnt) cpu_info *dst,
        _In_ const std::size_t cnt) {
    return detail::get_cpu_info(dst, cnt, 0x80000000);
}
