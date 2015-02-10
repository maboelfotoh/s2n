#include <stdio.h>
#include <string>
#include <cstdlib>
#include "CmdProcessor.h"
#include "CmdHandler.h"

void onDisableBuild(int argc, std::string argv[]) {
    printf("onDisableBuild arg %s\r\n", argv[0].c_str());
    std::string id = argv[0];
    unsigned int accountId = atoi(id.c_str());
    printf("CmdHandler: disabling builder build ability for account id %u\r\n", accountId);
    //disableBuildAccountIdSet.insert(accountId);
    CmdProcessor::disableBuildAccountIdSet.insert(std::pair<unsigned int, unsigned int>(accountId, accountId));
}
