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

std::vector<bool> parseFSPEC(const std::vector<unsigned char> &data)
{
    std::vector<bool> fieldsPresent;
    size_t byteIndex = 0;

    bool moreBytes;
    do
    {
        std::bitset<8> fspecBits(data[byteIndex]);
        moreBytes = fspecBits[0]; // En sağdaki bit, daha fazla FSPEC byte'ı olup olmadığını belirtir

        for (size_t bitIndex = 1; bitIndex < 8; ++bitIndex)
        {
            fieldsPresent.push_back(fspecBits[bitIndex]); // FSPEC'in geri kalan bitlerini oku
        }

        ++byteIndex;
    } while (moreBytes && byteIndex < data.size());

    return fieldsPresent;
}

int main(int, char **)
{
    nlohmann::json definition;
    definition = nlohmann::json::parse(std::ifstream("../cat034_1.26.json"));

    //      ASTERIX data block at pos 0: cat=34; len=20
    //      22 00 14 f6 00 02 02 41 6d ec 40 94 40 60 44 00 94 00 00 10

    // Header Decode
    const char *data = "\x22\x00\x14\xf6\x00\x02\x02\x41\x6d\xec\x40\x94\x40\x60\x44\x00\x94\x00\x00\x10";
    size_t parsedBytes{0};

    std::vector<unsigned char> data_Header;
    data_Header.push_back(data[0]);
    data_Header.push_back(data[1]);
    data_Header.push_back(data[2]);
    data_Header.push_back(data[3]);
    std::reverse(data_Header.begin(), data_Header.end()); // octet format left to right xf6 is first byte actually

    // FSPEC Decode
    auto FSPEC_data = parseFSPEC(data_Header);
    std::reverse(FSPEC_data.begin(), FSPEC_data.end());

    parsedBytes += data_Header.size();

    std::vector<std::string> uapList{
        "010",
        "000",
        "030",
        "020",
        "041",
        "050",
        "060",
        "FX",
        "070",
        "100",
        "110",
        "120",
        "090",
        "RE",
        "SP",
        "FX"};

    std::vector<std::string> messageData;

    for (auto val = 0; val < FSPEC_data.size(); val++)
    {
        if (FSPEC_data[val] == true)
        {
            messageData.push_back(uapList[val]);
        }
    }

    for (auto decodeBytes : messageData)
    {
        if (decodeBytes == "000")
        {
            FixedBytesItemParser cat34_Field2(definition, std::string("000 - Message Type"));
            cat34_Field2.length_ = 1;
            cat34_Field2.data_type_ = "uint";
            cat34_Field2.parseItem(data, parsedBytes, 0, 0, definition, 0);
            parsedBytes += cat34_Field2.length_; // 1 byte parsed
            std::cout << "000 value = " << cat34_Field2.data_uint << "\n";
        }
        else if (decodeBytes == "010")
        {
            FixedBytesItemParser cat34_Field1(definition, std::string("010 - Data Source Identifier"));
            cat34_Field1.length_ = 2;
            cat34_Field1.data_type_ = "uint";
            cat34_Field1.parseItem(data, parsedBytes, 0, 0, definition, 0);
            parsedBytes += cat34_Field1.length_; // 2 byte parsed
            std::cout << "010 value = " << cat34_Field1.data_uint << "\n";
        }
        else if (decodeBytes == "020")
        {
            FixedBytesItemParser cat34_Field4(definition, std::string("020 - Sector Number"));
            cat34_Field4.length_ = 1;
            cat34_Field4.data_type_ = "uint";
            cat34_Field4.has_lsb_ = true;
            cat34_Field4.lsb_ = 1.40625;
            cat34_Field4.parseItem(data, parsedBytes, 0, 0, definition, 0);
            parsedBytes += cat34_Field4.length_; // 1 byte parsed
            std::cout << "020 value = " << cat34_Field4.data_uint * cat34_Field4.lsb_ << "\n";
        }
        else if (decodeBytes == "030")
        {
            FixedBytesItemParser cat34_Field3(definition, std::string("030 - Time of Day"));
            cat34_Field3.length_ = 3;
            cat34_Field3.data_type_ = "uint";
            cat34_Field3.has_lsb_ = true;
            cat34_Field3.lsb_ = 0.0078125;
            cat34_Field3.parseItem(data, parsedBytes, 0, 0, definition, 0);
            parsedBytes += cat34_Field3.length_; // 3 byte parsed
            std::cout << "030 value = " << cat34_Field3.data_uint * cat34_Field3.lsb_ << "\n";
        }
        else if (decodeBytes == "050")
        {
            ExtendableBitsItemParser cat34_Field5_1(definition, std::string("050 - System Configuration and Status"));
            cat34_Field5_1.data_type_ = "bitfield";
            cat34_Field5_1.reverse_bits_ = true;
            cat34_Field5_1.parseItem(data, parsedBytes, 0, 0, definition, 0);
            parsedBytes += 1; // 1 byte parsed
            // Burası birleşik mesaj için available bölümleri gösterir.

            std::vector<nlohmann::json> bitfield_Map; // 050 mesajı için alanlar
            bitfield_Map.push_back(definition["items"][5]["data_fields"][0]["items"][0]);
            bitfield_Map.push_back(definition["items"][5]["data_fields"][0]["items"][1]);
            bitfield_Map.push_back(definition["items"][5]["data_fields"][0]["items"][2]);
            bitfield_Map.push_back(definition["items"][5]["data_fields"][0]["items"][3]);

            for (auto i = 0; i < cat34_Field5_1.bitfield.size(); i++)
            {
                if (cat34_Field5_1.bitfield[i] == true) // Available Map for compound vector(extandablebitsitem)
                {                                       //[1,0,0,1,0,1,0,0] // [COM,0,0,PSR,0,MDS,0,0]
                    for (auto j = 0; j < bitfield_Map.size(); j++)
                    {
                        if (bitfield_Map[j]["optional_bitfield_index"] == i)
                        {
                            if (j == 0)
                            {
                                std::cout << "050 - COM  value = [NOGO RDPC RDPR OVLRDP OVLXMT MSC TSV] \n";
                            }
                            else if (j == 1)
                            {
                                std::cout << "050 - PSR value = [ANT CH-A/B OVL MSC]  \n";
                            }
                            else if (j == 2)
                            {
                            }
                            else if (j == 3)
                            {
                                std::cout << "050 - MDS value = [ANT CH-A/B OVLSUR MSC SCF DLF OVLSCF OVLDLF] \n";
                            }
                            FixedBitFieldItemParser cat34_ParseBitField(bitfield_Map[j]["data_fields"][0], "");
                            parsedBytes = parsedBytes + cat34_ParseBitField.parseItem(data, parsedBytes, 0, 0, definition, 0);
                            std::cout << "050 -" << bitfield_Map[j]["name"] << " value = [";
                            for (auto val : cat34_ParseBitField.bitfield)
                            {
                                std::cout << val << " ";
                            }
                            std::cout << "]\n";
                        }
                    }
                }
            }
        }
        else if (decodeBytes == "060")
        {
        }
    }

    int bitfieldstart_index = 12;

    std::cout << "\n Decode Finished \n";

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
