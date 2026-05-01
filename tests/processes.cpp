// <copyright file="proceses.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include <gtest/gtest.h>

#include "processes.h"


TEST(processes, command_line) {
    const auto cmdline = LYRA_DETAIL_NAMESPACE::get_command_line();
    EXPECT_FALSE(cmdline.empty());
}
TEST(processes, own_path) {
    const auto path = LYRA_DETAIL_NAMESPACE::get_executable_path();
    EXPECT_FALSE(path.empty());
}

TEST(processes, path_by_id) {
    const auto pid = LYRA_DETAIL_NAMESPACE::get_process_id();
    const auto path = LYRA_DETAIL_NAMESPACE::get_executable_path(pid);
    EXPECT_FALSE(path.empty());
    const auto expected = LYRA_DETAIL_NAMESPACE::get_executable_path();
    EXPECT_EQ(path, expected);
}
