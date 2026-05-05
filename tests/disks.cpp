// <copyright file="disks.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include <gtest/gtest.h>

#include <vector>

#include "com_scope.h"
#include "vds_api.h"
#include "wbem.h"


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
}
#endif /* defined(_WIN32) */
