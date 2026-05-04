// <copyright file="convert_string.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include <gtest/gtest.h>

#include <vector>

#include "visus/autodoc/convert_string.h"
#include "results.h"


TEST(convert_string, utf16_invalid_arg) {
    using namespace LYRA_DETAIL_NAMESPACE;
    const auto input = static_cast<LYRA_NAMESPACE::unicode_type *>(nullptr);
    std::size_t size = 0;
    EXPECT_EQ(LYRA_NAMESPACE::to_utf8(nullptr, size, input), results::invalid_argument);
}


TEST(convert_string, utf16_to_utf8) {
    using namespace LYRA_DETAIL_NAMESPACE;
    const std::vector<std::uint8_t> expected { 0xd0, 0xbf, 0xd1, 0x80, 0xd0, 0xb8, 0xd0, 0xb2, 0xd0, 0xb5, 0xd1, 0x82, 0 };
    const LYRA_NAMESPACE::unicode_type input[] = { 0x43f, 0x440, 0x438, 0x432, 0x435, 0x442, 0 };

    std::size_t size = 0;
    EXPECT_EQ(LYRA_NAMESPACE::to_utf8(nullptr, size, input), results::insufficient_buffer);
    EXPECT_EQ(size, expected.size());
    std::vector<char> buffer(size);
    EXPECT_EQ(LYRA_NAMESPACE::to_utf8(buffer.data(), size, input), results::success);
    EXPECT_STREQ(buffer.data(), reinterpret_cast<const char*>(expected.data()));
}


TEST(convert_string, utf32_invalid_arg) {
    using namespace LYRA_DETAIL_NAMESPACE;
    const auto input = static_cast<char32_t *>(nullptr);
    std::size_t size = 0;
    EXPECT_EQ(LYRA_NAMESPACE::to_utf8(nullptr, size, input), results::invalid_argument);
}


TEST(convert_string, utf32_to_utf8) {
    using namespace LYRA_DETAIL_NAMESPACE;
    const std::vector<std::uint8_t> expected { 0xd0, 0xbf, 0xd1, 0x80, 0xd0, 0xb8, 0xd0, 0xb2, 0xd0, 0xb5, 0xd1, 0x82, 0 };
    const char32_t input[] = { 0x0000043f, 0x00000440, 0x00000438, 0x00000432, 0x00000435, 0x00000442, 0 };

    std::size_t size = 0;
    EXPECT_EQ(LYRA_NAMESPACE::to_utf8(nullptr, size, input), results::insufficient_buffer);
    EXPECT_EQ(size, expected.size());
    std::vector<char> buffer(size);
    EXPECT_EQ(LYRA_NAMESPACE::to_utf8(buffer.data(), size, input), results::success);
    EXPECT_STREQ(buffer.data(), reinterpret_cast<const char*>(expected.data()));
}


TEST(convert_string, wchar_invalid_arg) {
    using namespace LYRA_DETAIL_NAMESPACE;
    const auto input = static_cast<wchar_t *>(nullptr);
    std::size_t size = 0;
    EXPECT_EQ(LYRA_NAMESPACE::to_utf8(nullptr, size, input), results::invalid_argument);
}


TEST(convert_string, wchar_to_utf8) {
    using namespace LYRA_DETAIL_NAMESPACE;
    const std::vector<std::uint8_t> expected { 0xd0, 0xbf, 0xd1, 0x80, 0xd0, 0xb8, 0xd0, 0xb2, 0xd0, 0xb5, 0xd1, 0x82, 0 };
    const wchar_t input[] = L"привет";

    std::size_t size = 0;
    EXPECT_EQ(LYRA_NAMESPACE::to_utf8(nullptr, size, input), results::insufficient_buffer);
    EXPECT_EQ(size, expected.size());
    std::vector<char> buffer(size);
    EXPECT_EQ(LYRA_NAMESPACE::to_utf8(buffer.data(), size, input), results::success);
    EXPECT_STREQ(buffer.data(), reinterpret_cast<const char*>(expected.data()));
}

