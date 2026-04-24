// <copyright file="setup_api.inl" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2016 - 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>


/*
 * LYRA_DETAIL_NAMESPACE::enum_class_devices
 */
template<class TCallback>
std::size_t LYRA_DETAIL_NAMESPACE::enum_class_devices(
        _In_opt_ const GUID *class_guid,
        _In_ TCallback cb,
        _In_ const DWORD flags) {
    auto handle = ::SetupDiGetClassDevs(class_guid, nullptr, NULL, flags);
    if (handle == INVALID_HANDLE_VALUE) {
        const auto error = ::GetLastError();
        LYRA_TRACE(_T("SetupDiGetClassDevs failed with error code %d."), error);
        throw std::system_error(error, std::system_category());
    }

    // See http://support.microsoft.com/kb/259695/de
    SP_DEVINFO_DATA info { 0 };
    info.cbSize = sizeof(info);

    DWORD retval = 0;
    for (; ::SetupDiEnumDeviceInfo(handle, retval, &info); ++retval) {
        if (!cb(handle, info)) {
            LYRA_TRACE("Aborting device enumeration at index %d.", retval);
            break;
        }
    }

    {
        const auto error = ::GetLastError();
        ::SetupDiDestroyDeviceInfoList(handle);
        if ((error != NO_ERROR) && (error != ERROR_NO_MORE_ITEMS)) {
            LYRA_TRACE(_T("SetupDiEnumDeviceInfo failed with error code %d."),
                error);
            throw std::system_error(error, std::system_category());
        }
    }

    return retval;
}


/*
 * LYRA_DETAIL_NAMESPACE::enum_device_interfaces
 */
template<class TCallback>
std::size_t LYRA_DETAIL_NAMESPACE::enum_device_interfaces(
        _In_ HDEVINFO handle,
        _In_ SP_DEVINFO_DATA& info,
        _In_ const GUID& interface_guid,
        _In_ TCallback cb) {
    SP_DEVICE_INTERFACE_DATA data { 0 };
    data.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);

    DWORD i = 0;
    for (::SetupDiEnumDeviceInterfaces(handle, &info, &interface_guid,
            i, &data); ++i) {
        if (!cb(hDevInfo, devInfo, devIfData)) {
            break;
        }
    }

    {
        const auto error = ::GetLastError();
        if ((error != NO_ERROR) && (error != ERROR_NO_MORE_ITEMS)) {
            LYRA_TRACE(_T("SetupDiEnumDeviceInterfaces failed with error ")
                _T("code %d."), error);
            throw std::system_error(error, std::system_category());
        }
    }

    return i;
}


/*
 * LYRA_DETAIL_NAMESPACE::enum_driver_info
 */
template<class TCallback>
std::size_t LYRA_DETAIL_NAMESPACE::enum_driver_info(
        _In_ HDEVINFO handle,
        _In_opt_ SP_DEVINFO_DATA *info,
        _In_ const DWORD type,
        _In_ const TCallback cb) {
    if (!::SetupDiBuildDriverInfoList(handle, info, type)) {
        const auto error = ::GetLastError();
        LYRA_TRACE(_T("SetupDiBuildDriverInfoList failed with error code %d."),
            error);
        throw std::system_error(error, std::system_category());
    }

    SP_DRVINFO_DATA_W di { 0 };
    di.cbSize = sizeof(di);

    DWORD retval = 0;
    for (; ::SetupDiEnumDriverInfoW(handle, info, type, retval, &di);
            ++retval) {
        if (!cb(handle, info, di)) {
            break;
        }
    }

    {
        const auto error = ::GetLastError();
        if ((error != NO_ERROR) && (error != ERROR_NO_MORE_ITEMS)) {
            LYRA_TRACE(_T("SetupDiEnumDriverInfoW failed with error code %d."),
                error);
            throw std::system_error(error, std::system_category());
        }
    }

    return retval;
}
