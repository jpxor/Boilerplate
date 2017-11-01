
#include "util/strutil.h"

namespace StringUtil{

    bool startswith(std::string start, std::string text){
        return 0 == text.compare(0, start.length(), start);
    }

}