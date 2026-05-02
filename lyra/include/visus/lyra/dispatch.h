// <copyright file="dispatch.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_DISPATCH_H)
#define _LYRA_DISPATCH_H
#pragma once

#include <cinttypes>
#include <map>

#include "visus/lyra/dispatch_list.h"
#include "visus/lyra/trace.h"


LYRA_DETAIL_NAMESPACE_BEGIN

/// <summary>
/// Invokes the given <paramref name="callback" /> with a pointer to the
/// corresponding <typeparamref name="TTraits" /> instance.
/// </summary>
/// <typeparam name="TType"></typeparam>
/// <typeparam name="TCallback"></typeparam>
/// <typeparam name="Values"></typeparam>
/// <param name="value"></param>
/// <param name="callback"></param>
/// <param name=""></param>
/// <returns></returns>
template<class TType, template<TType> class TTraits, class TCallback,
    TType... Values>
bool dispatch_traits(_In_ const TType value, _In_ TCallback callback,
    _In_ const dispatch_list<TType, Values...>);

LYRA_DETAIL_NAMESPACE_END

#include "visus/lyra/dispatch.inl"

#endif /* !defined(_LYRA_DISPATCH_H) */
