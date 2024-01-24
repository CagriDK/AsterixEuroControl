#pragma once

#include <string>
#include "../lib/json.hpp"
#include <fstream>
#include <vector>
#include <functional>
#include "items/string_conv.h"
#include "items/itemparser.h"
#include "items/extendablebitsitemparser.h"
#include "items/compounditemparser.h"
#include "items/fixedbitfielditemparser.h"
#include "items/fixedbytesitemparser.h"
#include "items/fixedbitsitemparser.h"
#include "items/optionalitemparser.h"

class CATMessageBase
{

public:
    CATMessageBase();
    virtual ~CATMessageBase();

    // Decode Data Network(TCP/IP & UDP)
    virtual bool decodeData() = 0;
    virtual void to_json(nlohmann::json &j) = 0;
    // virtual void from_json() = 0;

private:
    // Decode First 4 Bytes (FSPEC)
    virtual size_t decodeHeader() = 0;
    virtual void copy_to_Json(nlohmann::json &j, nlohmann::json &cat_Map, std::string key) = 0;

protected:

    struct MessageHeader
    {
        int cat_type;
        int message_len;
        std::vector<std::string> uap_list;
    };

    const char *m_data;
    nlohmann::json m_cat_definition{""};
    nlohmann::json mapping;
};