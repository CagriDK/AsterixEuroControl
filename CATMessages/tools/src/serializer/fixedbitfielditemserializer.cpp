#include "../../include/serializer/fixedbitfielditemserializer.h"
#include "../../include/serializer/fixedbitsitemserializer.h"

using namespace std;
using namespace nlohmann;

FixedBitFieldItemSerializer::FixedBitFieldItemSerializer(const nlohmann::json &item_definition)
    : ItemSerializerBase(item_definition)
{
    assert(type_ == "fixed_bitfield");

    optional_ = item_definition.contains("optional") && item_definition.at("optional") == true;

    if (optional_)
    {
        if (!item_definition.contains("optional_variable_name"))
            throw runtime_error("serializing fixed bitfield item '" + name_ +
                                "' optional but no variable given");

        optional_variable_name_ = item_definition.at("optional_variable_name");
        optional_variable_name_parts_ = split(optional_variable_name_, '.');

        if (!item_definition.contains("optional_variable_value"))
            throw runtime_error("serializing fixed bitfield item '" + name_ +
                                "' optional but no variable value given");

        optional_variable_value_ = item_definition.at("optional_variable_value");
    }

    if (!item_definition.contains("length"))
        throw runtime_error("serializing fixed bitfield item '" + name_ + "' without length");

    length_ = item_definition.at("length");

    if (length_ > 8)
        throw runtime_error("serializing fixed bitfield item '" + name_ + "' with too big length");

    if (!item_definition.contains("items"))
        throw runtime_error("serializing fixed bitfield item '" + name_ + "' without sub-items");

    const json &items = item_definition.at("items");

    if (!items.is_array())
        throw runtime_error("serializing fixed bitfield item '" + name_ +
                            "' sub-items specification is not an array");

    std::string item_name;
    FixedBitsItemSerializer *item{nullptr};

    for (const json &data_item_it : items)
    {
        item_name = data_item_it.at("name");
        item = new FixedBitsItemSerializer(data_item_it, length_); // leave out own name
        assert(item);
        items_.push_back(std::unique_ptr<ItemSerializerBase>{item});
    }
}

void FixedBitFieldItemSerializer::serializeItem(nlohmann::json &jData, size_t index, size_t size,
                               size_t current_parsed_bytes, std::vector<char> &target, 
                               bool debug) 
{

    if(optional_ && !variableHasValue(jData, optional_variable_name_parts_, optional_variable_value_))
    {
        return; //no parse
    }

    auto prev_current_bit = static_cast<FixedBitsItemSerializer *>(items_[0].get())->start_bit_;
    unsigned int bit_diff{0};

    for(auto idx = 0; idx < items_.size(); idx++)
    {
        items_[idx]->serializeItem(jData, 0, 0, 0, target, debug);

        unsigned int start_bit, bit_length{0};

        unsigned int byte_length = static_cast<FixedBitsItemSerializer *>(items_[idx].get())->byte_length_;
        bit_length = static_cast<FixedBitsItemSerializer *>(items_[idx].get())->bit_length_;
        start_bit = static_cast<FixedBitsItemSerializer *>(items_[idx].get())->start_bit_;

        // ard arda gelen alanlar arasında başlangıç ve bit lengthleri arasındaki fark kaynaklandığında eklenecektir.
        bit_diff = prev_current_bit - start_bit;
        prev_current_bit = start_bit;

        if (bit_diff != 0 && (bit_diff != bit_length))
        {
            for (size_t i = 0; i < static_cast<int>(bit_diff - bit_length); i++)
            {
                bitfield.push_back(false);
            }
        }

        // Eğer ilk index doldurulmamış ise 0 ile doldur.
        if(idx == 0 && (start_bit + bit_length != byte_length * 8))
        {
            unsigned int fill_Index = (byte_length * 8) - (start_bit + bit_length);
            for(auto id = 0; id < fill_Index; id++)
            {
                bitfield.push_back(false);
            }
        }

        std::copy(static_cast<FixedBitsItemSerializer *>(items_[idx].get())->vecData.begin(),
                  static_cast<FixedBitsItemSerializer *>(items_[idx].get())->vecData.end(),
                  std::back_inserter(bitfield));

        // Eğer son indexde hala start_bit 0 değil ise 0'a gelene kadar 0 doldur.
        if(idx == (items_.size() -1) && (start_bit != 0))
        {
            for(auto id = 0; id < start_bit; id++)
            {
                bitfield.push_back(false);
            }
        }

    }

    for(auto id = 0; id < bitfield.size() /8; ++id)
    {
        char convertedByte{0};
        for(int i{id * 8}; i < (id + 1)* 8; i++)
        {
            if(bitfield[i])
            {
                convertedByte |= 1 << (7 + (id * 8) - i);
            }
        }
        target.push_back(convertedByte);
    }
}