#include "../../include/serializer/extendablebitsitemserializer.h"

#include <algorithm>

using namespace std;
using namespace nlohmann;

ExtendableBitsItemSerializer::ExtendableBitsItemSerializer(const nlohmann::json& item_definition)
    : ItemSerializerBase(item_definition)
{
    assert(type_ == "extendable_bits");

    if (!item_definition.contains("data_type"))
        throw runtime_error("extendable bits item '" + name_ + "' parsing without data type");

    data_type_ = item_definition.at("data_type");

    reverse_bits_ =
        (item_definition.contains("reverse_bits") && item_definition.at("reverse_bits") == true);

    reverse_order_ =
        (item_definition.contains("reverse_order") && item_definition.at("reverse_order") == true);
}

void ExtendableBitsItemSerializer::serializeItem(nlohmann::json &jData, size_t index, size_t size,
                               size_t current_parsed_bytes, std::vector<char> &target, 
                               bool debug) 
{

}