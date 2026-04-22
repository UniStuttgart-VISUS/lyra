// <copyright file="dispatch_list.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_DISPATCH_LIST_H)
#define _LYRA_DISPATCH_LIST_H
#pragma once

#include "visus/lyra/api.h"


LYRA_DETAIL_NAMESPACE_BEGIN

/// <summary>
/// A list to dispatch on (for instance enumeration) values that are defined at
/// compile time.
/// </summary>
/// <typeparam name="TType">The (enumeration) type.</typeparam>
/// <typeparam name="Values">The values to dispatch.</typeparam>
template<class TType, TType... Values> struct dispatch_list final { };

LYRA_DETAIL_NAMESPACE_END

#endif /* !defined(_LYRA_DISPATCH_LIST_H) */
