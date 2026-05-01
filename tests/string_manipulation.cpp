// <copyright file="string_manipulation.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include <gtest/gtest.h>

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
