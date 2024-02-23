#include "../../include/serializer/extendableitemserializer.h"

using namespace std;
using namespace nlohmann;

ExtendableItemSerializer::ExtendableItemSerializer(const nlohmann::json& item_definition)
    : ItemSerializerBase(item_definition)
{
    assert(type_ == "extendable");

    if (!item_definition.contains("items"))
        throw runtime_error("parsing extendable item '" + name_ + "' without items");

    const json& items = item_definition.at("items");

    if (!items.is_array())
        throw runtime_error("parsing extendable item '" + name_ +
                            "' items specification is not an array");

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

void ExtendableItemSerializer::serializeItem(nlohmann::json &jData, size_t index, size_t size,
                               size_t current_parsed_bytes, std::vector<char> &target, 
                               bool debug) 
{
    unsigned int extend = 1;
    unsigned cnt = 0;
    for(auto& df_item : items_)
    {
        df_item->serializeItem(jData, 0, 0, 0, target, debug);
    }
}