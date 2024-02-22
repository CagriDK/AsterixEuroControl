#pragma once

#include "itemserializerbase.h"

class RepetetiveItemSerializer : public ItemSerializerBase
{
  public:
    RepetetiveItemSerializer(const nlohmann::json& item_definition);
    virtual ~RepetetiveItemSerializer() {}

    virtual void serializeItem(nlohmann::json &jData, size_t index, size_t size,
                               size_t current_parsed_bytes, std::vector<char> &target, 
                               bool debug) override;

  protected:
    std::unique_ptr<ItemSerializerBase> repetition_item_;
    std::vector<std::unique_ptr<ItemSerializerBase>> items_;
};
