// <copyright file="win32_disks.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LYRA_WIN32_DISKS_H)
#define _LYRA_WIN32_DISKS_H
#pragma once

#if defined(_WIN32)
#include <string>
#include <vector>

#include <sdkddkver.h>
#include <Windows.h>
#include <winioctl.h>

#include "visus/autodoc/result_type.h"


LYRA_DETAIL_NAMESPACE_BEGIN

/// <summary>
/// Issues an <c>IOCTL_DISK_GET_DRIVE_GEOMETRY</c> control code to the given
/// handle and retrieves the drive geometry information.
/// </summary>
/// <param name="handle"></param>
/// <param name="geometry"></param>
/// <exception cref="std::system_error">If the operation fails.</exception>
LYRA_TEST_API void get_disk_geometry(_Out_ DISK_GEOMETRY& geometry,
    _In_ HANDLE handle);

#if (_WIN32_WINNT >= 0x0500)
/// <summary>
/// Issues an <c>IOCTL_DISK_GET_DRIVE_GEOMETRY_EX</c> control code to the given
/// handle and retrieves the drive geometry information.
/// </summary>
/// <param name="handle"></param>
/// <param name="geometry"></param>
/// <exception cref="std::system_error">If the operation fails.</exception>
LYRA_TEST_API void get_disk_geometry(_Out_ DISK_GEOMETRY_EX& geometry,
    _In_ HANDLE handle);
#endif /* (_WIN32_WINNT >= 0x0500) */

/// <summary>
/// Gets the paths to all disk devices on the system.
/// </summary>
/// <returns>A vector of device paths, which can be opened using
/// <see cref="CreateFileW" /> for further inspection.</returns>
LYRA_TEST_API std::vector<std::wstring> get_disk_paths(void);

///// <summary>
///// Issues an <c>IOCTL_DISK_GET_DRIVE_LAYOUT</c> control code to the given
///// handle and retrieves the drive layout information.
///// </summary>
///// <param name="layout"></param>
///// <param name="partitions"></param>
///// <param name="handle"></param>
///// <exception cref="std::system_error">If the operation fails.</exception>
//LYRA_TEST_API void get_drive_layout(_Out_ DRIVE_LAYOUT_INFORMATION& layout,
//    _Out_ std::vector< PARTITION_INFORMATION>& partitions,
//    _In_ HANDLE handle);

#if (_WIN32_WINNT >= 0x0500)
/// <summary>
/// Issues an <c>IOCTL_DISK_GET_DRIVE_LAYOUT_EX</c> control code to the given
/// handle and retrieves the drive layout information.
/// </summary>
/// <param name="layout"></param>
/// <param name="partitions"></param>
/// <param name="handle"></param>
/// <exception cref="std::system_error">If the operation fails.</exception>
LYRA_TEST_API void get_drive_layout(_Out_ DRIVE_LAYOUT_INFORMATION_EX& layout,
    _Out_ std::vector< PARTITION_INFORMATION_EX>& partitions,
    _In_ HANDLE handle);
#endif /* (_WIN32_WINNT >= 0x0500) */

/// <summary>
/// Gets the disk extents (typically one) for the given volume.
/// </summary>
/// <param name="volume_path"></param>
/// <returns></returns>
LYRA_TEST_API std::vector<DISK_EXTENT> get_volume_extents(
    _In_ const wchar_t *volume_path);

/// <summary>
/// Gets the paths for all volumes on the system.
/// </summary>
/// <returns></returns>
LYRA_TEST_API std::vector<std::wstring> get_volume_paths(void);

/// <summary>
/// Gets all paths (volume names etc.) assigned to the given volume, which must
/// be identified by its GUID path.
/// </summary>
/// <param name="volume_path"></param>
/// <returns></returns>
LYRA_TEST_API std::vector<std::wstring> get_volume_paths(
    _In_z_ const wchar_t *volume_path);

/// <summary>
/// Issues an IOCL that receives data, possibly retrying if the output buffer
/// was too small.
/// </summary>
/// <param name="handle"></param>
/// <param name="control_code"></param>
/// <param name="data"></param>
/// <param name="cnt"></param>
/// <param name="expected"></param>
/// <returns></returns>
LYRA_TEST_API std::vector<std::uint8_t> io_control(_In_ HANDLE handle,
    _In_ const DWORD control_code,
    _In_reads_bytes_opt_(cnt) const void *data,
    _In_ const std::size_t cnt,
    _In_ const std::size_t expected = 512);

/// <summary>
/// Issues an IOCL that receives data, possibly retrying if the output buffer
/// was too small.
/// </summary>
/// <param name="handle"></param>
/// <param name="control_code"></param>
/// <param name="expected"></param>
/// <returns></returns>
inline std::vector<std::uint8_t> io_control(_In_ HANDLE handle,
        _In_ const DWORD control_code,
        _In_ const std::size_t expected = 512) {
    return io_control(handle, control_code, nullptr, 0, expected);
}

LYRA_DETAIL_NAMESPACE_END

#endif /* defined(_WIN32) */
#endif /* !defined(_LYRA_WIN32_DISKS_H) */
