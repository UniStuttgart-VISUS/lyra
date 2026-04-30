// <copyright file="unique_variable.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_UNIQUE_VARIABLE_H)
#define _LYRA_UNIQUE_VARIABLE_H
#pragma once

#include "visus/lyra/api.h"

#if defined(__COUNTER__)
#define LYRA_UNIQUE_VARIABLE(p) LYRA_CONCAT(p, __COUNTER__)
#else /* defined(__COUNTER__) */
#define LYRA_UNIQUE_VARIABLE(p) LYRA_CONCAT(p, __LINE__)
#endif /* defined(__COUNTER__) */

#endif /* !defined(_LYRA_UNIQUE_VARIABLE_H) */
