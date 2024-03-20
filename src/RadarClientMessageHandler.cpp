#include "../include/RadarClientMessageHandler.h"
#include "../include/AppConfig.h"

RadarClientMessageHandler::RadarClientMessageHandler()
{
}

RadarClientMessageHandler::~RadarClientMessageHandler()
{
}

void RadarClientMessageHandler::asterixMessageParser(const std::vector<char> &data, bool TEST_SAMPLE_CASE)
{
    Config::getInstance();
    //std::cout<<std::dec<<"Config::Server Port = "<<Config::getInstance().getServerPort()<<std::endl;

    size_t byte_array_size = data.size() / 2;
    char data_bytes[byte_array_size];
    hexStringToBytes(data, data_bytes, byte_array_size);

    int catType = static_cast<int>(static_cast<unsigned char>(data_bytes[0]));
    json jData;

    
    json cat21_definition = nlohmann::json::parse(std::ifstream(Config::getInstance().getCAT21Definition()));
    json cat34_definition = nlohmann::json::parse(std::ifstream(Config::getInstance().getCAT34Definition()));
    json cat48_definition = nlohmann::json::parse(std::ifstream(Config::getInstance().getCAT48Definition()));
    json cat62_definition = nlohmann::json::parse(std::ifstream(Config::getInstance().getCAT62Definition()));

    json cat21_sample = nlohmann::json::parse(std::ifstream(Config::getInstance().getCAT21JsonSample()));
    json cat34_sample = nlohmann::json::parse(std::ifstream(Config::getInstance().getCAT34JsonSample()));
    json cat48_sample = nlohmann::json::parse(std::ifstream(Config::getInstance().getCAT48JsonSample()));
    json cat62_sample = nlohmann::json::parse(std::ifstream(Config::getInstance().getCAT62JsonSample()));

    switch (catType)
    {
    case 21:
        std::cout << "CAT CATEGORY: " << catType << "\n";
        m_cat21.decodeData(data_bytes, jData, cat21_definition, TEST_SAMPLE_CASE, cat21_sample);
        break;
    case 34:
        std::cout << "CAT CATEGORY: " << catType << "\n";
        m_cat34.decodeData(data_bytes, jData, cat34_definition, TEST_SAMPLE_CASE, cat34_sample);
        break;
    case 48:
        std::cout << "CAT CATEGORY: " << catType << "\n";
        m_cat48.decodeData(data_bytes, jData, cat48_definition, TEST_SAMPLE_CASE, cat48_sample);
        break;
    case 62:
        std::cout << "CAT CATEGORY: " << catType << "\n";
        m_cat62.decodeData(data_bytes, jData, cat62_definition, TEST_SAMPLE_CASE, cat62_sample);
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