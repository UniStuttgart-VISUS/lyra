// <copyright file="property_set.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "visus/lyra/property_set.h"

#include <memory>
#include <type_traits>

#include "visus/lyra/property_traits.h"

#include "property_set_impl.h"


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
 * LYRA_NAMESPACE::property_set::contains
 */
bool LYRA_NAMESPACE::property_set::contains(
        _In_z_ const char *name) const noexcept {
    if ((this->_impl == nullptr) || (name == nullptr)) {
        return false;
    }

    const auto it = this->_impl->find(name);
    return (it != nullptr);
}


/*
 * LYRA_NAMESPACE::property_set::empty
 */
bool LYRA_NAMESPACE::property_set::empty(void) const noexcept {
    return ((this->_impl == nullptr) || this->_impl->values.empty());
}


/*
 * LYRA_NAMESPACE::property_set::get
 */
bool LYRA_NAMESPACE::property_set::get(
        _Out_ value_type& dst,
        _Out_ std::size_t& cnt,
        _Out_ property_type& type,
        _In_z_ const char *name) const noexcept {
    if ((this->_impl == nullptr) || (name == nullptr)) {
        return false;
    }

    const auto retval = this->_impl->find(name);
    if (retval == nullptr) {
        return false;
    }

    std::visit([&dst, &cnt, &type](const auto& v) {
        typedef std::decay_t<decltype(v)> value_type;
        typedef detail::property_variant_access<value_type> accessor;
        dst = accessor::get(v);
        cnt = accessor::count(v);
        type = accessor::type;
    }, *retval);

    return true;
}


/*
 * LYRA_NAMESPACE::property_set::properties
 */
std::size_t LYRA_NAMESPACE::property_set::properties(
        _Out_writes_opt_(cnt) const char **dst,
        _In_ std::size_t cnt) const noexcept {
    const auto retval = this->size();

    if (dst == nullptr) {
        cnt = 0;
    }

    if ((cnt > 0) && (retval > 0)) {
        auto cur = static_cast<std::size_t>(0);
        const auto end = this->_impl->values.end();
        const auto retval = this->_impl->values.size();

        for (auto it = this->_impl->values.begin();
                (it != end) && (cur < cnt);
                ++it, ++cur) {
            *dst++ = it->first.c_str();
        }
    }

    return retval;
}


/*
 * LYRA_NAMESPACE::property_set::size
 */
std::size_t LYRA_NAMESPACE::property_set::size(void) const noexcept {
    return (this->_impl != nullptr) ? this->_impl->values.size() : 0;
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
