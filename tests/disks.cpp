// <copyright file="disks.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <vector>

#include "com_scope.h"
#include "file.h"
#include "vds_api.h"
#include "wbem.h"
#include "win32_disks.h"


#if false && defined(_WIN32)
// This requires admin, so we do not test it.
TEST(disks, vds) {
    LYRA_DETAIL_NAMESPACE::com_scope com;
    auto service = LYRA_DETAIL_NAMESPACE::load_vds_service();
    EXPECT_NE(service, nullptr);

    LYRA_DETAIL_NAMESPACE::foreach_vds_service_provider(service.get(), [](auto provider) { });
}
#endif /* defined(_WIN32) */


#if defined(_WIN32)
TEST(disks, wql) {
    LYRA_DETAIL_NAMESPACE::com_scope com;
    LYRA_DETAIL_NAMESPACE::initialise_wbem_security();

    auto services = LYRA_DETAIL_NAMESPACE::make_wbem_services(L"ROOT\\CIMV2");
    EXPECT_NE(services, nullptr);

    auto disk_enum = LYRA_DETAIL_NAMESPACE::query_wql(services.get(), L"SELECT * FROM Win32_DiskDrive");
    EXPECT_NE(disk_enum, nullptr);
    const auto cnt_disks = LYRA_DETAIL_NAMESPACE::foreach_wbem(disk_enum.get(), [](IWbemClassObject *disk) {
        wil::unique_variant value;
        EXPECT_HRESULT_SUCCEEDED(disk->Get(L"Name", 0, value.addressof(), nullptr, nullptr));
        EXPECT_NE(value.pbstrVal, nullptr);
        return false;
    });
    EXPECT_EQ(cnt_disks, 1);

    auto mount_enum = LYRA_DETAIL_NAMESPACE::query_wql(services.get(), L"SELECT * FROM Win32_MountPoint");
    EXPECT_NE(mount_enum, nullptr);
    const auto cnt_mounts = LYRA_DETAIL_NAMESPACE::foreach_wbem(mount_enum.get(), [](IWbemClassObject *) { });
    EXPECT_GE(cnt_mounts, 1);

    auto vol_enum = LYRA_DETAIL_NAMESPACE::query_wql(services.get(), L"SELECT * FROM Win32_Volume");
    EXPECT_NE(vol_enum, nullptr);
    const auto cnt_vols = LYRA_DETAIL_NAMESPACE::foreach_wbem(vol_enum.get(), [](IWbemClassObject *volume) {
        LYRA_DETAIL_NAMESPACE::property_set_impl ps;
        LYRA_DETAIL_NAMESPACE::copy_wbem_properties(ps, volume);
    });
}
#endif /* defined(_WIN32) */

TEST(disks, paths) {
#if defined(_WIN32)
    const auto paths = LYRA_DETAIL_NAMESPACE::get_disk_paths();
    EXPECT_GE(paths.size(), 1);
    EXPECT_THAT(paths, testing::Each(testing::Not(testing::IsEmpty())));
#endif /* defined(_WIN32) */
}

#if defined(_WIN32)
TEST(disks, geometry) {
    const auto paths = LYRA_DETAIL_NAMESPACE::get_disk_paths();
    EXPECT_GE(paths.size(), 1);

    LYRA_DETAIL_NAMESPACE::unique_file handle(::CreateFileW(paths[0].c_str(),
        0,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        nullptr,
        OPEN_EXISTING,
        0,
        nullptr));
    EXPECT_TRUE(handle);

    {
        DISK_GEOMETRY geometry;
        EXPECT_NO_THROW(LYRA_DETAIL_NAMESPACE::get_disk_geometry(geometry, handle.get()));
        EXPECT_GT(geometry.BytesPerSector, 0);
    }

#if (_WIN32_WINNT >= 0x0500)
    {
        DISK_GEOMETRY_EX geometry;
        EXPECT_NO_THROW(LYRA_DETAIL_NAMESPACE::get_disk_geometry(geometry, handle.get()));
        EXPECT_GT(geometry.DiskSize.QuadPart, 0);
    }
#endif /* (_WIN32_WINNT >= 0x0500) */
}
#endif /* defined(_WIN32) */

#if defined(_WIN32)
TEST(disks, layout) {
    const auto paths = LYRA_DETAIL_NAMESPACE::get_disk_paths();
    EXPECT_GE(paths.size(), 1);

    LYRA_DETAIL_NAMESPACE::unique_file handle(::CreateFileW(paths[0].c_str(),
        0,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        nullptr,
        OPEN_EXISTING,
        0,
        nullptr));
    EXPECT_TRUE(handle);

    // Note: this API only works for MBR disks.
    //{
    //    DRIVE_LAYOUT_INFORMATION layout;
    //    std::vector<PARTITION_INFORMATION> partitions;
    //    EXPECT_NO_THROW(LYRA_DETAIL_NAMESPACE::get_drive_layout(layout, partitions, handle.get()));
    //    EXPECT_EQ(layout.PartitionCount, partitions.size());
    //}

#if (_WIN32_WINNT >= 0x0500)
    {
        DRIVE_LAYOUT_INFORMATION_EX layout;
        std::vector<PARTITION_INFORMATION_EX> partitions;
        EXPECT_NO_THROW(LYRA_DETAIL_NAMESPACE::get_drive_layout(layout, partitions, handle.get()));
        EXPECT_EQ(layout.PartitionCount, partitions.size());
    }
#endif /* (_WIN32_WINNT >= 0x0500) */
}
#endif /* defined(_WIN32) */

TEST(disks, volumes) {
#if defined(_WIN32)
    const auto paths = LYRA_DETAIL_NAMESPACE::get_volume_paths();
    EXPECT_GE(paths.size(), 1);
    EXPECT_THAT(paths, testing::Each(testing::Not(testing::IsEmpty())));
    const auto mounts = LYRA_DETAIL_NAMESPACE::get_volume_paths(paths[0].c_str());
    EXPECT_GE(mounts.size(), 1);
    const auto extents = LYRA_DETAIL_NAMESPACE::get_volume_extents(paths[0].c_str());
    const auto volumes = LYRA_DETAIL_NAMESPACE::get_volumes();
    EXPECT_EQ(paths.size(), volumes.size());
#endif /* defined(_WIN32) */
}
