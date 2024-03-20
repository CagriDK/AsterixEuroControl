#pragma once
#include "Tools.h"

class Config
{
public:
    struct MessageHeader{
        uint32_t MessageType;
        uint32_t MessageSize;
    };

    struct UDPIPPort
    {
        std::string ip;
        int port;
        bool trackPlot;
        bool radarPlot;
        int multiInstance;
    };

public:
    static Config &getInstance()
    {
        if (!instance)
        {
            instance = new Config();
        }
        return *instance;
    }

    GET(uint16_t, m_serverPort, ServerPort)
    GET(std::vector<UDPIPPort>, m_udp_addres, UDPRadarIPPort)
    GET(uint16_t, m_GuiPort, GuiPort)
    GET(std::string, cat_21_definition, CAT21Definition)
    GET(std::string, cat_34_definition, CAT34Definition)
    GET(std::string, cat_48_definition, CAT48Definition)
    GET(std::string, cat_62_definition, CAT62Definition)
    GET(std::string, cat_21_json_sample, CAT21JsonSample)
    GET(std::string, cat_34_json_sample, CAT34JsonSample)
    GET(std::string, cat_48_json_sample, CAT48JsonSample)
    GET(std::string, cat_62_json_sample, CAT62JsonSample)
private:
    Config();
    ~Config();

    bool m_configOK = false;
    bool parseFile(json jData);

    uint16_t m_serverPort = 0;
    uint16_t m_GuiPort = 0;
    std::vector<UDPIPPort> m_udp_addres;

    std::string cat_21_definition;
    std::string cat_34_definition;
    std::string cat_48_definition;
    std::string cat_62_definition;

    std::string cat_21_json_sample;
    std::string cat_34_json_sample;
    std::string cat_48_json_sample;
    std::string cat_62_json_sample;
    
private:
    template <typename field>
    bool checkControlAndSet(field &val, const json &jData, const std::string str, bool check, double max_value = 0, double min_value = 0);
    
    template <typename fieldArray>
    bool checkControlAndSetArray(fieldArray &valArray, const json &jData, const std::string str, const std::string &subField_1 = "", const std::string &subField_2 = "");
    
public:
    static Config *instance;
};