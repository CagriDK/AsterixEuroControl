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
    nlohmann::json definition, mapping;
    definition = nlohmann::json::parse(std::ifstream("../CATMessages/034/cat034_1.26.json"));
    //definition = nlohmann::json::parse(std::ifstream("../CATMessages/034/cat021_0.26.json"));

    //      ASTERIX data block at pos 0: cat=34; len=20
    //      22 00 14 f6 00 02 02 41 6d ec 40 94 40 60 44 00 94 00 00 10
    
    //      ASTERIX data block at pos 0: cat=21; len=36
    //      15 00 24 fb 81 13 80 00 05 40 30 1c 29 3e 00 6e 4c c9 ff e6 06 d6 49 6c 2e 00 08 08 18 dc 31 ca 08 20 00 15

    const char *data = "\x22\x00\x14\xf6\x00\x02\x02\x41\x6d\xec\x40\x94\x40\x60\x44\x00\x94\x00\x00\x10";
    //const char *data = "\x15\x00\x24\xfb\x81\x13\x80\x00\x05\x40\x30\x1c\x29\x3e\x00\x6e\x4c\xc9\xff\xe6\x06\xd6\x49\x6c\x2e\x00\x08\x08\x18\xdc\x31\xca\x08\x20\x00\x15";

    std::cout << "CAT CATEGORY = " << static_cast<int>(static_cast<unsigned char>(data[0])) << "\n";

    CAT34 temp_Parser34(definition, data);

    CATMessageBase *temp_Parser = &temp_Parser34;
    temp_Parser->decodeData();
}
