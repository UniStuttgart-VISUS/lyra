// <copyright file="callable_traits.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include <gtest/gtest.h>

#include <type_traits>

#include "callable_traits.h"

struct clazz final {
    bool method(double, char) { return true; }
};

void c_function(int) { }

struct test_functor final {
    double operator()(float, long) { return 0.0; }
};


TEST(callable_traits, method) {
    typedef LYRA_DETAIL_NAMESPACE::callable_traits<decltype(&clazz::method)> traits;
    EXPECT_TRUE((std::is_same_v<traits::return_type, bool>));
    EXPECT_EQ(traits::arguments, 2);
    EXPECT_TRUE((std::is_same_v<traits::this_type, clazz>));
    EXPECT_TRUE((std::is_same_v<traits::argument<0>, double>));
    EXPECT_TRUE((std::is_same_v<traits::argument<1>, char>));
}

TEST(callable_traits, c_function) {
    typedef LYRA_DETAIL_NAMESPACE::callable_traits<decltype(c_function)> traits;
    EXPECT_TRUE((std::is_same_v<traits::return_type, void>));
    EXPECT_EQ(traits::arguments, 1);
    EXPECT_TRUE((std::is_same_v<traits::argument<0>, int>));
}

TEST(callable_traits, functor) {
    test_functor f;
    typedef LYRA_DETAIL_NAMESPACE::callable_traits<decltype(f)> traits;
    EXPECT_TRUE((std::is_same_v<traits::return_type, double>));
    EXPECT_EQ(traits::arguments, 2);
    EXPECT_TRUE((std::is_same_v<traits::argument<0>, float>));
    EXPECT_TRUE((std::is_same_v<traits::argument<1>, long>));
}

TEST(callable_traits, lambda) {
    auto lambda = [](double, char) -> bool { return true; };
    typedef LYRA_DETAIL_NAMESPACE::callable_traits<decltype(lambda)> traits;
    EXPECT_TRUE((std::is_same_v<traits::return_type, bool>));
    EXPECT_EQ(traits::arguments, 2);
    EXPECT_TRUE((std::is_same_v<traits::argument<0>, double>));
    EXPECT_TRUE((std::is_same_v<traits::argument<1>, char>));
}
