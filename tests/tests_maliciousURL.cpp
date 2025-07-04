#include <gtest/gtest.h>
#include <algorithm>
#include "../../src/maliciousURL/maliciousURL.hpp"

class MaliciousURLTest : public ::testing::Test {
protected:
    MaliciousURL url{10, 2, "URL", "Malicious", "Firewall", "2025-05-05T10:30:00", "malicious.net", "HTTPS"};
};

TEST_F(MaliciousURLTest, ConstructorAndGettersWorkCorrectly) {
    EXPECT_EQ(url.getIndicatorId(), 10) << "IndicatorId getter failed";
    EXPECT_EQ(url.getSeverity(), 2) << "Severity getter failed";
    EXPECT_EQ(url.getType(), "URL") << "Type getter failed";
    EXPECT_EQ(url.getDescription(), "Malicious") << "Description getter failed";
    EXPECT_EQ(url.getOrigin(), "Firewall") << "Origin getter failed";
    EXPECT_EQ(url.getTimestamp(), "2025-05-05T10:30:00") << "Timestamp getter failed";
    EXPECT_EQ(url.getURL(), "malicious.net") << "URL getter failed";
    EXPECT_EQ(url.getProtocol(), "HTTPS") << "Protocol getter failed";
}

TEST_F(MaliciousURLTest, SettersWorkCorrectly) {
    url.setURL("example.com");
    url.setProtocol("HTTP");

    EXPECT_EQ(url.getURL(), "example.com") << "URL setter failed";
    EXPECT_EQ(url.getProtocol(), "HTTP") << "Protocol setter failed";
}

TEST_F(MaliciousURLTest, ToCSVReturnsCorrectFormat) {
    std::string expectedCSV = "10,2,URL,Malicious,Firewall,2025-05-05T10:30:00,,,,,,malicious.net,HTTPS";

    std::string actualCSV = url.toCSV();
    EXPECT_EQ(actualCSV, expectedCSV) << "CSV output did not match expected format.\nExpected: " 
                                     << expectedCSV << "\nActual: " << actualCSV;
}

TEST_F(MaliciousURLTest, ToCSVHasCorrectNumberOfFields) {
    std::string csv = url.toCSV();
    // Count commas to ensure there are 12 commas (13 fields)
    size_t commaCount = std::count(csv.begin(), csv.end(), ',');
    EXPECT_EQ(commaCount, 12) << "CSV output should have 13 fields separated by 12 commas.";
}
