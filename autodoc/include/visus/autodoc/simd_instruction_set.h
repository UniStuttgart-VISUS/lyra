// <copyright file="simd_instruction_set.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2025 - 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_SIMD_INSTRUCTION_SET_H)
#define _LYRA_SIMD_INSTRUCTION_SET_H
#pragma once

#include "visus/autodoc/api.h"


LYRA_NAMESPACE_BEGIN

/// <summary>
/// Possible SIMD instruction sets the <see cref="simd_detector" /> can check
/// for.
/// </summary>
enum class simd_instruction_set {

    /// <summary>
    /// No SIMD is supported.
    /// </summary>
    none = 0,

    /// <summary>
    /// MMX is supported.
    /// </summary>
    mmx,

    ///// <summary>
    ///// AMD extensions to MMX are supported.
    ///// </summary>
    //mmx_ext,

    /// <summary>
    /// SSE is supported.
    /// </summary>
    sse,

    /// <summary>
    /// SSE2 is supported.
    /// </summary>
    sse2,

    /// <summary>
    /// SSE3 is supported.
    /// </summary>
    sse3,

    /// <summary>
    /// Supplemental SSE3 (SSSE3) is supported.
    /// </summary>
    ssse3,

    /// <summary>
    /// SSE 4.1 is supported.
    /// </summary>
    sse4_1,

    /// <summary>
    /// SSE 4.2 is supported.
    /// </summary>
    sse4_2,

    ///// <summary>
    ///// SSE 4a is supported.
    ///// </summary>
    //sse4a,

    /// <summary>
    /// AVX is supported.
    /// </summary>
    avx,

    /// <summary>
    /// AVX2 is supported.
    /// </summary>
    avx2,

    /// <summary>
    /// AVX-512F (base functions) is supported.
    /// </summary>
    avx512_foundation,

    /// <summary>
    /// AVX-512F (base functions) is supported.
    /// </summary>
    avx512 = avx512_foundation,

    /// <summary>
    /// AVX-512F (base functions) is supported.
    /// </summary>
    avx512f = avx512,

    /// <summary>
    /// AVX-512PF is supported.
    /// </summary>
    avx512_prefetch,

    /// <summary>
    /// AVX-512PF is supported.
    /// </summary>
    avx512pf = avx512_prefetch,

    /// <summary>
    /// AVX-512DQ is supported.
    /// </summary>
    avx512_double_quad_word,

    /// <summary>
    /// AVX-512DQ is supported.
    /// </summary>
    avx512dq = avx512_double_quad_word,

    /// <summary>
    /// AVX-512BW is supported.
    /// </summary>
    avx512_byte_word,

    /// <summary>
    /// AVX-512BW is supported.
    /// </summary>
    avx512bw = avx512_byte_word,

    /// <summary>
    /// AVX-512VL is supported.
    /// </summary>
    avx512_vector_length,

    /// <summary>
    /// AVX-512VL is supported.
    /// </summary>
    avx512vl = avx512_vector_length,

    /// <summary>
    /// AVX-512CD is supported.
    /// </summary>
    avx512_conflict_detection,

    /// <summary>
    /// AVX-512CD is supported.
    /// </summary>
    avx512cd = avx512_conflict_detection,

    /// <summary>
    /// AVX-512ER is supported.
    /// </summary>
    avx512_exponential_reciprocal,

    /// <summary>
    /// AVX-512ER is supported.
    /// </summary>
    avx512er = avx512_exponential_reciprocal,

    /// <summary>
    /// AVX-512IFMA is supported.
    /// </summary>
    avx512_integer_fused_multiply_add,

    /// <summary>
    /// AVX-512IFMA is supported.
    /// </summary>
    avx512_integer_fused_madd = avx512_integer_fused_multiply_add,

    /// <summary>
    /// AVX-512IFMA is supported.
    /// </summary>
    avx512_integer_fmadd = avx512_integer_fused_multiply_add,

    /// <summary>
    /// AVX-512IFMA is supported.
    /// </summary>
    avx512ifma = avx512_integer_fused_multiply_add,

    /// <summary>
    /// AVX-512VBMI is supported.
    /// </summary>
    avx512_vector_bit_manipulation,

    /// <summary>
    /// AVX-512VBMI is supported.
    /// </summary>
    avx512vbmi = avx512_vector_bit_manipulation,

    /// <summary>
    /// AVX-512VBMI2 is supported.
    /// </summary>
    avx512_vector_bit_manipulation2,

    /// <summary>
    /// AVX-512VBMI is supported.
    /// </summary>
    avx512vbmi2 = avx512_vector_bit_manipulation2,

    /// <summary>
    /// AVX-512 4FMAPS is supported.
    /// </summary>
    avx512_vector_fused_multiply_accumulate_single,

    /// <summary>
    /// AVX-512 4FMAPS is supported.
    /// </summary>
    avx5124fmaps = avx512_vector_fused_multiply_accumulate_single,

    /// <summary>
    /// AVX-512 4VNNIW is supported.
    /// </summary>
    avx512_vector_neural_network_instructions_word,

    /// <summary>
    /// AVX-512 4VNNIW is supported.
    /// </summary>
    avx5124vnniw = avx512_vector_neural_network_instructions_word,

    /// <summary>
    /// AVX-512 VPOPCNTDQ is supported.
    /// </summary>
    avx512_vector_popcount_dword_qword,

    /// <summary>
    /// AVX-512 VPOPCNTDQ is supported.
    /// </summary>
    avx512_vector_popcount = avx512_vector_popcount_dword_qword,

    /// <summary>
    /// AVX-512 VPOPCNTDQ is supported.
    /// </summary>
    avx512vpopcntdq = avx512_vector_popcount_dword_qword,

    /// <summary>
    /// AVX-512 VNNI is supported.
    /// </summary>
    avx512_vector_neural_network_instructions,

    /// <summary>
    /// AVX-512 4VNNIW is supported.
    /// </summary>
    avx512vnni = avx512_vector_neural_network_instructions,

    /// <summary>
    /// AVX-512 BITALG is supported.
    /// </summary>
    avx512_bit_algorithms,

    /// <summary>
    /// AVX-512 BITALG is supported.
    /// </summary>
    avx512bitalg = avx512_bit_algorithms,

    /// <summary>
    /// AVX-512 GFNI is supported.
    /// </summary>
    avx512_galois_field_new_instructions,

    /// <summary>
    /// AVX-512 GFNI is supported.
    /// </summary>
    avx512gfni = avx512_galois_field_new_instructions,

    /// <summary>
    /// AVX-512 VPCLMULQDQ is supported.
    /// </summary>
    avx512_carry_less_multiply_quad_word,

    /// <summary>
    /// AVX-512 VPCLMULQDQ is supported.
    /// </summary>
    avx512vpclmulqdq = avx512_carry_less_multiply_quad_word,

    /// <summary>
    /// AVX-512 VAES is supported.
    /// </summary>
    avx512_vector_aes,

    /// <summary>
    /// AVX-512 VAES is supported.
    /// </summary>
    avx512vaes = avx512_vector_aes,

    /// <summary>
    /// AVX-512 BF16 is supported.
    /// </summary>
    avx512_bfloat16,

    /// <summary>
    /// AVX-512 BF16 is supported.
    /// </summary>
    avx512bf16,
};

LYRA_NAMESPACE_END

#endif /* !defined(_LYRA_SIMD_INSTRUCTION_SET_H) */
