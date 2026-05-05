// <copyright file="hardware.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include <gtest/gtest.h>

#include "visus/autodoc/hardware.h"


TEST(hardware, get) {
    const auto properties = LYRA_NAMESPACE::hardware::get(LYRA_NAMESPACE::collection_flags::none);
    EXPECT_FALSE(properties.empty());
}
