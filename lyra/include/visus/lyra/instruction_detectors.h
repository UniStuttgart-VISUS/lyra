// <copyright file="instruction_detectors.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2025 - 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_INSTRUCTION_DETECTORS_H)
#define _LYRA_INSTRUCTION_DETECTORS_H
#pragma once

#include "visus/lyra/cpu_info_detector.h"


LYRA_NAMESPACE_BEGIN

namespace instruction_detectors {

    /// <summary>
    /// Checks whether the POPCNT instruction is supported via bit 23 of ECX in
    /// function 1. POPCNT allows for counting the number of set bits in a word.
    /// </summary>
    typedef cpu_info_detector<0x00000001, cpu_info_register::ecx,
        detail::cpu_info_bit(23)> popcnt;

    /// <summary>
    /// Checks whether the <see cref="_xgetbv" /> intrinsic is supported via bit
    /// 27 of ECX in function 1.
    /// </summary>
    typedef cpu_info_detector<0x00000001, cpu_info_register::ecx,
        detail::cpu_info_bit(27)> xgetbv;

} /* namespace instruction_detectors */

LYRA_NAMESPACE_END

#endif /* !defined(_LYRA_INSTRUCTION_DETECTORS_H) */
