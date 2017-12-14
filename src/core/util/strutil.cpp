
#include "util/strutil.h"

namespace StringUtil{

    bool startswith(std::string start, std::string text){
        return 0 == text.compare(0, start.length(), start);
    }

    bool contains(std::string str, std::list<std::string> container){
        return std::end(container) != std::find(std::begin(container), std::end(container), str);
    }
}