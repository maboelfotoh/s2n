#include <string>

class User {
	// holds hash values of last 3 chat messages sent by user
	unsigned int lastChatMsgs[3];
	int msgIndex;
public:
	int accountID;
	User(int accountID);
	bool addChatMsg(const char * msg, unsigned int len);
};
