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

#include <cassert>
#include <exception>
#include <iomanip>
#include <iostream>
#include <string>
#include <algorithm>

#include "../../include/parser/itemparser.h"
#include "../../include/parser/extendablebitsitemparser.h"
#include "../../include/parser/compounditemparser.h"
#include "../../include/parser/fixedbitfielditemparser.h"
#include "../../include/parser/fixedbytesitemparser.h"
#include "../../include/parser/fixedbitsitemparser.h"
#include "../../include/parser/optionalitemparser.h"
#include "../../include/parser/dynamicbytesitemparser.h"
#include "../../include/parser/extendableitemparser.h"
#include "../../include/parser/repetetiveitemparser.h"
#include "../../include/parser/skipbytesitemparser.h"

using namespace std;
using namespace nlohmann;

ItemParserBase::ItemParserBase(const nlohmann::json &item_definition)
    : item_definition_(item_definition)
{
    if (!item_definition.contains("name"))
        throw runtime_error("item construction without JSON name definition");

    name_ = item_definition.at("name");

    if (!item_definition.contains("type"))
        throw runtime_error("item '" + name_ + "' construction without data type definition");

    type_ = item_definition.at("type");
}

ItemParserBase *ItemParserBase::createItemParser(const nlohmann::json &item_definition)
{
    if (!item_definition.contains("name"))
        throw runtime_error("item creation without JSON name definition");

    std::string name = item_definition.at("name");

    if (!item_definition.contains("type"))
        throw runtime_error("item '" + name + "' creation without data type definition");

    std::string type = item_definition.at("type");

    if (type == "item")
    {
        return new ItemParser(item_definition);
    }
    else if (type == "skip_bytes")
    {
        return new SkipBytesItemParser(item_definition);
    }
    else if (type == "extendable_bits")
    {
        return new ExtendableBitsItemParser(item_definition);
    }
    else if (type == "extendable")
    {
        return new ExtendableItemParser(item_definition);
    }
    if (type == "extendable_bits")
    {
        return new ExtendableBitsItemParser(item_definition);
    }
    else if (type == "fixed_bytes")
    {
        return new FixedBytesItemParser(item_definition);
    }
    else if (type == "compound")
    {
        return new CompoundItemParser(item_definition);
    }
    else if (type == "fixed_bitfield")
    {
        return new FixedBitFieldItemParser(item_definition);
    }
    else if (type == "optional_item")
    {
        return new OptionalItemParser(item_definition);
    }
    else if (type == "dynamic_bytes")
    {
        return new DynamicBytesItemParser(item_definition);
    }
    else if (type == "repetitive")
    {
        return new RepetetiveItemParser(item_definition);
    }
    else
        throw runtime_error("item creation name '" + name + "' with unknown type '" + type + "'");
}

std::string ItemParserBase::name() const { return name_; }

std::string ItemParserBase::type() const { return type_; }

bool variableHasValue(const nlohmann::json &data,
                      const std::vector<std::string> &variable_name_parts,
                      const nlohmann::json &variable_value)
{
    const nlohmann::json *val_ptr = &data;
    // std::vector <std::string> sub_keys = split(variable_name, '.');
    for (const std::string &sub_key : variable_name_parts)
    {
        if (val_ptr->contains(sub_key))
        {
            if (sub_key == variable_name_parts.back()) // last found
            {
                val_ptr = &val_ptr->at(sub_key);
                break;
            }

            if (val_ptr->at(sub_key).is_object()) // not last, step in
                val_ptr = &val_ptr->at(sub_key);
            else // not last key, and not object
            {
                val_ptr = nullptr;
                break;
            }
        }
        else // not found
        {
            val_ptr = nullptr;
            break;
        }
    }

    if (val_ptr == nullptr || *val_ptr == nullptr) // not found
        return false;
    else
        return *val_ptr == variable_value;
}
