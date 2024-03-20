#pragma once 
#include <vector>
#include <iostream>
#include "../CATMessages/AsterixMessages.h"

class RadarClientMessageHandler
{
public:
    RadarClientMessageHandler();
    ~RadarClientMessageHandler();

    void asterixMessageParser(const std::vector<char> &data, bool TEST_SAMPLE_CASE);

private:
    void hexStringToBytes(const std::vector<char> &hex_vector, char *bytes, size_t max_length);

    AsterixCat21Messages m_cat21;
    AsterixCat34Messages m_cat34;
    AsterixCat48Messages m_cat48;
    AsterixCat62Messages m_cat62;

    int logFormat = 4;
    bool isLogEnable = true;
};