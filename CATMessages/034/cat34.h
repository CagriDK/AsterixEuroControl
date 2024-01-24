#pragma once

#include "../CATMessageBase.h"
#include "cat34Messages.h"

class CAT34 : public CATMessageBase
{
public:
    CAT34(nlohmann::json cat_def, const char *data);
    virtual ~CAT34() override;
    // Decode Data Network(TCP/IP & UDP)
    virtual bool decodeData() override;

    virtual void to_json(nlohmann::json &j);
    virtual void from_json(Cat34Record &);

private:
    virtual size_t decodeHeader() override;
    virtual void copy_to_Json(nlohmann::json &j, nlohmann::json &cat_Map, std::string key);

    template <typename T>
    void copy_from_Json(const nlohmann::json &j, const std::string &key, T &value);

    CATMessageBase::MessageHeader header_info;
    size_t parsedBytes{0};

private:
    std::vector<std::string> CAT34_uap_order{"010", "000", "030", "020", "041", "050", "060", "FX", "070", "100", "110", "120", "090", "RE", "SP", "FX"};
    std::map<std::string, int> CAT34_items_order{{"000", 0}, {"010", 1}, {"020", 2}, {"030", 3}, {"041", 4}, {"050", 5}, {"060", 6}, {"070", 7}, {"090", 8}, {"100", 9}, {"110", 10}, {"120", 11}};

    std::vector<std::string> CAT62_uap_order{"010","-","015","070","105","100","185","FX","210","060","245","380","040","080","290","FX","200","295","136","130","135","220","390","FX","270","300","110","120","510","500","340","FX","-","-","-","-","-","RE","SP","FX"};
    std::map<std::string, int> CAT62_items_order{{"010", 0}, {"015", 1}, {"040", 2}, {"060", 3}, {"070", 4}, {"080", 5}, {"100", 6}, {"105", 7}, {"110", 8}, {"120", 9}, {"130", 10}, {"135", 11}, {"136", 12}, {"185", 13}, {"200", 14}, {"210", 15}, {"220", 16}, {"245", 17}, {"270", 18}, {"290", 19}, {"295", 20}, {"300", 21}, {"340", 22}, {"380", 23}, {"390", 24}, {"500", 25}, {"510", 26}};
                        
    std::vector<std::string> CAT48_uap_order{"010","140","020","040","070","090","130","FX","220","240","250","161","042","200","170","FX","210","030","080","100","110","120","230","FX","260","055","050","065","060","SP","RE","FX"};
    std::map<std::string, int> CAT48_items_order{{"010", 0}, {"020", 1}, {"030", 2}, {"040", 3}, {"042", 4}, {"050", 5}, {"055", 6}, {"060", 7}, {"065", 8}, {"070", 9}, {"080", 10}, {"090", 11}, {"100", 12}, {"110", 13}, {"120", 14}, {"130", 15}, {"140", 16}, {"161", 17}, {"170", 18}, {"200", 19}, {"210", 20}, {"220", 21}, {"230", 22}, {"240", 23}, {"250", 24}, {"260", 25}};

    nlohmann::json cat34_json;
    Cat34Record cat34_message;
};