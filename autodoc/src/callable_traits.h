// <copyright file="callablre_traits.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_CALLABLE_TRAITS_H)
#define _LYRA_CALLABLE_TRAITS_H
#pragma once

#include <functional>
#include <tuple>
#include <type_traits>

#include "visus/autodoc/api.h"


LYRA_DETAIL_NAMESPACE_BEGIN

/// <summary>
/// Implements a traits type for callables as described at
/// http://functionalcpp.wordpress.com/2013/08/05/function-traits/, which allows
/// for reasoning about the return type and argument types of a callable
/// (I can't believe that actually worked ...).
/// </summary>
/// <typeparam name="TCallable">The type of the callable.</typeparam>
template<class TCallable> struct callable_traits;

/// <summary>
/// Specialisation for C++-style function pointers.
/// </summary>
/// <typeparam name="TReturn">The return type of the function.</typeparam>
/// <typeparam name="TArgs">The arguments of the function.</typeparam>
template<class TReturn, class... TArgs>
struct callable_traits<TReturn(TArgs...)> {

    /// <summary>
    /// The types of the arguments.
    /// </summary>
    typedef std::tuple<TArgs...> argument_types;

    /// <summary>
    /// The type returned by the callable.
    /// </summary>
    typedef TReturn return_type;

    /// <summary>
    /// Retrieves the <typeparamref name="Index" />th argument type.
    /// </summary>
    template<std::size_t Index>
    using argument = std::tuple_element_t<Index, argument_types>;

    /// <summary>
    /// The number of arguments the callable expects.
    /// </summary>
    static constexpr  std::size_t arguments = sizeof...(TArgs);
};

/// <summary>
/// Specialisation for C function pointers.
/// </summary>
/// <typeparam name="TReturn">The return type of the function.</typeparam>
/// <typeparam name="TArgs">The arguments of the function.</typeparam>
template<class TReturn, class... TArgs>
struct callable_traits<TReturn(*)(TArgs...)> final
    : callable_traits<TReturn(TArgs...)> {
};

/// <summary>
/// Specialisation for pointer to member functions.
/// </summary>
/// <typeparam name="TType">The type of the class that contains the member
/// function.</typeparam>
/// <typeparam name="TReturn">The return type of the function.</typeparam>
/// <typeparam name="TArgs">The arguments of the function.</typeparam>
template<class TType, class TReturn, class... TArgs>
struct callable_traits<TReturn(TType:: *)(TArgs...)> final
    : callable_traits<TReturn(TArgs...)> {

    /// <summary>
    /// The type of the <see langword="this" /> pointer, which is the implicit
    /// first argument.
    /// </summary>
    typedef TType this_type;
};

/// <summary>
/// Specialisation for pointer to constant member functions.
/// </summary>
/// <typeparam name="TType">The type of the class that contains the member
/// function.</typeparam>
/// <typeparam name="TReturn">The return type of the function.</typeparam>
/// <typeparam name="TArgs">The arguments of the function.</typeparam>
template<class TType, class TReturn, class... TArgs>
struct callable_traits<TReturn(TType:: *)(TArgs...) const> final
    : callable_traits<TReturn(TArgs...)> {

    /// <summary>
    /// The type of the <see langword="this" /> pointer, which is the implicit
    /// first argument.
    /// </summary>
    typedef TType this_type;
};

/// <summary>
/// Specialistation for pointers to members.
/// </summary>
/// <typeparam name="TType">The type of the class that contains the member
/// function.</typeparam>
/// <typeparam name="TReturn">The return type of the member.</typeparam>
template<class TType, class TReturn>
struct callable_traits<TReturn(TType:: *)>
    : callable_traits<TReturn(TType&)> {
};

/// <summary>
/// The base implementation for functor types.
/// </summary>
/// <typeparam name="TCallable">The type of the callable.</typeparam>
template<class TCallable> struct callable_traits {

    /// <summary>
    /// The traits for the function call operator of the callable.
    /// </summary>
    typedef callable_traits<decltype(&TCallable::operator ())> call_traits;

    /// <summary>
    /// The types of the arguments.
    /// </summary>
    typedef typename call_traits::argument_types argument_types;

    /// <summary>
    /// The type returned by the callable.
    /// </summary>
    typedef typename call_traits::return_type return_type;

    /// <summary>
    /// The type of the <see langword="this" /> pointer, which is the implicit
    /// first argument.
    /// </summary>
    typedef typename call_traits::this_type this_type;

    /// <summary>
    /// Retrieves the <typeparamref name="Index" />th argument type.
    /// </summary>
    template<std::size_t Index>
    using argument = std::tuple_element_t<Index, argument_types>;

    /// <summary>
    /// The number of arguments the callable expects.
    /// </summary>
    static constexpr  std::size_t arguments = call_traits::arguments;
};

/// <summary>
/// Specialisation for lvalue reference of functor.
/// </summary>
/// <typeparam name="TCallable">The type of the callable.</typeparam>
template<class TCallable>
struct callable_traits<TCallable&> final : callable_traits<TCallable> {};

/// <summary>
/// Specialisation for rvalue reference of functor.
/// </summary>
/// <typeparam name="TCallable">The type of the callable.</typeparam>
template<class TCallable>
struct callable_traits<TCallable&&> final : callable_traits<TCallable> {};

/// <summary>
/// Determines the number of arguments of the specified
/// <see cref="TCallable" />.
/// </summary>
/// <typeparam name="TCallable">The callable to count the arguments
/// of.</typeparam>
template<class TCallable>
struct argument_count final : std::integral_constant<std::size_t,
    callable_traits<TCallable>::arguments> {
};

/// <summary>
/// Determines the number of arguments of the specified
/// <see cref="TCallable" />.
/// </summary>
/// <typeparam name="TCallable">The callable to count the arguments
/// of.</typeparam>
template<class TCallable>
constexpr auto argument_count_v = argument_count<TCallable>::value;

/// <summary>
/// Resolves the type of the <paramref name="Index" />th argument of the
/// given <see cref="TCallable" />.
/// </summary>
/// <typeparam name="TCallable">A callable type.</typeparam>
/// <typeparam name="Index">The zero-based index of the argument to retrieve
/// the type of.</typeparam>
template<class TCallable, std::size_t Index>
struct argument_type final {

    /// <summary>
    /// The type of the requested argument of the callable.
    /// </summary>
    typedef typename callable_traits<TCallable>::template argument<Index> type;
};

/// <summary>
/// Resolves the type of the <paramref name="Index" />th argument of the
/// given <see cref="TCallable" />.
/// </summary>
/// <typeparam name="TCallable">A callable type.</typeparam>
/// <typeparam name="Index">The zero-based index of the argument to retrieve
/// the type of.</typeparam>
template<class TCallable, std::size_t Index>
using argument_type_t = typename argument_type<TCallable, Index>::type;

/// <summary>
/// Derives the return type of the specified <see cref="TCallable" />.
/// </summary>
/// <typeparam name="TCallable">The type of a function pointer or functor.
/// </typeparam>
template<class TCallable> struct return_type final {

    /// <summary>
    /// The type returned by the callable.
    /// </summary>
    typedef typename callable_traits<TCallable>::return_type type;
};

/// <summary>
/// Derives the return type of a callable <see cref="TFunc" />.
/// </summary>
/// <typeparam name="TCallable">The type of a function pointer or functor.
/// </typeparam>
template<class TCallable>
using return_type_t = typename return_type<TCallable>::type;

/// <summary>
/// Derives the type of the <see langword="this" /> pointer of a
/// pointer-to-member call or of a functor.
/// </summary>
/// <typeparam name="TCallable">The type of a functor or pointer-to-member
/// callable</typeparam>
template<class TCallable> struct this_type final {

    /// <summary>
    /// The type of the <see langword="this" /> pointer, which is the implicit
    /// first argument.
    /// </summary>
    typedef typename callable_traits<TCallable>::this_type type;
};

/// <summary>
/// Derives the type of the <see langword="this" /> pointer of a
/// pointer-to-member call or of a functor.
/// </summary>
/// <typeparam name="TCallable">The type of a functor or pointer-to-member
/// callable</typeparam>
template<class TCallable>
using this_type_t = typename this_type<TCallable>::type;

LYRA_DETAIL_NAMESPACE_END

#endif /* !defined(_LYRA_CALLABLE_TRAITS_H) */
