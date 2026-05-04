// <copyright file="sysfs_device.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include <gtest/gtest.h>

#include "sysfs_device.h"


#if !defined(_WIN32)
TEST(sysfs_device, from_path) {
    auto devices = LYRA_DETAIL_NAMESPACE::sysfs_device::from_path();
    EXPECT_FALSE(devices.empty());
}
#endif /* !defined(_WIN32) */
