#include "../../include/serializer/itemserializer.h"

using namespace std;
using namespace nlohmann;

ItemSerializer::ItemSerializer(const nlohmann::json& item_definition) : ItemSerializerBase(item_definition)
{
    assert(type_ == "item");

    if (!item_definition.contains("number"))
        throw runtime_error("serializing item '" + name_ + "' without number");

    number_ = item_definition.at("number");

    const json& data_fields = item_definition.at("data_fields");

    if (!data_fields.is_array())
        throw runtime_error("serializing item '" + name_ + "' data fields container is not an array");

    std::string item_name;
    std::string item_number;
    ItemSerializerBase* item{nullptr};

    for (const json& data_item_it : data_fields)
    {
        item_name = data_item_it.at("name");

        if (data_item_it.contains("number"))
            item_number = data_item_it.at("number");
        else
            item_number = "";

        item = ItemSerializerBase::createItemSerializer(data_item_it);
        assert(item);
        data_fields_.push_back(std::unique_ptr<ItemSerializerBase>{item});
    }
}

void ItemSerializer::serializeItem(nlohmann::json &jData, size_t index, size_t size,
                               size_t current_parsed_bytes, std::vector<char> &target, 
                               bool debug) 
{
    for(auto &df_item : data_fields_)
    {
        df_item->serializeItem(jData, 0, 0, 0, target, debug);
    }
}

std::string ItemSerializer::number() const { return number_; }

