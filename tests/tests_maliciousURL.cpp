#include <gtest/gtest.h>
#include "../../src/maliciousURL/maliciousURL.hpp"

TEST(MaliciousURLTest, ConstrutorEGettersFuncionam) {
    MaliciousURL url(1, 5, "URL", "Teste de URL", "Sistema", "2025-01-01T12:00:00", "www.site.com", "HTTPS");

    EXPECT_EQ(url.getIndicatorId(), 1);
    EXPECT_EQ(url.getSeverity(), 5);
    EXPECT_EQ(url.getType(), "URL");
    EXPECT_EQ(url.getDescription(), "Teste de URL");
    EXPECT_EQ(url.getOrigin(), "Sistema");
    EXPECT_EQ(url.getTimestamp(), "2025-01-01T12:00:00");
    EXPECT_EQ(url.getURL(), "www.site.com");
    EXPECT_EQ(url.getProtocol(), "HTTPS");
}

TEST(MaliciousURLTest, SettersFuncionam) {
    MaliciousURL url(1, 5, "URL", "Teste", "Origem", "Data", "a.com", "HTTP");

    url.setURL("b.com");
    url.setProtocol("FTP");

    EXPECT_EQ(url.getURL(), "b.com");
    EXPECT_EQ(url.getProtocol(), "FTP");
}

TEST(MaliciousURLTest, ToCSVCorreto) {
    MaliciousURL url(10, 2, "URL", "Malicioso", "Firewall", "2025-05-05T10:30:00", "malicious.net", "HTTPS");

    std::string esperado = "10,2,URL,Malicioso,Firewall,2025-05-05T10:30:00,,,,,,,malicious.net,HTTPS";
    EXPECT_EQ(url.toCSV(), esperado);
}
