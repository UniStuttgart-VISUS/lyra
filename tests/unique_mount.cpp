// <copyright file="unique_mount.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include <gtest/gtest.h>

#include "unique_mount.h"


#if !defined(_WIN32)
TEST(unique_mount, sys2tmp) {
    LYRA_DETAIL_NAMESPACE::unique_mount mount("none", "sysfs");
    EXPECT_TRUE(mount || static_cast<const std::string&>(mount).empty());
}

TEST(unique_mount, sys2explicit) {
    std::string path = "/tmp/lyra-sys";
    LYRA_DETAIL_NAMESPACE::unique_mount mount("none", path.c_str(), "sysfs");
    EXPECT_TRUE(mount || static_cast<const std::string&>(mount).empty());
}
#endif /* !defined(_WIN32) */
