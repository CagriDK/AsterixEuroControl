#pragma once
#include "gtest/gtest.h"
#include "../CATMessages/AsterixMessages.h"
#include "../include/appConfig.h"

TEST(ExampleTest, Test2)
{
    Config::getInstance();
    // const char *fullSampleData62 = "3e00f8bffffffe010264378dc0011c71c8011c71c803b718039e000087fffe78880e0080049483244d7621011040e2012c01de01010158ffe0c8c8c8c800c8c8c8c8c8c8f0ffffffffe0c8c8c8c8c8c8c8c8c8c8c8c8c832c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8004000100004005ffffff0010234324142464646400026ac40414231324141424132424142346464640190646401080c2d20474147414741405441524441313254415244413133128054415244413134c947c864fe7e00000101471c72238e39406400645a0e0064640191640190ff8000c800c800c891a3da7410b4c8b4c8b410fe01026400471c000493d8a2008c";

    json cat48_sample = nlohmann::json::parse(std::ifstream(Config::getInstance().getCAT48JsonSample()));
    json cat48_definition = nlohmann::json::parse(std::ifstream(Config::getInstance().getCAT48Definition()));

    std::string sample = cat48_sample["Hex Data"];
    std::vector<char> vec(sample.begin(), sample.end());

    size_t byte_array_size = vec.size() / 2;
    char data_bytes[byte_array_size];

    AsterixCatMessageBase::hexStringToBytes(vec, data_bytes, byte_array_size);

    json jData;
    AsterixCat48Messages m_cat48;
    m_cat48.decodeData(data_bytes, jData, cat48_definition, false, cat48_sample);

    double epsilon10 = std::pow(10, 1);
    double epsilon1 = std::pow(1, 1);
    double epsilon01 = 0.1;

    EXPECT_EQ(jData.at("encodedHexData"), sample);

    EXPECT_EQ(jData.at("010").at("SAC"), 0);
    EXPECT_EQ(jData.at("010").at("SIC"), 2);

    EXPECT_EQ(jData.at("020").at("FX"), 0);
    EXPECT_EQ(jData.at("020").at("RAB"), 0);
    EXPECT_EQ(jData.at("020").at("RDP"), 0);
    EXPECT_EQ(jData.at("020").at("SIM"), 0);
    EXPECT_EQ(jData.at("020").at("SPI"), 0);
    EXPECT_EQ(jData.at("020").at("TYP"), 5);

    EXPECT_EQ(jData.at("030").at("Warning/Error Conditions")[0].at("W/E Value"), 2);
    EXPECT_EQ(jData.at("030").at("Warning/Error Conditions")[0].at("extend"), 1);
    EXPECT_EQ(jData.at("030").at("Warning/Error Conditions")[1].at("W/E Value"), 1);
    EXPECT_EQ(jData.at("030").at("Warning/Error Conditions")[1].at("extend"), 0);

    EXPECT_FLOAT_EQ(jData.at("040").at("RHO"), 197.68359375);
    EXPECT_FLOAT_EQ(jData.at("040").at("THETA"), 340.13671875);

    EXPECT_EQ(jData.at("042").at("X-Component"), 100);
    EXPECT_EQ(jData.at("042").at("Y-Component"), 100);

    EXPECT_EQ(jData.at("050").at("G"), 0);
    EXPECT_EQ(jData.at("050").at("L"), 0);
    EXPECT_EQ(jData.at("050").at("Mode-2 code"), 1000);
    EXPECT_EQ(jData.at("050").at("V"), 0);

    EXPECT_EQ(jData.at("055").at("G"), 0);
    EXPECT_EQ(jData.at("055").at("L"), 0);
    EXPECT_EQ(jData.at("055").at("Mode-1 code"), 16);
    EXPECT_EQ(jData.at("055").at("V"), 0);

    EXPECT_EQ(jData.at("060").at("QXi"), 100);
    EXPECT_EQ(jData.at("065").at("QXi"), 100);

    EXPECT_EQ(jData.at("070").at("G"), 0);
    EXPECT_EQ(jData.at("070").at("L"), 0);
    EXPECT_EQ(jData.at("070").at("Mode-3/A reply"), 1000);
    EXPECT_EQ(jData.at("070").at("V"), 0);

    EXPECT_EQ(jData.at("080").at("QXi"), 100);

    EXPECT_FLOAT_EQ(jData.at("090").at("Flight Level"), 330.0);
    EXPECT_EQ(jData.at("090").at("G"), 0);
    EXPECT_EQ(jData.at("090").at("V"), 0);

    EXPECT_FLOAT_EQ(jData.at("100").at("Mode-C reply"), 330.0);
    EXPECT_EQ(jData.at("100").at("G"), 0);
    EXPECT_EQ(jData.at("100").at("V"), 0);
    EXPECT_EQ(jData.at("100").at("QXi"), 100);

    EXPECT_EQ(jData.at("110").at("3D-Height"), 1000);

    EXPECT_EQ(jData.at("120").at("CAL").at("D"), 1);
    EXPECT_EQ(jData.at("120").at("CAL").at("CAL"), 100);

    EXPECT_EQ(jData.at("120").at("RDS").at("Raw Doppler Speed")[0].at("DOP"), 100);
    EXPECT_EQ(jData.at("120").at("RDS").at("Raw Doppler Speed")[0].at("AMB"), 100);
    EXPECT_EQ(jData.at("120").at("RDS").at("Raw Doppler Speed")[0].at("FRQ"), 100);
    EXPECT_EQ(jData.at("120").at("RDS").at("Raw Doppler Speed")[1].at("DOP"), 50);
    EXPECT_EQ(jData.at("120").at("RDS").at("Raw Doppler Speed")[1].at("AMB"), 50);
    EXPECT_EQ(jData.at("120").at("RDS").at("Raw Doppler Speed")[1].at("FRQ"), 50);
    EXPECT_EQ(jData.at("120").at("RDS").at("REP"), 2);

    std::vector<bool> expectedAvailable120 = {true, true, false, false, false, false, false, false};
    for (size_t i = 0; i < expectedAvailable120.size(); ++i)
    {
        EXPECT_EQ(jData.at("120").at("available")[i].get<bool>(), expectedAvailable120[i]);
    }

    EXPECT_NEAR(jData.at("130").at("SRL").at("value"), 10, epsilon01);
    EXPECT_EQ(jData.at("130").at("SRR").at("value"), 100);
    EXPECT_EQ(jData.at("130").at("SAM").at("value"), 100);
    EXPECT_NEAR(jData.at("130").at("PRL").at("value"), 6, epsilon01);
    EXPECT_EQ(jData.at("130").at("PAM").at("value"), 100);
    EXPECT_NEAR(jData.at("130").at("RPD").at("value"), 0.2, epsilon01);
    EXPECT_NEAR(jData.at("130").at("APD").at("value"), 1.3, epsilon01);

    std::vector<bool> expectedAvailable130 = {true, true, true, true, true, true, true, false};
    for (size_t i = 0; i < expectedAvailable130.size(); ++i)
    {
        EXPECT_EQ(jData.at("130").at("available")[i].get<bool>(), expectedAvailable130[i]);
    }

    EXPECT_FLOAT_EQ(jData.at("140").at("Time-of-Day"), 27354.6015625);

    EXPECT_EQ(jData.at("161").at("TRACK NUMBER"), 100);

    EXPECT_EQ(jData.at("170").at("CDM"), 0);
    EXPECT_EQ(jData.at("170").at("CNF"), 0);
    EXPECT_EQ(jData.at("170").at("DOU"), 0);
    EXPECT_EQ(jData.at("170").at("FX"), 1);
    EXPECT_EQ(jData.at("170").at("GHO"), 0);
    EXPECT_EQ(jData.at("170").at("MAH"), 0);
    EXPECT_EQ(jData.at("170").at("RAD"), 2);
    EXPECT_EQ(jData.at("170").at("SUP"), 0);
    EXPECT_EQ(jData.at("170").at("TCC"), 0);
    EXPECT_EQ(jData.at("170").at("TRE"), 0);

    EXPECT_FLOAT_EQ(jData.at("200").at("CALCULATED GROUNDSPEED"), 434.3994140625);
    EXPECT_FLOAT_EQ(jData.at("200").at("CALCULATED HEADING"), 124.002685546875);

    EXPECT_FLOAT_EQ(jData.at("210").at("Sigma (X)"), 0.5);
    EXPECT_FLOAT_EQ(jData.at("210").at("Sigma (Y)"), 0.5);
    EXPECT_NEAR(jData.at("210").at("Sigma (V)"), 22, epsilon01);
    EXPECT_NEAR(jData.at("210").at("Sigma (H)"), 20, epsilon01);

    EXPECT_EQ(jData.at("220").at("AIRCRAFT ADDRESS"), 1000);

    EXPECT_EQ(jData.at("230").at("AIC"), 1);
    EXPECT_EQ(jData.at("230").at("ARC"), 1);
    EXPECT_EQ(jData.at("230").at("B1A"), 1);
    EXPECT_EQ(jData.at("230").at("B1B"), 5);
    EXPECT_EQ(jData.at("230").at("COM"), 1);
    EXPECT_EQ(jData.at("230").at("MSSC"), 1);
    EXPECT_EQ(jData.at("230").at("SI"), 0);
    EXPECT_EQ(jData.at("230").at("STAT"), 0);

    EXPECT_EQ(jData.at("240").at("Aircraft Identification"), "DLH65A  ");

    EXPECT_EQ(jData.at("250").at("Mode S MB Data")[0].at("BDS1"), 4);
    EXPECT_EQ(jData.at("250").at("Mode S MB Data")[0].at("BDS2"), 0);
    EXPECT_EQ(jData.at("250").at("Mode S MB Data")[0].at("MB Data"), "c0780031bc0000");
    EXPECT_EQ(jData.at("250").at("REP"), 1);

    EXPECT_EQ(jData.at("260").at("MB DATA"), "c0780031bc0000");
}