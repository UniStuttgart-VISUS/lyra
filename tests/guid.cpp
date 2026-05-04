// <copyright file="guid.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2025 - 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include <gtest/gtest.h>

#include "visus/autodoc/guid.h"


TEST(guid, dft_ctor) {
    LYRA_NAMESPACE::guid g;
    EXPECT_TRUE(g.empty());
}

TEST(guid, explicit_ctor) {
    LYRA_NAMESPACE::guid g(0xCC01C664, 0x14DF, 0x4D30, 0xA9E9, 0xE8, 0x12, 0x9B, 0xB0, 0x92, 0x48);
    EXPECT_FALSE(g.empty());
}
