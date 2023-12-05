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


#include "src/extendablebitsitemparser.h"
#include "src/fixedbytesitemparser.h"
#include "src/fixedbitfielditemparser.h"
#include "src/fixedbitsitemparser.h"

#include "CATMessages/034/cat34.h"

int main(int, char **)
{
    nlohmann::json definition;
    definition = nlohmann::json::parse(std::ifstream("../cat034_1.26.json"));

    //      ASTERIX data block at pos 0: cat=34; len=20
    //      22 00 14 f6 00 02 02 41 6d ec 40 94 40 60 44 00 94 00 00 10
    const char *data = "\x22\x00\x14\xf6\x00\x02\x02\x41\x6d\xec\x40\x94\x40\x60\x44\x00\x94\x00\x00\x10";

    std::cout<<"CAT CATEGORY = " <<static_cast<int>(static_cast<unsigned char>(data[0]))<<"\n";
    CAT34 temp_Parser34(definition,data);
    temp_Parser34.decodeData();


    //    {
    //        "data_blocks": [
    //            {
    //                "category": 34,
    //                "content": {
    //                    "index": 3,
    //                    "length": 17,
    //                    "records": [
    //                        {
    //                            "000": {
    //                                "Message Type": 2
    //                            },
    //                            "010": {
    //                                "SAC": 0,
    //                                "SIC": 2
    //                            },
    //                            "020": {
    //                                "Sector Number": 90.0
    //                            },
    //                            "030": {
    //                                "Time of Day": 33499.84375
    //                            },
    //                            "050": {
    //                                "COM": {
    //                                    "MSC": 0,
    //                                    "NOGO": 0,
    //                                    "OVL RDP": 0,
    //                                    "OVL XMT": 0,
    //                                    "RDPC": 1,
    //                                    "RDPR": 0,
    //                                    "TSV": 0
    //                                },
    //                                "MDS": {
    //                                    "ANT": 0,
    //                                    "CH-A/B": 2,
    //                                    "DLF": 0,
    //                                    "MSC": 0,
    //                                    "OVL DLF": 0,
    //                                    "OVL SCF": 0,
    //                                    "OVL SUR": 0,
    //                                    "SCF": 1
    //                                },
    //                                "PSR": {
    //                                    "ANT": 0,
    //                                    "CH-A/B": 3,
    //                                    "MSC": 0,
    //                                    "OVL": 0
    //                                },
    //                                "available": [
    //                                    false,
    //                                    false,
    //                                    true,
    //                                    false,
    //                                    true,
    //                                    false,
    //                                    false,
    //                                    true
    //                                ]
    //                            },
    //                            "060": {
    //                                "COM": {
    //                                    "RED-RDP": 0,
    //                                    "RED-XMT": 0
    //                                },
    //                                "MDS": {
    //                                    "CLU": 1,
    //                                    "RED-RAD": 0
    //                                },
    //                                "PSR": {
    //                                    "POL": 0,
    //                                    "RED-RAD": 0,
    //                                    "STC": 0
    //                                },
    //                                "available": [
    //                                    false,
    //                                    false,
    //                                    true,
    //                                    false,
    //                                    true,
    //                                    false,
    //                                    false,
    //                                    true
    //                                ]
    //                            },
    //                            "FSPEC": [
    //                                true,
    //                                true,
    //                                true,
    //                                true,
    //                                false,
    //                                true,
    //                                true,
    //                                false
    //                            ]
    //                        }
    //                    ]
    //                },
    //                "length": 20
    //            }
    //        ]
    //    }
}
