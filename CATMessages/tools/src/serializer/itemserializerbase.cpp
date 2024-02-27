#include <cassert>
#include <exception>
#include <iomanip>
#include <iostream>
#include <string>
#include <algorithm>

#include "../../include/serializer/itemserializer.h"
#include "../../include/serializer/extendablebitsitemserializer.h"
#include "../../include/serializer/compounditemserializer.h"
#include "../../include/serializer/fixedbitfielditemserializer.h"
#include "../../include/serializer/fixedbytesitemserializer.h"
#include "../../include/serializer/fixedbitsitemserializer.h"
#include "../../include/serializer/optionalitemserializer.h"
#include "../../include/serializer/dynamicbytesitemserializer.h"
#include "../../include/serializer/extendableitemserializer.h"
#include "../../include/serializer/repetetiveitemserializer.h"
#include "../../include/serializer/skipbytesitemserializer.h"

using namespace std;
using namespace nlohmann;

ItemSerializerBase::ItemSerializerBase(const nlohmann::json &item_definition)
    : item_definition_(item_definition)
{
    if (!item_definition.contains("name"))
        throw runtime_error("item construction without JSON name definition");

    name_ = item_definition.at("name");

    if (!item_definition.contains("type"))
        throw runtime_error("item '" + name_ + "' construction without data type definition");

    type_ = item_definition.at("type");
}

ItemSerializerBase *ItemSerializerBase::createItemSerializer(const nlohmann::json &item_definition)
{
    if (!item_definition.contains("name"))
        throw runtime_error("item creation without JSON name definition");

    std::string name = item_definition.at("name");

    if (!item_definition.contains("type"))
        throw runtime_error("item '" + name + "' creation without data type definition");

    std::string type = item_definition.at("type");

    if (type == "item")
    {
        return new ItemSerializer(item_definition);
    }
    else if (type == "skip_bytes")
    {
        // return new SkipBytesItemSerializer(item_definition);
    }
    else if (type == "extendable_bits")
    {
        return new ExtendableBitsItemSerializer(item_definition);
    }
    else if (type == "extendable")
    {
        // return new ExtendableItemSerializer(item_definition);
    }
    if (type == "extendable_bits")
    {
        return new ExtendableBitsItemSerializer(item_definition);
    }
    else if (type == "fixed_bytes")
    {
        return new FixedBytesItemSerializer(item_definition);
    }
    else if (type == "compound")
    {
        return new CompoundItemSerializer(item_definition);
    }
    else if (type == "fixed_bitfield")
    {
        return new FixedBitFieldItemSerializer(item_definition);
    }
    else if (type == "optional_item")
    {
        return new OptionalItemSerializer(item_definition);
    }
    else if (type == "dynamic_bytes")
    {
        return new DynamicBytesItemSerializer(item_definition);
    }
    else if (type == "repetitive")
    {
        return new RepetetiveItemSerializer(item_definition);
    }
    else
        throw runtime_error("item creation name '" + name + "' with unknown type '" + type + "'");
}

std::string ItemSerializerBase::name() const { return name_; }

std::string ItemSerializerBase::type() const { return type_; }

bool ItemSerializerBase::variableHasValue(const nlohmann::json &data,
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
