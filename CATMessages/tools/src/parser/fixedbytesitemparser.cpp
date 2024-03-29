/*
 * This file is part of ATSDB.
 *
 * ATSDB is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ATSDB is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with ATSDB.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "../../include/parser/fixedbytesitemparser.h"
#include <iostream>
#include "../../include/string_conv.h"

using namespace nlohmann;

FixedBytesItemParser::FixedBytesItemParser(const nlohmann::json& item_definition)
    : ItemParserBase(item_definition)
{
    assert(type_ == "fixed_bytes");

    if (!item_definition.contains("length"))
        std::cerr<<"fixed bytes item '" + name_ + "' parsing without length\n";

    length_ = item_definition.at("length");

    if (!item_definition.contains("data_type"))
        std::cerr<<"fixed bytes item '" + name_ + "' parsing without data type\n";

    data_type_ = item_definition.at("data_type");

    reverse_bits_ =
        (item_definition.contains("reverse_bits") && item_definition.at("reverse_bits") == true);

    reverse_bytes_ =
        (item_definition.contains("reverse_bytes") && item_definition.at("reverse_bytes") == true);

    negative_bit_pos_ = length_ * 8 - 1;

    if (item_definition.contains("lsb"))
    {
        has_lsb_ = true;
        lsb_ = item_definition.at("lsb");
    }
};

size_t FixedBytesItemParser::parseItem(const char* data, size_t index, size_t size,
                                       size_t current_parsed_bytes, nlohmann::json& target,
                                       bool debug)
{

    unsigned char tmp{0};
    data_uint = 0;
    
    const char* current_data = &data[index];

    if (data_type_ == "string")
    {
        std::string data_str(reinterpret_cast<char const*>(current_data),
                             length_ - 1);  // -1 to account for end 0

        //assert(!target.contains(name_));
        target.emplace(name_, std::move(data_str));

        return length_;
    }
    else if (data_type_ == "uint")
    {
        if (reverse_bytes_)
        {
            for (int cnt = length_ - 1; cnt >= 0; --cnt)
            {
                tmp = *reinterpret_cast<const unsigned char*>(&current_data[cnt]);

                if (reverse_bits_)
                    reverseBits(tmp);

                data_uint = (data_uint << 8) + tmp;
            }
        }
        else
        {
            for (size_t cnt = 0; cnt < length_; ++cnt)
            {
                tmp = *reinterpret_cast<const unsigned char*>(&current_data[cnt]);

                if (reverse_bits_)
                    reverseBits(tmp);

                data_uint = (data_uint << 8) + tmp;
            }
        }

        if (has_lsb_)
            target.emplace(name_, lsb_ * data_uint);
        else
            target.emplace(name_, data_uint);

        return length_;
    }
    else if (data_type_ == "int")
    {
        if (reverse_bytes_)
        {
            for (int cnt = length_ - 1; cnt >= 0; --cnt)
            {
                tmp = *reinterpret_cast<const unsigned char*>(&current_data[cnt]);

                if (reverse_bits_)
                    reverseBits(tmp);

                data_uint = (data_uint << 8) + tmp;
            }
        }
        else
        {
            for (size_t cnt = 0; cnt < length_; ++cnt)
            {
                tmp = *reinterpret_cast<const unsigned char*>(&current_data[cnt]);

                if (reverse_bits_)
                    reverseBits(tmp);

                data_uint = (data_uint << 8) + tmp;
            }
        }

        if ((data_uint & (1 << negative_bit_pos_)) != 0)
            data_int = data_uint | ~((1 << negative_bit_pos_) - 1);
        else
            data_int = data_uint;

        if (has_lsb_)
            target.emplace(name_, lsb_ * data_int);
        else
            target.emplace(name_, data_int);

        return length_;
    }
    else if (data_type_ == "bin")
    {
        std::string data_str =
            binary2hex(reinterpret_cast<const unsigned char*>(current_data), length_);

        target.emplace(name_, std::move(data_str));

        return length_;
    }

    return 0;
};
