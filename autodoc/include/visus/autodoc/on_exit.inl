// <copyright file="on_exit.inl" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2021 - 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>


/*
 * LYRA_DETAIL_NAMESPACE::on_exit_guard<TExitHandler>::Invoke
 */
template<class TExitHandler>
void LYRA_DETAIL_NAMESPACE::on_exit_guard<TExitHandler>::invoke(void) noexcept {
    if (*this) {
        this->_handler();
        this->_invoked = true;
    }
}


/*
 * LYRA_DETAIL_NAMESPACE::on_exit_guard<T>::operator =
 */
template<class TExitHandler>
LYRA_DETAIL_NAMESPACE::on_exit_guard<TExitHandler>&
LYRA_DETAIL_NAMESPACE::on_exit_guard<TExitHandler>::operator =(
        _Inout_ on_exit_guard&& rhs) noexcept {
    if (this != std::addressof(rhs)) {
        // TODO: do we want to invoke here?
        this->_handler = std::move(rhs._handler);
        this->_invoke = rhs._invoke;
        rhs._invoked = true;
    }

    return *this;
}
