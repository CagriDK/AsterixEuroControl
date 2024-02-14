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

#include "../../include/parser/extendableitemparser.h"

using namespace std;
using namespace nlohmann;

ExtendableItemParser::ExtendableItemParser(const nlohmann::json& item_definition)
    : ItemParserBase(item_definition)
{
    assert(type_ == "extendable");

    if (!item_definition.contains("items"))
        throw runtime_error("parsing extendable item '" + name_ + "' without items");

    const json& items = item_definition.at("items");

    if (!items.is_array())
        throw runtime_error("parsing extendable item '" + name_ +
                            "' items specification is not an array");

    std::string item_name;
    ItemParserBase* item{nullptr};

    for (const json& data_item_it : items)
    {
        item_name = data_item_it.at("name");
        item = ItemParserBase::createItemParser(data_item_it); // leave out own name
        assert(item);
        items_.push_back(std::unique_ptr<ItemParserBase>{item});
    }
}

size_t ExtendableItemParser::parseItem(const char* data, size_t index, size_t size,
                                       size_t current_parsed_bytes, nlohmann::json& target,
                                       bool debug)
{
    size_t parsed_bytes{0};

    unsigned int extend = 1;
    unsigned int cnt = 0;

    assert(!target.contains(name_));
    target[name_] = json::array();
    json& j_data = target.at(name_);

    while (extend)
    {
        for (auto& data_item_it : items_)
        {
            parsed_bytes += data_item_it->parseItem(data, index + parsed_bytes, size, parsed_bytes,
                                                    j_data[cnt], debug);

            if (debug && !j_data.at(cnt).contains("extend"))
                throw runtime_error("parsing extendable item '" + name_ +
                                    "' without extend information");

            extend = j_data.at(cnt).at("extend");

            ++cnt;
        }
    }

    return parsed_bytes;
}

