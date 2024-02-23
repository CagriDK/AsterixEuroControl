#pragma once

#include <cstddef>
#include <sstream>
#include <bitset>
#include <cassert>
#include <exception>
#include <string>

#include "../iteminfo.h"

#include "../../../../lib/json.hpp"

class ItemSerializerBase
{
public:
    ItemSerializerBase(const nlohmann::json &item_definition);
    virtual ~ItemSerializerBase() {}

    static ItemSerializerBase *createItemSerializer(const nlohmann::json &item_definition);

    virtual void serializeItem(nlohmann::json &jData, size_t index, size_t size,
                               size_t current_parsed_bytes, std::vector<char> &target,
                               bool debug) = 0;

    std::string name() const;
    std::string longNamePrefix() const;
    std::string longName() const;
    std::string type() const;

public:
    const nlohmann::json &item_definition_;
    std::string name_;
    std::string type_;

    bool variableHasValue(const nlohmann::json &data,
                          const std::vector<std::string> &variable_name_parts,
                          const nlohmann::json &variable_value);

    inline unsigned char reverseBits(unsigned char b)
    {
        b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
        b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
        b = (b & 0xAA) >> 1 | (b & 0x55) << 1;

        return b;
    }

    inline bool isASCII(const std::string &str)
    {
        for (char c : str)
        {
            if (static_cast<unsigned char>(c) > 127)
            {
                return false;
            }
        }
        return true;
    }

    inline std::vector<std::string> split(const std::string &s, char delim)
    {
        std::vector<std::string> elems;
        std::stringstream ss(s);
        std::string item;
        while (std::getline(ss, item, delim))
        {
            elems.push_back(item);
        }
        return elems;
    }
};
