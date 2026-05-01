// <copyright file="hash.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include <gtest/gtest.h>

#include "visus/lyra/hash.h"

#include "string_manipulation.h"


TEST(hash, sha256) {
    {
        LYRA_NAMESPACE::hash hash(LYRA_NAMESPACE::sha256);
        EXPECT_EQ(hash.length(), 256 / 8);
        EXPECT_TRUE(hash.append("", 0));
        const auto result = LYRA_DETAIL_NAMESPACE::to_hex_string(hash.finish(), hash.length());
        EXPECT_STREQ(result.c_str(), "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855");
    }

    {
        LYRA_NAMESPACE::hash hash(LYRA_NAMESPACE::sha256);
        EXPECT_EQ(hash.length(), 256 / 8);
        EXPECT_TRUE(hash.append("Hello, World!", 13));
        const auto result = LYRA_DETAIL_NAMESPACE::to_hex_string(hash.finish(), hash.length());
        EXPECT_STREQ(result.c_str(), "dffd6021bb2bd5b0af676290809ec3a53191dd81c7f70a4b28688a362182986f");
    }
}

TEST(hash, sha512) {
    {
        LYRA_NAMESPACE::hash hash(LYRA_NAMESPACE::sha512);
        EXPECT_EQ(hash.length(), 512 / 8);
        EXPECT_TRUE(hash.append("", 0));
        const auto result = LYRA_DETAIL_NAMESPACE::to_hex_string(hash.finish(), hash.length());
        EXPECT_STREQ(result.c_str(), "cf83e1357eefb8bdf1542850d66d8007d620e4050b5715dc83f4a921d36ce9ce47d0d13c5d85f2b0ff8318d2877eec2f63b931bd47417a81a538327af927da3e");
    }

    {
        LYRA_NAMESPACE::hash hash(LYRA_NAMESPACE::sha512);
        EXPECT_EQ(hash.length(), 512 / 8);
        EXPECT_TRUE(hash.append("Hello, World!", 13));
        const auto result = LYRA_DETAIL_NAMESPACE::to_hex_string(hash.finish(), hash.length());
        EXPECT_STREQ(result.c_str(), "374d794a95cdcfd8b35993185fef9ba368f160d8daf432d08ba9f1ed1e5abe6cc69291e0fa2fe0006a52570ef18c19def4e617c33ce52ef0a6e5fbe318cb0387");
    }
}
