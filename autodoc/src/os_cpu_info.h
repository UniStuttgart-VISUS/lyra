// <copyright file="os_cpu_info.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2025 - 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_OS_CPU_INFO_H)
#define _LYRA_OS_CPU_INFO_H
#pragma once

#include <cinttypes>
#include <cstdlib>
#include <type_traits>
#include <vector>

#if !defined(_WIN32)
#include <sys/types.h>
#endif /* !defined(_WIN32) */

#include "visus/autodoc/affinity_mask.h"
#include "visus/autodoc/fnv1a.h"
#include "visus/autodoc/property_set.h"

LYRA_DETAIL_NAMESPACE_BEGIN


#if defined(_WIN32) && (_WIN32_WINNT >= 0x0601)
/// <summary>
/// Enumerates all <see cref="SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX" />.
/// </summary>
template<class TCallback>
std::size_t enumerate_cpu_info(_In_ TCallback callback);
#endif /* defined(_WIN32) && (_WIN32_WINNT >= 0x0601) */

#if defined(_WIN32) && (_WIN32_WINNT >= 0x0601)
/// <summary>
/// Enumerates all <see cref="SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX" /> that
/// are relevant to reconstruct the CPU topology of a system. These are the
/// packages, the cores and NUMA nodes. Hyperthreading siblings are not included
/// in this enumeration, but can be detected via the <c>LTP_PC_SMT</c> flag if
/// the relationship type is <c>RelationProcessorCore</c>.
/// </summary>
template<class TCallback> void enumerate_cpu_toplogy(_In_ TCallback callback);
#endif /* defined(_WIN32) && (_WIN32_WINNT >= 0x0601) */

/// <summary>
/// Gets the CPU affinity of the current process.
/// </summary>
/// <returns>A vector of Booleans representing the CPU affinity of the current
/// process.</returns>
LYRA_TEST_API std::vector<bool> get_process_cpu_affinity(void);

/// <summary>
/// Gets the CPU affinity of the specified thread.
/// </summary>
/// <param name="thread">The handle of the thread for which to retrieve the CPU
/// affinity. On Windows, use <see cref="GetCurrentThread" /> for the calling
/// thread. On Linux, use <see cref="gettid" /> for the calling thread.</param>
/// <returns>A vector of Booleans representing the CPU affinity of the specified
/// thread.</returns>
LYRA_TEST_API std::vector<bool> get_thread_cpu_affinity(
    _In_ const affinity_mask::thread_handle thread);

/// <summary>
/// Gets the CPU affinity for the calling thread.
/// </summary>
/// <returns>A vector of Booleans representing the CPU affinity of the calling
/// thread.</returns>
LYRA_TEST_API std::vector<bool> get_thread_cpu_affinity(void);

#if defined(_WIN32)
/// <summary>
/// Gets the CPU features as reported by
/// <see cref="IsProcessorFeaturePresent" />.
/// </summary>
/// <returns></returns>
LYRA_TEST_API property_set get_processor_features(void);
#endif /* defined(_WIN32) */

#if defined(_WIN32) && (_WIN32_WINNT >= 0x0601)
/// <summary>
/// Gets the logical processor information for the specified relationship type.
/// </summary>
/// <param name="buffer">A buffer to store the information. This buffer will be
/// resized if it is too small to hold the information.</param>
/// <param name="relationship">The relationship type for which to retrieve the
/// information.</param>
/// <returns>A pointer to the logical processor information, or
/// <see langword="nullptr" /> in case of an error. The pointer is only valid as
/// long as <see cref="buffer" /> is not modified.</returns>
LYRA_TEST_API SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX *
get_logical_processor_info(
    _Inout_ std::vector<std::uint8_t>& buffer,
    _In_ const LOGICAL_PROCESSOR_RELATIONSHIP relationship);
#endif /* defined(_WIN32) && (_WIN32_WINNT >= 0x0601) */

/// <summary>
/// Gets the maximum number of CPUs that the operating system can use.
/// </summary>
/// <returns>The maximum number of CPUs that the operating system can use at the
/// time of the call.</returns>
LYRA_TEST_API std::size_t get_os_max_cpus(void);

#if defined(_WIN32) && (_WIN32_WINNT >= 0x0601)
/// <summary>
/// Gets the processor groups of the system.
/// </summary>
/// <returns>A vector of <see cref="PROCESSOR_GROUP_INFO" /> structures describing
/// the processor groups of the system.</returns>
LYRA_TEST_API std::vector<PROCESSOR_GROUP_INFO> get_processor_groups(void);
#endif /* defined(_WIN32) && (_WIN32_WINNT >= 0x0601) */

LYRA_DETAIL_NAMESPACE_END


#if defined(_WIN32) && (_WIN32_WINNT >= 0x0601)
/// <summary>
/// Test for equality of two <see cref="GROUP_AFFINITY" />s.
/// </summary>
template<> struct std::equal_to<GROUP_AFFINITY> final {
    typedef GROUP_AFFINITY value_type;

    inline bool operator () (_In_ const value_type& lhs,
            _In_ const value_type& rhs) const noexcept {
        return (lhs.Group == rhs.Group) && (lhs.Mask == rhs.Mask);
    }
};


/// <summary>
/// Implements a hash function for <see cref="GROUP_AFFINITY" />.
/// </summary>
template<> struct std::hash<GROUP_AFFINITY> final {
    typedef LYRA_NAMESPACE::fnv1a<std::size_t> hash_type;
    typedef GROUP_AFFINITY value_type;

    inline std::size_t operator ()(_In_ const value_type& v) const noexcept {
        hash_type retval;
        auto g = reinterpret_cast<const std::uint8_t *>(&v.Group);
        retval(g, g + sizeof(v.Group));
        auto m = reinterpret_cast<const std::uint8_t *>(&v.Mask);
        retval(m, m + sizeof(v.Mask));
        return retval;
    }
};

/// <summary>
/// Establishes an order of <see cref="GROUP_AFFINITY" />s.
/// </summary>
template<> struct std::less<GROUP_AFFINITY> final {
    typedef GROUP_AFFINITY value_type;

    inline bool operator ()(_In_ const value_type& lhs,
            _In_ const value_type& rhs) const noexcept {
        if (lhs.Group < rhs.Group) {
            return true;

        } else if (lhs.Group > rhs.Group) {
            return false;

        } else {
            return (lhs.Mask < rhs.Mask);
        }
    }
};
#endif /* defined(_WIN32) && (_WIN32_WINNT >= 0x0601) */

#include "os_cpu_info.inl"

#endif /* !defined(_LYRA_OS_CPU_INFO_H) */
