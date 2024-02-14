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
    void to_json(nlohmann::json &j);
    template <typename T>
    void from_json(T& record);

private:
    // Decode First 4 Bytes (FSPEC)
    virtual size_t decodeHeader() = 0;

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