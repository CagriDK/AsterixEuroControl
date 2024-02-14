// #pragma once

// #include <iostream>

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