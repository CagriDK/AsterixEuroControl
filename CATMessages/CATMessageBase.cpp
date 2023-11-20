#include "CATMessageBase.h"

CATMessageBase::CATMessageBase(const std::string &cat_definition, const std::string &data_block_definition)
{
    m_cat_definition = (nlohmann::json::parse(std::ifstream(cat_definition.c_str())));
    m_data_block_definition = (nlohmann::json::parse(std::ifstream(cat_definition.c_str())));
};

CATMessageBase::~CATMessageBase()
{
    
}







