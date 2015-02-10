#include "CmdHandler.h"

void onDisableBuild(int argc, std::string argv[]) {
    std::string id = argv[1];
    unsigned int accountId = atoi(id.c_str());
    //disableBuildAccountIdSet.insert(accountId);
    disableBuildAccountIdSet.insert(accountId, accountId);
}
