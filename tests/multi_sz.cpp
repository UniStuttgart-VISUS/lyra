// <copyright file="multi_sz.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include <gtest/gtest.h>

#include "visus/autodoc/multi_sz.h"


TEST(multi_sz, ctor) {
    LYRA_NAMESPACE::multi_sz msz;
    EXPECT_TRUE(msz.empty());
    EXPECT_EQ(msz.count(), std::size_t(0));
    EXPECT_EQ(msz.length(), std::size_t(0));
}

TEST(multi_sz, array_ctor) {
    const char *strings[] = { "Horst", "Hugo", "Heinz", "Hans" };
    LYRA_NAMESPACE::multi_sz msz(strings, 4);
    EXPECT_FALSE(msz.empty());
    EXPECT_EQ(msz.count(), std::size_t(4));
    EXPECT_EQ(msz.length(), std::size_t(23));
    EXPECT_STREQ(msz.at(0), "Horst");
    EXPECT_STREQ(msz.at(1), "Hugo");
    EXPECT_STREQ(msz.at(2), "Heinz");
    EXPECT_STREQ(msz.at(3), "Hans");
    EXPECT_EQ(msz.at(4), nullptr);
    EXPECT_STREQ(msz[0], "Horst");
    EXPECT_STREQ(msz[1], "Hugo");
    EXPECT_STREQ(msz[2], "Heinz");
    EXPECT_STREQ(msz[3], "Hans");
    EXPECT_EQ(msz[4], nullptr);
}

TEST(multi_sz, copy_ctor) {
    const char *strings[] = { "Horst", "Hugo" };
    LYRA_NAMESPACE::multi_sz msz(strings, 2);
    EXPECT_FALSE(msz.empty());
    EXPECT_EQ(msz.count(), std::size_t(2));

    LYRA_NAMESPACE::multi_sz clone(msz);
    EXPECT_EQ(msz.count(), clone.count());

    EXPECT_EQ(msz.length(), clone.length());
    EXPECT_STREQ(msz.at(0), clone.at(0));
    EXPECT_STREQ(msz.at(1), clone.at(1));

    EXPECT_TRUE(msz == clone);
    EXPECT_FALSE(msz != clone);
}

TEST(multi_sz, move_ctor) {
    const char *strings[] = { "Horst", "Hugo" };
    LYRA_NAMESPACE::multi_sz msz(strings, 2);
    EXPECT_FALSE(msz.empty());
    EXPECT_EQ(msz.count(), std::size_t(2));

    LYRA_NAMESPACE::multi_sz clone(std::move(msz));
    EXPECT_EQ(clone.count(), std::size_t(2));
    EXPECT_TRUE(msz.empty());
    EXPECT_STREQ(clone.at(0), "Horst");
    EXPECT_STREQ(clone.at(1), "Hugo");

    EXPECT_FALSE(msz == clone);
    EXPECT_TRUE(msz != clone);

    msz = std::move(clone);
    EXPECT_EQ(msz.count(), std::size_t(2));
    EXPECT_TRUE(clone.empty());
    EXPECT_STREQ(msz.at(0), "Horst");
    EXPECT_STREQ(msz.at(1), "Hugo");

    EXPECT_FALSE(msz == clone);
    EXPECT_TRUE(msz != clone);
}

TEST(multi_sz, clear) {
    const char *strings[] = { "Horst", "Hugo" };
    LYRA_NAMESPACE::multi_sz msz(strings, 2);
    EXPECT_FALSE(msz.empty());
    msz.clear();
    EXPECT_TRUE(msz.empty());
}

TEST(multi_sz, add) {
    LYRA_NAMESPACE::multi_sz msz;
    EXPECT_TRUE(msz.empty());

    msz.add("Horst");
    EXPECT_FALSE(msz.empty());
    EXPECT_EQ(msz.count(), std::size_t(1));
    EXPECT_STREQ(msz.at(0), "Horst");
    EXPECT_EQ(msz.at(1), nullptr);

    msz.add("Hugo");
    EXPECT_FALSE(msz.empty());
    EXPECT_EQ(msz.count(), std::size_t(2));
    EXPECT_STREQ(msz.at(0), "Horst");
    EXPECT_STREQ(msz.at(1), "Hugo");
    EXPECT_EQ(msz.at(2), nullptr);

    msz.add("Heinz");
    EXPECT_FALSE(msz.empty());
    EXPECT_EQ(msz.count(), std::size_t(3));
    EXPECT_STREQ(msz.at(0), "Horst");
    EXPECT_STREQ(msz.at(1), "Hugo");
    EXPECT_STREQ(msz.at(2), "Heinz");
    EXPECT_EQ(msz.at(3), nullptr);
}

//TEST(multi_sz, remove_if) {
//    {
//        LYRA_NAMESPACE::multi_sz msz;
//        EXPECT_TRUE(msz.empty());
//        msz.remove_if([](const char *s) { return *s == 'H'; });
//        EXPECT_TRUE(msz.empty());
//    }
//
//    {
//        const char *strings[] = { "Horst", "Hugo", "Heinz", "Hans" };
//        LYRA_NAMESPACE::multi_sz msz(strings, 4);
//        EXPECT_FALSE(msz.empty());
//        msz.remove_if([](const char *s) { return *s == 'H'; });
//        EXPECT_TRUE(msz.empty());
//    }
//
//    {
//        const char *strings[] = { "Horst", "Hugo", "Holger", "Hans" };
//        LYRA_NAMESPACE::multi_sz msz(strings, 4);
//        EXPECT_FALSE(msz.empty());
//        msz.remove_if([](const char *s) { return s[0] == 'H' && s[1] == 'o'; });
//        EXPECT_EQ(msz.count(), std::size_t(2));
//        EXPECT_STREQ(msz.at(0), "Hugo");
//        EXPECT_STREQ(msz.at(1), "Hans");
//    }
//}

//TEST(multi_sz, remove) {
//    const char *strings[] = { "Horst", "Hugo", "Heinz", "Hans" };
//    LYRA_NAMESPACE::multi_sz msz(strings, 4);
//    EXPECT_FALSE(msz.empty());
//
//    msz.remove("Hugo");
//    EXPECT_EQ(msz.count(), std::size_t(3));
//    EXPECT_STREQ(msz.at(0), "Horst");
//    EXPECT_STREQ(msz.at(1), "Heinz");
//    EXPECT_STREQ(msz.at(2), "Hans");
//
//    msz.remove("Horst");
//    EXPECT_EQ(msz.count(), std::size_t(2));
//    EXPECT_STREQ(msz.at(0), "Heinz" );
//    EXPECT_STREQ(msz.at(1), "Hans" );
//
//    msz.remove("Hans");
//    EXPECT_EQ(msz.count(), std::size_t(1));
//    EXPECT_STREQ(msz.at(0), "Heinz" );
//
//    msz.remove("Hans"); // [sic]
//    EXPECT_EQ(msz.count(), std::size_t(1));
//    EXPECT_STREQ(msz.at(0), "Heinz" );
//
//    msz.remove("Heinz");
//    EXPECT_EQ(msz.count(), std::size_t(0));
//    EXPECT_EQ(msz.at(0), nullptr);
//}

//TEST(multi_sz, insert) {
//    LYRA_NAMESPACE::multi_sz msz;
//    EXPECT_TRUE(msz.empty());
//
//    msz.insert(0, "Hugo");
//    EXPECT_EQ(std::size_t(1), msz.count());
//    EXPECT_STREQ("Hugo", msz.at(0));
//    msz.insert(0, "Horst");
//    EXPECT_EQ(std::size_t(2), msz.count());
//    EXPECT_STREQ("Horst", msz.at(0));
//    EXPECT_STREQ("Hugo", msz.at(1));
//    msz.insert(1, "Heinz");
//    EXPECT_EQ(std::size_t(3), msz.count());
//    EXPECT_STREQ("Horst", msz.at(0));
//    EXPECT_STREQ("Heinz", msz.at(1));
//    EXPECT_STREQ("Hugo", msz.at(2));
//
//    msz.insert(3, "Egon");
//    EXPECT_EQ(std::size_t(4), msz.count());
//    EXPECT_STREQ("Horst", msz.at(0));
//    EXPECT_STREQ("Heinz", msz.at(1));
//    EXPECT_STREQ("Hugo", msz.at(2));
//    EXPECT_STREQ("Egon", msz.at(3));
//
//    msz.insert(100, "Walter");
//    EXPECT_EQ(std::size_t(5), msz.count(), L"Element added.", LINE_INFO());
//    EXPECT_STREQ("Horst", msz.at(0));
//    EXPECT_STREQ("Heinz", msz.at(1));
//    EXPECT_STREQ("Hugo", msz.at(2));
//    EXPECT_STREQ("Egon", msz.at(3));
//    EXPECT_STREQ("Walter", msz.at(4));
//}

TEST(multi_sz, iterator) {
    {
        LYRA_NAMESPACE::multi_sz msz;
        EXPECT_TRUE(msz.empty()) << "multi-sz is intially empty.";

        auto it = msz.begin();
        EXPECT_TRUE(it == msz.end()) << "Immediately at end";
    }

    {
        const char *strings[] = { "Horst", "Hugo", "Heinz", "Hans" };
        LYRA_NAMESPACE::multi_sz msz(strings, 4);
        EXPECT_FALSE(msz.empty()) << "multi-sz is intially not empty.";

        auto it = msz.begin();
        EXPECT_FALSE(it == msz.end()) << "Not at end";
        EXPECT_STREQ("Horst", *it) << "At \"Horst\".";

        ++it;
        EXPECT_FALSE(it == msz.end()) << "Not at end";
        EXPECT_STREQ("Hugo", *it) << "At \"Hugo\".";
        ++it;
        EXPECT_FALSE(it == msz.end()) << "Not at end";
        EXPECT_STREQ("Heinz", *it) << "At \"Heinz\".";

        ++it;
        EXPECT_FALSE(it == msz.end()) << "Not at end";
        EXPECT_STREQ("Hans", *it) << "At \"Hans\".";

        ++it;
        EXPECT_TRUE(it == msz.end()) << "At end";
    }
}

TEST(multi_sz, copy) {
    {
        std::vector<std::string> strings;
        LYRA_DETAIL_NAMESPACE::multi_sz_copy(std::back_inserter(strings), (char *) nullptr);
        EXPECT_TRUE(strings.empty());
    }

    {
        LYRA_NAMESPACE::multi_sz msz;
        std::vector<std::string> strings;
        LYRA_DETAIL_NAMESPACE::multi_sz_copy(std::back_inserter(strings), msz.data());
        EXPECT_TRUE(strings.empty());
    }

    {
        LYRA_NAMESPACE::multi_sz msz;
        msz.add("Horst");
        std::vector<std::string> strings;
        LYRA_DETAIL_NAMESPACE::multi_sz_copy(std::back_inserter(strings), msz.data());
        EXPECT_EQ(std::size_t(1), strings.size());
        EXPECT_STREQ("Horst", strings.front().c_str());
    }

    {
        LYRA_NAMESPACE::multi_sz msz;
        msz.add("Horst");
        msz.add("Egon");
        std::vector<std::string> strings;
        LYRA_DETAIL_NAMESPACE::multi_sz_copy(std::back_inserter(strings), msz.data());
        EXPECT_EQ(std::size_t(2), strings.size());
        EXPECT_STREQ("Horst", strings.front().c_str());
        EXPECT_STREQ("Egon", strings.back().c_str());
    }
}
