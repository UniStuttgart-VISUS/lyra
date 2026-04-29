// <copyright file="environment.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include <gtest/gtest.h>

#include "visus/lyra/environment.h"


TEST(environment, cwd) {
    auto env = LYRA_NAMESPACE::environment::get();
    EXPECT_FALSE(env.empty());
    EXPECT_NE(env.get<LYRA_NAMESPACE::environment::current_directory>(), nullptr);
}

TEST(environment, variables) {
    {
        auto vars = LYRA_NAMESPACE::environment::get_variables();
        EXPECT_FALSE(vars.empty());
    }
    {
        auto vars = LYRA_NAMESPACE::environment::get_variables(LYRA_NAMESPACE::collection_flags::no_sensitive);
        EXPECT_TRUE(vars.empty());
    }
}
