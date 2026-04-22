// <copyright file="result_type.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_RESULT_TYPE_H)
#define _LYRA_RESULT_TYPE_H
#pragma once

#include "visus/lyra/api.h"


LYRA_NAMESPACE_BEGIN

#if defined(_WIN32)
/// <summary>
/// The type used to indicate the result of an operation.
/// </summary>
typedef HRESULT result_type;

#else /* defined(_WIN32) */
/// <summary>
/// The type used to indicate the result of an operation.
/// </summary>
typedef int result_type;

#endif /* defined(_WIN32) */

LYRA_NAMESPACE_END

#endif /* !defined(_LYRA_RESULT_TYPE_H) */
