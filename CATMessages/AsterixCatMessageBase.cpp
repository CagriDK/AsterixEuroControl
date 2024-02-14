#include "AsterixCatMessageBase.h"

AsterixCatMessageBase::AsterixCatMessageBase(){

};

AsterixCatMessageBase::~AsterixCatMessageBase()
{

}

bool AsterixCatMessageBase::baseDecodeData(const char* data, const json &cat_definition,const std::map<std::string,int> &cat_items_order,const std::vector<std::string> &uap_list, json &cat_data_return)
{
    m_data = "";
    parsedBytes = 0;
    header_info.uap_list.clear();
    header_info.message_len = 0;
    header_info.cat_type = 0;
    m_data = data;
    m_uap_list = uap_list;
    cat_data_return.clear();

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
            ItemParser itemParser(cat_definition["items"][cat_items_order.at(decodeBytes)]);
            parsedBytes += itemParser.parseItem(m_data, parsedBytes, 0, 0, cat_data_return, 0);
            //std::cout << cat_data_return[decodeBytes].dump(4) << std::endl;
        }
    }
    std::cout << cat_data_return.dump(4) << std::endl;

    /*
    std::vector<char> a;

    baseEncodeData(cat_data_return,cat_definition,cat_items_order,uap_list,a);
    encodeHeader(0x22,a); //Test amaçlı
    char * ret = reinterpret_cast<char*>(&a[0]);

    baseDecodeData(ret,cat_definition,cat_items_order,uap_list,cat_data_return);
    
    */

    return true;
}

size_t AsterixCatMessageBase::decodeHeader()
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

            header_info.uap_list.push_back(m_uap_list[val]);
        }
    }
    parsedBytes += byteIndex;
    return 0;
}


