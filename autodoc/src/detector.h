// <copyright file="detector.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2025 - 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_DETECTOR_H)
#define _LYRA_DETECTOR_H
#pragma once

#include <type_traits>

#include "visus/autodoc/api.h"


LYRA_DETAIL_NAMESPACE_BEGIN

/// <summary>
/// Detector for a method as described on
/// https://stackoverflow.com/questions/87372/check-if-a-class-has-a-member-function-of-a-given-signature.
/// </summary>
/// <typeparam name="TFunc">The type of the method to be detected, which must
/// be a template that receives a type and that can only be instantiated if
/// the method to be tested exists.</typeparam>
/// <typeparam name="TVoid">A type that collapses to <see langword="void" />.
/// </typeparam>
/// <typeparam name="TArgs">The arguments that are required to instantiate
/// <typeparamref name="TFunc" />.</typeparam>
template<template<class...> class TFunc, class TVoid, class... TArgs>
struct detector : std::false_type { };


/// <summary>
/// Specialisation for types that have the method.
/// </summary>
/// <typeparam name="TFunc"></typeparam>
/// <typeparam name="TArgs"></typeparam>
template<template<class...> class TFunc, class... TArgs>
struct detector<TFunc, std::void_t<TFunc<TArgs...>>, TArgs...>
    : std::true_type { };

LYRA_DETAIL_NAMESPACE_END

#endif /* !defined(_LYRA_DETECTOR_H) */
