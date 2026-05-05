// <copyright file="sysfs_device.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "sysfs_device.h"


#if !defined(_WIN32)
TEST(sysfs_device, from_path) {
    using namespace testing;
    using LYRA_DETAIL_NAMESPACE::sysfs_device;

    auto devices = sysfs_device::from_path();
    EXPECT_FALSE(devices.empty());
    EXPECT_THAT(devices, Each(Property(&sysfs_device::name, Not(IsEmpty()))));
}
#endif /* !defined(_WIN32) */
