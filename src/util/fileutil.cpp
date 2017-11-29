
#include "util/fileutil.h"
#include "util/strutil.h"
#include <fstream>


namespace FileUtil {
    std::string read_file(std::string file_path){
        std::ifstream file(file_path);
        std::string text;

        std::string line;
        while(std::getline(file, line)){
            text += line;
        }
        return text;
    }
 
    std::unordered_map<std::string,std::string> map_file(std::string file_path, std::list<std::string> keys){
        std::unordered_map<std::string, std::string> map;
        std::ifstream file(file_path);

        std::string line;
        std::string key = "";
        while(std::getline(file, line)){
            
            StringUtil::trim(line); 
            if( StringUtil::contains(line, keys) ){
                key = line;
                continue;
            }

            if(key == ""){
                continue;
            }

            //add line to map value
            map[key] += (line + "\n");
        }
        return map;
    }
} 