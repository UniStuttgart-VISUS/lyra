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
        return nullptr;
    }

    if (type != traits::value) {
        return nullptr;
    }

    return traits::data(*static_cast<const typename traits::type *>(value));
}
