#include<string>
#include"../lib/json.hpp"
#include <fstream>
#include <vector>

class CATMessageBase
{
public:
    CATMessageBase(const std::string& cat_definition, const std::string& data_block_definition);
    virtual ~CATMessageBase();

    //Decode File .bin
    virtual bool decodeFile() = 0; 

    //Decode Data Network(TCP/IP & UDP)
    virtual bool decodeData() = 0;

private:
    //Decode First 4 Bytes (FSPEC)
    virtual size_t decodeHeader() = 0;

    //Decode UAP
    virtual std::vector<std::string> decodeUAPList() = 0;

    //Decode All Data Blocks
    virtual bool decodeDataBlocks() = 0;

    //Decode Specified Data Block
    virtual bool decodeDataBlock() = 0;

private:
    const char* m_data;
    nlohmann::json m_cat_definition{""};
    nlohmann::json m_data_block_definition{""};

    // std::vector<unsigned char> data_Header;     //First 4 byte declares header
    // std::vector<std::string> uap_List;          //UAP_List constant. Order comes as it is in'.json'
    // std::vector<std::string> data_Block;        //Data after 4 byte of header as (data+4)
};