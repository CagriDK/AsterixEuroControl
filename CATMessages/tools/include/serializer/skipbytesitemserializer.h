#pragma once

#include "itemserializerbase.h"

// skips fixed number of bytes
class SkipBytesItemSerializer : public ItemSerializerBase
{
  public:
    SkipBytesItemSerializer(const nlohmann::json& item_definition);
    virtual ~SkipBytesItemSerializer() {}

    virtual void serializeItem(nlohmann::json &jData, size_t index, size_t size,
                               size_t current_parsed_bytes, std::vector<char> &target, 
                               bool debug) override;

  protected:
    unsigned int length_{0};
};

