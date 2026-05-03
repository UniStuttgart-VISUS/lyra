// <copyright file="property_set.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "visus/lyra/property_set.h"

#include <memory>
#include <type_traits>

#include <nlohmann/json.hpp>

#include "visus/lyra/multi_sz.h"
#include "visus/lyra/property_traits.h"

#include "property_set_impl.h"


LYRA_NAMESPACE_BEGIN

/// <summary>
/// Converts a <see cref="property_set" /> to a JSON object.
/// </summary>
static void to_json(nlohmann::json& j, const property_set& p) {
    j = nlohmann::json::object();

    p.visit([&j](const char *name, const auto value, const std::size_t cnt) {
        if (cnt == 1) {
            j[name] = *value;

        } else {
            auto a = nlohmann::json::array();

            // Note: std::copy_n does not work somehow at this point ...
            for (std::size_t i = 0; i < cnt; ++i) {
                a.push_back(value[i]);
            }

            j[name] = a;
        }
    });
}

/// <summary>
/// Converts a <see cref="multi_sz" /> to a JSON value.
/// </summary>
static void to_json(nlohmann::json& j, const multi_sz& v) {
    if (v.count() == 1) {
        auto vv = v.data();
        assert(vv != nullptr);
        j = std::string(vv);
    } else {
        j = static_cast<std::vector<std::string>>(v);
    }
}

LYRA_NAMESPACE_END


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
 * LYRA_NAMESPACE::property_set::property_set
 */
LYRA_NAMESPACE::property_set::property_set(
        _In_ detail::property_set_impl&& impl)
    : _impl(new detail::property_set_impl(std::move(impl))) { }


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
    cnt = 0;
    type = property_type::none;

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
 * LYRA_NAMESPACE::property_set::json
 */
_Ret_maybenull_z_ const char *LYRA_NAMESPACE::property_set::json(
        void) const noexcept {
    if (this->_impl == nullptr) {
        return nullptr;
    }

    if (this->_impl->json.empty()) {
        ::nlohmann::json j = *this;
        this->_impl->json = j.dump();
    }

    return this->_impl->json.c_str();
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


/*
 * LYRA_NAMESPACE::property_set::visit
 */
std::size_t LYRA_NAMESPACE::property_set::visit(
        _In_ const visitor_type visitor,
        _In_opt_ void *user_data) const {
    auto retval = static_cast<std::size_t>(0);

    if ((visitor == nullptr) || (this->_impl == nullptr)) {
        return retval;
    }

    for (auto& p : this->_impl->values) {
        ++retval;

        const bool next = std::visit([&](const auto& v) {
            typedef std::decay_t<decltype(v)> value_type;
            typedef detail::property_variant_access<value_type> accessor;
            return visitor(p.first.c_str(),
                accessor::get(v),
                accessor::count(v),
                accessor::type, user_data);
        }, p.second);

        if (!next) {
            break;
        }
    }

    return retval;
}


///*
// * LYRA_NAMESPACE::property_set::merge
// */
//void LYRA_NAMESPACE::property_set::merge(_In_ property_set&& other) {
//    if (this == std::addressof(other)) {
//        return;
//    }
//
//    if (other._impl == nullptr) {
//        return;
//    }
//
//    if (this->_impl == nullptr) {
//        this->_impl = other._impl;
//        other._impl = nullptr;
//        return;
//    }
//
//    auto& dst = this->_impl->values;
//    auto& src = other._impl->values;
//    for (auto& s : src) {
//        if (dst.find(s.first) == dst.end()) {
//            dst.emplace(std::move(s.first), std::move(s.second));
//        }
//        other._impl->values.clear();
//    }
//}


/*
 * LYRA_NAMESPACE::property_set::merge_to
 */
void LYRA_NAMESPACE::property_set::merge_to(
        _Inout_ detail::property_set_impl& dst){
    if (this->_impl == std::addressof(dst)) {
        return;
    }

    if (this->_impl == nullptr) {
        return;
    }

    if (dst.values.empty()) {
        dst = std::move(*this->_impl);
        this->_impl = nullptr;
        return;
    }

    assert(this->_impl != nullptr);
    auto& src = this->_impl->values;
    for (auto& s : src) {
        if (dst.values.find(s.first) == dst.values.end()) {
            dst.values.emplace(std::move(s.first), std::move(s.second));
        }
        this->_impl = nullptr;
    }
}
