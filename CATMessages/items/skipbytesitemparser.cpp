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

#include "skipbytesitemparser.h"

using namespace std;
using namespace nlohmann;

SkipBytesItemParser::SkipBytesItemParser(const nlohmann::json& item_definition)
    : ItemParserBase(item_definition)
{
    assert(type_ == "skip_bytes");

    if (!item_definition.contains("length"))
        throw runtime_error("fixed bytes item '" + name_ + "' parsing without length");

    length_ = item_definition.at("length");
}

size_t SkipBytesItemParser::parseItem(const char* data, size_t index, size_t size,
                                      size_t current_parsed_bytes, nlohmann::json& target,
                                      bool debug)
{
    return length_;
}

