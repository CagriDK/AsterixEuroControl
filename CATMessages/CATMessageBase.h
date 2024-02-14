#pragma once

#include <string>
#include "../lib/json.hpp"
#include <fstream>
#include <vector>
#include <functional>
#include "tools/include/string_conv.h"
#include "tools/include/parser/itemparser.h"
#include "tools/include/parser/extendablebitsitemparser.h"
#include "tools/include/parser/compounditemparser.h"
#include "tools/include/parser/fixedbitfielditemparser.h"
#include "tools/include/parser/fixedbytesitemparser.h"
#include "tools/include/parser/fixedbitsitemparser.h"
#include "tools/include/parser/optionalitemparser.h"

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