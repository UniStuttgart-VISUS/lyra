// <copyright file="invoke.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "property_set_impl.h"

#include "visus/lyra/convert_string.h"


/*
 * LYRA_DETAIL_NAMESPACE::property_set_impl::add
 */
void LYRA_DETAIL_NAMESPACE::property_set_impl::add(
        _Inout_ std::string&& key,
        _In_opt_z_ const char *value) {
    if (value != nullptr) {
        this->add(std::move(key), multi_sz::for_string(value));
    }
}


/*
 * LYRA_DETAIL_NAMESPACE::property_set_impl::add
 */
void LYRA_DETAIL_NAMESPACE::property_set_impl::add(
        _In_z_ const key_type::value_type *key,
        _In_opt_z_ const wchar_t *value) {
    if (value != nullptr) {
        this->add(key, to_utf8(value));
    }
}


/*
 * LYRA_DETAIL_NAMESPACE::property_set_impl::add
 */
void LYRA_DETAIL_NAMESPACE::property_set_impl::add(
        _In_z_ const key_type::value_type *key,
        _In_opt_z_ const char16_t *value) {
    if (value != nullptr) {
        this->add(key, to_utf8(value));
    }
}


/*
 * LYRA_DETAIL_NAMESPACE::property_set_impl::find
 */
_Ret_maybenull_ const LYRA_DETAIL_NAMESPACE::property_set_impl::value_type *
LYRA_DETAIL_NAMESPACE::property_set_impl::find(
        _In_ const key_type& key) const noexcept {
    auto it = this->values.find(key);
    return (it != this->values.end()) ? std::addressof(it->second) : nullptr;
}


/*
 * LYRA_DETAIL_NAMESPACE::merge
 */
LYRA_DETAIL_NAMESPACE::property_set_impl& LYRA_DETAIL_NAMESPACE::merge(
        _In_ property_set_impl& dst,
        _Inout_ property_set&& src) {
    if (src._impl != nullptr) {
        for (auto& s : src._impl->values) {
            assert(dst.find(s.first) == nullptr);
            dst.values.emplace(std::move(s.first), std::move(s.second));
        }
        src._impl->values.clear();
    }

    return dst;
}


/*
 * LYRA_DETAIL_NAMESPACE::realise
 */
LYRA_NAMESPACE::property_set& LYRA_DETAIL_NAMESPACE::realise(
        _In_ property_set& dst,
        _Inout_ property_set_impl&& src) {
    assert(dst._impl == nullptr);
    dst._impl = new property_set_impl();
    dst._impl->values = std::move(src.values);
    return dst;
}


/*
 * LYRA_DETAIL_NAMESPACE::to_property_set
 */
LYRA_NAMESPACE::property_set LYRA_DETAIL_NAMESPACE::to_property_set(
        _Inout_ property_set_impl&& src) {
    property_set retval;
    realise(retval, std::move(src));
    return retval;
}
