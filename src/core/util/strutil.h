#pragma once

#include <string>
#include <list>

#include <algorithm> 
#include <locale>
#include <cctype>

namespace StringUtil
{
    bool startswith(std::string start, std::string text);
    bool contains(std::string str, std::list<std::string> container);

    /*
    * Trim from: 
    * https://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring/217605#217605
    */
    static inline void ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch)
        {
            return !std::isspace(ch);
        }));
    }

    static inline void rtrim(std::string &s){
        s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) 
        {   
            return !std::isspace(ch);
        }).base(), s.end());
    }

    static inline void trim(std::string &s) {
        ltrim(s);
        rtrim(s);
    }

}