// <copyright file="fnv1a.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include <gtest/gtest.h>

#include "visus/autodoc/fnv1a.h"


TEST(fnv1a, char32) {
    typedef std::uint32_t value_type;
    // Reference values from https://github.com/lcn2/fnv?/test_fnv.c

    {
        const char *input = "";
        LYRA_NAMESPACE::fnv1a<value_type> hash;
        hash(input, input + ::strlen(input));
        EXPECT_EQ(value_type(0x811c9dc5UL), static_cast<value_type>(hash));
    }

    {
        const char *input = "a";
        LYRA_NAMESPACE::fnv1a<value_type> hash;
        hash(input, input + ::strlen(input));
        EXPECT_EQ(value_type(0xe40c292cUL), static_cast<value_type>(hash));
    }

    {
        const char *input = "b";
        LYRA_NAMESPACE::fnv1a<value_type> hash;
        hash(input, input + ::strlen(input));
        EXPECT_EQ(value_type(0xe70c2de5UL), static_cast<value_type>(hash));
    }

    {
        const char *input = "foobar";
        LYRA_NAMESPACE::fnv1a<value_type> hash;
        hash(input, input + ::strlen(input));
        EXPECT_EQ(value_type(0xbf9cf968UL), static_cast<value_type>(hash));
    }
}

TEST(fnv1a, char64) {
    // Reference values from https://github.com/lcn2/fnv?/test_fnv.c
    typedef std::uint64_t value_type;

    {
        const char *input = "";
        LYRA_NAMESPACE::fnv1a<value_type> hash;
        hash(input, input + ::strlen(input));
        EXPECT_EQ(value_type(0xcbf29ce484222325ULL), static_cast<value_type>(hash));
    }

    {
        const char *input = "a";
        LYRA_NAMESPACE::fnv1a<value_type> hash;
        hash(input, input + ::strlen(input));
        EXPECT_EQ(value_type(0xaf63dc4c8601ec8cULL), static_cast<value_type>(hash));
    }

    {
        const char *input = "b";
        LYRA_NAMESPACE::fnv1a<value_type> hash;
        hash(input, input + ::strlen(input));
        EXPECT_EQ(value_type(0xaf63df4c8601f1a5ULL), static_cast<value_type>(hash));
    }

    {
        const char *input = "foobar";
        LYRA_NAMESPACE::fnv1a<value_type> hash;
        hash(input, input + ::strlen(input));
        EXPECT_EQ(value_type(0x85944171f73967e8ULL), static_cast<value_type>(hash));
    }
}
