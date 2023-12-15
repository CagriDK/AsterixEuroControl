#include "cat34.h"
#include <iostream>

CAT34::CAT34(nlohmann::json cat_def, nlohmann::json cat_map, const char *data)
{
    m_cat_definition = cat_def;
    mapping = cat_map;
    m_data = data;
};

CAT34::~CAT34()
{

};

bool CAT34::decodeData()
{
    decodeHeader();

    for (auto decodeBytes : header_info.uap_list)
    {
        ItemParser testParser(m_cat_definition["items"][CAT34_items_order.at(decodeBytes)]);
        parsedBytes += testParser.parseItem(m_data, parsedBytes, 0, 0, mapping, 0);
        std::cout << mapping[decodeBytes].dump(4) << std::endl;
    }

    return true;
}

size_t CAT34::decodeHeader()
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