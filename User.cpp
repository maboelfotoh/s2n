#include "util.h"
#include "User.h"

void User::addChatMsg(std::string msg) {
	unsigned int hash = hash(msg.c_str());
	lastChatMsgs[msgIndex] = hash;
	msgIndex = (msgIndex + 1) % 3;
}
