#pragma once

#define ValueTypeX(value) std::remove_reference<decltype(value)>::type

#define GET(Type, MemberName, FaceName) \
    Type get##FaceName() const          \
    {                                   \
        return MemberName;              \
    }                                   \

#define SET(Type, MemberName, FaceName) \
    void set##FaceName(Type value)      \
    {                                   \
        MemberName = value;             \
    }                                   \

#define GETSET(Type, MemberName, FaceName) \
    GET(Type, MemberName, FaceName)        \
    SET(Type, MemberName, FaceName)

#include <string>
#include <fstream>
#include <iostream>
#include <mutex>
#include <chrono>
#include <thread>
#include <vector>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <ctime>
#include <algorithm>
#include "../lib/json.hpp"

#include <sys/types.h>
#include <sys/stat.h>

using json = nlohmann::json;

class Templates
{
    public:
    template <typename T>
    static T getValue();
};

class Tools
{
    public:
    // get the current time FUNCTION
    static double getEpochTime();

    // check if the file exist FUNCTION
    static bool isFileExist(const std::string &filePath);

    // check if the directory exist FUNCTION
    static bool isDirectoryExist(const std::string &directoryPath);

    // create a directory FUNCTION
    static int MakeDirectory(const std::string &dirPath);

    static void waitReal_usec(size_t waitMicSec);

    static bool WriteToFile(const std::string &filePath, const std::string &data);

}




// void copy_to_Json(nlohmann::json &j, nlohmann::json &cat_Map, std::string key)
// {
//     if (j[key].empty())
//     {
//         std::cout << key << " : message is not exist!" << std::endl;
//     }
//     else
//     {
//         for (const auto &item : j.items())
//         {
//             cat_Map[item.key()] = item.value();
//         }
//     }
// }

// template <typename T>
// void copy_from_Json(const nlohmann::json &j, const std::string &key, T &value)
// {
//     if (j.contains(key) && !j[key].is_null())
//     {
//         value = j[key].get<T>();
//     }
//     else
//     {
//         // std::cout << key << " : message is not exist!" << std::endl;
//     }
// }