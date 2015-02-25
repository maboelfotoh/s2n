#include <stdio.h>
#include <unistd.h>
#include <fstream>
#include <string>
#include "CmdProcessor.h"
#include "Config.h"

void readConfig() {
	char wd[256];
	getcwd(wd, 255);
	printf("Sv: Config: reading build banlist file from %s/s2hooker/buildbanlist.txt\r\n", wd);
	std::ifstream infile("/tmp/s2hooker/buildbanlist.txt");
	int accountID;
	std::string nickname;
	while(infile >> accountID >> nickname) {
		CmdProcessor::disableBuildAccountIdSet.insert(std::pair<unsigned int, unsigned int>(accountID, accountID));
	}
}
