#include <map>
//#include <unordered_set>
#include <cstdarg>
#include <string>
//#include <regex>
#include "CmdHandler.h"

typedef void (*handler_method)(int, std::string[]);

class CmdProcessor {
public:
    static std::map<std::string, handler_method> filters;
    static std::map<unsigned int, unsigned int> userMap;
    //static std::unordered_set<uint32_t> disableBuildAccountIdSet;
    //static std::unordered_set<uint32_t> disableBuildConnIdSet;
    static std::map<unsigned int, unsigned int> disableBuildAccountIdSet;
    static std::map<unsigned int, unsigned int> disableBuildConnIdSet;

    static void init();
    static void process(char*, int);
};

typedef std::map<std::string, handler_method> filter_map;
typedef std::map<std::string, handler_method>::iterator filter_iterator;
//typedef filter_map::iterator filter_iterator;
