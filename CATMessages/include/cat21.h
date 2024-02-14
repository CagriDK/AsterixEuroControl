#pragma once

#include "../CATMessageBase.h"
#include "../categories-types/cat21Message.h"

class CAT21 : public CATMessageBase
{
public:
    CAT21(nlohmann::json cat_def, const char *data);
    virtual ~CAT21() override;
    // Decode Data Network(TCP/IP & UDP)
    virtual bool decodeData() override;

    virtual void to_json(nlohmann::json &j);
    virtual void from_json(Cat21Record &);

private:
    virtual size_t decodeHeader() override;

    CATMessageBase::MessageHeader header_info;
    size_t parsedBytes{0};

private:          
    std::vector<std::string> CAT21_uap_order{"010","040","030","130","080","140","090","FX","210","230","145","150","151","152","155","FX","157","160","165","170","095","032","200","FX","020","220","146","148","110","070","131","FX","-","-","-","-","-","RE","SP","FX"};
    std::map<std::string, int> CAT21_items_order{{"010", 0}, {"020", 1}, {"030", 2}, {"032", 3}, {"040", 4}, {"070", 5}, {"080", 6}, {"090", 7}, {"095", 8}, {"110", 9}, {"130", 10}, {"131", 11}, {"140", 12}, {"145", 13}, {"146", 14}, {"148", 15}, {"150", 16}, {"151", 17}, {"152", 18}, {"155", 19}, {"157", 20}, {"160", 21}, {"165", 22}, {"170", 23}, {"200", 24}, {"210", 25},{"220", 26},{"230", 27}};

    nlohmann::json cat21_json;
    Cat21Record cat21_message;
};