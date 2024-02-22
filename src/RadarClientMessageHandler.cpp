#include "../include/RadarClientMessageHandler.h"

RadarClientMessageHandler::RadarClientMessageHandler()
{
}

RadarClientMessageHandler::~RadarClientMessageHandler()
{
}

void RadarClientMessageHandler::asterixMessageParser(const std::vector<char> &data)
{
    size_t byte_array_size = data.size() / 2;
    char data_bytes[byte_array_size];
    hexStringToBytes(data, data_bytes, byte_array_size);

    int catType = static_cast<int>(static_cast<unsigned char>(data_bytes[0]));
    json jData;
    switch (catType)
    {
    case 21:
        std::cout << "CAT CATEGORY: " << catType << "\n";
        m_cat21.decodeData(data_bytes, jData);
        break;
    case 34:
        std::cout << "CAT CATEGORY: " << catType << "\n";
        m_cat34.decodeData(data_bytes, jData);
        break;
    case 48:
        std::cout << "CAT CATEGORY: " << catType << "\n";
        m_cat48.decodeData(data_bytes, jData);
        break;
    case 62:
        std::cout << "CAT CATEGORY: " << catType << "\n";
        m_cat62.decodeData(data_bytes, jData);
        break;
    default:
        break;
    }
}

void RadarClientMessageHandler::hexStringToBytes(const std::vector<char> &hex_vector, char *bytes, size_t max_length)
{
    std::string hex(hex_vector.begin(), hex_vector.end());
    size_t length = hex.length();
    for (size_t i = 0; i < length; i += 2)
    {
        if (i / 2 >= max_length)
        {
            return;
        }
        std::string byteString = hex.substr(i, 2);
        unsigned char byte = static_cast<unsigned char>(std::stoul(byteString, nullptr, 16));
        bytes[i / 2] = byte;
    }
}