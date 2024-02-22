#pragma once
#include <memory>
#include <vector>

#include "itemserializerbase.h"

// parses item into array, extend into next has to signified by bool extend
class ExtendableItemSerializer : public ItemSerializerBase
{
  public:
    ExtendableItemSerializer(const nlohmann::json& item_definition);
    virtual ~ExtendableItemSerializer() {}

    virtual void serializeItem(nlohmann::json &jData, size_t index, size_t size,
                               size_t current_parsed_bytes, std::vector<char> &target, 
                               bool debug) override;

  protected:
    std::vector<std::unique_ptr<ItemSerializerBase>> items_;
};
