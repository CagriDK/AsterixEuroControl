#include "../include/AppConfig.h"

Config *Config::instance = nullptr;

const std::string configFileName = "appConfig.json";

Config::Config()
{
    m_configOK = false;
    if (Tools::isFileExist("./" + configFileName))
    {
        try
        {
            json jData;
            std::ifstream input("./" + configFileName);
            input >> jData;
            input.close();

            if (parseFile(jData))
            {
                m_configOK = true;
            }
            else
            {
                std::cout << "Config is not ok! Missing field(s) in file" << configFileName << "." << std::endl;
            }
        }
        catch (const std::exception &e)
        {
            std::cerr <<"AppConfig error: "<< e.what() << '\n';
        }
    }
    else
    {
        std::cout << "Config file " << configFileName << " does not exist." << std::endl;
    }
}

Config::~Config()
{
    delete instance;
}

bool Config::parseFile(json jData)
{
    bool checkVal = true;
    checkVal &= checkControlAndSet(m_serverPort, jData, "ServerPort",false);
    checkVal &= checkControlAndSet(m_GuiPort, jData, "RadarControlGuiPort",false);
    checkVal &= checkControlAndSetArray(m_udp_addres, jData, "RadarUDPAdresses", "ip", "port");
    return checkVal;
}

template <typename field>
bool Config::checkControlAndSet(field &val, const json &jData,const std::string str, bool check, double max_value, double min_value)
{
    bool retVal = false;
    if (jData.contains(str))
    {
        retVal = true;
        if (check)
        {
            if (jData[str] > max_value)
            {
                std::string warningMsg = str + " parameter can not be bigger than " + std::to_string(max_value) + ". Limited to " + std::to_string(max_value) + ".";
                val = max_value;
                return retVal;
            }
            else if (jData[str] < min_value)
            {
                std::string warningMsg = str + " parameter can not be smaller than " + std::to_string(min_value) + ". Limited to " + std::to_string(min_value) + ".";
                val = min_value;
                return retVal;
            }
        }
        else
        {
            val = jData[str];
        }
    }
    else
    {
        std::cout << "Missing parameter \"" + str + "\" in config file." << std::endl;
    }
    return retVal;
}

template <typename fieldArray>
bool Config::checkControlAndSetArray(fieldArray &valArray, const json &jData, const std::string str, const std::string &subField_1, const std::string &subField_2)
{
    bool retVal = false;
    if (jData.contains(str))
    {
        retVal = true;
        if (jData[str].is_array())
        {
            for (auto &val : jData[str])
            {
                if (subField_1 != "" && subField_2 != "")
                {
                    UDPIPPort temp;
                    temp.ip = val[subField_1];
                    temp.port = val[subField_2];
                    temp.trackPlot = false;
                    temp.radarPlot = false;
                    temp.multiInstance = 1;
                    valArray.push_back(temp);
                }
            }
        }
    }
    else
    {
        std::cout << "Missing parameter \"" + str + "\" in config file." << std::endl;
    }
    return retVal;
}