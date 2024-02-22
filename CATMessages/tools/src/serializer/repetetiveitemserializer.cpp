#include "../../include/serializer/repetetiveitemserializer.h"

using namespace std;
using namespace nlohmann;

RepetetiveItemSerializer::RepetetiveItemSerializer(const nlohmann::json& item_definition)
    : ItemSerializerBase(item_definition)
{
    assert(type_ == "repetitive");

    if (!item_definition.contains("repetition_item"))
        throw runtime_error("repetitive item '" + name_ +
                            "' parsing without repetition item specification");

    const json& repetition_item = item_definition.at("repetition_item");

    if (!repetition_item.is_object())
        throw runtime_error("parsing repetitive item '" + name_ +
                            "' repetition item specification is not an object");

    if (repetition_item.at("name") != "REP")
        throw runtime_error("parsing repetitive item '" + name_ +
                            "' repetition item specification has to be named 'REP'");

    repetition_item_.reset(ItemSerializerBase::createItemSerializer(repetition_item));
    assert(repetition_item_);

    if (!item_definition.contains("items"))
        throw runtime_error("parsing repetitive item '" + name_ + "' without items");

    const json& items = item_definition.at("items");

    if (!items.is_array())
        throw runtime_error("parsing repetitive item '" + name_ +
                            "' items specification is not an array");

    std::string item_name;
    ItemSerializerBase* item{nullptr};

    for (const json& data_item_it : items)
    {
        item_name = data_item_it.at("name");
        item = ItemSerializerBase::createItemSerializer(data_item_it);
        assert(item);
        items_.push_back(std::unique_ptr<ItemSerializerBase>{item});
    }
}

void RepetetiveItemSerializer::serializeItem(nlohmann::json &jData, size_t index, size_t size,
                               size_t current_parsed_bytes, std::vector<char> &target, 
                               bool debug) 
{

}