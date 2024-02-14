#pragma once

#include "../AsterixCatMessageBase.h"
#include "../categories-types/cat62Message.h"

class AsterixCat62Messages : public AsterixCatMessageBase
{
public:
    AsterixCat62Messages();
    virtual ~AsterixCat62Messages() override;
    // Decode Data Network(TCP/IP & UDP)
    bool decodeData(const char *data, json &cat62_json_data);

    void to_json(nlohmann::json &j);
    void from_json(Cat62Record &);

private:
    std::vector<std::string> cat62_uap_order{"010", "-", "015", "070", "105", "100", "185", "FX", "210", "060", "245", "380", "040", "080", "290", "FX", "200", "295", "136", "130", "135", "220", "390", "FX", "270", "300", "110", "120", "510", "500", "340", "FX", "-", "-", "-", "-", "-", "RE", "SP", "FX"};
    std::map<std::string, int> cat62_items_order{{"010", 0}, {"015", 1}, {"040", 2}, {"060", 3}, {"070", 4}, {"080", 5}, {"100", 6}, {"105", 7}, {"110", 8}, {"120", 9}, {"130", 10}, {"135", 11}, {"136", 12}, {"185", 13}, {"200", 14}, {"210", 15}, {"220", 16}, {"245", 17}, {"270", 18}, {"290", 19}, {"295", 20}, {"300", 21}, {"340", 22}, {"380", 23}, {"390", 24}, {"500", 25}, {"510", 26}};
};