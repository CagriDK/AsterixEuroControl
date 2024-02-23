#include "../../include/serializer/compounditemserializer.h"

using namespace std;
using namespace nlohmann;

CompoundItemSerializer::CompoundItemSerializer(const nlohmann::json& item_definition)
    : ItemSerializerBase(item_definition)
{
    assert(type_ == "compound");

    if (!item_definition.contains("field_specification"))
        throw runtime_error("compound item '" + name_ + "' parsing without field specification");

    const json& field_specification = item_definition.at("field_specification");

    if (!field_specification.is_object())
        throw runtime_error("parsing compound item '" + name_ +
                            "' field specification is not an object");

    field_specification_.reset(ItemSerializerBase::createItemSerializer(field_specification));
    assert(field_specification_);

    if (!item_definition.contains("items"))
        throw runtime_error("parsing compound item '" + name_ + "' without items");

    const json& items = item_definition.at("items");

    if (!items.is_array())
        throw runtime_error("parsing compound item '" + name_ +
                            "' field specification is not an array");

    std::string item_name;
    ItemSerializerBase* item{nullptr};

    for (const json& data_item_it : items)
    {
        item_name = data_item_it.at("name");
        item = ItemSerializerBase::createItemSerializer(data_item_it); // leave out own name
        assert(item);
        items_.push_back(std::unique_ptr<ItemSerializerBase>{item});
    }
}

void CompoundItemSerializer::serializeItem(nlohmann::json &jData, size_t index, size_t size,
                               size_t current_parsed_bytes, std::vector<char> &target, 
                               bool debug) 
{
    field_specification_->serializeItem(jData, 0, 0, 0, target, debug);

    for(auto& df_item : items_)
    {
        df_item->serializeItem(jData, 0, 0, 0, target, debug);
    }
}
