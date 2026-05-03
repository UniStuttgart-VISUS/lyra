// <copyright file="property_set.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_PROPERTY_SET_H)
#define _LYRA_PROPERTY_SET_H
#pragma once

#include <functional>
#include <map>
#include <utility>

#include "visus/lyra/property_traits.h"


LYRA_NAMESPACE_BEGIN

// Forward declarations.
namespace detail { struct property_set_impl; }

/// <summary>
/// Represents a group of properties about the system.
/// </summary>
class LYRA_API property_set final {

public:

    /// <summary>
    /// The type-erased value type used to represent a property.
    /// </summary>
    typedef const void *value_type;

    /// <summary>
    /// The visitor callback that allows for enumerating all properties.
    /// </summary>
    typedef bool (*visitor_type)(const char *, value_type, std::size_t,
        property_type, void *);

    /// <summary>
    /// Initialises a new instance.
    /// </summary>
    inline property_set(void) noexcept : _impl(nullptr) { }

    /// <summary>
    /// Clone <see cref="other" />
    /// </summary>
    /// <param name="other">The object to be cloned.</param>
    property_set(_In_ const property_set& other);

    /// <summary>
    /// Move <see cref="other" />
    /// </summary>
    /// <param name="other">The object to be moved.</param>
    inline property_set(_Inout_ property_set&& other) noexcept
            : _impl(other._impl) {
        other._impl = nullptr;
    }

    /// <summary>
    /// Initialises a new instance.
    /// </summary>
    /// <param name="impl">The contents of the property set.</param>
    explicit property_set(_In_ detail::property_set_impl&& impl);

    /// <summary>
    /// Answer whether the property set contains a property with the specified
    /// name.
    /// </summary>
    /// <param name="name">The name of the property to search. The encoding of
    /// this string is assumed to be UTF-8. If this is not the case, the answer
    /// might be wrong.</param>
    /// <returns><see langword="true" /> if the property set contains a property
    /// with the specified name, <see langword="false" /> otherwise.</returns>
    bool contains(_In_z_ const char *name) const noexcept;

    /// <summary>
    /// Answer whether the property set is empty.
    /// </summary>
    /// <returns><see langword="true" /> if there is no property in the set,
    /// <see langword="false" /> otherwise.</returns>
    bool empty(void) const noexcept;

    /// <summary>
    /// Gets a pointer to the value of the property with the specified name.
    /// </summary>
    /// <param name="dst"></param>
    /// <param name="cnt"></param>
    /// <param name="type"></param>
    /// <param name="name"></param>
    /// <returns></returns>
    bool get(_Out_ value_type& dst, _Out_ std::size_t& cnt,
        _Out_ property_type& type, _In_z_ const char *name) const noexcept;

    /// <summary>
    /// Gets a pointer to the property identified by the
    /// <typeparam name="TProp" /> descriptor if the described property is in
    /// the property set.
    /// </summary>
    /// <typeparam name="TProp"></typeparam>
    /// <param name="cnt"></param>
    /// <returns></returns>
    template<class TProp> _Ret_maybenull_
    typename detail::property_traits<typename TProp::type>::pointer get(
        _Out_ std::size_t& cnt) const noexcept;

    /// <summary>
    /// Gets a pointer to the property identified by the
    /// <typeparam name="TProp" /> descriptor if the described property is in
    /// the property set.
    /// </summary>
    /// <typeparam name="TProp"></typeparam>
    /// <param name=""></param>
    /// <returns></returns>
    template<class TProp> _Ret_maybenull_
    inline typename detail::property_traits<typename TProp::type>::pointer get(
            void) const noexcept {
        std::size_t cnt;
        auto retval = this->get<TProp>(cnt);
        return (cnt > 0) ? retval : nullptr;
    }

    /// <summary>
    /// Answer the JSON representation of the property set.
    /// </summary>
    /// <returns></returns>
    _Ret_maybenull_z_ const char *json(void) const noexcept;

    /// <summary>
    /// Answer the number (and names) of the properties in the set.
    /// </summary>
    /// <param name="dst">A buffer to receive pointers to at most
    /// <paramref name="cnt" /> property names.</param>
    /// <param name="cnt">The number of elements in <paramref name="dst" />.
    /// </param>
    /// <returns>The total number of properties in the set.</returns>
    std::size_t properties(_Out_writes_opt_(cnt) const char **dst = nullptr,
        _In_ std::size_t cnt = 0) const noexcept;

    /// <summary>
    /// Answer the size of the property set.
    /// </summary>
    /// <returns>The total number of properties in the set.</returns>
    std::size_t size(void) const noexcept;

    /// <summary>
    /// Assignment.
    /// </summary>
    /// <param name="rhs">The right-hand-side operand.</param>
    /// <returns><c>*<see langword="this" /></c>.</returns>
    property_set& operator =(_In_ const property_set& rhs);

    /// <summary>
    /// Move assignment.
    /// </summary>
    /// <param name="rhs">The right-hand-side operand.</param>
    /// <returns><c>*<see langword="this" /></c>.</returns>
    property_set& operator =(_Inout_ property_set&& rhs) noexcept;

    /// <summary>
    /// Invoke <paramref name="visitor" /> for each property in the set.
    /// </summary>
    /// <param name="visitor">A visitor that will be invoked for each property.
    /// </param>
    /// <param name="user_data">An optional pointer to user defined data passed
    /// to the visitor.</param>
    /// <returns>The number of properties visited.</returns>
    std::size_t visit(_In_ const visitor_type visitor,
        _In_opt_ void *user_data) const;

    /// <summary>
    /// Invoke <paramref name="visitor" /> for each property in the set.
    /// </summary>
    /// <typeparam name="TVisitor"></typeparam>
    /// <param name="visitor"></param>
    /// <returns></returns>
    template<class TVisitor, std::size_t... Idx> std::size_t visit(
        _In_ const TVisitor& visitor,
        std::index_sequence<Idx...> = std::index_sequence<>()) const noexcept;

private:

    typedef detail::property_set_impl impl;

    template<class TVisitor, class TType>
    static constexpr auto visit_array = std::is_invocable_v<TVisitor,
        const char *, const TType *, std::size_t>;

    template<class TVisitor, class TType>
    static constexpr auto visit_array_r = std::is_invocable_r_v<bool, TVisitor,
        const char *, const TType *, std::size_t>;

    template<class TVisitor, class TType>
    static constexpr auto visit_scalar = std::is_invocable_v<TVisitor,
        const char *, const TType&>;

    template<class TVisitor, class TType>
    static constexpr auto visit_scalar_r = std::is_invocable_r_v<bool, TVisitor,
        const char *, const TType&>;

    //void merge(_In_ property_set&& other);

    void merge_to(_Inout_ detail::property_set_impl& dst);

    impl *_impl;

    friend impl;
};

LYRA_NAMESPACE_END

#include "visus/lyra/property_set.inl"

#endif /* !defined(_LYRA_PROPERTY_SET_H) */
