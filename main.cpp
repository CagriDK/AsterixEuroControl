#include <iostream>

#include <chrono>
#include <cstdlib>
#include <thread>
#include <memory>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <cassert>
#include <algorithm>

#include "CATMessages/034/cat34.h"

int main(int, char **)
{
    nlohmann::json definition,mapping;
    definition = nlohmann::json::parse(std::ifstream("../CATMessages/034/cat034_1.26.json"));
    //      ASTERIX data block at pos 0: cat=34; len=20
    //      22 00 14 f6 00 02 02 41 6d ec 40 94 40 60 44 00 94 00 00 10

    const char *data = "\x22\x00\x14\xf6\x00\x02\x02\x41\x6d\xec\x40\x94\x40\x60\x44\x00\x94\x00\x00\x10";

    std::cout<<"CAT CATEGORY = " <<static_cast<int>(static_cast<unsigned char>(data[0]))<<"\n";

    CAT34 temp_Parser34(definition,data);

    CATMessageBase* temp_Parser = &temp_Parser34;
    temp_Parser->decodeData();

}
