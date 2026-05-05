// <copyright file="vds_api.inl" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>


/*
 * LYRA_DETAIL_NAMESPACE::foreach_vds_service_provider
 */
template<class TCallback>
void LYRA_DETAIL_NAMESPACE::foreach_vds_service_provider(
        _In_ IVdsService *vds, _In_ TCallback callback) {
    THROW_HR_IF(E_POINTER, vds == nullptr);

    wil::com_ptr<IEnumVdsObject> enumerator;
    THROW_IF_FAILED(vds->QueryProviders(VDS_QUERY_HARDWARE_PROVIDERS
        | VDS_QUERY_SOFTWARE_PROVIDERS | VDS_QUERY_VIRTUALDISK_PROVIDERS,
        enumerator.put()));

    wil::com_ptr<IVdsHwProvider> hw_provider;
    wil::com_ptr<IUnknown> provider;
    wil::com_ptr<IVdsSwProvider> sw_provider;
    wil::com_ptr<IVdsVdProvider> vd_provider;

    while (enumerator->Next(1, provider.put(), nullptr) == S_OK) {
        if (SUCCEEDED(provider->QueryInterface(IID_IVdsHwProvider,
                hw_provider.put_void()))) {
            LYRA_TRACE("Found hardware provider.");
            callback(hw_provider.get());

        } else if (SUCCEEDED(provider->QueryInterface(IID_IVdsSwProvider,
                sw_provider.put_void()))) {
            LYRA_TRACE("Found software provider.");
            callback(sw_provider.get());

        } else if (SUCCEEDED(provider->QueryInterface(IID_IVdsVdProvider,
                vd_provider.put_void()))) {
            LYRA_TRACE("Found virtual disk provider.");
            callback(vd_provider.get());

        } else {
            LYRA_TRACE("Found provider of unknown type.");
            callback(provider.get());
        }
    }
}
