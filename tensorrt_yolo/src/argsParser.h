


#ifndef __COMMAND_LINE_H_
#define __COMMAND_LINE_H_

#include <stdint.h>
#include <stdlib.h>
#include <vector>
#include <string>

#include <string.h>
#include <strings.h>

/**
 * args line parser 
 */
class argsParser {
public:
    argsParser(const int argc, char** argv);

    /**
     * Parse Flag
     */
    bool ParseFlag(const std::string argName) const;

    /**
     * Parse String
     */
    const char* ParseString(const std::string  argName) const;
    // const char* ParseString2(const std::string argName, const char* defaultValue = NULL, bool allowOtherDelimiters = true) const;

    /**
     * Parse String list delimited by ","
     */
    std::vector<std::string> ParseStringList(std::string argName, const char delimiter = ',') const;

    /**
     * The argument count that the object was created with from main()
     */
    int argc;
    char** argv;
};

#endif
