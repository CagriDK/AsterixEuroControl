/*
 * This file is part of ATSDB.
 *
 * ATSDB is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ATSDB is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with ATSDB.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "../include/string_conv.h"

#include <iostream>
#include <cassert>
#include <cstring>
#include <iomanip>
#include <sstream>
#include <string>

int char2int(char input)
{
    if (input >= '0' && input <= '9')
        return input - '0';
    if (input >= 'A' && input <= 'F')
        return input - 'A' + 10;
    if (input >= 'a' && input <= 'f')
        return input - 'a' + 10;
    throw std::invalid_argument("Invalid input string");
}

// This function assumes src to be a zero terminated sanitized string with
// an even number of [0-9a-f] characters, and target to be sufficiently large
// Returns size
size_t hex2bin(const char* src, char* target)
{
    size_t src_len = strlen(src);
    assert(src_len % 2 == 0);

    while (*src && src[1])
    {
        *(target++) = static_cast<char>(char2int(*src) * 16 + char2int(src[1]));
        src += 2;
    }

    return src_len / 2;
}

char getIcaoChar(unsigned char c)
{
    char ch;

    ch = '?';
    if (1 <= c && c <= 26)
    {
        ch = static_cast<char>('A' + (c - 1));
    }
    else if (c == 32)
    {
        ch = ' ';
    }
    else if (48 <= c && c <= 57)
    {
        ch = static_cast<char>('0' + (c - 48));
    }

    return ch;
}

std::vector<bool> stringToIcaoBitsets(const std::string &input,size_t character_bit_length)
{
    std::vector<bool> bitVector;

    for (char c : input)
    {
        unsigned int value = 0;
        if(islower(c))
        {
            std::cout<<"Serializing a non proper value for string! Value should be UPPERCASE ";
            return std::vector<bool>(input.size(),32);
        }
        //c = putchar(toupper(c));
        if ('A' <= c && c <= 'Z')
        {
            value = c - 'A' + 1;
        }
        else if (c == ' ')
        {
            value = 32; 
        }
        else if ('0' <= c && c <= '9')
        {
            value = c - '0' + 48;
        }

        for (int i = character_bit_length; i >= 0; --i)
        {
            bitVector.push_back((value >> i) & 1);
        }

    }

    return bitVector;
}

std::vector<bool> stringToAsciiBitsets(const std::string &input,size_t character_bit_length)
{
    std::vector<bool> bitVector;
    for(char c : input)
    {
        unsigned char value = static_cast<unsigned char>(c);

        for(int i = character_bit_length; i >= 0; --i)
        {
            bitVector.push_back((value >> i) & 1);
        }
    }

    return bitVector;
}

constexpr char hexmap[] = {'0', '1', '2', '3', '4', '5', '6', '7',
                           '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

std::string binary2hex(const unsigned char* data, unsigned int len)
{
    std::string s(len * 2, ' ');
    for (unsigned int i = 0; i < len; ++i)
    {
        s[2 * i] = hexmap[(data[i] & 0xF0) >> 4];
        s[2 * i + 1] = hexmap[data[i] & 0x0F];
    }
    return s;
}

std::vector<char> hex2binary(const std::string &hex)
{
    std::vector<char> bytes;

    for (size_t i = 0; i < hex.length(); i += 2)
    {
        std::string byteString = hex.substr(i, 2);
        char byte = static_cast<char>(std::stoi(byteString, nullptr, 16));
        bytes.push_back(byte);
    }

    return bytes;
}

// std::string binary2hex(const unsigned char* src, unsigned int length)
//{
//    std::stringstream ss;
//    for(unsigned int i=0; i < length; ++i)
//        ss << std::setfill('0') << std::setw(2) << std::hex << (int)src[i];
//    return ss.str();
//}

std::vector<std::string>& split(const std::string& s, char delim, std::vector<std::string>& elems)
{
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim))
    {
        elems.push_back(item);
    }
    return elems;
}

std::string toString(const nlohmann::json& j)
{
    if (j.type() == nlohmann::json::value_t::string)
    {
        return j.get<std::string>();
    }

    return j.dump();
}

bool isASCII(const std::string& s)
{
    return !std::any_of(s.begin(), s.end(),
                        [](char c) { return static_cast<unsigned char>(c) > 127; });
}

// std::vector<std::string> split(const std::string &s, char delim)
//{
//    std::vector<std::string> elems;
//    return split(s, delim, elems);
//}
