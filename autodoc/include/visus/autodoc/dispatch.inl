// <copyright file="dispatch.inl" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>


/*
 * LYRA_DETAIL_NAMESPACE::dispatch_traits
 */
template<class TType, template<TType> class TTraits, class TCallback,
    TType... Values>
bool LYRA_DETAIL_NAMESPACE::dispatch_traits(_In_ const TType value,
        _In_ TCallback callback,
        _In_ const dispatch_list<TType, Values...>) {
    // Note: Do not make this static!
    const std::map<TType, std::function<void(void)>> dispatchers = {
        {
            Values,
            ([&callback](void) {
                typedef TTraits<Values> traits_type;
                callback(static_cast<traits_type *>(nullptr));
            })
        }...
    };

    auto it = dispatchers.find(value);
    if (it == dispatchers.end()) {
        LYRA_TRACE(_T("The enumeration value %" PRIu64 " could not be ")
            _T("dispatched because it is not in the dispatch list."),
            static_cast<std::uint64_t>(value));
        return false;
    }

    it->second();
    return true;
}
