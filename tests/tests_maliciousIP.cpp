#include <gtest/gtest.h>
#include "../../src/maliciousIP/maliciousIP.hpp"

// Test constructor and getter methods
TEST(MaliciousIPTest, ConstructorAndGettersWork) {
    MaliciousIP ip(7, 4, "IP", "Suspicious activity", "SystemX", "2025-12-12T08:00:00", "192.168.0.1", "Brazil", "ISPName");

    EXPECT_EQ(ip.getIndicatorId(), 7);
    EXPECT_EQ(ip.getSeverity(), 4);
    EXPECT_EQ(ip.getType(), "IP");
    EXPECT_EQ(ip.getDescription(), "Suspicious activity");
    EXPECT_EQ(ip.getOrigin(), "SystemX");
    EXPECT_EQ(ip.getTimestamp(), "2025-12-12T08:00:00");
    EXPECT_EQ(ip.getIP(), "192.168.0.1");
    EXPECT_EQ(ip.getCountry(), "Brazil");
    EXPECT_EQ(ip.getISP(), "ISPName");
}

// Test setter methods
TEST(MaliciousIPTest, SettersWork) {
    MaliciousIP ip(7, 4, "IP", "Test", "TestOrigin", "2025-01-01T00:00:00", "1.1.1.1", "USA", "OldISP");

    ip.setIP("8.8.8.8");
    ip.setCountry("Germany");
    ip.setISP("NewISP");

    EXPECT_EQ(ip.getIP(), "8.8.8.8");
    EXPECT_EQ(ip.getCountry(), "Germany");
    EXPECT_EQ(ip.getISP(), "NewISP");
}

// Test toCSV output format
TEST(MaliciousIPTest, ToCSVCorrectFormat) {
    MaliciousIP ip(15, 2, "IP", "Malicious traffic", "Firewall", "2025-06-10T14:15:00", "203.0.113.45", "Canada", "Bell");

    std::string expected = "15,2,IP,Malicious traffic,Firewall,2025-06-10T14:15:00,,,203.0.113.45,Canada,Bell,,";
    EXPECT_EQ(ip.toCSV(), expected);
}
