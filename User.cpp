#include "util.h"
#include "User.h"

User::User(int accountID) {
	this->msgIndex = 0;
	this->accountID = accountID;
	for(int i = 0; i < 3; ++i)
		lastChatMsgs[i] = 0;
}

bool User::addChatMsg(const char * msg, unsigned int len) {
	unsigned int h = hash(msg, len);
	for(int i = msgIndex - 1; i >= 0; --i)
		if(h == lastChatMsgs[i]) return false;
	lastChatMsgs[msgIndex] = h;
	msgIndex = (msgIndex + 1) % 3;
	return true;
}
