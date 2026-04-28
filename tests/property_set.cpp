// <copyright file="property_set.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "property_set_impl.h"
#include "visus/lyra/property_set.h"
#include "visus/lyra/version.h"


TEST(property_set, default_ctor) {
    LYRA_NAMESPACE::property_set properties;

    EXPECT_TRUE(properties.empty());
}


TEST(property_set, no_init_ctor) {
    LYRA_NAMESPACE::property_set properties;
    LYRA_DETAIL_NAMESPACE::property_set_impl impl;
    LYRA_DETAIL_NAMESPACE::realise(properties, std::move(impl));

    EXPECT_TRUE(properties.empty());
}


TEST(property_set, init_ctor) {
    LYRA_NAMESPACE::property_set properties;
    LYRA_DETAIL_NAMESPACE::property_set_impl impl;
    impl.add("foo", std::string("bar"));
    LYRA_DETAIL_NAMESPACE::realise(properties, std::move(impl));

    EXPECT_FALSE(properties.empty());
}


TEST(property_set, empty_contains) {
    LYRA_NAMESPACE::property_set properties;

    EXPECT_FALSE(properties.contains("foo"));
    EXPECT_FALSE(properties.contains("answer"));
    EXPECT_FALSE(properties.contains(nullptr));
}


TEST(property_set, single_contains) {
    LYRA_NAMESPACE::property_set properties;
    LYRA_DETAIL_NAMESPACE::property_set_impl impl;
    impl.add("foo", "bar");
    LYRA_DETAIL_NAMESPACE::realise(properties, std::move(impl));

    EXPECT_TRUE(properties.contains("foo"));
    EXPECT_FALSE(properties.contains("answer"));
    EXPECT_FALSE(properties.contains(nullptr));
}


TEST(property_set, multiple_contains) {
    LYRA_NAMESPACE::property_set properties;
    LYRA_DETAIL_NAMESPACE::property_set_impl impl;
    impl.add("foo", "bar");
    impl.add("answer", 42);
    LYRA_DETAIL_NAMESPACE::realise(properties, std::move(impl));

    EXPECT_TRUE(properties.contains("foo"));
    EXPECT_TRUE(properties.contains("answer"));
    EXPECT_FALSE(properties.contains(nullptr));
}


TEST(property_set, empty_property_names) {
    LYRA_NAMESPACE::property_set properties;

    EXPECT_EQ(0, properties.properties());
    EXPECT_EQ(0, properties.properties(nullptr, 1));
    const char *names[1];
    EXPECT_EQ(0, properties.properties(names, std::size(names)));
}


TEST(property_set, single_property_name) {
    LYRA_NAMESPACE::property_set properties;
    LYRA_DETAIL_NAMESPACE::property_set_impl impl;
    impl.add("foo", "bar");
    LYRA_DETAIL_NAMESPACE::realise(properties, std::move(impl));

    EXPECT_EQ(1, properties.properties());
    EXPECT_EQ(1, properties.properties(nullptr, 1));
    const char *names[1];
    EXPECT_EQ(1, properties.properties(names, std::size(names)));
    EXPECT_STREQ("foo", names[0]);
}


TEST(property_set, multiple_property_names) {
    LYRA_NAMESPACE::property_set properties;
    LYRA_DETAIL_NAMESPACE::property_set_impl impl;
    impl.add("foo", "bar");
    impl.add("answer", 42);
    LYRA_DETAIL_NAMESPACE::realise(properties, std::move(impl));

    EXPECT_EQ(2, properties.properties());
    EXPECT_EQ(2, properties.properties(nullptr, 2));
    std::vector<const char *> names(2);
    EXPECT_EQ(2, properties.properties(names.data(), names.size()));
    EXPECT_THAT(names, ::testing::UnorderedElementsAre(::testing::StrEq("foo"), ::testing::StrEq("answer")));
}


TEST(property_set, empty_get) {
    LYRA_NAMESPACE::property_set properties;

    LYRA_NAMESPACE::property_set::value_type value;
    std::size_t size;
    LYRA_NAMESPACE::property_type type;

    EXPECT_FALSE(properties.get(value, size, type, "foo"));
}


TEST(property_set, single_get) {
    LYRA_NAMESPACE::property_set properties;
    LYRA_DETAIL_NAMESPACE::property_set_impl impl;
    impl.add("foo", "bar");
    LYRA_DETAIL_NAMESPACE::realise(properties, std::move(impl));

    LYRA_NAMESPACE::property_set::value_type value;
    std::size_t size;
    LYRA_NAMESPACE::property_type type;

    EXPECT_TRUE(properties.get(value, size, type, "foo"));
    EXPECT_EQ(type, LYRA_NAMESPACE::property_type::string);
    EXPECT_STREQ("bar", static_cast<const LYRA_NAMESPACE::multi_sz *>(value)->data());

    EXPECT_FALSE(properties.get(value, size, type, "answer"));

    EXPECT_FALSE(properties.get(value, size, type, nullptr));
}


TEST(property_set, multiple_get) {
    LYRA_NAMESPACE::property_set properties;
    LYRA_DETAIL_NAMESPACE::property_set_impl impl;
    impl.add("foo", "bar");
    impl.add("answer", 42);
    LYRA_DETAIL_NAMESPACE::realise(properties, std::move(impl));

    LYRA_NAMESPACE::property_set::value_type value;
    std::size_t size;
    LYRA_NAMESPACE::property_type type;

    EXPECT_TRUE(properties.get(value, size, type, "foo"));
    EXPECT_EQ(type, LYRA_NAMESPACE::property_type::string);
    EXPECT_STREQ("bar", static_cast<const LYRA_NAMESPACE::multi_sz *>(value)->data());

    EXPECT_TRUE(properties.get(value, size, type, "answer"));
    EXPECT_EQ(type, LYRA_NAMESPACE::property_type::int32);
    EXPECT_EQ(42, *static_cast<const std::int32_t *>(value));

    EXPECT_FALSE(properties.get(value, size, type, nullptr));
}


TEST(property_set, version_get) {
    LYRA_NAMESPACE::property_set properties;
    LYRA_DETAIL_NAMESPACE::property_set_impl impl;
    impl.add<LYRA_NAMESPACE::version::major>(42);
    impl.add<LYRA_NAMESPACE::version::minor>(43);
    impl.add<LYRA_NAMESPACE::version::patch>(44);
    LYRA_DETAIL_NAMESPACE::realise(properties, std::move(impl));

    EXPECT_NE(properties.get<LYRA_NAMESPACE::version::major>(), nullptr);
    EXPECT_EQ(*properties.get<LYRA_NAMESPACE::version::major>(), 42);
    EXPECT_NE(properties.get<LYRA_NAMESPACE::version::minor>(), nullptr);
    EXPECT_EQ(*properties.get<LYRA_NAMESPACE::version::minor>(), 43);
    EXPECT_NE(properties.get<LYRA_NAMESPACE::version::patch>(), nullptr);
    EXPECT_EQ(*properties.get<LYRA_NAMESPACE::version::patch>(), 44);
    EXPECT_EQ(properties.get<LYRA_NAMESPACE::version::prerelease>(), nullptr);
}
