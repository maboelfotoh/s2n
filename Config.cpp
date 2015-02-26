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
	int accountID;
	std::string nickname;
	std::ifstream infile;

	infile.open("/tmp/s2hooker/adminlist.txt", std::ifstream::in);
	while(infile >> accountID >> nickname) {
		CmdProcessor::adminSet.insert(std::pair<unsigned int, unsigned int>(accountID, accountID));
	}
	infile.close();
	infile.open("/tmp/s2hooker/buildbanlist.txt", std::ifstream::in);
	while(infile >> accountID >> nickname) {
		CmdProcessor::disableBuildAccountIdSet.insert(std::pair<unsigned int, std::string>(accountID, nickname));
	}
	infile.close();
}

void updateConfig() {
	std::ofstream outfile("/tmp/s2hooker/buildbanlist.txt");
	std::map<unsigned int, std::string>::iterator it = CmdProcessor::disableBuildAccountIdSet.begin();
	for(; it != CmdProcessor::disableBuildAccountIdSet.end(); ++it) {
		outfile << it->first << " " << it->second << "\n";
	}
	outfile.close();
}
