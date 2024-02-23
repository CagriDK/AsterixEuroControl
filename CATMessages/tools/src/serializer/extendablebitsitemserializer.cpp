#include "../../include/serializer/extendablebitsitemserializer.h"

#include <algorithm>

using namespace std;
using namespace nlohmann;

ExtendableBitsItemSerializer::ExtendableBitsItemSerializer(const nlohmann::json& item_definition)
    : ItemSerializerBase(item_definition)
{
    assert(type_ == "extendable_bits");

    if (!item_definition.contains("data_type"))
        throw runtime_error("extendable bits item '" + name_ + "' parsing without data type");

    data_type_ = item_definition.at("data_type");

    reverse_bits_ =
        (item_definition.contains("reverse_bits") && item_definition.at("reverse_bits") == true);

    reverse_order_ =
        (item_definition.contains("reverse_order") && item_definition.at("reverse_order") == true);
}

void ExtendableBitsItemSerializer::serializeItem(nlohmann::json &jData, size_t index, size_t size,
                               size_t current_parsed_bytes, std::vector<char> &target, 
                               bool debug) 
{
    json current_data = jData;
    std::vector<unsigned char> temp_target;

    if(data_type_ == "bitfield")
    {
        bool value = true;
        std::vector<bool> available = current_data[name_];
        unsigned int parsed_bytes = 1;

        if(reverse_order_)
        {
            std::reverse(available.begin(), available.end());
        }

        while(value != false)
        {
            unsigned char convertedByte = 0;
            std::vector<bool> tempVec;

            for(size_t cnt{0}; cnt < 8; ++cnt)
            {
                tempVec.push_back(available[cnt] * parsed_bytes);
            }

            if(reverse_bits_)
            {

            }
            else
            {
                std::transform(tempVec.begin(),tempVec.end(),tempVec.begin(), [](bool val) {return !val;});
            }

            for(size_t cnt{0}; cnt < tempVec.size(); ++cnt)
            {
                if(tempVec[cnt])
                {
                    convertedByte |= 1 << (7 - cnt);
                }
            }

            if(reverse_order_)
            {
                value = tempVec[(parsed_bytes -1) * 8];
            }
            else{ 
                value = tempVec[(8 * parsed_bytes) - 1];
            }
        
            ++parsed_bytes;
            target.push_back(convertedByte);
        }

        //{[0] 1 0 0 1 0 0 1} => LSB FSPEC biti value check ilk bit yada son byteın -7 değerinden baslayacak
    }
    else
    {
        throw runtime_error("extendable bits item '" + name_ + "' parsing without data type '" + data_type_ + "'");
    }
}