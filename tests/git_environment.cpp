// <copyright file="git_environment.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include <gtest/gtest.h>

#include "git_environment.h"


TEST(git_environment, current) {
    EXPECT_NO_THROW(LYRA_DETAIL_NAMESPACE::git_environment::current());
    EXPECT_NO_THROW(LYRA_DETAIL_NAMESPACE::git_environment::current().head());
    EXPECT_NO_THROW(LYRA_DETAIL_NAMESPACE::git_environment::current().remotes());
}
