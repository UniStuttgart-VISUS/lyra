// <copyright file="affinity_mask.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_AFFINITY_MASK_H)
#define _LYRA_AFFINITY_MASK_H
#pragma once

#include <cstdlib>
#include <type_traits>

#if defined(_WIN32)
#include <Windows.h>
#else /* defined(_WIN32) */
#include <sched.h>
#include <unistd.h>

#include <sys/types.h>
#endif /* defined(_WIN32) */

#include "visus/autodoc/api.h"


LYRA_NAMESPACE_BEGIN

/// <summary>
/// A platform-independent wrapper for a CPU affinity mask.
/// </summary>
class LYRA_API affinity_mask final {

public:

#if defined(_WIN32)
#if (_WIN32_WINNT >= 0x0601)
    /// <summary>
    /// The native affinity mask type on Windows 7 and later.
    /// </summary>
    typedef GROUP_AFFINITY& mask_type;
#else /* (_WIN32_WINNT >= 0x0601) */
    /// <summary>
    /// The native affinity mask type before Windows 7.
    /// </summary>
    typedef DWORD_PTR mask_type;
#endif /* (_WIN32_WINNT >= 0x0601) */
#else /* defined(_WIN32) */
    /// <summary>
    /// The native affinity mask type on POSIX-compliant systems.
    /// </summary>
    typedef cpu_set_t *mask_type;
#endif /* defined(_WIN32) */

#if defined(_WIN32)
    /// <summary>
    /// A handlde to a thread on Windows.
    /// </summary>
    typedef HANDLE thread_handle;
#else /* defined(_WIN32) */
    /// <summary>
    /// A handle to a thread on POSIX-compliant systems.
    /// </summary>
    typedef pid_t thread_handle;
#endif /* !defined(_WIN32) */

    /// <summary>
    /// Gets the affinity mask for the given thread.
    /// </summary>
    /// <param name="thread">A handle to the thread.</param>
    /// <returns>The current affinity mask of the specified thread.</returns>
    /// <exception cref="std::system_error">If the affinity mask could not be
    /// be determined.</exception>
    static affinity_mask thread(_In_ const thread_handle thread);

    /// <summary>
    /// Gets the affinity mask for the current thread.
    /// </summary>
    /// <returns>The current affinity mask of the calling thread.</returns>
    /// <exception cref="std::system_error">If the affinity mask could not be
    /// be determined.</exception>
    static affinity_mask thread(void);

    /// <summary>
    /// Initialises a new instance.
    /// </summary>
    /// <remarks>
    /// The mask created by this constructor is always invalid.
    /// </remarks>
    affinity_mask(void) noexcept;

    /// <summary>
    /// Initialises a new instance.
    /// </summary>
    /// <remarks>
    /// The constructor may need to perform allocations. If these fail, the
    /// resulting object will be invalid, which can be tested by converting it
    /// to <see langword="bool" />. On Windows, the constructor will never fail.
    /// </remarks>
    /// <param name="mask">The affinity mask.</param>
    /// <param name="size">The size of the mask. This parameter is irrelevant
    /// on Windows.</param>
    affinity_mask(_In_ mask_type mask,
        _In_ const std::size_t size = 0) noexcept;

    /// <summary>
    /// Initialises a new instance.
    /// </summary>
    /// <remarks>
    /// The constructor may fail due to memory allocation failure or failed API
    /// calls. In this case case, the resulting object will be invalid, which
    /// can be tested by converting it to <see langword="bool" />. Any invalid
    /// index will be silently ignored.
    /// </remarks>
    /// <param name="indices">The zero-based indices of the logical CPUs that
    /// are set in the mask.</param>
    /// <param name="cnt">The number of elements in <paramref name="indices" />.
    /// </param>
    affinity_mask(_In_reads_(cnt) const std::size_t *indices,
        _In_ const std::size_t cnt) noexcept;

    /// <summary>
    /// Initialises a new instance.
    /// </summary>
    /// <remarks>
    /// The constructor may fail due to memory allocation failure or failed API
    /// calls. In this case case, the resulting object will be invalid, which
    /// can be tested by converting it to <see langword="bool" />. If the index
    /// is invalid, the mask will be invalid, too.
    /// </remarks>
    /// <param name="idx"></param>
    inline explicit affinity_mask(_In_ const std::size_t idx) noexcept
        : affinity_mask(&idx, 1) { }

    /// <summary>
    /// Clone <paramref name="other" />
    /// </summary>
    /// <remarks>
    /// The clone might be invalid on non-Windows platforms if the allocation
    /// fails.
    /// </remarks>
    /// <param name="other">The object to be cloned.</param>
    affinity_mask(_In_ const affinity_mask& other) noexcept;

    /// <summary>
    /// Move <paramref name="other" />
    /// </summary>
    /// <param name="other">The object to be moved.</param>
    affinity_mask(_Inout_ affinity_mask&& other) noexcept;

    /// <summary>
    /// Finalises the instance.
    /// </summary>
    ~affinity_mask(void) noexcept;

    /// <summary>
    /// Clears any data in the mask.
    /// </summary>
    void clear(void) noexcept;

    /// <summary>
    /// Assignment.
    /// </summary>
    /// <param name="rhs">The right-hand-side operand.</param>
    /// <returns><c>*<see langword="this" /></c>.</returns>
    affinity_mask& operator =(_In_ const affinity_mask& rhs) noexcept;

    /// <summary>
    /// Move assignment.
    /// </summary>
    /// <param name="rhs">The right-hand-side operand.</param>
    /// <returns><c>*<see langword="this" /></c>.</returns>
    affinity_mask& operator =(_Inout_ affinity_mask&& rhs) noexcept;

#if defined(_WIN32)
    /// <summary>
    /// Gets the mask encapsulated by this object.
    /// </summary>
    /// <returns>The mask in this object.</returns>
    inline const std::decay_t<mask_type>& get(void) const noexcept {
#else /* defined(_WIN32) */
    inline const std::decay_t<mask_type> get(void) const noexcept {
#endif /* defined(_WIN32) */
        return this->_mask;
    }

#if !defined(_WIN32)
    /// <summary>
    /// Gets thes ize of the mask encapsulated by this object.
    /// </summary>
    /// <returns>The size of the mask.</returns>
    inline std::size_t size(void) const noexcept {
        return this->_size;
    }
#endif /* defined(_WIN32) */

    /// <summary>
    /// Tests whether the <paramref name="idx" />-th logical CPU is set in the
    /// mask.
    /// </summary>
    /// <remarks>
    /// On Windows systems later than Windows 7, the index is divided by the
    /// size of a logical processor group, so to select the first CPU of the
    /// second group, you can use consecutive indices.
    /// </remarks>
    /// <param name="idx">The zero-based index of the CPU.</param>
    /// <returns><see langword="true" /> if the CPU is set in the mask,
    /// <see langword="false" /> otherwise.</returns>
    bool test(_In_ std::size_t idx) const noexcept;

    /// <summary>
    /// Indicates whether the mask is valid, i.e. selects any logical CPU.
    /// </summary>
    /// <returns><see langword="true" /> if the mask is valid,
    /// <see langword="false" /> otherwise.</returns>
    operator bool(void) const noexcept;

#if defined(_WIN32)
    /// <summary>
    /// Convert the object to the native affinity mask.
    /// </summary>
    /// <returns>The native affinity mask.</returns>
    inline operator const std::decay_t<mask_type>&(void) const noexcept {
        return this->_mask;
    }
#endif /* defined(_WIN32) */

#if defined(_WIN32)
    /// <summary>
    /// Convert the object to a pointer to a native affinity mask.
    /// </summary>
    /// <returns>The native affinity mask.</returns>
    inline operator const std::decay_t<mask_type> *(void) const noexcept {
        return &this->_mask;
    }
#endif /* defined(_WIN32) */

    /// <summary>
    /// Checks whether this mask and <paramref name="rhs" /> select the same
    /// logical CPUs.
    /// </summary>
    /// <param name="rhs">The right-hand-side operand.</param>
    /// <returns><see langword="true" /> if the masks select the same logical
    /// CPUs, <see langword="false" /> otherwise.</returns>
    bool operator ==(_In_ const affinity_mask& rhs) const noexcept;

    /// <summary>
    /// Checks whether this mask and <paramref name="rhs" /> select different
    /// logical CPUs.
    /// </summary>
    /// <param name="rhs">The right-hand-side operand.</param>
    /// <returns><see langword="false" /> if the masks select the same logical
    /// CPUs, <see langword="true" /> otherwise.</returns>
    inline bool operator !=(_In_ const affinity_mask& rhs) const noexcept {
        return !(*this == rhs);
    }

private:

    std::decay_t<mask_type> _mask;
#if !defined(_WIN32)
    std::size_t _size;
#endif /* !defined(_WIN32) */
};

LYRA_NAMESPACE_END

#endif /* !defined(_LYRA_AFFINITY_MASK_H) */
