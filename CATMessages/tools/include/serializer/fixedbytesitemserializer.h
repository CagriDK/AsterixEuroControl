#pragma once

#include "itemserializerbase.h"

class FixedBytesItemSerializer : public ItemSerializerBase
{
  public:
    FixedBytesItemSerializer(const nlohmann::json& item_definition);
    virtual ~FixedBytesItemSerializer() {}

    virtual void serializeItem(nlohmann::json &jData, size_t index, size_t size,
                               size_t current_parsed_bytes, std::vector<char> &target, 
                               bool debug) override;

  public:
    size_t length_{0};
    std::string data_type_;
    bool reverse_bits_{false};
    bool reverse_bytes_{false};
    unsigned int negative_bit_pos_{0};
    bool has_lsb_{false};
    double lsb_{1.0};
    size_t data_uint{0};
    int data_int{0};
};
