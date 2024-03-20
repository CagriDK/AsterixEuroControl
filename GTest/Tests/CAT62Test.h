#include "gtest/gtest.h"
#include "../CATMessages/AsterixMessages.h"
#include "../include/appConfig.h"

void hexStringToBytes(const std::vector<char> &hex_vector, char *bytes, size_t max_length)
{
    std::string hex(hex_vector.begin(), hex_vector.end());
    size_t length = hex.length();
    for (size_t i = 0; i < length; i += 2)
    {
        if (i / 2 >= max_length)
        {
            return;
        }
        std::string byteString = hex.substr(i, 2);
        unsigned char byte = static_cast<unsigned char>(std::stoul(byteString, nullptr, 16));
        bytes[i / 2] = byte;
    }
}

TEST(ExampleTest, Test1)
{
    Config::getInstance();
    //const char *fullSampleData62 = "3e00f8bffffffe010264378dc0011c71c8011c71c803b718039e000087fffe78880e0080049483244d7621011040e2012c01de01010158ffe0c8c8c8c800c8c8c8c8c8c8f0ffffffffe0c8c8c8c8c8c8c8c8c8c8c8c8c832c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8004000100004005ffffff0010234324142464646400026ac40414231324141424132424142346464640190646401080c2d20474147414741405441524441313254415244413133128054415244413134c947c864fe7e00000101471c72238e39406400645a0e0064640191640190ff8000c800c800c891a3da7410b4c8b4c8b410fe01026400471c000493d8a2008c";

    json cat62_sample = nlohmann::json::parse(std::ifstream(Config::getInstance().getCAT62JsonSample()));
    json cat62_definition = nlohmann::json::parse(std::ifstream(Config::getInstance().getCAT62Definition()));

    std::string sample = cat62_sample["Hex Data"];
    std::vector<char> vec(sample.begin(), sample.end());

    size_t byte_array_size = vec.size() / 2;
    char data_bytes[byte_array_size];

    hexStringToBytes(vec, data_bytes, byte_array_size);

    json jData;
    AsterixCat62Messages m_cat62;
    m_cat62.decodeData(data_bytes, jData, cat62_definition, false, cat62_sample);

    double epsilon = std::pow(10,1);

    EXPECT_EQ(jData.at("encodedHexData"), sample);

    EXPECT_NEAR(jData.at("010").at("SAC"), 1,epsilon);
    EXPECT_NEAR(jData.at("010").at("SIC"), 2,epsilon);

    EXPECT_NEAR(jData.at("015").at("Service Identification"), 100,epsilon);

    EXPECT_NEAR(jData.at("040").at("Track Number"), 478,epsilon);

    EXPECT_NEAR(jData.at("060").at("CH"), 0,epsilon);
    EXPECT_NEAR(jData.at("060").at("Mode-3/A reply"), 7000,epsilon);

    EXPECT_NEAR(jData.at("070").at("Time Of Track Information"), 28443.5,epsilon);

    EXPECT_NEAR(jData.at("080").at("AAC"), 0,epsilon);
    EXPECT_NEAR(jData.at("080").at("ADS"), 1,epsilon);
    EXPECT_NEAR(jData.at("080").at("AFF"), 0,epsilon);
    EXPECT_NEAR(jData.at("080").at("AMA"), 0,epsilon);
    EXPECT_NEAR(jData.at("080").at("CNF"), 0,epsilon);
    EXPECT_NEAR(jData.at("080").at("CST"), 0,epsilon);
    EXPECT_NEAR(jData.at("080").at("FPC"), 0,epsilon);
    EXPECT_NEAR(jData.at("080").at("FX"), 1,epsilon);
    EXPECT_NEAR(jData.at("080").at("FX2"), 1,epsilon);
    EXPECT_NEAR(jData.at("080").at("FX3"), 1,epsilon);
    EXPECT_NEAR(jData.at("080").at("FX4"), 0,epsilon);
    EXPECT_NEAR(jData.at("080").at("KOS"), 0,epsilon);
    EXPECT_NEAR(jData.at("080").at("MD4"), 0,epsilon);
    EXPECT_NEAR(jData.at("080").at("MD5"), 0,epsilon);
    EXPECT_NEAR(jData.at("080").at("MDS"), 1,epsilon);
    EXPECT_NEAR(jData.at("080").at("ME"), 0,epsilon);
    EXPECT_NEAR(jData.at("080").at("MI"), 0,epsilon);
    EXPECT_NEAR(jData.at("080").at("MON"), 0,epsilon);
    EXPECT_NEAR(jData.at("080").at("MRH"), 0,epsilon);
    EXPECT_NEAR(jData.at("080").at("PSR"), 1,epsilon);
    EXPECT_NEAR(jData.at("080").at("SIM"), 0,epsilon);
    EXPECT_NEAR(jData.at("080").at("SPI"), 0,epsilon);
    EXPECT_NEAR(jData.at("080").at("SRC"), 0,epsilon);
    EXPECT_NEAR(jData.at("080").at("SSR"), 0,epsilon);
    EXPECT_NEAR(jData.at("080").at("STP"), 0,epsilon);
    EXPECT_NEAR(jData.at("080").at("SUC"), 0,epsilon);
    EXPECT_NEAR(jData.at("080").at("TSB"), 0,epsilon);
    EXPECT_NEAR(jData.at("080").at("TSE"), 0,epsilon);

    EXPECT_NEAR(jData.at("100").at("X"), 121740.0,epsilon);
    EXPECT_NEAR(jData.at("100").at("Y"), 118528.0,epsilon);

    ASSERT_NEAR(jData.at("105").at("Latitude"), 100,epsilon);
    ASSERT_NEAR(jData.at("105").at("Longitude"), 100,epsilon);

    EXPECT_NEAR(jData.at("110").at("SUM").at("M5"), 0,epsilon);
    EXPECT_NEAR(jData.at("110").at("SUM").at("ID"), 1,epsilon);
    EXPECT_NEAR(jData.at("110").at("SUM").at("DA"), 1,epsilon);
    EXPECT_NEAR(jData.at("110").at("SUM").at("M1"), 1,epsilon);
    EXPECT_NEAR(jData.at("110").at("SUM").at("M2"), 1,epsilon);
    EXPECT_NEAR(jData.at("110").at("SUM").at("M3"), 1,epsilon);
    EXPECT_NEAR(jData.at("110").at("SUM").at("MC"), 1,epsilon);
    EXPECT_NEAR(jData.at("110").at("SUM").at("X"), 0,epsilon);

    EXPECT_NEAR(jData.at("110").at("PMN").at("PIN"), 0,epsilon);
    EXPECT_NEAR(jData.at("110").at("PMN").at("NAT"), 1,epsilon);
    EXPECT_NEAR(jData.at("110").at("PMN").at("MIS"), 1,epsilon);

    ASSERT_NEAR(jData.at("110").at("POS").at("Latitude"), 100.0,epsilon);
    ASSERT_NEAR(jData.at("110").at("POS").at("Longitude"), 50.0,epsilon);

    EXPECT_NEAR(jData.at("110").at("GA").at("RES"), 1,epsilon);
    EXPECT_NEAR(jData.at("110").at("GA").at("GA"), 100,epsilon);

    EXPECT_NEAR(jData.at("110").at("EM1").at("Extended Mode 1 Code"), 100,epsilon);

    EXPECT_NEAR(jData.at("110").at("TOS").at("TOS"), 0.7,epsilon); // 0.7

    EXPECT_NEAR(jData.at("110").at("XP").at("X5"), 0,epsilon);
    EXPECT_NEAR(jData.at("110").at("XP").at("XC"), 1,epsilon);
    EXPECT_NEAR(jData.at("110").at("XP").at("X3"), 1,epsilon);
    EXPECT_NEAR(jData.at("110").at("XP").at("X2"), 1,epsilon);
    EXPECT_NEAR(jData.at("110").at("XP").at("X1"), 0,epsilon);

    std::vector<bool> expectedAvailable = {true, true, true, true, true, true, true, false};
    for (size_t i = 0; i < expectedAvailable.size(); ++i)
    {
        EXPECT_EQ(jData.at("110").at("available")[i].get<bool>(), expectedAvailable[i]);
    }

    EXPECT_NEAR(jData.at("120").at("Mode-2 code"), 100,epsilon);
    EXPECT_NEAR(jData.at("130").at("Altitude"), 100,epsilon);
    EXPECT_NEAR(jData.at("135").at("QNH"), 0,epsilon);
    EXPECT_NEAR(jData.at("135").at("Calculated Track Barometric Altitude"), 100,epsilon);
    EXPECT_NEAR(jData.at("136").at("Measured Flight Level"), 16.0,epsilon);
    EXPECT_NEAR(jData.at("185").at("Vx"), 33.75,epsilon);
    EXPECT_NEAR(jData.at("185").at("Vy"), -0.5,epsilon);
    EXPECT_NEAR(jData.at("200").at("ADF"), 0,epsilon);
    EXPECT_NEAR(jData.at("200").at("LONG"), 3,epsilon);
    EXPECT_NEAR(jData.at("200").at("TRANS"), 3,epsilon);
    EXPECT_NEAR(jData.at("200").at("VERT"), 0,epsilon);
    EXPECT_NEAR(jData.at("210").at("Ax"), 30,epsilon); // -28
    EXPECT_NEAR(jData.at("210").at("Ay"), -30,epsilon); // -28
    EXPECT_NEAR(jData.at("220").at("Rate of Climb/Descent"), 593.75,epsilon);
    EXPECT_NEAR(jData.at("245").at("STI"), 2,epsilon);
    EXPECT_EQ(jData.at("245").at("Target Identification"), "AIRCID56");
    EXPECT_NEAR(jData.at("270").at("LENGTH"), 100,epsilon);
    EXPECT_NEAR(jData.at("270").at("FX"), 1,epsilon);
    EXPECT_NEAR(jData.at("270").at("ORIENTATION"), 100,epsilon); // 98.4375
    EXPECT_NEAR(jData.at("270").at("FX2"), 1,epsilon);
    EXPECT_NEAR(jData.at("270").at("WIDTH"), 100,epsilon);
    EXPECT_NEAR(jData.at("270").at("FX3"), 0,epsilon);

    EXPECT_NEAR(jData.at("290").at("TRK").at("Age"), 50,epsilon); // 36
    EXPECT_NEAR(jData.at("290").at("PSR").at("Age"), 50,epsilon); // 36
    EXPECT_NEAR(jData.at("290").at("SSR").at("Age"), 50,epsilon); // 36
    EXPECT_NEAR(jData.at("290").at("MDS").at("Age"), 50,epsilon); // 36
    EXPECT_NEAR(jData.at("290").at("ADS").at("Age"), 50,epsilon); // 36
    EXPECT_NEAR(jData.at("290").at("ES").at("Age"), 50,epsilon);  // 36
    EXPECT_NEAR(jData.at("290").at("VDL").at("Age"), 50,epsilon); // 36
    EXPECT_NEAR(jData.at("290").at("UAT").at("Age"), 50,epsilon); // 36
    EXPECT_NEAR(jData.at("290").at("LOP").at("Age"), 50,epsilon); // 36
    EXPECT_NEAR(jData.at("290").at("MLT").at("Age"), 50,epsilon); // 36

    std::vector<bool> expectedAvailable290 = {
        true, true, true, true, true, true, true, true, true, true, true,
        false, false, false, false, false};
    for (size_t i = 0; i < expectedAvailable290.size(); ++i)
    {
        EXPECT_EQ(jData.at("290").at("available")[i].get<bool>(), expectedAvailable290[i]);
    }

    EXPECT_NEAR(jData.at("295").at("MFL").at("Age"), 50,epsilon); // 36
    EXPECT_NEAR(jData.at("295").at("MD1").at("Age"), 50,epsilon); // 36
    EXPECT_NEAR(jData.at("295").at("MD2").at("Age"), 50,epsilon); // 36
    EXPECT_NEAR(jData.at("295").at("MDA").at("Age"), 50,epsilon); // 36
    EXPECT_NEAR(jData.at("295").at("MD4").at("Age"), 50,epsilon); // 36
    EXPECT_NEAR(jData.at("295").at("MD5").at("Age"), 50,epsilon); // 36
    EXPECT_NEAR(jData.at("295").at("MHG").at("Age"), 50,epsilon); // 36
    EXPECT_NEAR(jData.at("295").at("IAS").at("Age"), 50,epsilon); // 36
    EXPECT_NEAR(jData.at("295").at("TAS").at("Age"), 50,epsilon); // 36
    EXPECT_NEAR(jData.at("295").at("SAL").at("Age"), 50,epsilon); // 36
    EXPECT_NEAR(jData.at("295").at("FSS").at("Age"), 50,epsilon); // 36
    EXPECT_NEAR(jData.at("295").at("TID").at("Age"), 50,epsilon); // 36
    EXPECT_NEAR(jData.at("295").at("COM").at("Age"), 50,epsilon); // 36
    EXPECT_NEAR(jData.at("295").at("SAB").at("Age"), 50,epsilon); // 36
    EXPECT_NEAR(jData.at("295").at("ACS").at("Age"), 50,epsilon); // 36
    EXPECT_NEAR(jData.at("295").at("BVR").at("Age"), 50,epsilon); // 36
    EXPECT_NEAR(jData.at("295").at("GVR").at("Age"), 50,epsilon); // 36
    EXPECT_NEAR(jData.at("295").at("RAN").at("Age"), 50,epsilon); // 36
    EXPECT_NEAR(jData.at("295").at("TAR").at("Age"), 50,epsilon); // 36
    EXPECT_NEAR(jData.at("295").at("TAN").at("Age"), 50,epsilon); // 36
    EXPECT_NEAR(jData.at("295").at("GSP").at("Age"), 50,epsilon); // 36
    EXPECT_NEAR(jData.at("295").at("VUN").at("Age"), 50,epsilon); // 36
    EXPECT_NEAR(jData.at("295").at("MET").at("Age"), 50,epsilon); // 36
    EXPECT_NEAR(jData.at("295").at("EMC").at("Age"), 50,epsilon); // 36
    EXPECT_NEAR(jData.at("295").at("POS").at("Age"), 50,epsilon); // 36
    EXPECT_NEAR(jData.at("295").at("GAL").at("Age"), 50,epsilon); // 36
    EXPECT_NEAR(jData.at("295").at("PUN").at("Age"), 50,epsilon); // 36
    EXPECT_NEAR(jData.at("295").at("MB").at("Age"), 50,epsilon);  // 36
    EXPECT_NEAR(jData.at("295").at("IAR").at("Age"), 50,epsilon); // 36
    EXPECT_NEAR(jData.at("295").at("MAC").at("Age"), 50,epsilon); // 36
    EXPECT_NEAR(jData.at("295").at("BPS").at("Age"), 50,epsilon); // 36

    std::vector<bool> expectedAvailable295 = {
        true, true, true, true, true, true, true, true, true, true, true,
        true, true, true, true, true, true, true, true, true, true, true,
        true, true, true, true, true, true, true, true, true, true, true,
        true, true, false, false, false, false, false};
    for (size_t i = 0; i < expectedAvailable295.size(); ++i)
    {
        EXPECT_EQ(jData.at("295").at("available")[i].get<bool>(), expectedAvailable295[i]);
    }

    EXPECT_NEAR(jData.at("300").at("VFI"), 100,epsilon);

    // Field 340 subfields
    EXPECT_NEAR(jData.at("340").at("SID").at("SAC"), 1,epsilon);
    EXPECT_NEAR(jData.at("340").at("SID").at("SIC"), 2,epsilon);
    EXPECT_NEAR(jData.at("340").at("POS").at("RHO"), 100,epsilon);
    EXPECT_NEAR(jData.at("340").at("POS").at("THETA"), 100,epsilon); // 99.99755859375
    EXPECT_NEAR(jData.at("340").at("HEI").at("HEIGHT"), 100,epsilon);
    EXPECT_NEAR(jData.at("340").at("MDC").at("V"), 1,epsilon);
    EXPECT_NEAR(jData.at("340").at("MDC").at("G"), 0,epsilon);
    EXPECT_NEAR(jData.at("340").at("MDC").at("Last Measured Mode C Code"), 1270,epsilon);
    EXPECT_NEAR(jData.at("340").at("MDA").at("V"), 1,epsilon);
    EXPECT_NEAR(jData.at("340").at("MDA").at("G"), 0,epsilon);
    EXPECT_NEAR(jData.at("340").at("MDA").at("L"), 1,epsilon);
    EXPECT_NEAR(jData.at("340").at("MDA").at("Mode-3/A reply"), 1000,epsilon);
    EXPECT_NEAR(jData.at("340").at("TYP").at("TYP"), 4,epsilon);
    EXPECT_NEAR(jData.at("340").at("TYP").at("SIM"), 0,epsilon);
    EXPECT_NEAR(jData.at("340").at("TYP").at("RAB"), 1,epsilon);
    EXPECT_NEAR(jData.at("340").at("TYP").at("TST"), 1,epsilon);

    std::vector<bool> expectedAvailable340 = {true, true, true, true, true, true, true, false};
    for (size_t i = 0; i < expectedAvailable340.size(); ++i)
    {
        EXPECT_EQ(jData.at("340").at("available")[i].get<bool>(), expectedAvailable340[i]);
    }

    ASSERT_FLOAT_EQ(jData.at("380").at("GSP").at("Ground Speed"), 0.018310546875);
    ASSERT_FLOAT_EQ(jData.at("380").at("MHG").at("Magnetic Heading"), 91.241455078125);

    std::vector<bool> expectedAvailable380 = {
        false, false, true, false, false, false, false, true, false, false,
        false, false, false, false, false, true, false, false, false, true,
        false, false, false, false};
    for (size_t i = 0; i < expectedAvailable380.size(); ++i)
    {
        EXPECT_EQ(jData.at("380").at("available")[i].get<bool>(), expectedAvailable380[i]);
    }

    EXPECT_NEAR(jData.at("390").at("TAG").at("SAC"), 1,epsilon);
    EXPECT_NEAR(jData.at("390").at("TAG").at("SIC"), 2,epsilon);
    EXPECT_EQ(jData.at("390").at("CSN").at("Callsign"), "42ABFFF");
    EXPECT_NEAR(jData.at("390").at("IFI").at("TYP"), 1,epsilon);
    EXPECT_NEAR(jData.at("390").at("IFI").at("NBR"), 9900,epsilon);
    EXPECT_NEAR(jData.at("390").at("FCT").at("GAT/OAT"), 1,epsilon);
    EXPECT_NEAR(jData.at("390").at("FCT").at("FR1/FR2"), 0,epsilon);
    EXPECT_NEAR(jData.at("390").at("FCT").at("RVSM"), 0,epsilon);
    EXPECT_NEAR(jData.at("390").at("FCT").at("HPR"), 0,epsilon);
    EXPECT_EQ(jData.at("390").at("TAC").at("Type of Aircraft"), "AB12");
    EXPECT_EQ(jData.at("390").at("WTC").at("Wake Turbulence Category"), "A");
    EXPECT_EQ(jData.at("390").at("DEP").at("Departure Airport"), "ABA2");
    EXPECT_EQ(jData.at("390").at("DST").at("Destination Airport"), "BAB4");
    EXPECT_NEAR(jData.at("390").at("RDS").at("NU1"), 100,epsilon);
    EXPECT_NEAR(jData.at("390").at("RDS").at("NU2"), 100,epsilon);
    EXPECT_NEAR(jData.at("390").at("RDS").at("LTR"), 100,epsilon);
    EXPECT_NEAR(jData.at("390").at("CFL").at("CFL"), 100,epsilon);
    EXPECT_NEAR(jData.at("390").at("CTL").at("Centre"), 100,epsilon);
    EXPECT_NEAR(jData.at("390").at("CTL").at("Position"), 100,epsilon);
    EXPECT_NEAR(jData.at("390").at("TOD").at("Time of Departure / Arrival").at(0).at("TYP"), 1,epsilon);
    EXPECT_NEAR(jData.at("390").at("TOD").at("Time of Departure / Arrival").at(0).at("DAY"), 0,epsilon);
    EXPECT_NEAR(jData.at("390").at("TOD").at("Time of Departure / Arrival").at(0).at("HOR"), 12,epsilon);
    EXPECT_NEAR(jData.at("390").at("TOD").at("Time of Departure / Arrival").at(0).at("MIN"), 45,epsilon);
    EXPECT_NEAR(jData.at("390").at("TOD").at("Time of Departure / Arrival").at(0).at("AVS"), 0,epsilon);
    EXPECT_NEAR(jData.at("390").at("TOD").at("Time of Departure / Arrival").at(0).at("SEC"), 32,epsilon);
    EXPECT_NEAR(jData.at("390").at("TOD").at("REP"), 1,epsilon);
    EXPECT_EQ(jData.at("390").at("AST").at("Aircraft Stand"), "GAGAGA");
    EXPECT_NEAR(jData.at("390").at("STS").at("EMP"), 1,epsilon);
    EXPECT_NEAR(jData.at("390").at("STS").at("AVL"), 0,epsilon);
    EXPECT_EQ(jData.at("390").at("STD").at("Standard Instrument Departure"), "TARDA12");
    EXPECT_EQ(jData.at("390").at("STA").at("Standard Instrument Arrival"), "TARDA13");
    EXPECT_NEAR(jData.at("390").at("PEM").at("VA"), 1,epsilon);
    EXPECT_NEAR(jData.at("390").at("PEM").at("Mode-3/A reply"), 1200,epsilon); // 0
    EXPECT_EQ(jData.at("390").at("PEC").at("Pre-Emergency Callsign"), "TARDA14");

    std::vector<bool> expectedAvailable390 = {
        true, true, true, true, true, true, true, true, true, true, true, true,
        true, true, true, true, true, true, true, true, false, false, false, false};
    for (size_t i = 0; i < expectedAvailable390.size(); ++i)
    {
        EXPECT_EQ(jData.at("390").at("available")[i].get<bool>(), expectedAvailable390[i]);
    }

    EXPECT_NEAR(jData.at("500").at("APC").at("APC (X-Component)"), 100.0,epsilon);
    EXPECT_NEAR(jData.at("500").at("APC").at("APC (Y-Component)"), 100.0,epsilon);
    EXPECT_NEAR(jData.at("500").at("COV").at("COV (XY Covariance Component)"), 100.0,epsilon);
    EXPECT_NEAR(jData.at("500").at("APW").at("APW (Latitude Component)"), 0.2,epsilon);  // 0.15625476837158203
    EXPECT_NEAR(jData.at("500").at("APW").at("APW (Longitude Component)"), 0.3,epsilon); // 0.15625476837158203
    EXPECT_NEAR(jData.at("500").at("AGA").at("AGA"), 100.0,epsilon);
    EXPECT_NEAR(jData.at("500").at("ABA").at("ABA"), 45.0,epsilon);               // 36
    EXPECT_NEAR(jData.at("500").at("ATV").at("ATV (X-Component)"), 50.0,epsilon); // 36
    EXPECT_NEAR(jData.at("500").at("ATV").at("ATV (Y-Component)"), 45.0,epsilon); // 36
    EXPECT_NEAR(jData.at("500").at("AA").at("AA (X-Component)"), 50.0,epsilon);   // 36
    EXPECT_NEAR(jData.at("500").at("AA").at("AA (Y-Component)"), 45.0,epsilon);   // 36
    EXPECT_NEAR(jData.at("500").at("ARC").at("ARC"), 100.0,epsilon);

    std::vector<bool> expectedAvailable500 = {
        true, true, true, true, true, true, true, true, true,
        false, false, false, false, false, false, false};
    for (size_t i = 0; i < expectedAvailable500.size(); ++i)
    {
        EXPECT_EQ(jData.at("500").at("available")[i].get<bool>(), expectedAvailable500[i]);
    }

    auto composedTrackNumbers = jData.at("510").at("Composed Track Number");
    EXPECT_NEAR(composedTrackNumbers[0].at("SYSTEM UNIT IDENTIFICATION"), 100,epsilon);
    EXPECT_NEAR(composedTrackNumbers[0].at("SYSTEM TRACK NUMBER"), 200,epsilon);
    EXPECT_NEAR(composedTrackNumbers[0].at("extend"), 1,epsilon);
    EXPECT_NEAR(composedTrackNumbers[1].at("SYSTEM UNIT IDENTIFICATION"), 100,epsilon);
    EXPECT_NEAR(composedTrackNumbers[1].at("SYSTEM TRACK NUMBER"), 200,epsilon);
    EXPECT_NEAR(composedTrackNumbers[1].at("extend"), 0,epsilon);
}