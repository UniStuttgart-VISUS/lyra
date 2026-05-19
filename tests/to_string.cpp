// <copyright file="to_string.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include <gtest/gtest.h>

#include "to_string.h"


TEST(to_string, stl) {
    EXPECT_EQ("42", LYRA_DETAIL_NAMESPACE::to_quoted_string(42));
    EXPECT_EQ("42", LYRA_DETAIL_NAMESPACE::to_unquoted_string(42));
    EXPECT_EQ("42", LYRA_DETAIL_NAMESPACE::to_quoted_string(42u));
    EXPECT_EQ("42", LYRA_DETAIL_NAMESPACE::to_unquoted_string(42u));
    EXPECT_EQ("42", LYRA_DETAIL_NAMESPACE::to_quoted_string(42ll));
    EXPECT_EQ("42", LYRA_DETAIL_NAMESPACE::to_unquoted_string(42ll));
    EXPECT_EQ("42", LYRA_DETAIL_NAMESPACE::to_quoted_string(42ull));
    EXPECT_EQ("42", LYRA_DETAIL_NAMESPACE::to_unquoted_string(42ull));
    {
        std::int32_t v = 42;
        EXPECT_EQ("42", LYRA_DETAIL_NAMESPACE::to_string(&v, false));
        EXPECT_EQ("42", LYRA_DETAIL_NAMESPACE::to_string(&v, true));
    }
    {
        std::uint32_t v = 42;
        EXPECT_EQ("42", LYRA_DETAIL_NAMESPACE::to_string(&v, false));
        EXPECT_EQ("42", LYRA_DETAIL_NAMESPACE::to_string(&v, true));
    }
    {
        std::int64_t v = 42;
        EXPECT_EQ("42", LYRA_DETAIL_NAMESPACE::to_string(&v, false));
        EXPECT_EQ("42", LYRA_DETAIL_NAMESPACE::to_string(&v, true));
    }
    {
        std::uint64_t v = 42;
        EXPECT_EQ("42", LYRA_DETAIL_NAMESPACE::to_string(&v, false));
        EXPECT_EQ("42", LYRA_DETAIL_NAMESPACE::to_string(&v, true));
    }
}

TEST(to_string, guid) {
    {
        LYRA_NAMESPACE::guid g;
        EXPECT_TRUE(g.empty());
        EXPECT_EQ("00000000-0000-0000-0000-000000000000", LYRA_DETAIL_NAMESPACE::to_unquoted_string(g));
        EXPECT_EQ("\"00000000-0000-0000-0000-000000000000\"", LYRA_DETAIL_NAMESPACE::to_quoted_string(g));
        EXPECT_EQ("00000000-0000-0000-0000-000000000000", LYRA_DETAIL_NAMESPACE::to_string(&g, false));
        EXPECT_EQ("\"00000000-0000-0000-0000-000000000000\"", LYRA_DETAIL_NAMESPACE::to_string(&g, true));
    }
    {
        auto g = LYRA_NAMESPACE::guid::parse("12344578-9ABC-DEF0-1234-0123456789AB");
        EXPECT_FALSE(g.empty());
        EXPECT_STRCASEEQ("12344578-9ABC-DEF0-1234-0123456789AB", LYRA_DETAIL_NAMESPACE::to_unquoted_string(g).c_str());
        EXPECT_STRCASEEQ("\"12344578-9ABC-DEF0-1234-0123456789AB\"", LYRA_DETAIL_NAMESPACE::to_quoted_string(g).c_str());
        EXPECT_STRCASEEQ("12344578-9ABC-DEF0-1234-0123456789AB", LYRA_DETAIL_NAMESPACE::to_string(&g, false).c_str());
        EXPECT_STRCASEEQ("\"12344578-9ABC-DEF0-1234-0123456789AB\"", LYRA_DETAIL_NAMESPACE::to_string(&g, true).c_str());
    }
}

TEST(to_string, bool) {
    EXPECT_EQ("0", LYRA_DETAIL_NAMESPACE::to_unquoted_string(false));
    EXPECT_EQ("0", LYRA_DETAIL_NAMESPACE::to_quoted_string(false));
    EXPECT_EQ("1", LYRA_DETAIL_NAMESPACE::to_unquoted_string(true));
    EXPECT_EQ("1", LYRA_DETAIL_NAMESPACE::to_quoted_string(true));
    {
        const bool v = false;
        EXPECT_EQ("0", LYRA_DETAIL_NAMESPACE::to_string(&v, false));
        EXPECT_EQ("0", LYRA_DETAIL_NAMESPACE::to_string(&v, true));
    }
    {
        const bool v = true;
        EXPECT_EQ("1", LYRA_DETAIL_NAMESPACE::to_string(&v, false));
        EXPECT_EQ("1", LYRA_DETAIL_NAMESPACE::to_string(&v, true));
    }
}

TEST(to_string, boolean) {
    EXPECT_EQ("0", LYRA_DETAIL_NAMESPACE::to_unquoted_string(LYRA_DETAIL_NAMESPACE::boolean(false)));
    EXPECT_EQ("0", LYRA_DETAIL_NAMESPACE::to_quoted_string(LYRA_DETAIL_NAMESPACE::boolean(false)));
    EXPECT_EQ("1", LYRA_DETAIL_NAMESPACE::to_unquoted_string(LYRA_DETAIL_NAMESPACE::boolean(true)));
    EXPECT_EQ("1", LYRA_DETAIL_NAMESPACE::to_quoted_string(LYRA_DETAIL_NAMESPACE::boolean(true)));
    {
        const boolean v = false;
        EXPECT_EQ("0", LYRA_DETAIL_NAMESPACE::to_string(&v, false));
        EXPECT_EQ("0", LYRA_DETAIL_NAMESPACE::to_string(&v, true));
    }
    {
        const boolean v = true;
        EXPECT_EQ("1", LYRA_DETAIL_NAMESPACE::to_string(&v, false));
        EXPECT_EQ("1", LYRA_DETAIL_NAMESPACE::to_string(&v, true));
    }
}

TEST(to_string, timestamp) {
    LYRA_NAMESPACE::timestamp v;
    EXPECT_EQ("0", LYRA_DETAIL_NAMESPACE::to_unquoted_string(v));
    EXPECT_EQ("0", LYRA_DETAIL_NAMESPACE::to_quoted_string(v));
    EXPECT_EQ("0", LYRA_DETAIL_NAMESPACE::to_string(&v, false));
    EXPECT_EQ("0", LYRA_DETAIL_NAMESPACE::to_string(&v, true));

}

TEST(to_string, string) {
    EXPECT_EQ("", LYRA_DETAIL_NAMESPACE::to_unquoted_string(nullptr));
    EXPECT_EQ("\"\"", LYRA_DETAIL_NAMESPACE::to_quoted_string(nullptr));
    EXPECT_EQ("bla", LYRA_DETAIL_NAMESPACE::to_unquoted_string("bla"));
    EXPECT_EQ("\"bla\"", LYRA_DETAIL_NAMESPACE::to_quoted_string("bla"));
}

TEST(to_string, multi_sz) {
    {
        const LYRA_NAMESPACE::multi_sz v;
        EXPECT_EQ("", LYRA_DETAIL_NAMESPACE::to_unquoted_string(v));
        EXPECT_EQ("\"\"", LYRA_DETAIL_NAMESPACE::to_quoted_string(v));
        EXPECT_EQ("", LYRA_DETAIL_NAMESPACE::to_string(&v, false));
        EXPECT_EQ("\"\"", LYRA_DETAIL_NAMESPACE::to_string(&v, true));
    }
    {
        const auto v = LYRA_NAMESPACE::multi_sz::for_string("bla");
        EXPECT_EQ("bla", LYRA_DETAIL_NAMESPACE::to_unquoted_string(v));
        EXPECT_EQ("\"bla\"", LYRA_DETAIL_NAMESPACE::to_quoted_string(v));
        EXPECT_EQ("bla", LYRA_DETAIL_NAMESPACE::to_string(&v, false));
        EXPECT_EQ("\"bla\"", LYRA_DETAIL_NAMESPACE::to_string(&v, true));
    }
    {
        std::vector<std::string> strings = { "bla", "bla" };
        const auto v = LYRA_NAMESPACE::multi_sz::for_range(strings.begin(), strings.end());
        EXPECT_EQ("bla,bla", LYRA_DETAIL_NAMESPACE::to_unquoted_string(v));
        EXPECT_EQ("\"bla,bla\"", LYRA_DETAIL_NAMESPACE::to_quoted_string(v));
        EXPECT_EQ("bla,bla", LYRA_DETAIL_NAMESPACE::to_string(&v, false));
        EXPECT_EQ("\"bla,bla\"", LYRA_DETAIL_NAMESPACE::to_string(&v, true));
    }
}
