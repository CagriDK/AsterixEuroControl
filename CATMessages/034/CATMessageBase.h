#pragma once

#include <string>
#include "../../lib/json.hpp"
#include <fstream>
#include <vector>
#include <functional>
#include "../../src/string_conv.h"
#include "../../src/itemparser.h"
#include "../../src/extendablebitsitemparser.h"
#include "../../src/compounditemparser.h"
#include "../../src/fixedbitfielditemparser.h"
#include "../../src/fixedbytesitemparser.h"
#include "../../src/fixedbitsitemparser.h"
#include "../../src/optionalitemparser.h"

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