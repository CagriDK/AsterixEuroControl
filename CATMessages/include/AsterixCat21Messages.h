#pragma once

#include "../AsterixCatMessageBase.h"
#include "../categories-types/cat21Message.h"

class AsterixCat21Messages : public AsterixCatMessageBase
{
public:
    AsterixCat21Messages();
    virtual ~AsterixCat21Messages() override;
    // Decode Data Network(TCP/IP & UDP)
    bool decodeData(const char *data, json &cat21_json_data, json &cat21_definition,bool use_sample, json &cat21_sample);

    void to_json(nlohmann::json &j);
    void from_json(Cat21Record &);

private:
    std::vector<std::string> cat21_uap_order{"010", "040", "030", "130", "080", "140", "090", "FX", "210", "230", "145", "150", "151", "152", "155", "FX", "157", "160", "165", "170", "095", "032", "200", "FX", "020", "220", "146", "148", "110", "070", "131", "FX", "-", "-", "-", "-", "-", "RE", "SP", "FX"};
    std::map<std::string, int> cat21_items_order{{"010", 0}, {"020", 1}, {"030", 2}, {"032", 3}, {"040", 4}, {"070", 5}, {"080", 6}, {"090", 7}, {"095", 8}, {"110", 9}, {"130", 10}, {"131", 11}, {"140", 12}, {"145", 13}, {"146", 14}, {"148", 15}, {"150", 16}, {"151", 17}, {"152", 18}, {"155", 19}, {"157", 20}, {"160", 21}, {"165", 22}, {"170", 23}, {"200", 24}, {"210", 25}, {"220", 26}, {"230", 27}};
};