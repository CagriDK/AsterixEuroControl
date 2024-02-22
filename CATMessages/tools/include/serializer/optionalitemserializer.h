#pragma once

#include "itemserializerbase.h"

class OptionalItemSerializer : public ItemSerializerBase
{
  public:
    OptionalItemSerializer(const nlohmann::json& item_definition);
    virtual ~OptionalItemSerializer() {}

    virtual void serializeItem(nlohmann::json &jData, size_t index, size_t size,
                               size_t current_parsed_bytes, std::vector<char> &target, 
                               bool debug) override;

  protected:
    std::string bitfield_name_;
    unsigned int bitfield_index_{0};
    std::vector<std::unique_ptr<ItemSerializerBase>> data_fields_;
};

