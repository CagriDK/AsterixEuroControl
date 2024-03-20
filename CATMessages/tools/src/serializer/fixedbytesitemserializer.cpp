#include "../../include/serializer/fixedbytesitemserializer.h"
#include <iostream>
#include "../../include/string_conv.h"

using namespace nlohmann;

FixedBytesItemSerializer::FixedBytesItemSerializer(const nlohmann::json& item_definition)
    : ItemSerializerBase(item_definition)
{
    assert(type_ == "fixed_bytes");

    if (!item_definition.contains("length"))
        std::cerr<<"fixed bytes item '" + name_ + "' serializing without length\n";

    length_ = item_definition.at("length");

    if (!item_definition.contains("data_type"))
        std::cerr<<"fixed bytes item '" + name_ + "' serializing without data type\n";

    data_type_ = item_definition.at("data_type");

    reverse_bits_ =
        (item_definition.contains("reverse_bits") && item_definition.at("reverse_bits") == true);

    reverse_bytes_ =
        (item_definition.contains("reverse_bytes") && item_definition.at("reverse_bytes") == true);

    negative_bit_pos_ = length_ * 8 - 1;

    if (item_definition.contains("lsb"))
    {
        has_lsb_ = true;
        lsb_ = item_definition.at("lsb");
    }
};

void FixedBytesItemSerializer::serializeItem(nlohmann::json &jData, size_t index, size_t size,
                               size_t current_parsed_bytes, std::vector<char> &target, 
                               bool debug) 
{
    json current_data = jData;
    std::vector<char> temp_target;

    if(data_type_ == "string")
    {
        std::string value = current_data[name_];
        if(!isASCII(value))
        {
            throw std::runtime_error("fixed bytes item '" + name_ + "' serializing with non-ASCII string");
        }
        for(unsigned char c : value)
        {
            temp_target.push_back(c);
        }
    temp_target.push_back('\0');
    }
    else if(data_type_ == "uint")
    {
        unsigned int value;

        if(has_lsb_)
        {
            float temp = current_data[name_];
            temp = temp / lsb_;
            value = static_cast<unsigned int>(std::round(temp));
        }
        else
        {
            value = static_cast<unsigned int>(current_data[name_]);
        }

        if (value < 0 || value > (std::pow(UINT8_MAX,length_)))
        {
            std::cout<<"Serializing a non proper value for : "+ name_ +" min value = 0 , " + "max value = " + std::to_string(std::pow(UINT8_MAX,length_) * lsb_) <<"\n";
        }

        for(size_t i = 0; i < length_; i++)
        {
            if(reverse_bits_)
            {
                reverseBits(value);
            }
            temp_target.push_back((value >> (i * 8)) & 0xFF);
        }

        if(reverse_bytes_)
        {
            std::reverse(temp_target.begin(), temp_target.end());
        }
    }
    else if(data_type_ == "int")
    {
        int value;
 
        if(has_lsb_)
        {
            float temp = current_data[name_];
            temp = temp / lsb_;
            value = static_cast<int>(std::round(temp));
        }
        else
        {
            value = static_cast<int>(current_data[name_]);
        }

        if (value < -std::pow(std::abs(UINT8_MAX),length_)/2 || value > (std::pow(UINT8_MAX,length_)/2))
        {
            std::cout<<"Serializing a non proper value for : "+ name_ + " min value = " + std::to_string(-std::pow(std::abs(UINT8_MAX),length_)/2 * lsb_)  + " max value = " + std::to_string(std::pow(UINT8_MAX,length_)/2 * lsb_)<<"\n";
        }

        for(size_t i = 0; i < length_; i++)
        {
            if(reverse_bits_)
            {
                reverseBits(value);
            }
            temp_target.push_back((value >> (i * 8)) & 0xFF);
        }
        if (reverse_bytes_)
        {
            std::reverse(temp_target.begin(), temp_target.end());
        }
    }
    else if(data_type_ == "bin")
    {
        std::string value = current_data[name_];
        std::vector<char> bytes = hex2binary(value);
        if (bytes.size() > length_ * 2)
        {
            std::cout<<"Serializing a non proper binary length for : " + name_ + " required length = " << std::to_string(length_) << "\n";
        }

        std::copy(bytes.rbegin(), bytes.rend(), std::back_inserter(temp_target));
    }

    std::reverse(temp_target.begin(), temp_target.end());
    std::copy(temp_target.begin(), temp_target.end(), std::back_inserter(target));
}