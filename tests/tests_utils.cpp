#include <gtest/gtest.h>
#include <ctime>
#include <string>
#include "../utils/utils.hpp"

// Utilitário para criar time_t de forma segura
std::time_t makeTime(int year, int month, int day) {
    std::tm time = {};
    time.tm_year = year - 1900;  // Ano desde 1900
    time.tm_mon = month - 1;     // Meses vão de 0 a 11
    time.tm_mday = day;
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;
    return std::mktime(&time);
}

TEST(UtilsTest, StringToTimeT_ValidFormat) {
    std::string timestamp = "2024-06-15T00:00:00Z";
    std::time_t result = stringToTimeT(timestamp);
    std::tm* tm_result = std::localtime(&result);
    EXPECT_EQ(tm_result->tm_year + 1900, 2024);
    EXPECT_EQ(tm_result->tm_mon + 1, 6);
    EXPECT_EQ(tm_result->tm_mday, 15);
}

TEST(UtilsTest, IsCurrentMonth_CorrectlyDetects) {
    // Get today's date
    std::time_t now = std::time(nullptr);
    std::tm* tm_now = std::localtime(&now);

    // Format current date as string timestamp
    char buffer[25];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%dT%H:%M:%SZ", tm_now);
    std::string currentTimestamp(buffer);

    EXPECT_TRUE(isCurrentMonth(currentTimestamp));
}

TEST(UtilsTest, RegisterLastMonth_TrueIfWithinLastMonth) {
    std::time_t now = std::time(nullptr);
    std::tm lastMonthTm = *std::localtime(&now);
    lastMonthTm.tm_mon -= 1;

    if (lastMonthTm.tm_mon < 0) {
        lastMonthTm.tm_mon += 12;
        lastMonthTm.tm_year -= 1;
    }

    lastMonthTm.tm_mday = 15;
    std::time_t lastMonthDate = std::mktime(&lastMonthTm);

    char buffer[25];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%dT%H:%M:%SZ", std::localtime(&lastMonthDate));
    std::string lastMonthTimestamp(buffer);

    EXPECT_TRUE(registerLastMonth(lastMonthTimestamp));
}

TEST(UtilsTest, IContains_MatchCaseInsensitive) {
    EXPECT_TRUE(icontains("Malicious Domain", "domain"));
    EXPECT_TRUE(icontains("Indicator", "INDICATOR"));
    EXPECT_FALSE(icontains("SafeHost", "malware"));
}

TEST(UtilsTest, ToLower_ConvertsCorrectly) {
    std::string original = "ThReAtLeVeL";
    std::string expected = "threatlevel";
    EXPECT_EQ(toLower(original), expected);
}