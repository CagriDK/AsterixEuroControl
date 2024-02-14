#include "../include/RadarClientMessageHandler.h"

RadarClientMessageHandler::RadarClientMessageHandler()
{
}

RadarClientMessageHandler::~RadarClientMessageHandler()
{
}

void RadarClientMessageHandler::asterixMessageParser(const std::vector<char> &data)
{
    // size_t byte_array_size = data.size() / 2;
    // char data_bytes[byte_array_size];
    // hexStringToBytes(data, data_bytes, byte_array_size);

    const char *data1 = "\x30\x00\x30\xFD\xF7\x02\x19\xC9\x35\x6D\x4D\xA0\xC5\xAF\xF1\xE0\x02\x00\x05\x28\x3C\x66\x0C\x10\xC2\x36\xD4\x18\x20\x01\xC0\x78\x00\x31\xBC\x00\x00\x40\x0D\xEB\x07\xB9\x58\x2E\x41\x00\x20\xF5";
    const size_t byte_array_size = 48;
    char data_bytes[byte_array_size]; // Char dizisini tanımla
    memcpy(data_bytes, data1, byte_array_size); // data'yı data_bytes'a kopyala

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