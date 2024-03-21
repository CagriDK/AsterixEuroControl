#include <iostream>
#include <stdexcept>

#include "include/RadarClientMessageHandler.h"
#include "include/AppConfig.h"

void terminal(std::string &input, std::string infoMessage, int minValue, int maxValue)
{
    std::getline(std::cin,input);
    std::cout<<"\n";

    while(input.empty() || input.size() > 1 || !std::isdigit(input[0]) || std::stoi(input) < minValue || std::stoi(input) > maxValue )
    {
        if(input.empty())
        {
            std::cout<<"Input character is empty, re-enter the mod selection ("<< minValue << " - " <<maxValue <<")\n\n";
        }
        else if(input.size() > 1)
        {
            std::cout<<"Extra character entered, re-enter the mod selection ("<< minValue << " - " <<maxValue <<")\n\n";
        }
        else if(!std::isdigit(input[0]))
        {
            std::cout<<"Input character is not a digit as the Expected ("<< minValue << " - " <<maxValue <<") which input is = " << input << " , re-enter the mod selection ("<< minValue << "- " <<maxValue <<")\n\n";
        }
        else if(std::stoi(input) < minValue)
        {
            std::cout<<"Input number is lower than the Expected ("<< minValue << " - " <<maxValue <<") which input is = " << input << " , re-enter the mod selection ("<< minValue << "- " <<maxValue <<")\n\n";
        }
        else if(std::stoi(input) > maxValue)
        {
            std::cout<<"Input number is higher than the Expected ("<< minValue << " - " <<maxValue <<") which input is = " << input << " , re-enter the mod selection ("<< minValue << "- " <<maxValue <<")\n\n";
        }
        
        std::cout<<infoMessage;
        std::getline(std::cin,input);
        std::cout<<"\n";
    }
}

int main(int, char **)
{
    std::cout<<"This program is a simple Asterix message parser and encoder. \n";
    std::cout<<"It can be used as 2 MOD's \n"; 
    std::cout<<"1-)Create the hex data's from json examples\n";
    std::cout<<"2-)Test this hex data if it parse and encoded correctly\n\n";
    std::cout<<"**************************************************************************************************\n";
    std::cout<<"Simply use MOD 1 to create your own data or Check MOD 1 output == MOD 2 output for hex comparison \n";
    std::cout<<"**************************************************************************************************\n\n";

    std::string infoMessage;

    infoMessage = "Please input the MOD number: \n MOD 1: Create Cat Message from json sample (Hex data will be created).\n MOD 2: Use Hex data and parse/encode (Json will be created, Hex data will be created).\n\n";

    std::cout<<infoMessage;
    std::string mod;
    terminal(mod, infoMessage, 1, 2);

    infoMessage = "Please input the CAT number: \n 1) CAT21: Air Traffic Control Surveillance Data\n 2) CAT34: Radar Plot Data\n 3) CAT48: VRS Data\n 4) CAT62: Derived Weather Information\n\n";

    std::cout<<infoMessage;
    std::string cat;
    terminal(cat, infoMessage, 1, 4);

    json cat_sample;
    switch (std::stoi(cat))
    {
    case 1:
        std::cout<<"This feature not added currently please use another type\n";
        throw std::runtime_error("Application closing...!\n");
        //cat_sample = nlohmann::json::parse(std::ifstream(Config::getInstance().getCAT21JsonSample()));
        break;
    case 2:
        cat_sample = nlohmann::json::parse(std::ifstream(Config::getInstance().getCAT34JsonSample()));
        break;
    case 3:
        cat_sample = nlohmann::json::parse(std::ifstream(Config::getInstance().getCAT48JsonSample()));
        break;
    case 4:
        cat_sample = nlohmann::json::parse(std::ifstream(Config::getInstance().getCAT62JsonSample()));
        break;
    
    default:
        break;
    }

    std::string sample = cat_sample["Hex Data"];
    std::vector<char> vec(sample.begin(), sample.end());

    bool USE_SAMPLE_SCENARIO_CREATE_HEX_DATA = stoi(mod) == 1 ? true : false;

    RadarClientMessageHandler radarMsgHandler;
    radarMsgHandler.asterixMessageParser(vec, USE_SAMPLE_SCENARIO_CREATE_HEX_DATA);

}

    //const char *fullSampleData34 = "22002ffff8000202416dec40320094406044009400001002200450146400c800471c8e390400c83c71c8b8e38ec064";
    //const char *fullSampleData48 = "300068fffffff80002356d4da0c5aff1e002000528fee464648964333b0003e810c236d4182001c0780031bc00004000643200320007b9582e4100404064e405020064014a00640028c080640200640064006400320032003220f5c0780031bc00000803e8640064";
    //const char *fullSampleData62 = "3e00f8bffffffe010264378dc0011c71c8011c71c803b718039e000087fffe78880e0080049483244d7621011040e2012c01de01010158ffe0c8c8c8c800c8c8c8c8c8c8f0ffffffffe0c8c8c8c8c8c8c8c8c8c8c8c8c832c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8004000100004005ffffff0010234324142464646400026ac40414231324141424132424142346464640190646401080c2d20474147414741405441524441313254415244413133128054415244413134c947c864fe7e00000101471c72238e39406400645a0e0064640191640190ff8000c800c800c891a3da7410b4c8b4c8b410fe01026400471c000493d8a2008c";

    //const char *data = "300030FDF70219C9356D4DA0C5AFF1E0020005283C660C10C236D4182001C0780031BC0000400DEB07B9582E410020F5"; 
    //const char *data = "3e0026975d200102378dc0ffe3be00c79ffffbffee0fcf2101108d16002a018301010118003d";
    //const char *data = "220014f6000202416dec40944060440094000010";
    //const char *data = "150024fb811380000540301c293e006e4cc9ffe606d6496c2e00080818dc31ca08200015";

    //const char *data = "3e0026975d200102378dc0f2a927ff5a840385ff4809bd210110483407f300e80101015804d4";
    //const char *data = "3e0026975d200102378dc0fc9f7e064d7d0129fe3d0ebd210110684804ac010401010158022c";
    //const char *data = "3e0029975da40102378dc003b718039e000087fffe0e0021011040e2012c01de01010158f00040005f";
    //const char *data = "3e0029975da40102378dc0f6ce1e04b5a70172fcbc00d72101106efc07e7024601010158f005c80000";
    //const char *data = "3e0029975da40102378dc0083fc20828b30074fd1e094a21011079a60676011d01010158f00578fffc";
    //const char *data = "3e0029975da40102378dc0f86135fb02e9fef1fded096c21011093400527013c01010158f4020c015b";
    //const char *data = "3e0029975da40102378dc0f9dba8078dd60295fdb2046a2101105dae07aa019601010158f005780000";
    //const char *data = "3e0029975da40102378dc0f20ebefb9dc5fea3020202af210110e7ad0560018501010158f005a00000";
    //const char *data = "3e0029975da40102378dc0f8c72bf99b39003f02c3009421011003a40624001301010158f403440176";
    //const char *data = "3e0029975da40102378dc0fb7c4f05a1ac01c5fde20ee1210110639f061d001101010158f403600119";
