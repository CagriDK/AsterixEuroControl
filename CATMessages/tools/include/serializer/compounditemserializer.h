#pragma once

#include <memory>
#include <vector>

#include "itemserializerbase.h"

class CompoundItemSerializer : public ItemSerializerBase
{
  public:
    CompoundItemSerializer(const nlohmann::json& item_definition);
    virtual ~CompoundItemSerializer() {}

    virtual void serializeItem(nlohmann::json &jData, size_t index, size_t size,
                               size_t current_parsed_bytes, std::vector<char> &target, 
                               bool debug) override;

  protected:
    std::unique_ptr<ItemSerializerBase> field_specification_;
    std::vector<std::unique_ptr<ItemSerializerBase>> items_;
};
