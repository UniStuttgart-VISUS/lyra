// <copyright file="graphics.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include <gtest/gtest.h>

#include "visus/lyra/graphics.h"


TEST(graphics, dxgi_adapters) {
    auto props = LYRA_NAMESPACE::graphics::get_dxgi_adapters();
    EXPECT_FALSE(props.empty());

    auto json = props.json();
    EXPECT_NE(json, nullptr);
}
