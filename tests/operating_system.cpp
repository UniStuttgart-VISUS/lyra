// <copyright file="operating_system.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include <gtest/gtest.h>

#include "visus/lyra/operating_system.h"


TEST(operating_system, version) {
    auto result = LYRA_NAMESPACE::operating_system::get_version();
    EXPECT_TRUE(!result.empty());
}
