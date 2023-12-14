#pragma once

#include "../CATMessageBase.h"
#include "cat34Messages.h"

struct MessageHeader
{
    int cat_type;
    int message_len;
    std::vector<std::string> uap_list;
};

class CAT34 : public CATMessageBase
{
public:
    CAT34(nlohmann::json cat_def, nlohmann::json cat_map, const char *data)
    {
        m_cat_definition = cat_def;
        mapping = cat_map;

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
                ItemParser testParser(m_cat_definition["items"][0]);
                parsedBytes += testParser.parseItem(m_data, parsedBytes, 0, 0, mapping, 0);
                std::cout << mapping["000"].dump(4) << std::endl;
                //std::cout << "000 Mapping Message Type value = " << toString(mapping[decodeBytes]["Message Type"]) << "\n";
            }
            else if (decodeBytes == "010")
            {
                ItemParser testParser(m_cat_definition["items"][1]);
                parsedBytes += testParser.parseItem(m_data, parsedBytes, 0, 0, mapping, 0);
                std::cout << mapping["010"].dump(4) << std::endl;
            }
            else if (decodeBytes == "020")
            {
                ItemParser testParser(m_cat_definition["items"][2]);
                parsedBytes += testParser.parseItem(m_data, parsedBytes, 0, 0, mapping, 0);
                std::cout << mapping["020"].dump(4) << std::endl;
            }
            else if (decodeBytes == "030")
            {
                ItemParser testParser(m_cat_definition["items"][3]);
                parsedBytes += testParser.parseItem(m_data, parsedBytes, 0, 0, mapping, 0);
                std::cout << mapping["030"].dump(4) << std::endl;
            }
            else if (decodeBytes == "041")
            {
                ItemParser testParser(m_cat_definition["items"][4]);
                parsedBytes += testParser.parseItem(m_data, parsedBytes, 0, 0, mapping, 0);
                std::cout << mapping["041"].dump(4) << std::endl;
            }
            else if (decodeBytes == "050")
            {
                ItemParser testParser(m_cat_definition["items"][5]);
                parsedBytes += testParser.parseItem(m_data, parsedBytes, 0, 0, mapping, 0);
                std::cout << mapping["050"].dump(4) << std::endl;
            }
            else if (decodeBytes == "060")
            {
                ItemParser testParser(m_cat_definition["items"][6]);
                parsedBytes += testParser.parseItem(m_data, parsedBytes, 0, 0, mapping, 0);
                std::cout << mapping["060"].dump(4) << std::endl;
            }
            else if (decodeBytes == "070")
            {
                ItemParser testParser(m_cat_definition["items"][7]);
                parsedBytes += testParser.parseItem(m_data, parsedBytes, 0, 0, mapping, 0);
                std::cout << mapping["070"].dump(4) << std::endl;
            }
            else if (decodeBytes == "090")
            {
                ItemParser testParser(m_cat_definition["items"][8]);
                parsedBytes += testParser.parseItem(m_data, parsedBytes, 0, 0, mapping, 0);
                std::cout << mapping["090"].dump(4) << std::endl;
            }
            else if (decodeBytes == "100")
            {
                ItemParser testParser(m_cat_definition["items"][9]);
                parsedBytes += testParser.parseItem(m_data, parsedBytes, 0, 0, mapping, 0);
                std::cout << mapping["100"].dump(4) << std::endl;
            }
            else if (decodeBytes == "110")
            {
                ItemParser testParser(m_cat_definition["items"][10]);
                parsedBytes += testParser.parseItem(m_data, parsedBytes, 0, 0, mapping, 0);
                std::cout << mapping["110"].dump(4) << std::endl;
            }
            else if (decodeBytes == "120")
            {

                ItemParser testParser(m_cat_definition["items"][11]);
                parsedBytes += testParser.parseItem(m_data, parsedBytes, 0, 0, mapping, 0);
                std::cout << mapping["120"].dump(4) << std::endl;
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
    nlohmann::json mapping{""};
    std::vector<std::string> CAT34_uap_order{"010", "000", "030", "020", "041", "050", "060", "FX", "070", "100", "110", "120", "090", "RE", "SP", "FX"};
    Cat34Record cat34_message;
};