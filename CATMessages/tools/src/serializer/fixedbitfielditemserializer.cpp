#include "../../include/serializer/fixedbitfielditemserializer.h"
#include "../../include/serializer/fixedbitsitemserializer.h"

using namespace std;
using namespace nlohmann;

FixedBitFieldItemSerializer::FixedBitFieldItemSerializer(const nlohmann::json &item_definition)
    : ItemSerializerBase(item_definition)
{
    assert(type_ == "fixed_bitfield");

    optional_ = item_definition.contains("optional") && item_definition.at("optional") == true;

    if (optional_)
    {
        if (!item_definition.contains("optional_variable_name"))
            throw runtime_error("parsing fixed bitfield item '" + name_ +
                                "' optional but no variable given");

        optional_variable_name_ = item_definition.at("optional_variable_name");
        optional_variable_name_parts_ = split(optional_variable_name_, '.');

        if (!item_definition.contains("optional_variable_value"))
            throw runtime_error("parsing fixed bitfield item '" + name_ +
                                "' optional but no variable value given");

        optional_variable_value_ = item_definition.at("optional_variable_value");
    }

    if (!item_definition.contains("length"))
        throw runtime_error("parsing fixed bitfield item '" + name_ + "' without length");

    length_ = item_definition.at("length");

    if (length_ > 8)
        throw runtime_error("parsing fixed bitfield item '" + name_ + "' with too big length");

    if (!item_definition.contains("items"))
        throw runtime_error("parsing fixed bitfield item '" + name_ + "' without sub-items");

    const json &items = item_definition.at("items");

    if (!items.is_array())
        throw runtime_error("parsing fixed bitfield item '" + name_ +
                            "' sub-items specification is not an array");

    std::string item_name;
    FixedBitsItemSerializer *item{nullptr};

    for (const json &data_item_it : items)
    {
        item_name = data_item_it.at("name");
        item = new FixedBitsItemSerializer(data_item_it, length_); // leave out own name
        assert(item);
        items_.push_back(std::unique_ptr<ItemSerializerBase>{item});
    }
}

void FixedBitFieldItemSerializer::serializeItem(nlohmann::json &jData, size_t index, size_t size,
                               size_t current_parsed_bytes, std::vector<char> &target, 
                               bool debug) 
{

}