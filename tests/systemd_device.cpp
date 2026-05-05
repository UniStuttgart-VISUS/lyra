// <copyright file="systemd_device.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "systemd_device.h"


#if defined(LYRA_USE_SYSTEMD)
TEST(systemd_device, enumerate) {
    using LYRA_DETAIL_NAMESPACE::systemd_device;
    using namespace testing;

    auto devices = systemd_device::enumerate();
    EXPECT_FALSE(devices.empty());
    EXPECT_THAT(devices, Each(Property(&systemd_device::name, Not(IsEmpty()))));
}
#endif /* defined(LYRA_USE_SYSTEMD) */
