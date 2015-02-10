#include <string>
#include <cstdlib>
#include "CmdProcessor.h"
#include "CmdHandler.h"

void onDisableBuild(int argc, std::string argv[]) {
    std::string id = argv[1];
    unsigned int accountId = atoi(id.c_str());
    //disableBuildAccountIdSet.insert(accountId);
    CmdProcessor::disableBuildAccountIdSet.insert(std::pair<unsigned int, unsigned int>(accountId, accountId));
}
