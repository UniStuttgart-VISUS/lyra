// <copyright file="smbios.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include <gtest/gtest.h>

#include "visus/lyra/smbios.h"


TEST(smbios, read) {
    LYRA_NAMESPACE::smbios::data data;
    EXPECT_NO_THROW(data = LYRA_NAMESPACE::smbios::read());

    std::vector<const LYRA_NAMESPACE::smbios::header_type *> entries;
    EXPECT_GT(data.entries(std::back_inserter(entries)), 0U);

    std::vector<const LYRA_NAMESPACE::smbios::baseboard_information_type *> baseboard;
    EXPECT_EQ(data.entries_of_type<LYRA_NAMESPACE::smbios::baseboard_information_type>(std::back_inserter(baseboard)), 1);

    auto baseboard_manufacturer = LYRA_NAMESPACE::smbios::get_string(baseboard[0], baseboard[0]->manufacturer);
    EXPECT_NE(baseboard_manufacturer, nullptr);
}


TEST(smbios, get_baseboard_information) {
    LYRA_NAMESPACE::smbios::data data;
    EXPECT_NO_THROW(data = LYRA_NAMESPACE::smbios::read());

    auto s = data.get<LYRA_NAMESPACE::smbios::baseboard_information_type>();
    EXPECT_NE(s, nullptr);

    auto ps = LYRA_NAMESPACE::smbios::get_baseboard_information(s, data.version());
    EXPECT_FALSE(ps.empty());
}


TEST(smbios, get_bios_information) {
    LYRA_NAMESPACE::smbios::data data;
    EXPECT_NO_THROW(data = LYRA_NAMESPACE::smbios::read());

    auto s = data.get<LYRA_NAMESPACE::smbios::bios_information_type>();
    EXPECT_NE(s, nullptr);

    auto ps = LYRA_NAMESPACE::smbios::get_bios_information(s, data.version());
    EXPECT_FALSE(ps.empty());
}


TEST(smbios, get_processor_information) {
    LYRA_NAMESPACE::smbios::data data;
    EXPECT_NO_THROW(data = LYRA_NAMESPACE::smbios::read());

    auto s = data.get<LYRA_NAMESPACE::smbios::processor_information_type>();
    EXPECT_NE(s, nullptr);

    auto ps = LYRA_NAMESPACE::smbios::get_processor_information(s, data.version());
    EXPECT_FALSE(ps.empty());
}


TEST(smbios, get_smbios) {
    auto properties = LYRA_NAMESPACE::smbios::get_smbios();
    EXPECT_FALSE(properties.empty());

    {
        std::size_t cnt;
        LYRA_NAMESPACE::property_type type;
        const void *value;
        EXPECT_TRUE(properties.get(value, cnt, type, u8"version"));
        EXPECT_EQ(cnt, 1);
        EXPECT_NE(value, nullptr);
        EXPECT_EQ(type, LYRA_NAMESPACE::property_type::properties);
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
