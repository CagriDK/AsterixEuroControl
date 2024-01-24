#include "cat62.h"
#include <iostream>

CAT62::CAT62(nlohmann::json cat_def, const char *data)
{
    m_cat_definition = cat_def;
    m_data = data;
};

CAT62::~CAT62(){

};

bool CAT62::decodeData()
{
    decodeHeader();

    for (auto decodeBytes : header_info.uap_list)
    {
        if (decodeBytes == "FX")
        {
            //"FX" (Field Extension Indicator)
            std::cout << "FX" << std::endl;
            continue;
        }
        else if (decodeBytes == "RE")
        {
            //"RE" (Reserved Expansion Field)
            std::cout << "RE" << std::endl;
            continue;
        }
        else if (decodeBytes == "SP")
        {
            //"SP" (Special Purpose Field)
            std::cout << "SP" << std::endl;
            continue;
        }
        else if (decodeBytes == "-")
        {
            //"-" (Empyt Line - Data Not Related With Radar)
            std::cout << "-" << std::endl;
            parsedBytes += 1;
            continue;
        }
        else
        {
            ItemParser testParser(m_cat_definition["items"][CAT62_items_order.at(decodeBytes)]);
            parsedBytes += testParser.parseItem(m_data, parsedBytes, 0, 0, mapping, 0);
            std::cout << mapping[decodeBytes].dump(4) << std::endl;
        }
    }
    std::cout << mapping.dump(4) << std::endl;
    // to_json(mapping);
    // from_json(cat62_message);

    return true;
}

size_t CAT62::decodeHeader()
{
    header_info.cat_type = static_cast<int>(static_cast<unsigned char>(m_data[0]));
    header_info.message_len = (static_cast<unsigned char>(m_data[1]) << 8) | static_cast<unsigned char>(m_data[2]);

    std::vector<bool> binaryRepresentation;
    size_t byteIndex = 3;

    bool moreBytes;
    do
    {
        std::bitset<8> fspecBits(m_data[byteIndex]);
        moreBytes = fspecBits[0]; // En sağdaki bit, daha fazla FSPEC byte'ı olup olmadığını belirtir

        for (int bitIndex = 7; bitIndex >= 0; --bitIndex)
        {
            binaryRepresentation.push_back(fspecBits[bitIndex]); // FSPEC'in geri kalan bitlerini oku
        }

        ++byteIndex;
    } while (moreBytes);

    // UAP list ile karşılaştır, fspec alanını, octet dizilimi ters olduğu için reverse yaptım.

    for (auto val = 0; val < binaryRepresentation.size(); val++)
    {
        if (binaryRepresentation[val] == true)
        {

            header_info.uap_list.push_back(CAT62_uap_order[val]);
        }
    }
    parsedBytes += byteIndex;
    return 0;
}

void CAT62::to_json(nlohmann::json &j)
{

}

void CAT62::from_json(Cat62Record &cat_data)
{

}

void CAT62::copy_to_Json(nlohmann::json &j, nlohmann::json &cat_Map, std::string key)
{
    if (j[key].empty())
    {
        std::cout << key << " : message is not exist!" << std::endl;
    }
    else
    {
        for (const auto &item : j.items())
        {
            cat_Map[item.key()] = item.value();
        }
    }
}

template <typename T>
void CAT62::copy_from_Json(const nlohmann::json &j, const std::string &key, T &value)
{
    if (j.contains(key) && !j[key].is_null())
    {
        value = j[key].get<T>();
    }
    else
    {
        // std::cout << key << " : message is not exist!" << std::endl;
    }
}