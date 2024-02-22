#pragma once

#include "itemserializerbase.h"

// calculates the index and length based on other decoded variable
class DynamicBytesItemSerializer : public ItemSerializerBase
{
  public:
    DynamicBytesItemSerializer(const nlohmann::json& item_definition);
    virtual ~DynamicBytesItemSerializer() {}

    virtual void serializeItem(nlohmann::json &jData, size_t index, size_t size,
                               size_t current_parsed_bytes, std::vector<char> &target, 
                               bool debug) override;

  protected:
    std::string length_variable_name_;
    bool substract_previous_{false};

    bool has_additative_factor_{false};
    int additative_factor_;
};

