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

private:
    Config();
    ~Config();

    bool m_configOK = false;
    bool parseFile(json jData);

    uint16_t m_serverPort = 0;
    uint16_t m_GuiPort = 0;
    std::vector<UDPIPPort> m_udp_addres;

private:
    template <typename field>
    bool checkControlAndSet(field &val, const json &jData, const std::string str, bool check, double max_value = 0, double min_value = 0);
    
    template <typename fieldArray>
    bool checkControlAndSetArray(fieldArray &valArray, const json &jData, const std::string str, const std::string &subField_1 = "", const std::string &subField_2 = "");
    
public:
    static Config *instance;
};