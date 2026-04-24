// <copyright file="setup_api.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2016 - 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_SETUP_API_H)
#define _LYRA_SETUP_API_H
#pragma once

#if defined(_WIN32)

#include <array>
#include <cinttypes>
#include <functional>
#include <system_error>
#include <vector>

#include <tchar.h>
#include <Windows.h>
#include <SetupAPI.h>
#include <devguid.h>

#include "visus/lyra/trace.h"


LYRA_DETAIL_NAMESPACE_BEGIN

/// <summary>
/// Retrieves the device installation parameters for the given device
/// information set, adds the given flags to the set and updates it.
/// </summary>
/// <exception cref="std::system_error">If any of the API calls failed.
/// </exception>
LYRA_TEST_API void add_device_install_flags(_In_ HANDLE handle,
    _In_opt_ SP_DEVINFO_DATA *info, _In_ const DWORD flags,
    _In_ const DWORD flags_ex);

/// <summary>
/// Invoke <paramref name="cb" /> for all devices of the given class.
/// </summary>
/// <remarks>
/// <para>The function continues enumerating devices as long as there are
/// more devices and <paramref cref="cb" /> returns <c>true</c>.</para>
/// <para><paramref name="cb" /> should not throw exceptions. Failing to
/// fulfil this requirement might result in a memory leak.</para>
/// </remarks>
/// <typeparam name="TCallback"></typeparam>
/// <param name="class_guid">A pointer to the class GUID to enumerate or
/// <c>nullptr</c> to enumerate all classes in combination with the
/// <c>DIGCF_ALLCLASSES</c> flag.</param>
/// <param name="cb">The callback to be invoked for all results.</param>
/// <param name="flags">The enumeration flags as described on
/// https://msdn.microsoft.com/en-us/library/windows/hardware/ff551069(v=vs.85).aspx
/// The parameter defaults to <c>DIGCF_PRESENT</c>.</param>
/// <returns>The number of elements that have been enumerated.</returns>
/// <exception cref="std::system_error">If any of the API calls failed.
/// </exception>
template<class TCallback> std::size_t enum_class_devices(
    _In_opt_ const GUID *class_guid,
    _In_ TCallback cb,
    _In_ const DWORD flags = DIGCF_PRESENT);

/// <summary>
/// Invokes a callback for all device interfaces in a device information
/// set.
/// </summary>
/// <typeparam name="TCallback"></typeparam>
/// <param name="handle"></param>
/// <param name="info"></param>
/// <param name="interface_guid"></param>
/// <param name="cb"></param>
/// <returns></returns>
/// <exception cref="std::system_error">If any of the API calls failed.
/// </exception>
template<class TCallback> std::size_t enum_device_interfaces(
    _In_ HANDLE handle,
    _In_ SP_DEVINFO_DATA& info,
    _In_ const GUID& interface_guid,
    _In_ TCallback cb);

/// <summary>
/// Enumerates a list of drivers.
/// </summary>
/// <typeparam name="TCallback"></typeparam>
/// <param name="handle">A handle to the device information set that
/// contains the info list to enumerate.</param>
/// <param name="devInfo">An optional pointer to an 
/// <see cref="SP_DEVINFO_DATA" /> structure that specifies a device
/// information element in <paramref name="handle" />. If this parameter
/// is <c>nullptr</c>, the function enumerates the global class driver list
/// that is associated with <paramref name="handle" />. Otherwise, it
/// lists the drivers for the specified device.</param>
/// <param name="type"><c>SPDIT_CLASSDRIVER</c> to enumerate a
/// class driver list, or <c>SPDIT_COMPATDRIVER</c> to list the drivers
/// compatible with the specified device.</param>
/// <param name="cb"></param>
/// <returns>The number of elements that have been enumerated.</returns>
/// <exception cref="std::system_error">If any of the API calls failed.
/// </exception>
template<class TCallback> std::size_t enum_driver_info(
    _In_ HANDLE handle,
    _In_opt_ SP_DEVINFO_DATA *info,
    _In_ const DWORD type,
    _In_ const TCallback cb);

/// <summary>
/// Gets the <see cref="SP_DEVICE_INTERFACE_DETAIL_DATA_W" /> for the given
/// device interface data.
/// </summary>
/// <exception cref="std::system_error">If any of the API calls failed.
/// </exception>
std::vector<std::uint8_t> get_device_interface_detail(
    _In_ HANDLE handle,
    _In_ SP_DEVICE_INTERFACE_DATA& data,
    _In_opt_ SP_DEVINFO_DATA *detail = nullptr);

/// <summary>
/// Retrieves a registry-stored property of the device identified by
/// <paramref name="handle" /> and <paramref name="info" />.
/// </summary>
/// <param name="handle">A handle to a device information set that
/// contains a device information element that represents the device for
/// which to retrieve a property.</param>
/// <param name="info">A pointer to an <see cref="SP_DEVINFO_DATA" /> structure
/// from <paramref name="handle" /> which specifies the device information
/// element.</param>
/// <param name="property">The property to be retrieved. Valid values can be
/// found at
/// https://msdn.microsoft.com/en-us/library/windows/hardware/ff551967(v=vs.85).aspx
/// </param>
/// <param name="type">If not <see langword="nullptr" />, retrieves the registry
/// type of the data returned.</param>
/// <returns></returns>
/// <exception cref="std::system_error">If any of the API calls failed.
/// </exception>
LYRA_TEST_API std::vector<std::uint8_t> get_device_registry_property(
    _In_ HANDLE handle,
    _In_ SP_DEVINFO_DATA& info,
    _In_ const DWORD property,
    _Out_opt_ DWORD *type= nullptr);

/// <summary>
/// Splits the driver version from <see cref="SP_DRVINFO_DATA_W" /> into
/// (in this order) major version, minor version, patch and revision.
/// </summary>
LYRA_TEST_API std::array<std::uint16_t, 4> split_driver_version(
    _In_ decltype(SP_DRVINFO_DATA_W::DriverVersion) version) noexcept;

LYRA_DETAIL_NAMESPACE_END

#include "setup_api.inl"

#endif /* defined(_WIN32) */
#endif /* !defined(_LYRA_SETUP_API_H) */
