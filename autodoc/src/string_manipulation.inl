// <copyright file="string_manipulation.inl" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>


/*
 * LYRA_DETAIL_NAMESPACE::null_terminate
 */
template<class TChar>
std::vector<TChar>& LYRA_DETAIL_NAMESPACE::null_terminate(
        _In_ std::vector<TChar>& str) {
    if (str.empty() || (str.back() != 0)) {
        str.push_back(static_cast<TChar>(0));
    }
    return str;
}


/*
 * LYRA_DETAIL_NAMESPACE:.remove_trailing_nulls
 */
template<class TIterator>
TIterator LYRA_DETAIL_NAMESPACE::remove_trailing_nulls(
        _In_ const TIterator begin, _In_ const TIterator end) {
    return trim_right_if(begin, end, [](const auto c) { return c == 0; });
}


/*
 * LYRA_DETAIL_NAMESPACE::replace_if
 */
template<class TIterator, class TPredicate>
std::size_t LYRA_DETAIL_NAMESPACE::replace_if(
        _In_ const TIterator begin,
        _In_ const TIterator end,
        _In_ const TPredicate predicate,
        _In_ const typename std::iterator_traits<TIterator>::value_type& value) {
    std::size_t retval = 0;

    for (auto it = begin; it != end; ++it) {
        if (predicate(*it)) {
            *it = value;
            ++retval;
        }
    }

    return retval;
}
