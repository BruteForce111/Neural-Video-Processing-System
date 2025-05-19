


#include "argsParser.h"

// constructor
argsParser::argsParser(const int pArgc, char** pArgv) {
    argc = pArgc;
    argv = pArgv;
}
// ParseFlag
bool argsParser::ParseFlag(std::string string_ref) const {
    if (argc < 1) return false;

    for (int i = 0; i < argc; i++) {
        const int string_start = std::string(argv[i]).find_last_of('-') + 1; 
        if (string_start == 0) continue;

        const char* string_argv = &argv[i][string_start];
        
        const char* equal_pos = strchr(string_argv, '=');

        const int argv_length = (int)(equal_pos == 0 ? strlen(string_argv) : equal_pos - string_argv);
        const int length = (int)(string_ref.size());

        if (length == argv_length && !strncasecmp(string_argv, string_ref.c_str(), length)) return true;
    }
    return false;
}

// ParseString
const char* argsParser::ParseString(std::string string_ref) const {
    if (argc < 1) return NULL;

    for (int i = 0; i < argc; i++) {
        const int string_start = std::string(argv[i]).find_last_of('-') + 1; 

        if (string_start == 0) continue;

        char* string_argv = (char*)&argv[i][string_start];
        const int length = (int)(string_ref.size());

        if (!strncasecmp(string_argv, string_ref.c_str(), length)) return (string_argv + length + 1);
        //*string_retval = &string_argv[length+1];
    }
    return NULL;
}


// ParseStringList eg. img1,img2,img3
std::vector<std::string> argsParser::ParseStringList(std::string argName, const char delimiter)  const{
    const char* ListStr = ParseString(argName);
    std::vector<std::string> result;
    if (ListStr == NULL) return result;
    int string_start = 0;
    int string_end = 0;

    int strLen = (int)strlen(ListStr);
    while(string_end < strLen){
        while (delimiter != ListStr[string_end] && string_end < strLen) string_end++;
        result.push_back(std::string(ListStr).substr(string_start,string_end-string_start));
        string_end++;
        string_start = string_end;
    }
    return result;
}
