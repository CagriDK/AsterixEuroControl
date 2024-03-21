#pragma once
#include "gtest/gtest.h"
#include "../CATMessages/AsterixMessages.h"
#include "../include/appConfig.h"

TEST(CAT_TEST, CAT34)
{
    Config::getInstance();
    // const char *fullSampleData34 = "22002ffff8000202416dec40320094406044009400001002200450146400c800471c8e390400c83c71c8b8e38ec064";

    json cat34_sample = nlohmann::json::parse(std::ifstream(Config::getInstance().getCAT34JsonSample()));
    json cat34_definition = nlohmann::json::parse(std::ifstream(Config::getInstance().getCAT34Definition()));

    std::string sample = cat34_sample["Hex Data"];
    std::vector<char> vec(sample.begin(), sample.end());

    size_t byte_array_size = vec.size() / 2;
    char data_bytes[byte_array_size];

    AsterixCatMessageBase::hexStringToBytes(vec, data_bytes, byte_array_size);

    json jData;
    AsterixCat34Messages m_cat34;
    m_cat34.decodeData(data_bytes, jData, cat34_definition, false, cat34_sample);

    double epsilon10 = std::pow(10, 1);
    double epsilon1 = std::pow(1, 1);
    double epsilon01 = 0.1;

    EXPECT_EQ(jData.at("encodedHexData"), sample);

    EXPECT_EQ(jData.at("000").at("Message Type"), 2);

    EXPECT_EQ(jData.at("010").at("SAC"), 0);
    EXPECT_EQ(jData.at("010").at("SIC"), 2);

    EXPECT_NEAR(jData.at("020").at("Sector Number"), 90.0, epsilon01);

    EXPECT_NEAR(jData.at("030").at("Time of Day"), 33499.84375, epsilon01);

    EXPECT_NEAR(jData.at("041").at("Antenna Rotation Speed"), 100.0, epsilon01);

    EXPECT_EQ(jData.at("050").at("COM").at("MSC"), 0);
    EXPECT_EQ(jData.at("050").at("COM").at("NOGO"), 0);
    EXPECT_EQ(jData.at("050").at("COM").at("OVL RDP"), 0);
    EXPECT_EQ(jData.at("050").at("COM").at("OVL XMT"), 0);
    EXPECT_EQ(jData.at("050").at("COM").at("RDPC"), 1);
    EXPECT_EQ(jData.at("050").at("COM").at("RDPR"), 0);
    EXPECT_EQ(jData.at("050").at("COM").at("TSV"), 0);

    EXPECT_EQ(jData.at("050").at("MDS").at("ANT"), 0);
    EXPECT_EQ(jData.at("050").at("MDS").at("CH-A/B"), 2);
    EXPECT_EQ(jData.at("050").at("MDS").at("DLF"), 0);
    EXPECT_EQ(jData.at("050").at("MDS").at("MSC"), 0);
    EXPECT_EQ(jData.at("050").at("MDS").at("OVL DLF"), 0);
    EXPECT_EQ(jData.at("050").at("MDS").at("OVL SCF"), 0);
    EXPECT_EQ(jData.at("050").at("MDS").at("OVL SUR"), 0);
    EXPECT_EQ(jData.at("050").at("MDS").at("SCF"), 1);

    EXPECT_EQ(jData.at("050").at("PSR").at("ANT"), 0);
    EXPECT_EQ(jData.at("050").at("PSR").at("CH-A/B"), 3);
    EXPECT_EQ(jData.at("050").at("PSR").at("MSC"), 0);
    EXPECT_EQ(jData.at("050").at("PSR").at("OVL"), 0);

    std::vector<bool> expectedAvailable050 = {true, false, false, true, false, true, false, false};
    for (size_t i = 0; i < expectedAvailable050.size(); ++i)
    {
        EXPECT_EQ(jData.at("050").at("available")[i].get<bool>(), expectedAvailable050[i]);
    }

    EXPECT_EQ(jData.at("060").at("COM").at("RED-RDP"), 0);
    EXPECT_EQ(jData.at("060").at("COM").at("RED-XMT"), 0);

    EXPECT_EQ(jData.at("060").at("MDS").at("CLU"), 1);
    EXPECT_EQ(jData.at("060").at("MDS").at("RED-RAD"), 0);

    EXPECT_EQ(jData.at("060").at("PSR").at("POL"), 0);
    EXPECT_EQ(jData.at("060").at("PSR").at("RED-RAD"), 0);
    EXPECT_EQ(jData.at("060").at("PSR").at("STC"), 0);

    std::vector<bool> expectedAvailable060 = {true, false, false, true, false, true, false, false};
    for (size_t i = 0; i < expectedAvailable060.size(); ++i)
    {
        EXPECT_EQ(jData.at("060").at("available")[i].get<bool>(), expectedAvailable060[i]);
    }

    EXPECT_EQ(jData.at("070").at("plot_count_values")[0].at("TYP"), 4);
    EXPECT_EQ(jData.at("070").at("plot_count_values")[0].at("COUNTER"), 4);
    EXPECT_EQ(jData.at("070").at("plot_count_values")[1].at("TYP"), 10);
    EXPECT_EQ(jData.at("070").at("plot_count_values")[1].at("COUNTER"), 20);
    EXPECT_EQ(jData.at("070").at("REP"), 2);

    EXPECT_NEAR(jData.at("090").at("RANGE ERROR"), -0.5, epsilon01);
    EXPECT_NEAR(jData.at("090").at("AZIMUTH ERROR"), 2.2, epsilon01);

    EXPECT_NEAR(jData.at("100").at("RHO-START"), 100, epsilon01);
    EXPECT_NEAR(jData.at("100").at("RHO-END"), 200, epsilon01);
    EXPECT_NEAR(jData.at("100").at("THETA-START"), 100, epsilon01);
    EXPECT_NEAR(jData.at("100").at("THETA-END"), 200, epsilon01);

    EXPECT_EQ(jData.at("110").at("TYP"), 4);

    EXPECT_NEAR(jData.at("120").at("Height of Data Source"), 200, epsilon01);
    EXPECT_NEAR(jData.at("120").at("Latitude"), 85, epsilon01);
    EXPECT_NEAR(jData.at("120").at("Longitude"), -100, epsilon01);
}