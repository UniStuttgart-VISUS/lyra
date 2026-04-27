// <copyright file="property.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "property.h"

#include <memory>


/*
 * LYRA_DETAIL_NAMESPACE::operator =
 */
LYRA_DETAIL_NAMESPACE::property& LYRA_DETAIL_NAMESPACE::property::operator =(
        _Inout_ property&& rhs) noexcept {
    if (this != std::addressof(rhs)) {
        this->_count = rhs._count;
        rhs._count = 0;

        this->clear();
        this->_data = rhs._data;
        rhs._data = nullptr;

        this->_delete = rhs._delete;
        rhs._delete = nullptr;

        this->_name = std::move(rhs._name);

        this->_type = rhs._type;
        rhs._type = property_type::none;
    }
    return *this;
}


/*
 * LYRA_DETAIL_NAMESPACE::property::clear
 */
void LYRA_DETAIL_NAMESPACE::property::clear(void) noexcept {
    if ((this->_delete != nullptr) && (this->_data != nullptr)) {
        this->_delete(this->_data);
        this->_data = nullptr;
    }
}
