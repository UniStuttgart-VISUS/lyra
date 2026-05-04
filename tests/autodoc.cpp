// <copyright file="autodoc.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include <gtest/gtest.h>

#include <filesystem>

#include "visus/autodoc/autodoc.h"
#include "visus/autodoc/timestamp.h"


TEST(autodoc, char) {
    const auto timestamp = LYRA_NAMESPACE::timestamp::now();
    const std::string base_path("autodoc_char");
    const auto enter_path = base_path + ".json";
    const auto exit_path = base_path + ".exit.json";

    std::filesystem::remove(enter_path);
    EXPECT_FALSE(std::filesystem::exists(enter_path));
    std::filesystem::remove(exit_path);
    EXPECT_FALSE(std::filesystem::exists(exit_path));

    volatile auto doc = new LYRA_NAMESPACE::autodoc(enter_path.c_str(),
        LYRA_NAMESPACE::collection_flags::no_sensitive,
        LYRA_NAMESPACE::collection_flags::no_sensitive | LYRA_NAMESPACE::collection_flags::no_immutable);
    delete doc;

    EXPECT_TRUE(std::filesystem::exists(enter_path));
    EXPECT_TRUE(std::filesystem::exists(exit_path));
}

TEST(autodoc, wchar_t) {
    const auto timestamp = LYRA_NAMESPACE::timestamp::now();
    const std::wstring base_path(L"autodoc_wchar_t");
    const auto enter_path = base_path + L".json";
    const auto exit_path = base_path + L".exit.json";

    volatile auto doc = new LYRA_NAMESPACE::autodoc(enter_path.c_str(),
        LYRA_NAMESPACE::collection_flags::no_sensitive,
        LYRA_NAMESPACE::collection_flags::no_sensitive | LYRA_NAMESPACE::collection_flags::no_immutable);
    delete doc;

    EXPECT_TRUE(std::filesystem::exists(enter_path));
    EXPECT_TRUE(std::filesystem::exists(exit_path));
}

TEST(autodoc, autodoc_write_raw_a) {
    const std::string path("autodoc_write_raw_a.json");

    std::filesystem::remove(path);
    EXPECT_FALSE(std::filesystem::exists(path));

    EXPECT_EQ(::autodoc_write_raw_a(path.c_str(), LYRA_NAMESPACE::collection_flags::no_sensitive), 0);
    EXPECT_TRUE(std::filesystem::exists(path));
}

TEST(autodoc, autodoc_write_raw_w) {
    const std::wstring path(L"autodoc_write_raw_w.json");

    std::filesystem::remove(path);
    EXPECT_FALSE(std::filesystem::exists(path));

    EXPECT_EQ(::autodoc_write_raw_w(path.c_str(), LYRA_NAMESPACE::collection_flags::no_sensitive), 0);
    EXPECT_TRUE(std::filesystem::exists(path));
}
