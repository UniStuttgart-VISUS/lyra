// <copyright file="simd_detector.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2025 - 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_SIMD_DETECTOR_H)
#define _LYRA_SIMD_DETECTOR_H
#pragma once

#include "visus/lyra/cpu_info_detector.h"
#include "visus/lyra/simd_instruction_set.h"


LYRA_NAMESPACE_BEGIN

/// <summary>
/// A functor that detects whether the current CPU and operating system support
/// the <pararamref name="Instructions" /> SIMD instruction set.
/// </summary>
/// <typeparam name="Instructions">The instruction set to test for.</typeparam>
template<simd_instruction_set Instructions> class simd_detector;

/// <summary>
/// Specialisation for <see cref="simd_instruction_set::none" />, which will
/// always yield <see langword="true" />.
/// </summary>
template<> class simd_detector<simd_instruction_set::none> final {
public:

    inline constexpr operator bool(void) const noexcept {
        return true;
    }
};

/// <summary>
/// Specialisation for MMX, which is stored in EDX bit 23 of function 1.
/// </summary>
template<> class simd_detector<simd_instruction_set::mmx> final
        : public cpu_info_detector<
    0x00000001, cpu_info_register::edx, detail::cpu_info_bit(23)> { };

/// <summary>
/// Specialisation for SSE, which is stored in EDX bit 25 of function 1.
/// </summary>
template<> class simd_detector<simd_instruction_set::sse> final
        : public cpu_info_detector<
    0x00000001, cpu_info_register::edx, detail::cpu_info_bit(25)> { };

/// <summary>
/// Specialisation for SSE2, which is stored in EDX bit 26 of function 1.
/// </summary>
template<> class simd_detector<simd_instruction_set::sse2> final
        : public cpu_info_detector<
    0x00000001, cpu_info_register::edx, detail::cpu_info_bit(26)> { };

/// <summary>
/// Specialisation for SSE3, which is stored in ECX bit 0 of function 1.
/// </summary>
template<> class simd_detector<simd_instruction_set::sse3> final
        : public cpu_info_detector<
    0x00000001, cpu_info_register::ecx, detail::cpu_info_bit(0)> { };

/// <summary>
/// Specialisation for SSSE3, which is stored in ECX bit 9 of function 1.
/// </summary>
template<> class simd_detector<simd_instruction_set::ssse3> final
        : public cpu_info_detector<
    0x00000001, cpu_info_register::ecx, detail::cpu_info_bit(9)> { };

/// <summary>
/// Specialisation for SSE 4.1, which is stored in ECX bit 19 of function 1.
/// </summary>
template<> class simd_detector<simd_instruction_set::sse4_1> final
        : public cpu_info_detector<
    0x00000001, cpu_info_register::ecx, detail::cpu_info_bit(19)> { };

/// <summary>
/// Specialisation for SSE 4.1, which is stored in ECX bit 20 of function 1.
/// </summary>
template<> class simd_detector<simd_instruction_set::sse4_2> final
        : public cpu_info_detector<
    0x00000001, cpu_info_register::ecx, detail::cpu_info_bit(20)> { };

/// <summary>
/// Specialisation for AVX, which is stored in EDX bit 28 of function 1.
/// </summary>
template<> class simd_detector<simd_instruction_set::avx> final
        : public cpu_info_detector<
    0x00000001, cpu_info_register::edx, detail::cpu_info_bit(28)> { };

/// <summary>
/// Specialisation for AVX2, which is stored in EBX bit 5 of function 7.
/// </summary>
template<> class simd_detector<simd_instruction_set::avx2> final
        : public cpu_info_detector<
    0x00000007, cpu_info_register::ebx, detail::cpu_info_bit(5)> { };

/// <summary>
/// Specialisation for AVX-512F, which is stored in EBX bit 16 of function 7.
/// </summary>
template<> class simd_detector<simd_instruction_set::avx512> final
        : public cpu_info_detector<
    0x00000007, cpu_info_register::ebx, detail::cpu_info_bit(16)> { };

/// <summary>
/// Specialisation for AVX-512PF, which is stored in EBX bit 26 of function 7.
/// </summary>
template<> class simd_detector<simd_instruction_set::avx512_prefetch> final
        : public cpu_info_detector<
    0x00000007, cpu_info_register::ebx, detail::cpu_info_bit(26)> { };

/// <summary>
/// Specialisation for AVX-512ER, which is stored in EBX bit 27 of function 7.
/// </summary>
template<> class simd_detector<simd_instruction_set::avx512er> final
        : public cpu_info_detector<
    0x00000007, cpu_info_register::ebx, detail::cpu_info_bit(27)> { };

/// <summary>
/// Specialisation for AVX-512CD, which is stored in EBX bit 28 of function 7.
/// </summary>
template<> class simd_detector<simd_instruction_set::avx512cd> final
        : public cpu_info_detector<
    0x00000007, cpu_info_register::ebx, detail::cpu_info_bit(28)> { };

/// <summary>
/// Specialisation for AVX-512DQ, which is stored in EBX bit 17 of function 7.
/// </summary>
template<> class simd_detector<simd_instruction_set::avx512dq> final
        : public cpu_info_detector<
    0x00000007, cpu_info_register::ebx, detail::cpu_info_bit(17)> { };

/// <summary>
/// Specialisation for AVX-512BW, which is stored in EBX bit 30 of function 7.
/// </summary>
template<> class simd_detector<simd_instruction_set::avx512_byte_word> final
        : public cpu_info_detector<
    0x00000007, cpu_info_register::ebx, detail::cpu_info_bit(30)> { };

/// <summary>
/// Specialisation for AVX-512VL, which is stored in EBX bit 31 of function 7.
/// </summary>
template<> class simd_detector<simd_instruction_set::avx512_vector_length> final
        : public cpu_info_detector<
    0x00000007, cpu_info_register::ebx, detail::cpu_info_bit(31)> { };

/// <summary>
/// Specialisation for AVX-512IFMA, which is stored in EBX bit 21 of function 7.
/// </summary>
template<>
class simd_detector<simd_instruction_set::avx512_integer_fused_multiply_add> final
        : public cpu_info_detector<
    0x00000007, cpu_info_register::ebx, detail::cpu_info_bit(21)> { };


/// <summary>
/// Checks whether the POPCNT instruction is supported via bit 23 of ECX in
/// function 1. POPCNT allows for counting the number of set bits in a word.
/// </summary>
typedef cpu_info_detector<0x00000001, cpu_info_register::ecx,
    detail::cpu_info_bit(23)> popcnt_detector;

/// <summary>
/// Checks whether the <see cref="_xgetbv" /> intrinsic is supported via bit
/// 27 of ECX in function 1.
/// </summary>
typedef cpu_info_detector<0x00000001, cpu_info_register::ecx,
    detail::cpu_info_bit(27)> xgetbv_detector;

LYRA_NAMESPACE_END

#endif /* !defined(_LYRA_SIMD_DETECTOR_H) */
