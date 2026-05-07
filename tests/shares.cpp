// <copyright file=" shares.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include <gtest/gtest.h>

#include "win32_shares.h"


#if defined(_WIN32)
TEST(shares, enumerate_exports) {
    EXPECT_NO_THROW(LYRA_DETAIL_NAMESPACE::enumerate_shares(nullptr, [](const SHARE_INFO_2& info) { }));
    EXPECT_LE(LYRA_DETAIL_NAMESPACE::enumerate_shares(nullptr, [](const SHARE_INFO_1& info) { return false; }), 1);
}
#endif /* defined(_WIN32) */


#if defined(_WIN32)
TEST(shares, enumerate_resources) {
    EXPECT_NO_THROW(LYRA_DETAIL_NAMESPACE::enumerate_resources(RESOURCE_GLOBALNET, RESOURCETYPE_ANY, 0, nullptr, [](const auto& r) {}));
    EXPECT_LE(LYRA_DETAIL_NAMESPACE::enumerate_resources(RESOURCE_GLOBALNET, RESOURCETYPE_ANY, 0, nullptr, [](const auto& r) { return false; }), 1);
    EXPECT_LE(LYRA_DETAIL_NAMESPACE::enumerate_resources(RESOURCE_CONNECTED, RESOURCETYPE_ANY, 0, nullptr, [](const auto& r) { return false; }), 1);
}
#endif /* defined(_WIN32) */

#if defined(_WIN32)
TEST(shares, enumerate_uses) {
    EXPECT_NO_THROW(LYRA_DETAIL_NAMESPACE::enumerate_use([](const USE_INFO_2& info) {}));
    EXPECT_LE(LYRA_DETAIL_NAMESPACE::enumerate_use([](const USE_INFO_1& info) { return false; }), 1);
}
#endif /* defined(_WIN32) */
