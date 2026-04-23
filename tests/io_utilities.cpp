// <copyright file="io_utilities.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include <gtest/gtest.h>

#include "invoke.h"


TEST(io_utilities, invoke) {
    auto result = LYRA_DETAIL_NAMESPACE::invoke("dir");
    EXPECT_TRUE(!result.empty());
}


TEST(io_utilities, winvoke) {
    auto result = LYRA_DETAIL_NAMESPACE::invoke(L"dir");
    EXPECT_TRUE(!result.empty());
}
