#include "../../include/serializer/fixedbitsitemserializer.h"

#include <algorithm>
#include <iostream>
#include "../../include/string_conv.h"

using namespace std;
using namespace nlohmann;

//#define debug_ name_=="Last Measured Mode C Code"

FixedBitsItemSerializer::FixedBitsItemSerializer(const nlohmann::json& item_definition,unsigned int byte_length)
    : ItemSerializerBase(item_definition), byte_length_(byte_length)
{
    assert(type_ == "fixed_bits");

    if (!item_definition.contains("start_bit"))
        throw runtime_error("fixed byte bitfield item '" + name_ + "' without start bit");

    start_bit_ = item_definition.at("start_bit");

    if (!item_definition.contains("bit_length"))
        throw runtime_error("fixed byte bitfield item '" + name_ + "' without bit length");

    bit_length_ = item_definition.at("bit_length");

    if (bit_length_ == 0)
        throw runtime_error("fixed byte bitfield item '" + name_ + "' with 0 bit length");

    if (start_bit_ + bit_length_ > byte_length * 8)
        throw runtime_error("fixed byte bitfield item '" + name_ + "' wrong length " +
                            to_string(byte_length * 8) + " for bitsize " +
                            to_string(start_bit_ + bit_length_));

    if (item_definition.contains("data_type"))
        data_type_ = item_definition.at("data_type");

    if (item_definition.contains("lsb"))
    {
        has_lsb_ = true;
        lsb_ = item_definition.at("lsb");
    }

    if (data_type_ == "int")
    {
        //negative_bit_pos_ = start_bit_ + bit_length_ - 1;

        if (byte_length_ == 1)
        {
            neg_bitmask1 = 1 << (bit_length_ - 1);
        }
        else if (byte_length_ <= 4)
        {
            neg_bitmask4 = 1 << (bit_length_ - 1);
        }
        else if (byte_length_ <= 8)
        {
            neg_bitmask8 = 1 << (bit_length_ - 1);
        }
    }

    if (data_type_ == "uint" || data_type_ == "int")
    {
        if (byte_length_ == 1)
        {
            bitmask1 = 1;
            for (unsigned cnt = 0; cnt < bit_length_ - 1; ++cnt)
            {
                bitmask1 <<= 1;
                bitmask1 += 1;
            }
            bitmask1 <<= start_bit_;
        }
        else if (byte_length_ <= 4)
        {
            bitmask4 = 1;
            for (unsigned cnt = 0; cnt < bit_length_ - 1; ++cnt)
            {
                bitmask4 <<= 1;
                bitmask4 += 1;
            }
            bitmask4 <<= start_bit_;
        }
        else if (byte_length_ <= 8)
        {
            bitmask8 = 1;
            for (unsigned cnt = 0; cnt < bit_length_ - 1; ++cnt)
            {
                bitmask8 <<= 1;
                bitmask8 += 1;
            }
            bitmask8 <<= start_bit_;
        }
        else
            throw runtime_error("fixed byte bitfield item '" + name_ + "' with length" +
                                to_string(byte_length_));
    }
    else if (data_type_ == "digits")
    {
        if (!item_definition.contains("num_digits"))
            throw runtime_error("fixed byte bitfield item '" + name_ +
                                "' data type digits without number of digits");

        num_digits_ = item_definition.at("num_digits");

        if (!item_definition.contains("digit_bit_length"))
            throw runtime_error("fixed byte bitfield item '" + name_ +
                                "' data type digits without digit bit length");

        digit_bit_length_ = item_definition.at("digit_bit_length");

        if (start_bit_ + num_digits_ * digit_bit_length_ > byte_length * 8)
            throw runtime_error("fixed byte bitfield item '" + name_ + "' wrong length " +
                                to_string(byte_length * 8) + " for digits bitsize " +
                                to_string(start_bit_ + num_digits_ * digit_bit_length_));
        if (byte_length_ == 1)
        {
            bitmask1 = 1;
            for (unsigned cnt = 0; cnt < digit_bit_length_ - 1; ++cnt)
            {
                bitmask1 <<= 1;
                bitmask1 += 1;
            }
            bitmask1 <<= start_bit_;

            for (unsigned cnt = 0; cnt < num_digits_; ++cnt)
            {
                digits_bitmasks1.push_back(bitmask1);
                bitmask1 <<= digit_bit_length_;
            }

            bitmask1 = 0;
            assert(digits_bitmasks1.size() == num_digits_);
        }
        else if (byte_length_ <= 4)
        {
            bitmask4 = 1;
            for (unsigned cnt = 0; cnt < digit_bit_length_ - 1; ++cnt)
            {
                bitmask4 <<= 1;
                bitmask4 += 1;
            }

            bitmask4 <<= start_bit_;

            for (unsigned cnt = 0; cnt < num_digits_; ++cnt)
            {
                digits_bitmasks4.push_back(bitmask4);
                bitmask4 <<= digit_bit_length_;
            }
            // reverse(digits_bitmasks4.begin(), digits_bitmasks4.end());

            bitmask4 = 0;
            assert(digits_bitmasks4.size() == num_digits_);
        }
        else if (byte_length_ <= 8)
        {
            bitmask8 = 1;
            for (unsigned cnt = 0; cnt < digit_bit_length_ - 1; ++cnt)
            {
                bitmask8 <<= 1;
                bitmask8 += 1;
            }

            bitmask8 <<= start_bit_;

            for (unsigned cnt = 0; cnt < num_digits_; ++cnt)
            {
                digits_bitmasks8.push_back(bitmask8);
                bitmask8 <<= digit_bit_length_;
            }
            // reverse(digits_bitmasks8.begin(), digits_bitmasks8.end());

            bitmask8 = 0;
            assert(digits_bitmasks8.size() == num_digits_);
        }
        else
            throw runtime_error("fixed byte bitfield item '" + name_ + "' with length" +
                                to_string(byte_length_));
    }
    else if (data_type_ == "icao_characters" || data_type_ == "ascii_characters")
    {
        if (!item_definition.contains("num_characters"))
            throw runtime_error("fixed byte bitfield item '" + name_ +
                                "' data type characters without number of characters");

        num_characters_ = item_definition.at("num_characters");

        if (!item_definition.contains("character_bit_length"))
            throw runtime_error("fixed byte bitfield item '" + name_ +
                                "' data type characters without characters bit length");

        character_bit_length_ = item_definition.at("character_bit_length");

        if (start_bit_ + num_digits_ * digit_bit_length_ > byte_length * 8)
            throw runtime_error("fixed byte bitfield item '" + name_ + "' wrong length " +
                                to_string(byte_length * 8) + " for digits bitsize " +
                                to_string(start_bit_ + num_digits_ * digit_bit_length_));

        if (data_type_ == "icao_characters" && character_bit_length_ != 6)
            throw invalid_argument("fixed byte bitfield item '" + name_ +
                                   "' wrong icao character bit length " +
                                   to_string(character_bit_length_));

        if (data_type_ == "ascii_characters" && character_bit_length_ != 8)
            throw invalid_argument("fixed byte bitfield item '" + name_ +
                                   "' wrong ascii character bit length " +
                                   to_string(character_bit_length_));

        if (byte_length_ == 1)
        {
            bitmask1 = 1;
            for (unsigned cnt = 0; cnt < character_bit_length_ - 1; ++cnt)
            {
                bitmask1 <<= 1;
                bitmask1 += 1;
            }
            bitmask1 <<= start_bit_;

            for (unsigned cnt = 0; cnt < num_characters_; ++cnt)
            {
                chars_bitmasks1.push_back(bitmask1);
                bitmask1 <<= character_bit_length_;
            }

            bitmask1 = 0;
            assert(chars_bitmasks1.size() == num_characters_);
        }
        else if (byte_length_ <= 4)
        {
            bitmask4 = 1;
            for (unsigned cnt = 0; cnt < character_bit_length_ - 1; ++cnt)
            {
                bitmask4 <<= 1;
                bitmask4 += 1;
            }

            bitmask4 <<= start_bit_;

            for (unsigned cnt = 0; cnt < num_characters_; ++cnt)
            {
                chars_bitmasks4.push_back(bitmask4);
                bitmask4 <<= character_bit_length_;
            }

            bitmask4 = 0;
            assert(chars_bitmasks4.size() == num_characters_);
        }
        else if (byte_length_ <= 8)
        {
            bitmask8 = 1;
            for (unsigned cnt = 0; cnt < character_bit_length_ - 1; ++cnt)
            {
                bitmask8 <<= 1;
                bitmask8 += 1;
            }

            bitmask8 <<= start_bit_;

            for (unsigned cnt = 0; cnt < num_characters_; ++cnt)
            {
                chars_bitmasks8.push_back(bitmask8);
                bitmask8 <<= character_bit_length_;
            }

            bitmask8 = 0;
            assert(chars_bitmasks8.size() == num_characters_);
        }
        else
            throw runtime_error("fixed byte bitfield item '" + name_ + "' with length" +
                                to_string(byte_length_));
    }
    else
        throw runtime_error("fixed byte bitfield item '" + name_ + "' with unknown data type '" +
                            data_type_ + "'");
}

void FixedBitsItemSerializer::serializeItem(nlohmann::json &jData, size_t index, size_t size,
                               size_t current_parsed_bytes, std::vector<char> &target, 
                               bool debug) 
{
    json current_data = jData;
    vecData.clear();
    if(data_type_ == "digits")
    {
        std::vector<bool> tempVecData;
        std::string serialized = "";

        int number;
        if(has_lsb_)
        {
            double temp = static_cast<double>(current_data[name_]) / lsb_;
            number = static_cast<int>(temp);
        }
        else
        {
            number = static_cast<int>(current_data[name_]);
        }

        while(num_digits_)
        {
            int digit = number % 10;
            number /= 10;

            std::bitset<3> binaryDigit(digit);
            for(size_t i = 0; i < digit_bit_length_; i++)
            {
                tempVecData.push_back(binaryDigit[i]);
            }
            num_digits_--;
        }
        //std::reverse(tempVecData.begin(), tempVecData.end());
        std::copy(tempVecData.rbegin(), tempVecData.rend(), std::back_inserter(vecData));
    }
    else if(data_type_ == "int" || data_type_ == "uint")
    {
        std::vector<bool> tempVecData;
        if (has_lsb_)
        {
            double tempData = static_cast<double>(jData[name_]) / lsb_;
            bit_value = static_cast<int>(tempData);
        }
        else
        {
            bit_value = static_cast<int>(current_data[name_]);
        }

        for(size_t i = 0; i < bit_length_; i++)
        {
            tempVecData.push_back(bit_value >> i & 1);
        }
        //std::reverse(tempVecData.begin(), tempVecData.end());
        std::copy(tempVecData.rbegin(), tempVecData.rend(), std::back_inserter(vecData));
    }
    else if (data_type_ == "icao_characters" || data_type_ == "ascii_characters")
    {
        std::string str = current_data[name_];
        std::vector<bool> characters_tmp;
        if (data_type_ == "icao_characters")
        {
            characters_tmp = stringToIcaoBitsets(str, character_bit_length_ - 1);
        }
        else
        {
            characters_tmp = stringToAsciiBitsets(str, character_bit_length_ - 1);
        }
        std::reverse(characters_tmp.begin(), characters_tmp.end());
        std::copy(characters_tmp.rbegin(), characters_tmp.rend(), std::back_inserter(vecData));
    }
}