// <copyright file="raw.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include <gtest/gtest.h>

#include "visus/autodoc/raw.h"


TEST(raw, get) {
    auto props = LYRA_NAMESPACE::raw::get();
    EXPECT_FALSE(props.empty());
    auto json = props.json();
    EXPECT_NE(json, nullptr);
}
