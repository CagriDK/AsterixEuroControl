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

#include "dynamicbytesitemparser.h"

using namespace std;
using namespace nlohmann;

DynamicBytesItemParser::DynamicBytesItemParser(const nlohmann::json& item_definition)
    : ItemParserBase(item_definition)
{
    assert(type_ == "dynamic_bytes");

    if (!item_definition.contains("length_variable"))
        throw runtime_error("dynamic bytes item '" + name_ + "' parsing without length variable");

    length_variable_name_ = item_definition.at("length_variable");

    substract_previous_ = item_definition.contains("substract_previous") &&
                          item_definition.at("substract_previous") == true;

    if (item_definition.contains("additative_factor"))
    {
        has_additative_factor_ = true;
        additative_factor_ = item_definition.at("additative_factor");
    }
}

size_t DynamicBytesItemParser::parseItem(const char* data, size_t index, size_t size,
                                         size_t current_parsed_bytes, nlohmann::json& target,
                                         bool debug)
{

    if (debug && !target.contains(length_variable_name_))
        throw runtime_error("dynamic bytes item '" + name_ + "' parsing without given length");

    size_t length = target.at(length_variable_name_);

    //    loginf << "UGA dynamic bytes item '"+name_+"' index " << index << " size " << size << "
    //    length " << length
    //           << " current pb " << current_parsed_bytes << logendl;

    if (substract_previous_)
    {
        if (length < current_parsed_bytes)
        {
            //            loginf << "UGA length " << (has_additative_factor_ ? length -
            //            current_parsed_bytes + additative_factor_ :
            //                                                                length -
            //                                                                current_parsed_bytes)<<
            //                                                                logendl;
            throw runtime_error("dynamic bytes item '" + name_ + "' length " +
                                std::to_string(length) + " < current parsed bytes " +
                                std::to_string(current_parsed_bytes));
        }

        assert(length >= current_parsed_bytes);

        length -= current_parsed_bytes;
    }

    if (has_additative_factor_)
    {

        length += additative_factor_;
    }

    assert(!target.contains(name_));

    target.emplace(name_, json::object({{"index", index}, {"length", length}}));

    return length;
}
