#pragma once

#include "../CATMessageBase.h"
#include "../categories-types/cat62Message.h"

class CAT62 : public CATMessageBase
{
public:
    CAT62(nlohmann::json cat_def, const char *data);
    virtual ~CAT62() override;
    // Decode Data Network(TCP/IP & UDP)
    virtual bool decodeData() override;

    virtual void to_json(nlohmann::json &j);
    virtual void from_json(Cat62Record &);

private:
    virtual size_t decodeHeader() override;

    CATMessageBase::MessageHeader header_info;
    size_t parsedBytes{0};

private:
    std::vector<std::string> CAT62_uap_order{"010","-","015","070","105","100","185","FX","210","060","245","380","040","080","290","FX","200","295","136","130","135","220","390","FX","270","300","110","120","510","500","340","FX","-","-","-","-","-","RE","SP","FX"};
    std::map<std::string, int> CAT62_items_order{{"010", 0}, {"015", 1}, {"040", 2}, {"060", 3}, {"070", 4}, {"080", 5}, {"100", 6}, {"105", 7}, {"110", 8}, {"120", 9}, {"130", 10}, {"135", 11}, {"136", 12}, {"185", 13}, {"200", 14}, {"210", 15}, {"220", 16}, {"245", 17}, {"270", 18}, {"290", 19}, {"295", 20}, {"300", 21}, {"340", 22}, {"380", 23}, {"390", 24}, {"500", 25}, {"510", 26}};
                        
    nlohmann::json cat62_json;
    Cat62Record cat62_message;
};