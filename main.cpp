#include <iostream>

#include "include/RadarClientMessageHandler.h"
#include "include/AppConfig.h"

int main(int, char **)
{

    //const char *data = "300030FDF70219C9356D4DA0C5AFF1E0020005283C660C10C236D4182001C0780031BC0000400DEB07B9582E410020F5"; 
    //const char *data = "3e0026975d200102378dc0ffe3be00c79ffffbffee0fcf2101108d16002a018301010118003d";
    //const char *data = "220014f6000202416dec40944060440094000010";
    const char *data = "150024fb811380000540301c293e006e4cc9ffe606d6496c2e00080818dc31ca08200015";

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
    
    //const char *fullSampleData34= "22002ffff8000202416dec40320094406044009400001002200450146400c800471c8e390400c88e38e48e38e400c7";

    std::vector<char> vec(data, data + strlen(data));
    RadarClientMessageHandler radarMsgHandler;
    radarMsgHandler.asterixMessageParser(vec);

    Config::getInstance();
    std::cout<<std::dec<<"Config::Server Port = "<<Config::getInstance().getServerPort()<<std::endl;
}
