// <copyright file="property.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_PROPERTY_H)
#define _LYRA_PROPERTY_H
#pragma once

#include <cstdlib>
#include <functional>
#include <initializer_list>
#include <iterator>
#include <memory>
#include <string>

#include "visus/lyra/property_traits.h"


LYRA_DETAIL_NAMESPACE_BEGIN

/// <summary>
/// Stores a type-erased property value. This type is effectively a unique
/// pointer that stores the property meta data as well.
/// </summary>
class LYRA_TEST_API property final {

public:

    /// <summary>
    /// Initialises a new instance.
    /// </summary>
    inline property(void)
        : _count(0),
        _data(nullptr),
        _delete(nullptr),
        _type(property_type::none) { }

    /// <summary>
    /// Initialises a new instance.
    /// </summary>
    /// <typeparam name="TData"></typeparam>
    /// <typeparam name="TDelete"></typeparam>
    /// <param name="name">The name of the property. The encoding of this string
    /// is assumed to be UTF-8 all the time.</param>
    /// <param name="data">The data representing the property. The object will
    /// take ownership of this pointer and free it using the provided
    /// <paramref name="deleter" />.</param>
    /// <param name="count"></param>
    /// <param name="deleter"></param>
    template<class TData, class TDelete> inline property(
            _Inout_ std::string&& name,
            _In_ const TData *data,
            _In_ std::size_t count,
            _In_ TDelete deleter)
        : _count(count),
            _data(static_cast<void *>(data)),
            _name(std::move(name)),
            _delete(deleter),
            _type(property_type_v<std::decay_t<TData>>) { }

    /// <summary>
    /// Initialises a new instance.
    /// </summary>
    /// <typeparam name="TType">The type of the property.</typeparam>
    /// <param name="name">The name of the property. The encoding of this string
    /// is assumed to be UTF-8 all the time.</param>
    /// <param name="data">The property to be stored.</param>
    template<class TType> inline property(
            _Inout_ std::string&& name,
            _In_ const TType& data,
            const property_traits<std::decay_t<TType>> *_ = nullptr)
        : _count(1),
            _data(new TType(data)),
            _delete(scalar_delete<TType>),
            _name(std::move(name)),
            _type(property_type_v<std::decay_t<TType>>) { }

    /// <summary>
    /// Initialises a new instance.
    /// </summary>
    /// <typeparam name="TType">The type of the property.</typeparam>
    /// <param name="name">The name of the property. The encoding of this string
    /// is assumed to be UTF-8 all the time.</param>
    /// <param name="data">The property to be stored.</param>
    template<class TType> inline property(
            _Inout_ std::string&& name,
            _In_ TType&& data,
            const property_traits<std::decay_t<TType>> *_ = nullptr)
        : _count(1),
            _data(new std::decay_t<TType>(std::move(data))),
            _delete(scalar_delete<TType>),
            _name(name),
            _type(property_type_v<std::decay_t<TType>>) { }

    /// <summary>
    /// Initialises a new (vector-valued) instance.
    /// </summary>
    /// <typeparam name="TType"></typeparam>
    /// <param name="name">The name of the property. The encoding of this string
    /// is assumed to be UTF-8 all the time.</param>
    /// <param name="data"></param>
    template<class TType> inline property(
            _Inout_ std::string&& name,
            _In_ const std::initializer_list<TType>& data,
            const property_traits<std::decay_t<TType>> *_ = nullptr)
        : _count(data.size()),
            _data(nullptr),
            _delete(vector_delete<TType>),
            _name(std::move(name)),
            _type(property_type_v<std::decay_t<TType>>) {
        auto d = std::make_unique< iterated_type<TIterator>[]>(this->_count);
        std::copy(data.begin(), data.end(), d.get());
        this->_data = d.release();
    }

    /// <summary>
    /// Initialises a new instance.
    /// </summary>
    /// <typeparam name="TIterator"></typeparam>
    /// <param name="name">The name of the property. The encoding of this string
    /// is assumed to be UTF-8 all the time.</param>
    /// <param name="begin"></param>
    /// <param name="end"></param>
    template<class TIterator> inline property(
            _Inout_ std::string&& name,
            _In_ const TIterator begin,
            _In_ const TIterator end)
        : _count(std::distance(begin, end)),
            _data(nullptr),
            _delete(vector_delete<iterated_type<TIterator>>),
            _name(std::move(name)),
            _type(property_type_v<std::decay_t<iterated_type<TIterator>>>) {
        auto d = std::make_unique<iterated_type<TIterator>[]>(this->_count);
        std::copy(begin, end, d.get());
        this->_data = d.release();
    }

    /// <summary>
    /// Initialise from move.
    /// </summary>
    /// <param name="other">The object to be moved.</param>
    inline property(_Inout_ property&& other) noexcept
            : _count(other._count),
            _data(other._data),
            _delete(other._delete),
            _name(std::move(other._name)),
            _type(other._type) {
        other._count = 0;
        other._data = nullptr;
        other._delete = nullptr;
        other._type = property_type::none;
    }

    /// <summary>
    /// Finalises the instance.
    /// </summary>
    inline ~property(void) noexcept {
        this->clear();
    }

    /// <summary>
    /// Provides access to the property data.
    /// </summary>
    /// <typeparam name="TType"></typeparam>
    /// <returns>A pointer to the property data.</returns>
    template<class TType> inline const TType *as(void) const noexcept {
        assert(this->_type == property_traits<TType>::value);
        return static_cast<const TType *>(this->_data);
    }

    /// <summary>
    /// Answer the number of elements in the property data.
    /// </summary>
    /// <returns></returns>
    inline std::size_t count(void) const noexcept {
        return this->_count;
    }

    /// <summary>
    /// Provides access to the property data.
    /// </summary>
    /// <returns>A pointer to the property data.</returns>
    inline const void *data(void) const noexcept {
        return this->_data;
    }

    /// <summary>
    /// Answer whether the property contains no data.
    /// </summary>
    /// <returns><see langword="true" /> if there is no data in the property,
    /// <see langword="false" /> otherwise.</returns>
    inline bool empty(void) const noexcept {
        return ((this->_data == nullptr) || (this->_count <= 0));
    }

    /// <summary>
    /// Answer the name of the property.
    /// </summary>
    /// <returns>The name of the property. It is assumed that this value is
    /// encoded in UTF-8 all the time.</returns>
    inline const std::string& name(void) const noexcept {
        return this->_name;
    }

    /// <summary>
    /// Answer the type of the property stored.
    /// </summary>
    /// <returns></returns>
    inline property_type type(void) const noexcept {
        return this->_type;
    }

    /// <summary>
    /// Move assignment.
    /// </summary>
    /// <param name="rhs">The right-hand-side operand.</param>
    /// <returns><c>*<see langword="this" /></c>.</returns>
    property& operator =(_Inout_ property&& rhs) noexcept;

private:

    template<class TIterator>
    using iterated_type = typename std::iterator_traits<TIterator>::value_type;

    template<class TType>
    static inline void scalar_delete(const void *data) noexcept {
        typedef std::decay_t<TType> type;
        delete static_cast<const type *>(data);
    }

    template<class TType>
    static inline void vector_delete(const void *data) noexcept {
        typedef std::decay_t<TType> type;
        delete[] static_cast<const type *>(data);
    }

    void clear(void) noexcept;

    std::size_t _count;
    const void *_data;
    std::function<void(const void *)> _delete;
    std::string _name;
    property_type _type;
};

LYRA_DETAIL_NAMESPACE_END

#endif /* !defined(_LYRA_PROPERTY_H) */
