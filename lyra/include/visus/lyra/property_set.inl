// <copyright file="property_set.inl" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>


/*
 * LYRA_NAMESPACE::property_set::get
 */
template<class TProp> _Ret_maybenull_
typename LYRA_DETAIL_NAMESPACE::property_traits<typename TProp::type>::pointer
LYRA_NAMESPACE::property_set::get(_Out_ std::size_t& cnt) const noexcept {
    typedef detail::property_traits<typename TProp::type> traits;

    property_type type;
    value_type value;

    if (!this->get(value, cnt, type, TProp::name)) {
        // The property does not exist.
        return nullptr;
    }

    assert(type == traits::value);
    if (type != traits::value) {
        // The property exists, but has a different type.
        return nullptr;
    }

    return static_cast<typename traits::pointer>(value);
}


/*
 * LYRA_NAMESPACE::property_set::visit
 */
template<class TVisitor, std::size_t... Idx>
std::size_t LYRA_NAMESPACE::property_set::visit(
        _In_ const TVisitor& visitor,
        std::index_sequence<Idx...>) const noexcept {
    typedef std::function<bool(const char *, value_type, std::size_t)>
        visitor_type;

    if (this->_impl == nullptr) {
        // If the object was disposed, bail out directly.
        return 0;
    }

    if constexpr (sizeof...(Idx) != detail::property_types::size) {
        // If a wrong number of indices was given, dispatch to the right one.
        return this->visit(
            visitor,
            std::make_index_sequence<detail::property_types::size>());
    }

    // Build a dispatch table that invokes the visitor with the right type.
    std::map<property_type, visitor_type> dispatch = {
        {
            detail::property_at<Idx>,
            [&visitor](const char *n, value_type v, std::size_t c) {
                typedef detail::property_traits_at<Idx> traits;
                auto w = static_cast<typename traits::pointer>(v);
                return visitor(n, *w, c);
            }
        }...
    };

    return this->visit([](const char *n, value_type v, std::size_t c,
            property_type t, void *d) {
        auto& table = *static_cast<std::map<property_type, visitor_type>*>(d);
        auto it = table.find(t);
        if (it == table.end()) {
            return true;
        }
        return it->second(n, v, c);
    }, &dispatch);
}
