#include <gtest/gtest.h>
#include "../../src/maliciousHash/maliciousHash.hpp"

// Test constructor and getters
TEST(MaliciousHashTest, ConstructorAndGettersWork) {
    MaliciousHash hash(7, 4, "Hash", "Suspicious file", "SystemScanner", "2025-06-30T09:15:00", "abcd1234", "SHA-256");

    EXPECT_EQ(hash.getIndicatorId(), 7);
    EXPECT_EQ(hash.getSeverity(), 4);
    EXPECT_EQ(hash.getType(), "Hash");
    EXPECT_EQ(hash.getDescription(), "Suspicious file");
    EXPECT_EQ(hash.getOrigin(), "SystemScanner");
    EXPECT_EQ(hash.getTimestamp(), "2025-06-30T09:15:00");
    EXPECT_EQ(hash.getHash(), "abcd1234");
    EXPECT_EQ(hash.getAlgorithm(), "SHA-256");
}

// Test setters
TEST(MaliciousHashTest, SettersWork) {
    MaliciousHash hash(1, 3, "Hash", "Initial", "Manual", "2025-01-01T00:00:00", "1111", "MD5");

    hash.setHash("ffff");
    hash.setAlgorithm("SHA1");

    EXPECT_EQ(hash.getHash(), "ffff");
    EXPECT_EQ(hash.getAlgorithm(), "SHA1");
}

// Test CSV output
TEST(MaliciousHashTest, ToCSVWorksCorrectly) {
    MaliciousHash hash(20, 5, "Hash", "Malware detected", "AV Engine", "2025-12-25T20:00:00", "deadbeef", "SHA-512");

    std::string expected = "20,5,Hash,Malware detected,AV Engine,2025-12-25T20:00:00,deadbeef,SHA-512,,,,,";
    EXPECT_EQ(hash.toCSV(), expected);
}
