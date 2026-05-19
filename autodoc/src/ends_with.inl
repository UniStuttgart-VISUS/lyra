// <copyright file="ends_with.inl" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>


/*
 * LYRA_DETAIL_NAMESPACE::ends_with
 */
template<class TChar>
bool LYRA_DETAIL_NAMESPACE::ends_with(
        _In_opt_z_ const TChar *str,
        _In_opt_z_ const TChar *suffix,
        _In_ const string_comparison comparison) noexcept {
    if (str == nullptr) {
        // If the string is null, we only consider it the same if the suffix is
        // als null.
        return (suffix == nullptr);
    }

    if (suffix == nullptr) {
        // If the suffix is null, it can never be contained unless the string
        // is also null, which is not the case at this point.
        assert(str != nullptr);
        return false;
    }

    const auto str_len = std::char_traits<TChar>::length(str);
    const auto suf_len = std::char_traits<TChar>::length(suffix);

    if (suf_len > str_len) {
        // If the suffix is longer than the string, it can never be contained.
        return false;
    }

    // Otherwise, we need to compare the actual contents, which we delegate to
    // the equals function for handling case sensitivity and other details.
    return equals(str + (str_len - suf_len), suffix, comparison);
}
