// <copyright file="setup_api.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2016 - 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "setup_api.h"


#if defined(_WIN32)
/*
 * LYRA_DETAIL_NAMESPACE::add_device_install_flags
 */
void LYRA_DETAIL_NAMESPACE::add_device_install_flags(
        _In_ HDEVINFO handle,
        _In_opt_ SP_DEVINFO_DATA *info,
        _In_ const DWORD flags,
        _In_ const DWORD flags_ex) {
    SP_DEVINSTALL_PARAMS_W params { 0 };
    params.cbSize = sizeof(params);

    if (!::SetupDiGetDeviceInstallParamsW(handle, info, &params)) {
        const auto error = ::GetLastError();
        LYRA_TRACE(_T("SetupDiGetDeviceInstallParamsW failed with error ")
            _T("code %d."), error);
        throw std::system_error(error, std::system_category());
    }

    params.Flags |= flags;
    params.FlagsEx |= flags_ex;

    if (!::SetupDiSetDeviceInstallParamsW(handle, info, &params)) {
        const auto error = ::GetLastError();
        LYRA_TRACE(_T("SetupDiSetDeviceInstallParamsW failed with error ")
            _T("code %d."), error);
        throw std::system_error(error, std::system_category());
    }
}


/*
 * LYRA_DETAIL_NAMESPACE::get_device_interface_detail
 */
std::vector<std::uint8_t> LYRA_DETAIL_NAMESPACE::get_device_interface_detail(
        _In_ HDEVINFO handle,
        _In_ SP_DEVICE_INTERFACE_DATA& data,
        _In_opt_ SP_DEVINFO_DATA *detail) {
    // Measure the buffer.
    DWORD size = 0;
    ::SetupDiGetDeviceInterfaceDetailW(handle, &data, nullptr, 0, &size,
        nullptr);
    {
        const auto error = ::GetLastError();
        if ((error != NO_ERROR) && (error != ERROR_INSUFFICIENT_BUFFER)) {
            LYRA_TRACE(_T("SetupDiGetDeviceInterfaceDetailW failed with error ")
                _T("code %d."), error);
            throw std::system_error(error, std::system_category());
        }
    }

    std::vector<std::uint8_t> retval(size);
    auto buffer = reinterpret_cast<PSP_DEVICE_INTERFACE_DETAIL_DATA_W>(
        retval.data());
    buffer->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA_W);

    // Retrieve the data.
    if (!::SetupDiGetDeviceInterfaceDetailW(handle, &data, buffer,
            static_cast<DWORD>(retval.size()), &size, detail)) {
        const auto error = ::GetLastError();
        LYRA_TRACE(_T("SetupDiGetDeviceInterfaceDetailW failed with error ")
            _T("code %d."), error);
        throw std::system_error(error, std::system_category());
    }

    return retval;
}


/*
 * LYRA_DETAIL_NAMESPACE::get_device_registry_property
 */
std::vector<std::uint8_t> LYRA_DETAIL_NAMESPACE::get_device_registry_property(
        _In_ HANDLE handle,
        _In_ SP_DEVINFO_DATA& info,
        _In_ const DWORD property,
        _Out_opt_ DWORD *type) {
    DWORD size = 0;

    // Measure the buffer.
    ::SetupDiGetDeviceRegistryPropertyW(handle, &info, property, nullptr,
        nullptr, 0, &size);
    {
        const auto error = ::GetLastError();
        if ((error != NO_ERROR) && (error != ERROR_INSUFFICIENT_BUFFER)) {
            LYRA_TRACE(_T("SetupDiGetDeviceRegistryPropertyW failed with ")
                _T("error code %d."), error);
            throw std::system_error(error, std::system_category());
        }
    }

    std::vector<std::uint8_t> retval(size);

    // Retrieve the data.
    if (!::SetupDiGetDeviceRegistryPropertyW(handle, &info, property,
            type, reinterpret_cast<BYTE *>(retval.data()),
            static_cast<DWORD>(retval.size()), &size)) {
        const auto error = ::GetLastError();
        LYRA_TRACE(_T("SetupDiGetDeviceRegistryPropertyW failed with ")
            _T("error code %d."), error);
        throw std::system_error(error, std::system_category());
    }

    return retval;
}


/*
 * LYRA_DETAIL_NAMESPACE::split_driver_version
 */
std::array<std::uint16_t, 4> LYRA_DETAIL_NAMESPACE::split_driver_version(
        _In_ decltype(SP_DRVINFO_DATA_W::DriverVersion) version) noexcept {
    auto revision = static_cast<std::uint16_t>(version & 0xFFFF);
    version >>= 16;
    auto patch = static_cast<std::uint16_t>(version & 0xFFFF);
    version >>= 16;
    auto minor = static_cast<std::uint16_t>(version & 0xFFFF);
    version >>= 16;
    auto major = static_cast<std::uint16_t>(version & 0xFFFF);
    version >>= 16;
    return { major, minor, patch, revision };
}
#endif /* defined(_WIN32) */
