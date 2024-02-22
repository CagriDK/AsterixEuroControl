#include "../../include/serializer/dynamicbytesitemserializer.h"

using namespace std;
using namespace nlohmann;

DynamicBytesItemSerializer::DynamicBytesItemSerializer(const nlohmann::json& item_definition)
    : ItemSerializerBase(item_definition)
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

void DynamicBytesItemSerializer::serializeItem(nlohmann::json &jData, size_t index, size_t size,
                               size_t current_parsed_bytes, std::vector<char> &target, 
                               bool debug) 
{

}