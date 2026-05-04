// <copyright file="timestamp.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include <gtest/gtest.h>

#include "visus/autodoc/timestamp.h"


static constexpr auto epoch_filetime = 116444736000000000LL;

#if defined(_WIN32)
static inline LYRA_NAMESPACE::timestamp::value_type make_filetime(
        const std::uint16_t year,
        const std::uint16_t month,
        const std::uint16_t day,
        const std::uint16_t hours,
        const std::uint16_t minutes,
        const std::uint16_t seconds,
        const std::uint16_t millis) {
    SYSTEMTIME utctime { 0 };
    utctime.wYear = year;
    utctime.wMonth = month;
    utctime.wDay = day;
    utctime.wHour = hours;
    utctime.wMinute = minutes;
    utctime.wSecond = seconds;
    utctime.wMilliseconds = millis;

    FILETIME filetime;
    EXPECT_TRUE(::SystemTimeToFileTime(&utctime, &filetime));

    LARGE_INTEGER retval;
    retval.HighPart = filetime.dwHighDateTime;
    retval.LowPart = filetime.dwLowDateTime;

    return retval.QuadPart;
}
#endif /* defined(_WIN32) */


static inline std::uint64_t time_t_to_filetime(const std::time_t t) {
    // From https://github.com/MicrosoftDocs/win32/blob/docs/desktop-src/SysInfo/converting-a-time-t-value-to-a-file-time.md
    return (t * 10000000LL) + 116444736000000000LL;
}


TEST(timestamp, from_system_clock) {
    typedef std::chrono::duration<LYRA_NAMESPACE::timestamp::value_type, std::ratio<1, 10000000>> dur;
    const auto max_dt = std::chrono::duration_cast<dur>(std::chrono::milliseconds(100)).count();

    const auto f = LYRA_NAMESPACE::timestamp::now();
    const auto s = LYRA_NAMESPACE::timestamp::from_system_clock();
    EXPECT_TRUE(s.value() - f.value() < max_dt);
}


TEST(timestamp, now) {
    typedef std::chrono::duration<LYRA_NAMESPACE::timestamp::value_type, std::ratio<1, 10000000>> dur;
    const auto max_dt = std::chrono::duration_cast<dur>(std::chrono::milliseconds(100)).count();

    const auto n = std::chrono::system_clock::now();
    const auto t = LYRA_NAMESPACE::timestamp::now();
    const auto s = std::chrono::duration_cast<dur>(n - std::chrono::system_clock::from_time_t(0)).count();
    EXPECT_TRUE(s - epoch_filetime - t <= max_dt);
}


TEST(timestamp, from_time_t) {
    const auto n = ::time(nullptr);
    const auto a = LYRA_NAMESPACE::timestamp::from_time_t(n);
    const auto r = time_t_to_filetime(n);
    EXPECT_EQ(a, r);
}


#if defined(_WIN32)
TEST(timestamp, filetime_roundtrip) {
    FILETIME e;
    e.dwLowDateTime = 42;
    e.dwHighDateTime = 17;

    const auto t = LYRA_NAMESPACE::timestamp::from_file_time(e);
    const auto a = static_cast<FILETIME>(t);

    EXPECT_EQ(e.dwLowDateTime, a.dwLowDateTime);
    EXPECT_EQ(e.dwHighDateTime, a.dwHighDateTime);
}
#endif /* defined(_WIN32) */


TEST(timestamp, system_clock_roundtrip) {
    const auto e = std::chrono::system_clock::now();
    const auto t = LYRA_NAMESPACE::timestamp(e);
    const auto a = static_cast<std::chrono::system_clock::time_point>(t);
    EXPECT_NEAR(e.time_since_epoch().count(), a.time_since_epoch().count(), 500);
}


TEST(timestamp, time_point_roundtip) {
    const auto e = std::chrono::system_clock::now();
    const auto t = LYRA_NAMESPACE::timestamp::from_time_point(e);
    const auto a = static_cast<std::chrono::system_clock::time_point>(t);
    EXPECT_NEAR(e.time_since_epoch().count(), a.time_since_epoch().count(), 500);
}

#if defined(_WIN32)
TEST(timestamp, create) {
    {
        const auto timestamp = LYRA_NAMESPACE::timestamp::create(2000, 1, 2, 3, 4, 5, 6);
        const auto expected = make_filetime(2000, 1, 2, 3, 4, 5, 6);
        EXPECT_EQ(expected, timestamp.value());
    }

    {
        const auto timestamp = LYRA_NAMESPACE::timestamp::create(2000, 1, 2, 3, 4, 5, 6, 7, 800);
        const auto filetime = make_filetime(2000, 1, 2, 3, 4, 5, 6);
        const auto expected = filetime + 70 + 8;
        EXPECT_EQ(expected, timestamp.value());
    }

    {
        const auto timestamp = LYRA_NAMESPACE::timestamp::create(2000, 1, 2, 3, 4, 5, 6, 7);
        const auto filetime = make_filetime(2000, 1, 2, 3, 4, 5, 6);
        const auto expected = filetime + 70;
        EXPECT_EQ(expected, timestamp.value());
    }
}
#endif /* defined(_WIN32) */

TEST(timestamp, arithmetics) {
    {
        const LYRA_NAMESPACE::timestamp::value_type expected = 1;
        auto actual = LYRA_NAMESPACE::timestamp();
        actual += std::chrono::nanoseconds(100);
        EXPECT_EQ(expected, actual.value());
    }

    {
        const LYRA_NAMESPACE::timestamp::value_type expected = 1;
        const auto t = LYRA_NAMESPACE::timestamp();
        const auto actual = t + std::chrono::nanoseconds(100);
        EXPECT_EQ(LYRA_NAMESPACE::timestamp::value_type(0), t.value());
        EXPECT_EQ(expected, actual.value());
    }

    {
        const LYRA_NAMESPACE::timestamp::value_type expected = 0;
        auto actual = LYRA_NAMESPACE::timestamp(1);
        actual -= std::chrono::nanoseconds(100);
        EXPECT_EQ(expected, actual.value());
    }

    {
        const LYRA_NAMESPACE::timestamp::value_type expected = 0;
        const auto t = LYRA_NAMESPACE::timestamp(1);
        const auto actual = t - std::chrono::nanoseconds(100);
        EXPECT_EQ(LYRA_NAMESPACE::timestamp::value_type(1), t.value());
        EXPECT_EQ(expected, actual.value());
    }
}
