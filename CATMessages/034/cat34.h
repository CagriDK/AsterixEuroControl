#include "../CATMessageBase.h"

struct MessageHeader
{
    int cat_type;
    int message_len;
    std::vector<std::string> uap_list;
};

class CAT34 : public CATMessageBase
{
public:
    CAT34(nlohmann::json cat_def, const char *data)
    {
        m_cat_definition = cat_def;
        m_data = data;
    };

    // Decode Data Network(TCP/IP & UDP)
    virtual bool decodeData() override
    {
        decodeHeader();

        for (auto decodeBytes : header_info.uap_list)
        {
            if (decodeBytes == "000")
            {
                FixedBytesItemParser parser(m_cat_definition["items"][0]["data_fields"][0], std::string("000 - Message Type"));
                parser.parseItem(m_data, parsedBytes, 0, 0, m_cat_definition, 0);
                parsedBytes += parser.length_; // 1 byte parsed
                std::cout << "000 value = " << parser.data_uint << "\n";
            }
            else if (decodeBytes == "010")
            {
                FixedBytesItemParser parser1(m_cat_definition["items"][1]["data_fields"][0], std::string("010 - Data Source Identifier SAC"));
                parser1.parseItem(m_data, parsedBytes, 0, 0, m_cat_definition, 0);
                parsedBytes += parser1.length_; // 1 byte parsed
                std::cout << "010 SAC value = " << parser1.data_uint << " ";

                FixedBytesItemParser parser2(m_cat_definition["items"][1]["data_fields"][1], std::string("010 - Data Source Identifier SIC"));
                parser2.parseItem(m_data, parsedBytes, 0, 0, m_cat_definition, 0);
                parsedBytes += parser2.length_; // 1 byte parsed
                std::cout << "010 SIC value = " << parser2.data_uint << "\n";
            }
            else if (decodeBytes == "020")
            {
                FixedBytesItemParser parser(m_cat_definition["items"][2]["data_fields"][0], std::string("020 - Sector Number"));
                parser.parseItem(m_data, parsedBytes, 0, 0, m_cat_definition, 0);
                parsedBytes += parser.length_; // 1 byte parsed
                std::cout << "020 value = " << parser.data_uint * parser.lsb_ << "\n";
            }
            else if (decodeBytes == "030")
            {
                FixedBytesItemParser parser(m_cat_definition["items"][3]["data_fields"][0], std::string("030 - Time of Day"));
                parser.parseItem(m_data, parsedBytes, 0, 0, m_cat_definition, 0);
                parsedBytes += parser.length_; // 3 byte parsed
                std::cout << "030 value = " << parser.data_uint * parser.lsb_ << "\n";
            }
            else if (decodeBytes == "041")
            {
                FixedBytesItemParser parser(m_cat_definition["items"][4]["data_fields"][0], std::string("041 - Antenna Rotation Speed"));
                parser.parseItem(m_data, parsedBytes, 0, 0, m_cat_definition, 0);
                parsedBytes += parser.length_; // 2 byte parsed
                std::cout << "041 value = " << parser.data_uint * parser.lsb_ << "\n";
            }
            else if (decodeBytes == "050")
            {
                ExtendableBitsItemParser parser(m_cat_definition["items"][5]["data_fields"][0]["field_specification"], std::string("050 - System Configuration and Status"));
                parser.parseItem(m_data, parsedBytes, 0, 0, m_cat_definition, 0);
                parsedBytes += 1; // 1 byte parsed

                std::vector<nlohmann::json> bitfield_Map; // 050 mesajı için alanlar
                bitfield_Map.push_back(m_cat_definition["items"][5]["data_fields"][0]["items"][0]);
                bitfield_Map.push_back(m_cat_definition["items"][5]["data_fields"][0]["items"][1]);
                bitfield_Map.push_back(m_cat_definition["items"][5]["data_fields"][0]["items"][2]);
                bitfield_Map.push_back(m_cat_definition["items"][5]["data_fields"][0]["items"][3]);

                for (auto i = 0; i < parser.bitfield.size(); i++)
                {
                    if (parser.bitfield[i] == true) // Available Map for compound vector(extandablebitsitem)
                    {                               //[1,0,0,1,0,1,0,0] // [COM,0,0,PSR,0,MDS,0,0]
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
                                FixedBitFieldItemParser parser(bitfield_Map[j]["data_fields"][0], "");
                                parsedBytes = parsedBytes + parser.parseItem(m_data, parsedBytes, 0, 0, m_cat_definition, 0);
                                std::cout << "050 -" << bitfield_Map[j]["name"] << " value = [";
                                for (auto val : parser.bitfield)
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
                ExtendableBitsItemParser parser(m_cat_definition["items"][6]["data_fields"][0]["field_specification"], std::string("050 - System Configuration and Status"));
                parser.parseItem(m_data, parsedBytes, 0, 0, m_cat_definition, 0);
                parsedBytes += 1; // 1 byte parsed

                std::vector<nlohmann::json> bitfield_Map; // 050 mesajı için alanlar
                bitfield_Map.push_back(m_cat_definition["items"][6]["data_fields"][0]["items"][0]);
                bitfield_Map.push_back(m_cat_definition["items"][6]["data_fields"][0]["items"][1]);
                bitfield_Map.push_back(m_cat_definition["items"][6]["data_fields"][0]["items"][2]);
                bitfield_Map.push_back(m_cat_definition["items"][6]["data_fields"][0]["items"][3]);

                for (auto i = 0; i < parser.bitfield.size(); i++)
                {
                    if (parser.bitfield[i] == true) // Available Map for compound vector(extandablebitsitem)
                    {                               //[1,0,0,1,0,1,0,0] // [COM,0,0,PSR,0,MDS,0,0]
                        for (auto j = 0; j < bitfield_Map.size(); j++)
                        {
                            if (bitfield_Map[j]["optional_bitfield_index"] == i)
                            {
                                if (j == 0)
                                {
                                    std::cout << "060 - COM  value = [RED-RDP RED-XMT] \n";
                                }
                                else if (j == 1)
                                {
                                    std::cout << "060 - PSR value = [POL RED-RAD STC]  \n";
                                }
                                else if (j == 2)
                                {
                                    std::cout << "060 - SSR value = [RED-RAD]  \n";
                                }
                                else if (j == 3)
                                {
                                    std::cout << "060 - MDS value = [RED-RAD CLU] \n";
                                }
                                FixedBitFieldItemParser parser(bitfield_Map[j]["data_fields"][0], "");
                                parsedBytes = parsedBytes + parser.parseItem(m_data, parsedBytes, 0, 0, m_cat_definition, 0);
                                std::cout << "060 -" << bitfield_Map[j]["name"] << " value = [";
                                for (auto val : parser.bitfield)
                                {
                                    std::cout << val << " ";
                                }
                                std::cout << "]\n";
                            }
                        }
                    }
                }
            }
            else if (decodeBytes == "070")
            {
                // const char *data = "\x02\x1A\xB3\x4C\xD5";
                // parsedBytes = 0;
                FixedBytesItemParser parser(m_cat_definition["items"][7]["data_fields"][0]["repetition_item"], std::string("070 - Message Count Values"));
                parser.parseItem(m_data, parsedBytes, 0, 0, m_cat_definition, 0);
                parsedBytes += parser.length_; // 1 byte parsed
                std::cout << "070 - plot_count_values = " << parser.data_uint << "\n";

                for (size_t i = 0; i < parser.data_uint; ++i)
                {
                    size_t start_index = 1 + i * 2; // Her tekrar 2 byte
                    unsigned char byte1 = m_data[start_index];
                    unsigned char byte2 = m_data[start_index + 1];

                    std::bitset<16> combined(byte1 << 8 | byte2); // İki byte'ı birleştir

                    // TYP ve COUNTER değerlerini al
                    unsigned int typ = (combined.to_ulong() >> 11) & 0x1F; // En üst 5 bit
                    unsigned int counter = combined.to_ulong() & 0x07FF;   // Alt 11 bit

                    std::cout << "070 - Typ: " << typ << ", Counter: " << counter << std::endl;
                }
            }
            else if (decodeBytes == "090")
            {
                FixedBytesItemParser parser1(m_cat_definition["items"][8]["data_fields"][0], std::string("090 - Collimation Error RANGE ERROR"));
                parser1.parseItem(m_data, parsedBytes, 0, 0, m_cat_definition, 0);
                parsedBytes += parser1.length_; // 1 byte parsed
                std::cout << "090 RANGE ERROR value = " << parser1.data_int * parser1.lsb_ << " ";

                FixedBytesItemParser parser2(m_cat_definition["items"][8]["data_fields"][1], std::string("090 - Collimation Error AZIMUTH ERROR"));
                parser2.parseItem(m_data, parsedBytes, 0, 0, m_cat_definition, 0);
                parsedBytes += parser2.length_; // 1 byte parsed
                std::cout << "010 AZIMUTH ERROR value = " << parser2.data_int * parser2.lsb_ << "\n";
            }
            else if (decodeBytes == "100")
            {
                FixedBytesItemParser parser1(m_cat_definition["items"][9]["data_fields"][0], std::string("100 - Generic Polar Window"));
                parser1.parseItem(m_data, parsedBytes, 0, 0, m_cat_definition, 0);
                parsedBytes += parser1.length_; // 1 byte parsed
                std::cout << "090 RHO-START value = " << parser1.data_int * parser1.lsb_ << " ";

                FixedBytesItemParser parser2(m_cat_definition["items"][9]["data_fields"][1], std::string("100 - Generic Polar Window"));
                parser2.parseItem(m_data, parsedBytes, 0, 0, m_cat_definition, 0);
                parsedBytes += parser2.length_; // 1 byte parsed
                std::cout << "010 RHO-END value = " << parser2.data_int * parser2.lsb_ << "\n";

                FixedBytesItemParser parser3(m_cat_definition["items"][9]["data_fields"][2], std::string("100 - Generic Polar Window"));
                parser3.parseItem(m_data, parsedBytes, 0, 0, m_cat_definition, 0);
                parsedBytes += parser3.length_; // 1 byte parsed
                std::cout << "090 THETA-START value = " << parser3.data_int * parser3.lsb_ << " ";

                FixedBytesItemParser parser4(m_cat_definition["items"][9]["data_fields"][3], std::string("100 - Generic Polar Window"));
                parser4.parseItem(m_data, parsedBytes, 0, 0, m_cat_definition, 0);
                parsedBytes += parser4.length_; // 1 byte parsed
                std::cout << "010 THETA-END value = " << parser4.data_int * parser4.lsb_ << "\n";
            }
            else if (decodeBytes == "110")
            {
                FixedBytesItemParser parser(m_cat_definition["items"][10]["data_fields"][0], std::string("110 - Data Filter"));
                parser.parseItem(m_data, parsedBytes, 0, 0, m_cat_definition, 0);
                parsedBytes += parser.length_; // 1 byte parsed
                std::cout << "110 TYP value = " << parser.data_uint << " ";
            }
            else if (decodeBytes == "120")
            {
                FixedBytesItemParser parser1(m_cat_definition["items"][11]["data_fields"][0], std::string("100 - 3D-Position Of Data Source"));
                parser1.parseItem(m_data, parsedBytes, 0, 0, m_cat_definition, 0);
                parsedBytes += parser1.length_; // 1 byte parsed
                std::cout << "090 Height of Data Source value = " << parser1.data_int * parser1.lsb_ << " ";

                FixedBytesItemParser parser2(m_cat_definition["items"][11]["data_fields"][1], std::string("100 - 3D-Position Of Data Source"));
                parser2.parseItem(m_data, parsedBytes, 0, 0, m_cat_definition, 0);
                parsedBytes += parser2.length_; // 1 byte parsed
                std::cout << "010 Latitude value = " << parser2.data_int * parser2.lsb_ << "\n";

                FixedBytesItemParser parser3(m_cat_definition["items"][11]["data_fields"][2], std::string("100 - 3D-Position Of Data Source"));
                parser3.parseItem(m_data, parsedBytes, 0, 0, m_cat_definition, 0);
                parsedBytes += parser3.length_; // 1 byte parsed
                std::cout << "090 Longitude value = " << parser3.data_int * parser3.lsb_ << " ";
            }
        }

        return true;
    }

    virtual size_t decodeHeader() override
    {
        header_info.cat_type = static_cast<int>(static_cast<unsigned char>(m_data[0]));
        header_info.message_len = (static_cast<unsigned char>(m_data[1]) << 8) | static_cast<unsigned char>(m_data[2]);

        std::vector<bool> binaryRepresentation;
        size_t byteIndex = 3;

        bool moreBytes;
        do
        {
            std::bitset<8> fspecBits(m_data[3]);
            moreBytes = fspecBits[0]; // En sağdaki bit, daha fazla FSPEC byte'ı olup olmadığını belirtir

            for (size_t bitIndex = 0; bitIndex < 8; ++bitIndex)
            {
                binaryRepresentation.push_back(fspecBits[bitIndex]); // FSPEC'in geri kalan bitlerini oku
            }

            ++byteIndex;
        } while (moreBytes && byteIndex < 4);

        // UAP list ile karşılaştır, fspec alanını, octet dizilimi ters olduğu için reverse yaptım.
        std::reverse(binaryRepresentation.begin(), binaryRepresentation.end());
        for (auto val = 0; val < binaryRepresentation.size(); val++)
        {
            if (binaryRepresentation[val] == true)
            {
                header_info.uap_list.push_back(CAT34_uap_order[val]);
            }
        }
        parsedBytes += byteIndex;
        return 0;
    }

private:
    const char *m_data;
    char *m_header = new char[4];

    MessageHeader header_info;
    size_t parsedBytes{0};

private:
    nlohmann::json m_cat_definition{""};
    std::vector<std::string> CAT34_uap_order{"010", "000", "030", "020", "041", "050", "060", "FX", "070", "100", "110", "120", "090", "RE", "SP", "FX"};
};