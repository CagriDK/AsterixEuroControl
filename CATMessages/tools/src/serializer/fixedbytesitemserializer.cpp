#include "../../include/serializer/fixedbytesitemserializer.h"
#include <iostream>
#include "../../include/string_conv.h"

using namespace nlohmann;

FixedBytesItemSerializer::FixedBytesItemSerializer(const nlohmann::json& item_definition)
    : ItemSerializerBase(item_definition)
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

void FixedBytesItemSerializer::serializeItem(nlohmann::json &jData, size_t index, size_t size,
                               size_t current_parsed_bytes, std::vector<char> &target, 
                               bool debug) 
{

}