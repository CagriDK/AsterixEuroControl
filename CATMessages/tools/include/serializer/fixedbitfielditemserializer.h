#pragma once

#include <memory>
#include <vector>

#include "itemserializerbase.h"

class FixedBitFieldItemSerializer : public ItemSerializerBase
{
  public:
    FixedBitFieldItemSerializer(const nlohmann::json& item_definition);
    virtual ~FixedBitFieldItemSerializer() {}

    virtual void serializeItem(nlohmann::json &jData, size_t index, size_t size,
                               size_t current_parsed_bytes, std::vector<char> &target, 
                               bool debug) override;

  std::vector<int> bitfield;
  protected:
    bool optional_{false};
    std::string optional_variable_name_;
    std::vector<std::string> optional_variable_name_parts_;
    nlohmann::json optional_variable_value_;
    size_t length_;  // byte length
    std::vector<std::unique_ptr<ItemSerializerBase>> items_;

};

