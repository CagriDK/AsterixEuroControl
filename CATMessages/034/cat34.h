#pragma once

#include "../CATMessageBase.h"
#include "cat34Message.h"

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

    nlohmann::json cat34_json;
    Cat34Record cat34_message;
};