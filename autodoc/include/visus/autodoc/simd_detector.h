// <copyright file="simd_detector.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2025 - 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_SIMD_DETECTOR_H)
#define _LYRA_SIMD_DETECTOR_H
#pragma once

#include "visus/autodoc/cpu_features.h"
#include "visus/autodoc/simd_instruction_set.h"


LYRA_DETAIL_NAMESPACE_BEGIN

/// <summary>
/// An adapter class to implement a SIMD detector based on a CPU feature
/// detector.
/// </summary>
template<class TFeature> class simd_feature {

public:

    inline operator bool(void) const noexcept {
        static const TFeature retval;
        return retval;
    }
};

LYRA_DETAIL_NAMESPACE_END


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
template<> class simd_detector<simd_instruction_set::none> {
public:

    inline constexpr operator bool(void) const noexcept {
        return true;
    }
};

/// <summary>
/// Specialisation for MMX, which is stored in EDX bit 23 of function 1.
/// </summary>
template<> class simd_detector<simd_instruction_set::mmx>
    : public detail::simd_feature<cpu_features::mmx> { };

/// <summary>
/// Specialisation for SSE, which is stored in EDX bit 25 of function 1.
/// </summary>
template<> class simd_detector<simd_instruction_set::sse>
    : public detail::simd_feature<cpu_features::sse> { };

/// <summary>
/// Specialisation for SSE2, which is stored in EDX bit 26 of function 1.
/// </summary>
template<> class simd_detector<simd_instruction_set::sse2>
    : public detail::simd_feature<cpu_features::sse2> { };

/// <summary>
/// Specialisation for SSE3, which is stored in ECX bit 0 of function 1.
/// </summary>
template<> class simd_detector<simd_instruction_set::sse3>
    : public detail::simd_feature<cpu_features::sse3> { };

/// <summary>
/// Specialisation for SSSE3, which is stored in ECX bit 9 of function 1.
/// </summary>
template<> class simd_detector<simd_instruction_set::ssse3>
    : public detail::simd_feature<cpu_features::ssse3> { };

/// <summary>
/// Specialisation for SSE 4.1, which is stored in ECX bit 19 of function 1.
/// </summary>
template<> class simd_detector<simd_instruction_set::sse4_1>
    : public detail::simd_feature<cpu_features::sse4_1> { };

/// <summary>
/// Specialisation for SSE 4.1, which is stored in ECX bit 20 of function 1.
/// </summary>
template<> class simd_detector<simd_instruction_set::sse4_2>
    : public detail::simd_feature<cpu_features::sse4_2> { };

/// <summary>
/// Specialisation for AVX, which is stored in EDX bit 28 of function 1.
/// </summary>
template<> class simd_detector<simd_instruction_set::avx>
    : public detail::simd_feature<cpu_features::avx> { };

/// <summary>
/// Specialisation for AVX VNNI, which is stored in EAX bit 4 of function 7.
/// </summary>
template<> class simd_detector<simd_instruction_set::avxvnni>
    : public detail::simd_feature<cpu_features::avx_vnni> { };

/// <summary>
/// Specialisation for AVX IFMA, which is stored in EAX bit 23 of function 7.
/// </summary>
template<> class simd_detector<simd_instruction_set::avxifma>
    : public detail::simd_feature<cpu_features::avx_ifma> { };

/// <summary>
/// Specialisation for AVX2, which is stored in EBX bit 5 of function 7.
/// </summary>
template<> class simd_detector<simd_instruction_set::avx2>
    : public detail::simd_feature<cpu_features::avx2> { };

/// <summary>
/// Specialisation for AVX-512F, which is stored in EBX bit 16 of function 7.
/// </summary>
template<> class simd_detector<simd_instruction_set::avx512f>
    : public detail::simd_feature<cpu_features::avx512_f> { };

/// <summary>
/// Specialisation for AVX-512PF, which is stored in EBX bit 26 of function 7.
/// </summary>
template<> class simd_detector<simd_instruction_set::avx512_prefetch>
    : public detail::simd_feature<cpu_features::avx512_pf> { };

/// <summary>
/// Specialisation for AVX-512DQ, which is stored in EBX bit 17 of function 7.
/// </summary>
template<> class simd_detector<simd_instruction_set::avx512dq>
    : public detail::simd_feature<cpu_features::avx512_dq> { };

/// <summary>
/// Specialisation for AVX-512BW, which is stored in EBX bit 30 of function 7.
/// </summary>
template<> class simd_detector<simd_instruction_set::avx512bw>
    : public detail::simd_feature<cpu_features::avx512_bw> { };

/// <summary>
/// Specialisation for AVX-512VL, which is stored in EBX bit 31 of function 7.
/// </summary>
template<> class simd_detector<simd_instruction_set::avx512vl>
    : public detail::simd_feature<cpu_features::avx512_vl> { };

/// <summary>
/// Specialisation for AVX-512CD, which is stored in EBX bit 28 of function 7.
/// </summary>
template<> class simd_detector<simd_instruction_set::avx512cd>
    : public detail::simd_feature<cpu_features::avx512_cd> { };

/// <summary>
/// Specialisation for AVX-512ER, which is stored in EBX bit 27 of function 7.
/// </summary>
template<> class simd_detector<simd_instruction_set::avx512er>
    : public detail::simd_feature<cpu_features::avx512_er> { };

/// <summary>
/// Specialisation for AVX-512IFMA, which is stored in EBX bit 21 of function 7.
/// </summary>
template<> class simd_detector<simd_instruction_set::avx512ifma>
    : public detail::simd_feature<cpu_features::avx512_ifma> { };

/// <summary>
/// Specialisation for AVX-512VBMI, which is stored in ECX bit 1 of function 7.
/// </summary>
template<> class simd_detector<simd_instruction_set::avx512vbmi>
    : public detail::simd_feature<cpu_features::avx512_vbmi> { };

/// <summary>
/// Specialisation for AVX-512VBMI2, which is stored in ECX bit 6 of function 7.
/// </summary>
template<> class simd_detector<simd_instruction_set::avx512vbmi2>
    : public detail::simd_feature<cpu_features::avx512_vmbi2> { };

/// <summary>
/// Specialisation for AVX-512 4FMAPS, which is stored in EDX bit 3 of
/// function 7.
/// </summary>
template<> class simd_detector<simd_instruction_set::avx5124fmaps>
    : public detail::simd_feature<cpu_features::avx512_4fmaps> { };

/// <summary>
/// Specialisation for AVX-512 4VNNIW, which is stored in EDX bit 2 of
/// function 7.
/// </summary>
template<> class simd_detector<simd_instruction_set::avx5124vnniw>
    : public detail::simd_feature<cpu_features::avx512_4vnniw> { };

/// <summary>
/// Specialisation for AVX-512 VPOPCNTDQ, which is stored in ECX bit 14 of
/// function 7.
/// </summary>
template<> class simd_detector<simd_instruction_set::avx512vpopcntdq>
: public detail::simd_feature<cpu_features::avx512_vpopcntdq> { };

/// <summary>
/// Specialisation for AVX-512 VNNI, which is stored in ECX bit 11 of
/// function 7.
/// </summary>
template<> class simd_detector<simd_instruction_set::avx512vnni>
    : public detail::simd_feature<cpu_features::avx512_vnni> { };

/// <summary>
/// Specialisation for AVX-512 BITALG, which is stored in ECX bit 12 of
/// function 7.
/// </summary>
template<> class simd_detector<simd_instruction_set::avx512bitalg>
    : public detail::simd_feature<cpu_features::avx512_bitalg> { };

/// <summary>
/// Specialisation for AVX-512 BFLOAT16, which is stored in EAX bit 5 of
/// function 7.
/// </summary>
template<> class simd_detector<simd_instruction_set::avx512bf16>
    : public detail::simd_feature<cpu_features::avx512_bf16> { };

LYRA_NAMESPACE_END

#endif /* !defined(_LYRA_SIMD_DETECTOR_H) */
