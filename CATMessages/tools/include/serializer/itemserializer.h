#pragma once

#include "itemserializerbase.h"

class ItemSerializer : public ItemSerializerBase
{
  public:
    ItemSerializer(const nlohmann::json& item_definition);
    virtual ~ItemSerializer() {}

    virtual void serializeItem(nlohmann::json &jData, size_t index, size_t size,
                               size_t current_parsed_bytes, std::vector<char> &target, 
                               bool debug) override;
    std::string number() const;
    
protected:
    std::string number_;
    std::vector<std::unique_ptr<ItemSerializerBase>> data_fields_;
};
