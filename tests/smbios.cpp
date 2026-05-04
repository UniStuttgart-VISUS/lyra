// <copyright file="smbios.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include <gtest/gtest.h>

#include "visus/autodoc/smbios.h"
#include "visus/autodoc/version.h"


#if defined(_WIN32)
TEST(smbios, read) {
    // Note: this test would require root on Linux.
    LYRA_NAMESPACE::smbios::data data;
    EXPECT_TRUE(LYRA_SUCCEEDED(LYRA_NAMESPACE::smbios::read(data)));

    std::vector<const LYRA_NAMESPACE::smbios::header_type *> entries;
    EXPECT_GT(data.entries(std::back_inserter(entries)), 0U);

    std::vector<const LYRA_NAMESPACE::smbios::baseboard_information_type *> baseboard;
    EXPECT_EQ(data.entries_of_type<LYRA_NAMESPACE::smbios::baseboard_information_type>(std::back_inserter(baseboard)), 1);

    auto vendor = LYRA_NAMESPACE::smbios::get_string(baseboard[0], baseboard[0]->manufacturer);
    EXPECT_NE(vendor, nullptr);
}
#endif /* defined(_WIN32) */


#if defined(_WIN32)
TEST(smbios, get_baseboard_information) {
    // Note: this test would require root on Linux.
    LYRA_NAMESPACE::smbios::data data;
    EXPECT_TRUE(LYRA_SUCCEEDED(LYRA_NAMESPACE::smbios::read(data)));

    auto s = data.get<LYRA_NAMESPACE::smbios::baseboard_information_type>();
    EXPECT_NE(s, nullptr);

    auto ps = LYRA_NAMESPACE::smbios::get_baseboard_information(s, data.version());
    EXPECT_FALSE(ps.empty());
}
#endif /* defined(_WIN32) */


#if defined(_WIN32)
TEST(smbios, get_bios_information) {
    // Note: this test would require root on Linux.
    LYRA_NAMESPACE::smbios::data data;
    EXPECT_TRUE(LYRA_SUCCEEDED(LYRA_NAMESPACE::smbios::read(data)));

    auto s = data.get<LYRA_NAMESPACE::smbios::bios_information_type>();
    EXPECT_NE(s, nullptr);

    auto ps = LYRA_NAMESPACE::smbios::get_bios_information(s, data.version());
    EXPECT_FALSE(ps.empty());
}
#endif /* defined(_WIN32) */


#if defined(_WIN32)
TEST(smbios, get_processor_information) {
    // Note: this test would require root on Linux.
    LYRA_NAMESPACE::smbios::data data;
    EXPECT_TRUE(LYRA_SUCCEEDED(LYRA_NAMESPACE::smbios::read(data)));

    auto s = data.get<LYRA_NAMESPACE::smbios::processor_information_type>();
    EXPECT_NE(s, nullptr);

    auto ps = LYRA_NAMESPACE::smbios::get_processor_information(s, data.version());
    EXPECT_FALSE(ps.empty());
}
#endif /* defined(_WIN32) */

TEST(smbios, get) {
    auto properties = LYRA_NAMESPACE::smbios::get();
    EXPECT_FALSE(properties.empty());

    {
        auto json = properties.json();
        EXPECT_NE(json, nullptr);
    }

    {
        std::size_t cnt;
        LYRA_NAMESPACE::property_type type;
        const void *value;
        if (properties.get(value, cnt, type, u8"Version")) {
            EXPECT_EQ(cnt, 1);
            EXPECT_EQ(type, LYRA_NAMESPACE::property_type::properties);
            auto version = static_cast<const LYRA_NAMESPACE::property_set *>(value);
            EXPECT_FALSE(version->empty());
            EXPECT_NE(version->get<LYRA_NAMESPACE::version::major>(), nullptr);
            EXPECT_NE(version->get<LYRA_NAMESPACE::version::minor>(), nullptr);
        }
    }

    {
        std::size_t cnt = 0;
        auto property = properties.get<LYRA_NAMESPACE::smbios::baseboard>(cnt);
        EXPECT_NE(property, nullptr);
        EXPECT_GT(cnt, 0U);

    }

    {
        std::size_t cnt = 0;
        auto property = properties.get<LYRA_NAMESPACE::smbios::bios>(cnt);
        EXPECT_NE(property, nullptr);
        EXPECT_GT(cnt, 0U);
    }
}
