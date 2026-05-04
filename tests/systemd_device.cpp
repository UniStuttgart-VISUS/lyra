// <copyright file="systemd_device.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include <gtest/gtest.h>

#include "systemd_device.h"


#if !defined(_WIN32)
TEST(systemd_device, enumerate) {
    auto devices = LYRA_DETAIL_NAMESPACE::systemd_device::enumerate();
    EXPECT_FALSE(devices.empty());
}
#endif /* !defined(_WIN32) */
