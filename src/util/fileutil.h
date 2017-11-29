
#pragma once 

#include <string>
#include <unordered_map>

namespace FileUtil {
    std::string read_file(std::string file_path);
    std::unordered_map<std::string,std::string> map_file(std::string file_path, std::list<std::string> keys);
}