// <copyright file="property_type.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include <gtest/gtest.h>

#include "visus/lyra/property_set.h"
#include "visus/lyra/property_type.h"


TEST(property_type, property_type_t) {
    using namespace LYRA_NAMESPACE;
    using namespace LYRA_DETAIL_NAMESPACE;
    EXPECT_EQ(property_type::string, property_type_v<property_type_t<property_type::string>>);
    EXPECT_EQ(property_type::properties, property_type_v<property_type_t<property_type::properties>>);
    EXPECT_EQ(property_type::boolean, property_type_v<property_type_t<property_type::boolean>>);
    EXPECT_EQ(property_type::int32, property_type_v<property_type_t<property_type::int32>>);
    EXPECT_EQ(property_type::uint32, property_type_v<property_type_t<property_type::uint32>>);
    EXPECT_EQ(property_type::float32, property_type_v<property_type_t<property_type::float32>>);
}


TEST(property_type, property_type_v) {
    using namespace LYRA_NAMESPACE;
    using namespace LYRA_DETAIL_NAMESPACE;
    EXPECT_EQ(property_type::string, property_type_v<multi_sz>);
    EXPECT_EQ(property_type::properties, property_type_v<property_set>);
    EXPECT_EQ(property_type::boolean, property_type_v<bool>);
    EXPECT_EQ(property_type::int32, property_type_v<std::int32_t>);
    EXPECT_EQ(property_type::uint32, property_type_v<std::uint32_t>);
    EXPECT_EQ(property_type::float32, property_type_v<float>);
}
