#pragma once

#include "itemserializerbase.h"

// parses all bits per byte into array<bool>, the last of each byte signifying the extension into
// next byte
class ExtendableBitsItemSerializer : public ItemSerializerBase
{
  public:
    ExtendableBitsItemSerializer(const nlohmann::json& item_definition);
    virtual ~ExtendableBitsItemSerializer() {}

    virtual void serializeItem(nlohmann::json &jData, size_t index, size_t size,
                               size_t current_parsed_bytes, std::vector<char> &target, 
                               bool debug) override;

  public:
    std::vector<bool> bitfield;
    std::string data_type_;
    bool reverse_bits_{false};
    bool reverse_order_{false};
};
