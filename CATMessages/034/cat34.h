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
                FixedBytesItemParser cat34_Field2(m_cat_definition["items"][0]["data_fields"][0], std::string("000 - Message Type"));
                cat34_Field2.parseItem(m_data, parsedBytes, 0, 0, m_cat_definition, 0);
                parsedBytes += cat34_Field2.length_; // 1 byte parsed
                std::cout << "000 value = " << cat34_Field2.data_uint << "\n";
            }
            else if (decodeBytes == "010")
            {
                FixedBytesItemParser cat34_Field1(m_cat_definition["items"][1]["data_fields"][0], std::string("010 - Data Source Identifier SAC"));
                cat34_Field1.parseItem(m_data, parsedBytes, 0, 0, m_cat_definition, 0);
                parsedBytes += cat34_Field1.length_; // 1 byte parsed
                std::cout << "010 SAC value = " << cat34_Field1.data_uint << " ";

                FixedBytesItemParser cat34_Field1_1(m_cat_definition["items"][1]["data_fields"][1], std::string("010 - Data Source Identifier SIC"));
                cat34_Field1_1.parseItem(m_data, parsedBytes, 0, 0, m_cat_definition, 0);
                parsedBytes += cat34_Field1_1.length_; // 1 byte parsed
                std::cout << "010 SIC value = " << cat34_Field1_1.data_uint << "\n";
            }
            else if (decodeBytes == "020")
            {
                FixedBytesItemParser cat34_Field4(m_cat_definition["items"][2]["data_fields"][0], std::string("020 - Sector Number"));
                cat34_Field4.parseItem(m_data, parsedBytes, 0, 0, m_cat_definition, 0);
                parsedBytes += cat34_Field4.length_; // 1 byte parsed
                std::cout << "020 value = " << cat34_Field4.data_uint * cat34_Field4.lsb_ << "\n";
            }
            else if (decodeBytes == "030")
            {
                FixedBytesItemParser cat34_Field3(m_cat_definition["items"][3]["data_fields"][0], std::string("030 - Time of Day"));
                cat34_Field3.parseItem(m_data, parsedBytes, 0, 0, m_cat_definition, 0);
                parsedBytes += cat34_Field3.length_; // 3 byte parsed
                std::cout << "030 value = " << cat34_Field3.data_uint * cat34_Field3.lsb_ << "\n";
            }
            else if (decodeBytes == "050")
            {
                ExtendableBitsItemParser cat34_Field5_1(m_cat_definition["items"][5]["data_fields"][0]["field_specification"], std::string("050 - System Configuration and Status"));
                cat34_Field5_1.parseItem(m_data, parsedBytes, 0, 0, m_cat_definition, 0);
                parsedBytes += 1; // 1 byte parsedD

                std::vector<nlohmann::json> bitfield_Map; // 050 mesajı için alanlar
                bitfield_Map.push_back(m_cat_definition["items"][5]["data_fields"][0]["items"][0]);
                bitfield_Map.push_back(m_cat_definition["items"][5]["data_fields"][0]["items"][1]);
                bitfield_Map.push_back(m_cat_definition["items"][5]["data_fields"][0]["items"][2]);
                bitfield_Map.push_back(m_cat_definition["items"][5]["data_fields"][0]["items"][3]);

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
                                parsedBytes = parsedBytes + cat34_ParseBitField.parseItem(m_data, parsedBytes, 0, 0, m_cat_definition, 0);
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