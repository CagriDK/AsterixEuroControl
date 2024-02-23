#include "../../include/serializer/optionalitemserializer.h"

using namespace std;
using namespace nlohmann;

OptionalItemSerializer::OptionalItemSerializer(const nlohmann::json& item_definition)
    : ItemSerializerBase(item_definition)
{
    assert(type_ == "optional_item");

    if (!item_definition.contains("optional_bitfield_name"))
        throw runtime_error("optional item '" + name_ + "' parsing without bitfield name");

    bitfield_name_ = item_definition.at("optional_bitfield_name");

    if (!item_definition.contains("optional_bitfield_index"))
        throw runtime_error("optional item '" + name_ + "' parsing without bitfield index");

    bitfield_index_ = item_definition.at("optional_bitfield_index");

    if (!item_definition.contains("data_fields"))
        throw runtime_error("parsing optional item '" + name_ + "' without sub-items");

    const json& data_fields = item_definition.at("data_fields");

    if (!data_fields.is_array())
        throw runtime_error("parsing optional item '" + name_ +
                            "' data fields container is not an array");

    std::string item_name;
    ItemSerializerBase* item{nullptr};

    for (const json& data_item_it : data_fields)
    {
        item_name = data_item_it.at("name");
        item = ItemSerializerBase::createItemSerializer(data_item_it); // leave out own name
        assert(item);
        data_fields_.push_back(std::unique_ptr<ItemSerializerBase>{item});
    }
}

void OptionalItemSerializer::serializeItem(nlohmann::json &jData, size_t index, size_t size,
                               size_t current_parsed_bytes, std::vector<char> &target, 
                               bool debug) 
{
    if(!jData.contains(bitfield_name_))
    {
        throw runtime_error("parsing optional item '" + name_ + "' without defined bitfield '" + bitfield_name_ + "'");
    }

    if(!jData[bitfield_name_].is_array())
    {
        throw runtime_error("parsing optional item '" + name_ + "' with non-array bitfield '" + bitfield_name_ + "' is not an array");
    }

    if(!jData[bitfield_name_][bitfield_index_].is_boolean())
    {
        throw runtime_error("parsing optional item '" + name_ + "' with non-boolean bitfield '" + bitfield_name_ + "' value");
    }

    bool item_exists = jData[bitfield_name_].at(bitfield_index_);

    if(!item_exists)
    {
        return;
    }

    for(auto& df_item : data_fields_)
    {
        df_item->serializeItem(jData[name_], 0, 0, 0, target, debug);
    }
}