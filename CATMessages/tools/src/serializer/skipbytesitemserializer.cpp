#include "../../include/serializer/skipbytesitemserializer.h"

using namespace std;
using namespace nlohmann;

SkipBytesItemSerializer::SkipBytesItemSerializer(const nlohmann::json& item_definition)
    : ItemSerializerBase(item_definition)
{
    assert(type_ == "skip_bytes");

    if (!item_definition.contains("length"))
        throw runtime_error("fixed bytes item '" + name_ + "' parsing without length");

    length_ = item_definition.at("length");
}

void SkipBytesItemSerializer::serializeItem(nlohmann::json &jData, size_t index, size_t size,
                               size_t current_parsed_bytes, std::vector<char> &target, 
                               bool debug) 
{

}
