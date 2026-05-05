// <copyright file="wbem.inl" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>


/*
 * LYRA_DETAIL_NAMESPACE::foreach_wbem
 */
template<class TCallback, ULONG Batch>
std::size_t LYRA_DETAIL_NAMESPACE::foreach_wbem(
        _In_ IEnumWbemClassObject *enumerator,
        _In_ const TCallback callback,
        _In_ const WBEM_TIMEOUT_TYPE timeout) {
    static constexpr auto can_abort = std::is_invocable_r_v<bool, TCallback,
        IWbemClassObject *>;
    THROW_HR_IF(E_POINTER, enumerator == nullptr);

    HRESULT hr = WBEM_S_NO_ERROR;
    std::array<IWbemClassObject *, Batch> objects;
    std::size_t retval = 0;

    while (hr == WBEM_S_NO_ERROR) {
        ULONG cnt = 0;

        if (SUCCEEDED(hr = enumerator->Next(timeout, Batch, objects.data(),
                &cnt))) {
            LYRA_ON_EXIT([&](void) {
                for (ULONG i = 0; i < cnt; ++i) {
                    assert(objects[i] != nullptr);
                    objects[i]->Release();
                }
            });

            for (ULONG i = 0; i < cnt; ++i) {
                ++retval;
                if constexpr (can_abort) {
                    if (!callback(objects[i])) {
                        break;
                    }
                } else {
                    callback(objects[i]);
                }
            } /* for (ULONG i = 0; i < cnt; ++i)*/
        }
    } /* while (hr == WBEM_S_NO_ERROR) */

    THROW_HR_IF(hr, hr != WBEM_S_FALSE);
    return retval;
}
