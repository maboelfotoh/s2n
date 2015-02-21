#include <string>

class User {
	// holds hash values of last 3 chat messages sent by user
	unsigned int lastChatMsgs[3];
	int msgIndex = 0;
public:
	void addChatMsg(std::string msg);
};
