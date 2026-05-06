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
