// <copyright file="autodoc.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_AUTODOC_H)
#define _LYRA_AUTODOC_H
#pragma once

#include "visus/autodoc/api.h"


#if defined(__cplusplus)

/// <summary>
/// A RAII helper for collecting and persisting system information on
/// construction and destruction, respectively.
/// </summary>
class LYRA_API autodoc final {
};

#endif /* defined(__cplusplus) */

//extern "C" LYRA_API autodoc_write_a(_In_z_ const char *path);

#endif /* !defined(_LYRA_AUTODOC_H) */
