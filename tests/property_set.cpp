// <copyright file="property_set.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include <gtest/gtest.h>

#include "visus/lyra/property_set.h"


TEST(property_set, ctor) {
    LYRA_NAMESPACE::property_set properties;
    EXPECT_TRUE(properties.empty());
}
