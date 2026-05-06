// <copyright file="win32_disks.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if defined(_WIN32)
#include "win32_disks.h"

#include <cassert>
#include <memory>
#include <system_error>

#include <wil/resource.h>
#include <wil/result.h>

#include "visus/autodoc/multi_sz.h"
#include "visus/autodoc/on_exit.h"

#include "results.h"
#include "setup_api.h"
#include "string_manipulation.h"


/*
 * LYRA_DETAIL_NAMESPACE::disk_info::disk_info
 */
LYRA_DETAIL_NAMESPACE::disk_info::disk_info(_In_ const std::wstring& path,
        _In_ const std::wstring& device)
    : _device(device),
        _path(path) {
    wil::unique_hfile handle(::CreateFileW(
        this->_path.c_str(),
        0,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        nullptr,
        OPEN_EXISTING,
        0,
        nullptr));
    THROW_LAST_ERROR_IF(!handle);

    get_disk_geometry(this->_geometry, handle.get());
    get_drive_layout(this->_layout, this->_partitions, handle.get());
    this->_number = get_storage_device_number(handle.get());
}


/*
 * LYRA_DETAIL_NAMESPACE::volume_info::volume_info
 */
LYRA_DETAIL_NAMESPACE::volume_info::volume_info(_In_ const std::wstring& path)
        : _file_system(MAX_PATH + 1, L'\0'),
        _name(MAX_PATH + 1, L'\0'),
        _path(path) {

    THROW_LAST_ERROR_IF(!::GetVolumeInformationW(
        this->_path.c_str(),
        std::addressof(this->_name[0]),
        static_cast<DWORD>(this->_name.size()),
        &this->_serial,
        nullptr,
        &this->_flags,
        std::addressof(this->_file_system[0]),
        static_cast<DWORD>(this->_file_system.size())));
    remove_trailing_nulls(this->_name);
    remove_trailing_nulls(this->_file_system);

    // Note: we need to try/catch this as some volumes (e.g. the UEFI partition)
    // cannot be opened by normal users and we do not want to fail everything in
    // this case.
    try {
        this->_extents = get_volume_extents(this->_path.c_str());
    } catch (const std::exception& ex) {
        LYRA_TRACE("Failed to get volume extents for \"%ls\": %s",
            this->_path.c_str(), ex.what());
    }

    try {
        this->_mounts = get_volume_paths(this->_path.c_str());
    } catch (const std::exception& ex) {
        LYRA_TRACE("Failed to get volume paths for \"%ls\": %s",
            this->_path.c_str(), ex.what());
    }
}


/*
 * LYRA_DETAIL_NAMESPACE::volume_info::on_disk
 */
bool LYRA_DETAIL_NAMESPACE::volume_info::on_disk(
        _In_ const disk_info& disk) const noexcept {
    for (auto& e : this->_extents) {
        if (e.DiskNumber == disk.number().DeviceNumber) {
            return true;
        }
    }

    return false;
}


/*
 * LYRA_DETAIL_NAMESPACE::get_disks
 */
std::vector<LYRA_DETAIL_NAMESPACE::disk_info> LYRA_DETAIL_NAMESPACE::get_disks(
        void) {
    std::vector<LYRA_DETAIL_NAMESPACE::disk_info> retval;

    enum_class_device_interfaces(GUID_DEVINTERFACE_DISK, [&retval](HDEVINFO h,
            SP_DEVINFO_DATA& d, SP_DEVICE_INTERFACE_DATA& i) {
        std::vector<std::uint8_t> detail_buf;
        auto detail = get_device_interface_detail(detail_buf, h, i);
        assert(detail != nullptr);

        auto device_buf = LYRA_DETAIL_NAMESPACE::get_device_registry_property(
            h, d, SPDRP_PHYSICAL_DEVICE_OBJECT_NAME);
        assert(!device_buf.empty());
        auto device = reinterpret_cast<const wchar_t*>(device_buf.data());

        retval.emplace_back(detail->DevicePath, device);
        return true;
    }, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);

    return retval;
}


/*
 * LYRA_DETAIL_NAMESPACE::get_disk_geometry
 */
void LYRA_DETAIL_NAMESPACE::get_disk_geometry(_Out_ DISK_GEOMETRY& geometry,
        _In_ HANDLE handle) {
    if (!::DeviceIoControl(handle, IOCTL_DISK_GET_DRIVE_GEOMETRY, nullptr, 0,
            &geometry, sizeof(DISK_GEOMETRY), nullptr, nullptr)) {
        throw std::system_error(::GetLastError(), std::system_category());
    }
}


#if (_WIN32_WINNT >= 0x0500)
/*
 * LYRA_DETAIL_NAMESPACE::get_disk_geometry
 */
void LYRA_DETAIL_NAMESPACE::get_disk_geometry(_Out_ DISK_GEOMETRY_EX& geometry,
        _In_ HANDLE handle) {
    if (!::DeviceIoControl(handle, IOCTL_DISK_GET_DRIVE_GEOMETRY_EX, nullptr, 0,
            &geometry, sizeof(DISK_GEOMETRY_EX), nullptr, nullptr)) {
        throw std::system_error(::GetLastError(), std::system_category());
    }
}
#endif /* (_WIN32_WINNT >= 0x0500) */


/*
 * LYRA_DETAIL_NAMESPACE::get_disk_paths
 */
std::vector<std::wstring> LYRA_DETAIL_NAMESPACE::get_disk_paths(void) {
    std::vector<std::wstring> retval;

    enum_class_device_interfaces(GUID_DEVINTERFACE_DISK, [&retval](HDEVINFO h,
            SP_DEVINFO_DATA& d, SP_DEVICE_INTERFACE_DATA& i) {
        std::vector<std::uint8_t> buffer;
        auto detail = get_device_interface_detail(buffer, h, i);
        assert(detail != nullptr);
        retval.push_back(detail->DevicePath);
        return true;
    }, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);

    return retval;
}


///*
// * LYRA_DETAIL_NAMESPACE::get_drive_layout
// */
//void LYRA_DETAIL_NAMESPACE::get_drive_layout(
//        _Out_ DRIVE_LAYOUT_INFORMATION& layout,
//        _Out_ std::vector< PARTITION_INFORMATION>& partitions,
//        _In_ HANDLE handle) {
//    const auto d = io_control(handle, IOCTL_DISK_GET_DRIVE_LAYOUT,
//        nullptr, 0, sizeof(DRIVE_LAYOUT_INFORMATION));
//    auto l = reinterpret_cast<const DRIVE_LAYOUT_INFORMATION *>(d.data());
//    layout = *l;
//    partitions.resize(layout.PartitionCount);
//    std::copy_n(l->PartitionEntry, partitions.size(), partitions.begin());
//}


#if (_WIN32_WINNT >= 0x0500)
/*
 * LYRA_DETAIL_NAMESPACE::get_drive_layout
 */
void LYRA_DETAIL_NAMESPACE::get_drive_layout(
        _Out_ DRIVE_LAYOUT_INFORMATION_EX& layout,
        _Out_ std::vector< PARTITION_INFORMATION_EX>& partitions,
        _In_ HANDLE handle) {
    const auto d = io_control(handle, IOCTL_DISK_GET_DRIVE_LAYOUT_EX,
        sizeof(DRIVE_LAYOUT_INFORMATION_EX));
    auto l = reinterpret_cast<const DRIVE_LAYOUT_INFORMATION_EX *>(d.data());
    layout = *l;
    partitions.resize(layout.PartitionCount);
    std::copy_n(l->PartitionEntry, partitions.size(), partitions.begin());
}
#endif /* (_WIN32_WINNT >= 0x0500) */


/*
 * LYRA_DETAIL_NAMESPACE::get_storage_device_number
 */
STORAGE_DEVICE_NUMBER LYRA_DETAIL_NAMESPACE::get_storage_device_number(
        _In_ HANDLE handle) {
    STORAGE_DEVICE_NUMBER retval;
    if (!::DeviceIoControl(handle, IOCTL_STORAGE_GET_DEVICE_NUMBER, nullptr,
            0, &retval, sizeof(STORAGE_DEVICE_NUMBER), nullptr, nullptr)) {
        throw std::system_error(::GetLastError(), std::system_category());
    }
    return retval;
}


/*
 * LYRA_DETAIL_NAMESPACE::get_volumes
 */
std::vector<LYRA_DETAIL_NAMESPACE::volume_info>
LYRA_DETAIL_NAMESPACE::get_volumes(void) {
    std::vector<volume_info> retval;

    for (auto v : get_volume_paths()) {
        retval.emplace_back(v);
    }

    return retval;
}


/*
 * LYRA_DETAIL_NAMESPACE::get_volume_extents
 */
std::vector<DISK_EXTENT> LYRA_DETAIL_NAMESPACE::get_volume_extents(
        _In_ const wchar_t *volume_path) {
    THROW_HR_IF(E_POINTER, volume_path == nullptr);
    std::wstring path(volume_path);
    trim_right(path, std::array<wchar_t, 2> { L'\\', L'/' });

    wil::unique_hfile file(::CreateFileW(path.c_str(),
        FILE_READ_ATTRIBUTES | SYNCHRONIZE | FILE_TRAVERSE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        nullptr,
        OPEN_EXISTING,
        0,
        nullptr));
    THROW_LAST_ERROR_IF(!file);

    auto buffer = io_control(file.get(), IOCTL_VOLUME_GET_VOLUME_DISK_EXTENTS);
    auto extents = reinterpret_cast<const VOLUME_DISK_EXTENTS *>(buffer.data());

    std::vector<DISK_EXTENT> retval;
    retval.reserve(extents->NumberOfDiskExtents);
    std::copy_n(extents->Extents,
        extents->NumberOfDiskExtents,
        std::back_inserter(retval));

    return retval;
}


/*
 * LYRA_DETAIL_NAMESPACE::get_volume_paths
 */
std::vector<std::wstring> LYRA_DETAIL_NAMESPACE::get_volume_paths(void) {
    std::vector<std::wstring> retval;
    std::vector<wchar_t> vol(MAX_PATH);

    auto hFind= ::FindFirstVolumeW(vol.data(), static_cast<DWORD>(vol.size()));
    THROW_LAST_ERROR_IF(hFind == INVALID_HANDLE_VALUE);
    LYRA_ON_EXIT([hFind](void) { ::FindVolumeClose(hFind); });

    do {
        retval.push_back(vol.data());
    } while (::FindNextVolumeW(hFind, vol.data(),
        static_cast<DWORD>(vol.size())));

    return retval;
}


/*
 * LYRA_DETAIL_NAMESPACE::get_volume_paths
 */
std::vector<std::wstring> LYRA_DETAIL_NAMESPACE::get_volume_paths(
        _In_z_ const wchar_t *volume_path) {
    THROW_HR_IF(E_POINTER, volume_path == nullptr);

    // Determine the size of the multi-sz.
    DWORD size = 0;
    ::GetVolumePathNamesForVolumeNameW(volume_path, nullptr, 0, &size);
    {
        const auto error = ::GetLastError();
        if (error != ERROR_MORE_DATA) {
            throw std::system_error(error, std::system_category());
        }
    }

    // Get th data.
    std::vector<wchar_t> buffer(size);
    if (!::GetVolumePathNamesForVolumeNameW(volume_path, buffer.data(),
            static_cast<DWORD>(buffer.size()), &size)) {
        throw std::system_error(::GetLastError(), std::system_category());
    }

    std::vector<std::wstring> retval;
    multi_sz_copy(std::back_inserter(retval), buffer.data());
    return retval;
}


/*
 * LYRA_DETAIL_NAMESPACE::io_control
 */
std::vector<std::uint8_t> LYRA_DETAIL_NAMESPACE::io_control(
        _In_ HANDLE handle,
        _In_ const DWORD control_code,
        _In_reads_bytes_opt_(cnt) const void *data,
        _In_ const std::size_t cnt,
        _In_ const std::size_t expected) {
    std::vector<std::uint8_t> buffer(expected);

    while (true) {
        DWORD returned = 0;

        if (!::DeviceIoControl(handle,
                control_code,
                const_cast<void *>(data),
                static_cast<DWORD>(cnt),
                buffer.data(),
                static_cast<DWORD>(buffer.size()),
                &returned,
                nullptr)) {
            const auto error = ::GetLastError();
            if (error == ERROR_INSUFFICIENT_BUFFER) {
                // Buffer too small, try again with larger buffer. Make sure
                // that it also works for an expected size of 0.
                buffer.resize(buffer.size() * 2 + 1);
            } else {
                throw std::system_error(error, std::system_category());
            }

        } else {
            // Got everything, so truncate to actual size and return.
            buffer.resize(returned);
            return buffer;
        }
    }
}

#endif /* defined(_WIN32) */
