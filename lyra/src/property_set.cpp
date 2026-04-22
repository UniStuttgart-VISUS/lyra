// <copyright file="property_set.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "visus/lyra/property_set.h"

#include <memory>


/*
 * LYRA_NAMESPACE::property_set::property_set
 */
LYRA_NAMESPACE::property_set::property_set(_In_ const property_set& other)
        : _impl(nullptr) {
    if (other._impl != nullptr) {
        this->_impl = new detail::property_set_impl(*other._impl);
    }
}


/*
 * LYRA_NAMESPACE::property_set::empty
 */
bool LYRA_NAMESPACE::property_set::empty(void) const noexcept {
    return (this->_impl == nullptr);
}


/*
 * LYRA_NAMESPACE::property_set::operator =
 */
LYRA_NAMESPACE::property_set& LYRA_NAMESPACE::property_set::operator =(
        _In_ const property_set& rhs) {
    if (this != std::addressof(rhs)) {
        delete this->_impl;
        this->_impl = (rhs._impl != nullptr)
            ? new detail::property_set_impl(*rhs._impl)
            : nullptr;
    }

    return *this;
}


/*
 * LYRA_NAMESPACE::property_set::operator =
 */
LYRA_NAMESPACE::property_set& LYRA_NAMESPACE::property_set::operator =(
        _Inout_ property_set&& rhs) noexcept {
    if (this != std::addressof(rhs)) {
        delete this->_impl;
        this->_impl = rhs._impl;
        rhs._impl = nullptr;
    }

    return *this;
}
