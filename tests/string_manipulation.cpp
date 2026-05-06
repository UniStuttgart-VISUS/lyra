// <copyright file="string_manipulation.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include <gtest/gtest.h>

#include <array>

#include "equals.h"
#include "string_manipulation.h"


TEST(string_manipulation, string_without_null) {
    std::string input("Horst");
    const auto actual = LYRA_DETAIL_NAMESPACE::remove_trailing_nulls(input);
    EXPECT_EQ(actual, std::string("Horst"));
}

TEST(string_manipulation, string_with_null) {
    std::string input("Horst\0\0");
    const auto actual = LYRA_DETAIL_NAMESPACE::remove_trailing_nulls(input);
    EXPECT_EQ(actual, std::string("Horst"));
}

TEST(string_manipulation, vector_without_null) {
    std::vector<char> input({'H', 'o', 'r', 's', 't'});
    const auto actual = LYRA_DETAIL_NAMESPACE::remove_trailing_nulls(input);
    EXPECT_EQ(actual, std::vector<char>({'H', 'o', 'r', 's', 't'}));
}

TEST(string_manipulation, vector_with_null) {
    std::vector<char> input({'H', 'o', 'r', 's', 't', '\0', '\0'});
    const auto actual = LYRA_DETAIL_NAMESPACE::remove_trailing_nulls(input);
    EXPECT_EQ(actual, std::vector<char>({'H', 'o', 'r', 's', 't'}));
}

TEST(string_manipulation, terminate_without_null) {
    std::vector<char> input({ 'H', 'o', 'r', 's', 't' });
    const auto actual = LYRA_DETAIL_NAMESPACE::null_terminate(input);
    EXPECT_EQ(actual, std::vector<char>({ 'H', 'o', 'r', 's', 't', 0 }));
}

TEST(string_manipulation, terminate_with_null) {
    std::vector<char> input({ 'H', 'o', 'r', 's', 't', '\0', '\0' });
    const auto actual = LYRA_DETAIL_NAMESPACE::null_terminate(input);
    EXPECT_EQ(actual, std::vector<char>({ 'H', 'o', 'r', 's', 't', '\0', '\0' }));
}

TEST(string_manipulation, trim_left_if) {
    std::string str("\"hugo\"");
    str = LYRA_DETAIL_NAMESPACE::trim_left_if(str, [](char c) { return c == '"'; });
    EXPECT_EQ(str, std::string("hugo\""));
    str = LYRA_DETAIL_NAMESPACE::trim_left_if(str, [](char c) { return c == '"'; });
    EXPECT_EQ(str, std::string("hugo\""));

    auto ptr = "\"hugo\"";
    ptr = LYRA_DETAIL_NAMESPACE::trim_left_if(ptr, [](char c) { return c == '"'; });
    EXPECT_STREQ(ptr, "hugo\"");
    ptr = LYRA_DETAIL_NAMESPACE::trim_left_if(ptr, [](char c) { return c == '"'; });
    EXPECT_STREQ(ptr, "hugo\"");
}

TEST(string_manipulation, trim_right_if) {
    std::string str("\"hugo\"");
    str = LYRA_DETAIL_NAMESPACE::trim_right_if(str, [](char c) { return c == '"'; });
    EXPECT_EQ(str, std::string("\"hugo"));
    str = LYRA_DETAIL_NAMESPACE::trim_right_if(str, [](char c) { return c == '"'; });
    EXPECT_EQ(str, std::string("\"hugo"));

    char ptr[] = "\"hugo\"";
    LYRA_DETAIL_NAMESPACE::trim_right_if(ptr, [](char c) { return c == '"'; });
    EXPECT_STREQ(ptr, "\"hugo");
    LYRA_DETAIL_NAMESPACE::trim_right_if(ptr, [](char c) { return c == '"'; });
    EXPECT_STREQ(ptr, "\"hugo");
}

TEST(string_manipulation, trim_if) {
    std::string str("\"hugo\"");
    str = LYRA_DETAIL_NAMESPACE::trim_if(str, [](char c) { return c == '"'; });
    EXPECT_EQ(str, std::string("hugo"));
    str = LYRA_DETAIL_NAMESPACE::trim_right_if(str, [](char c) { return c == '"'; });
    EXPECT_EQ(str, std::string("hugo"));

    char ptr[] = "\"hugo\"";
    auto p = LYRA_DETAIL_NAMESPACE::trim_if(ptr, [](char c) { return c == '"'; });
    EXPECT_STREQ(p, "hugo");
    p = LYRA_DETAIL_NAMESPACE::trim_if(p, [](char c) { return c == '"'; });
    EXPECT_STREQ(p, "hugo");
}

TEST(string_manipulation, trim_left) {
    std::string str("\"hugo\"");
    str = LYRA_DETAIL_NAMESPACE::trim_left(str, std::vector<char> { '"' });
    EXPECT_EQ(str, std::string("hugo\""));
    str = LYRA_DETAIL_NAMESPACE::trim_left(str, std::vector<char> { '"' });
    EXPECT_EQ(str, std::string("hugo\""));

    auto ptr = "\"hugo\"";
    ptr = LYRA_DETAIL_NAMESPACE::trim_left(ptr, std::array<char, 1> { '"' });
    EXPECT_STREQ(ptr, "hugo\"");
    ptr = LYRA_DETAIL_NAMESPACE::trim_left(ptr, std::array<char, 1> { '"' });
    EXPECT_STREQ(ptr, "hugo\"");
}

TEST(string_manipulation, trim_right) {
    std::string str("\"hugo\"");
    str = LYRA_DETAIL_NAMESPACE::trim_right(str, std::vector<char>{ '"' });
    EXPECT_EQ(str, std::string("\"hugo"));
    str = LYRA_DETAIL_NAMESPACE::trim_right(str, std::vector<char>{ '"' });
    EXPECT_EQ(str, std::string("\"hugo"));

    char ptr[] = "\"hugo\"";
    LYRA_DETAIL_NAMESPACE::trim_right(ptr, std::array<char, 1> { '"' });
    EXPECT_STREQ(ptr, "\"hugo");
    LYRA_DETAIL_NAMESPACE::trim_right(ptr, std::array<char, 1> { '"' });
    EXPECT_STREQ(ptr, "\"hugo");
}

TEST(string_manipulation, trim) {
    std::string str("\"hugo\"");
    str = LYRA_DETAIL_NAMESPACE::trim(str, std::vector<char>{ '"' });
    EXPECT_EQ(str, std::string("hugo"));
    str = LYRA_DETAIL_NAMESPACE::trim(str, std::vector<char>{ '"' });
    EXPECT_EQ(str, std::string("hugo"));

    char ptr[] = "\"hugo\"";
    auto p = LYRA_DETAIL_NAMESPACE::trim(ptr, std::array<char, 1> { '"' });
    EXPECT_STREQ(p, "hugo");
    p = LYRA_DETAIL_NAMESPACE::trim(p, std::array<char, 1> { '"' });
    EXPECT_STREQ(p, "hugo");
}
