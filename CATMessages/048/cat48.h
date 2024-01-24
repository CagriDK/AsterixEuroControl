#pragma once

#include "../CATMessageBase.h"
#include "cat48Message.h"

class CAT48 : public CATMessageBase
{
public:
    CAT48(nlohmann::json cat_def, const char *data);
    virtual ~CAT48() override;
    // Decode Data Network(TCP/IP & UDP)
    virtual bool decodeData() override;

    virtual void to_json(nlohmann::json &j);
    virtual void from_json(Cat48Record &);

private:
    virtual size_t decodeHeader() override;
    virtual void copy_to_Json(nlohmann::json &j, nlohmann::json &cat_Map, std::string key);

    template <typename T>
    void copy_from_Json(const nlohmann::json &j, const std::string &key, T &value);

    CATMessageBase::MessageHeader header_info;
    size_t parsedBytes{0};

private:
    std::vector<std::string> CAT48_uap_order{"010","140","020","040","070","090","130","FX","220","240","250","161","042","200","170","FX","210","030","080","100","110","120","230","FX","260","055","050","065","060","SP","RE","FX"};
    std::map<std::string, int> CAT48_items_order{{"010", 0}, {"020", 1}, {"030", 2}, {"040", 3}, {"042", 4}, {"050", 5}, {"055", 6}, {"060", 7}, {"065", 8}, {"070", 9}, {"080", 10}, {"090", 11}, {"100", 12}, {"110", 13}, {"120", 14}, {"130", 15}, {"140", 16}, {"161", 17}, {"170", 18}, {"200", 19}, {"210", 20}, {"220", 21}, {"230", 22}, {"240", 23}, {"250", 24}, {"260", 25}};

    nlohmann::json cat48_json;
    Cat48Record cat48_message;
};