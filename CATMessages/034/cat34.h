#pragma once

#include "CATMessageBase.h"
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
    CAT34(nlohmann::json cat_def, nlohmann::json cat_map, const char *data);
    virtual ~CAT34() override;
    // Decode Data Network(TCP/IP & UDP)
    virtual bool decodeData() override;

private:
    virtual size_t decodeHeader() override;

    const char *m_data;
    char *m_header = new char[4];

    MessageHeader header_info;
    size_t parsedBytes{0};

private:
    nlohmann::json m_cat_definition{""};
    nlohmann::json mapping{""};
    std::vector<std::string> CAT34_uap_order{"010", "000", "030", "020", "041", "050", "060", "FX", "070", "100", "110", "120", "090", "RE", "SP", "FX"};
    std::map<std::string, int> CAT34_items_order{{"000", 0}, {"010", 1} , {"020", 2}, {"030", 3}, {"041", 4}, {"050", 5}, {"060", 6},{"070", 7}, {"090", 8},{"100", 9}, {"110", 10}, {"120", 11}};
    Cat34Record cat34_message;
};