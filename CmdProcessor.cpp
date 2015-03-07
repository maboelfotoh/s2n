#include <stdio.h>
#include <vector>
#include <cstdlib>
#include "CmdProcessor.h"

filter_map CmdProcessor::filters;
std::map<unsigned int, User*> CmdProcessor::userMap;
std::map<unsigned int, unsigned int> CmdProcessor::acc2connMap;
std::map<unsigned int, unsigned int> CmdProcessor::adminSet;
std::map<unsigned int, std::string> CmdProcessor::disableBuildAccountIdSet;
std::map<unsigned int, unsigned int> CmdProcessor::disableBuildConnIdSet;

void CmdProcessor::init() {
    //filters[onDisableBuild] = new std::regex("\\b(GET /disableBuild?id=)([^\r]+)");
    //filters.insert(std::pair<handler_method, std::string>(onDisableBuild, "GET /disableBuild?id="));
    filters["GET /disableBuild?id="] = onDisableBuild;
}

void CmdProcessor::process(char* buf, int len) {
    std::vector<std::string> vargs;
    long unsigned int eqsign, amp, i;
    std::string* args;
    //std::smatch match;
    std::string buffer(buf);
    printf("Sv: CmdProcessor: processing received command\r\n");
    for(filter_iterator iter = filters.begin(); iter != filters.end(); ++iter) {
        // function = iterator->first = key
        // regex = iterator->second = value
        //std::regex re = iter->second;
        /*
        if (std::regex_search(buf, match, iter->second)) {
            // The first sub_match is the whole string; the next
            // sub_match is the first parenthesized expression.
            if (match.size() == 0) continue;
            string args[] = new string[match.size()];
            for(int i = 0; i < match.size(); ++i) {
                args[i] = match[i].str();
            }
            iterator->first(match.size(), args);
            break;
        }
        */
        std::string pattern = iter->first;
        std::string param;
        if(std::string::npos != buffer.find(pattern)) {
            printf("Sv: CmdProcessor: found matching command handler\r\n");
            buffer = buffer.substr(0, buffer.find("HTTP/")) + "\0";
            amp = 0;
            do {
                eqsign = buffer.find("=", amp, 1);
                amp = buffer.find("&", eqsign + 1, 1);
                vargs.push_back(buffer.substr(eqsign + 1, amp) + "\0");
                printf("Sv: CmdProcessor: %s\r\n", (buffer.substr(eqsign + 1, amp) + "\0").c_str());
            } while(amp ^ std::string::npos);
            args = new std::string[vargs.size()];
            for(i = 0; i < vargs.size(); ++i)
                args[i] = vargs.at(i);
            printf("Sv: CmdProcessor: calling handler function\r\n");
            iter->second(vargs.size(), args);
        }
    }
}
