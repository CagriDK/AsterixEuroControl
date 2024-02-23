#pragma once

#include "itemserializerbase.h"

class FixedBitsItemSerializer: public ItemSerializerBase
{
  public:
    FixedBitsItemSerializer(const nlohmann::json& item_definition,unsigned int byte_length);
    virtual ~FixedBitsItemSerializer() {}

    virtual void serializeItem(nlohmann::json &jData, size_t index, size_t size,
                               size_t current_parsed_bytes, std::vector<char> &target, 
                               bool debug) override;


  unsigned int bit_value{0};
  unsigned int byte_length_{0};
  unsigned int start_bit_{0};
  unsigned int bit_length_{0};
  std::vector<bool> vecData;

  unsigned char parsedChar{0};
  int parsedInt{0};
  size_t parsedDigits{0};
  std::string parsedString{};

  protected:
    std::string data_type_{"uint"};
    //unsigned int negative_bit_pos_{0};
    bool has_lsb_{false};
    double lsb_{1.0};

    unsigned char bitmask1;
    unsigned int bitmask4;
    size_t bitmask8;

    unsigned char neg_bitmask1;
    unsigned int neg_bitmask4;
    size_t neg_bitmask8;

    // data type digits stuff
    unsigned int num_digits_{0};
    unsigned int digit_bit_length_{0};

    std::vector<unsigned char> digits_bitmasks1;
    std::vector<unsigned int> digits_bitmasks4;
    std::vector<size_t> digits_bitmasks8;

    // data type characters stuff

    unsigned int num_characters_{0};
    unsigned int character_bit_length_{0};

    std::vector<unsigned char> chars_bitmasks1;
    std::vector<unsigned int> chars_bitmasks4;
    std::vector<size_t> chars_bitmasks8;
};
