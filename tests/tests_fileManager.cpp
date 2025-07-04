#include <gtest/gtest.h>
#include <fstream>
#include <cstdio>  // for std::remove
#include "../../src/fileManager/fileManager.hpp"
#include "../../src/maliciousURL/maliciousURL.hpp"

TEST(FileManagerTest, SaveAndLoadDataWorks) {
    std::string tempFilename = "test_iocs.csv";

    // Create a list of IOCs
    std::vector<std::unique_ptr<Indicator>> indicators;
    indicators.push_back(std::make_unique<MaliciousURL>(
        1, 4, "URL", "Desc1", "Origin1", "2025-07-04T15:00:00", "evil.com", "HTTPS"));

    // Save to file
    bool saveSuccess = FileManager::saveData(tempFilename, indicators);
    ASSERT_TRUE(saveSuccess);

    // Load from file
    std::vector<std::unique_ptr<Indicator>> loaded = FileManager::loadData(tempFilename);
    ASSERT_EQ(loaded.size(), 1);

    // Cast to the expected derived type
    auto* loadedURL = dynamic_cast<MaliciousURL*>(loaded[0].get());
    ASSERT_NE(loadedURL, nullptr);

    // Check values
    EXPECT_EQ(loadedURL->getIndicatorId(), 1);
    EXPECT_EQ(loadedURL->getSeverity(), 4);
    EXPECT_EQ(loadedURL->getType(), "URL");
    EXPECT_EQ(loadedURL->getDescription(), "Desc1");
    EXPECT_EQ(loadedURL->getOrigin(), "Origin1");
    EXPECT_EQ(loadedURL->getTimestamp(), "2025-07-04T15:00:00");
    EXPECT_EQ(loadedURL->getURL(), "evil.com");
    EXPECT_EQ(loadedURL->getProtocol(), "HTTPS");

    // Clean up temporary file
    std::remove(tempFilename.c_str());
}
