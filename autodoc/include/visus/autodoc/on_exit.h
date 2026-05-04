// <copyright file="on_exit.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2021 - 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_ON_EXIT_H)
#define _LYRA_ON_EXIT_H
#pragma once

#include "visus/autodoc/unique_variable.h"


LYRA_DETAIL_NAMESPACE_BEGIN

/// <summary>
/// A scope guard object that invokes a user-defined function.
/// </summary>
/// <typeparam name="TExitHandler">The type of the lambda expression to
/// be invoked.Note that the lambda expression passed for this type must
/// not throw as it might be called from the destructor.</typeparam>
template<class TExitHandler> class on_exit_guard {

public:

    /// <summary>
    /// The type of the lambda expression to be invoked.
    /// </summary>
    typedef TExitHandler exit_handler_type;

    /// <summary>
    /// Initialises a new instance.
    /// </summary>
    /// <param name="handler">The lambda to be invoked.</param>
    inline explicit on_exit_guard(_Inout_ exit_handler_type&& handler)
        : _handler(std::forward<exit_handler_type>(handler)),
        _invoked(false) { }

    on_exit_guard(const on_exit_guard&) = delete;

    /// <summary>
    /// Move <paramref name="rhs" />.
    /// </summary>
    /// <param name="rhs">The object to be moved.</param>
    inline on_exit_guard(_Inout_ on_exit_guard&& rhs) noexcept
            : _handler(std::move(rhs._handler)),
            _invoked(rhs._invoked) {
        rhs._invoked = true;
    }

    /// <summary>
    /// Finalises the instance.
    /// </summary>
    inline ~on_exit_guard(void) {
        this->invoke();
    }

    /// <summary>
    /// If not yet called or cancelled, invoke the exit handler.
    /// </summary>
    void invoke(void) noexcept;

    /// <summary>
    /// Prevent the exit handler from being called.
    /// </summary>
    /// <remarks>
    /// This method marks the handler as being called without actually
    /// calling it. Note that this operation cannot be undone, it will
    /// ultimately prevent the lambda from being called.
    /// </remarks>
    inline void cancel(void) noexcept {
        this->_invoked = true;
    }

    on_exit_guard& operator =(const on_exit_guard&) = delete;

    /// <summary>
    /// Move <paramref name="rhs" />.
    /// </summary>
    /// <param name="rhs">The object to be moved.</param>
    /// <returns><c>*this</c></returns>
    on_exit_guard& operator =(on_exit_guard&& rhs) noexcept;

    /// <summary>
    /// Answer whether the exit handler is still to be invoked.
    /// </summary>
    /// <returns><c>true</c> if the lambda needs to be invoked, <c>false</c>
    /// otherwise.</returns>
    inline operator bool(void) const {
        return !this->_invoked;
    }

private:

    exit_handler_type _handler;
    bool _invoked;

};


/// <summary>
/// Create an exit guard that executes <paramref name="handler" /> if
/// it is destroyed.
/// </summary>
/// <typeparam name="TExitHandler"></typeparam>
/// <param name="handler"></param>
/// <returns></returns>
template<class TExitHandler>
detail::on_exit_guard<TExitHandler> on_exit(TExitHandler&& handler) {
    return detail::on_exit_guard<TExitHandler>(std::forward<TExitHandler>(
        handler));
}

LYRA_DETAIL_NAMESPACE_END

/// <summary>
/// Declares a exit guard variable that executes the
/// <paramref name="exit_handler" /> lambda expression when it is destroyed.
/// </summary>
#define LYRA_ON_EXIT(exit_handler)\
volatile auto LYRA_UNIQUE_VARIABLE(__lyra_scope_guard)\
    = LYRA_DETAIL_NAMESPACE::on_exit(exit_handler)

#include "visus/autodoc/on_exit.inl"

#endif /* !defined(_LYRA_ON_EXIT_H) */
