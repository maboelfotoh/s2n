#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <map>
#include <cstdarg>
#include "CmdProcessor.h"

typedef void (CmdProcessor::*handler_method)(int, string[]);
typedef std::map<handler_method, std::regex> filter_map;
typedef filter_map::iterator filter_iterator;

class Connection {
    int sockfd;
    char buffer[256];
public:
    Connection();
    ~Connection();
    read(char*, int);
    write(char*, int);
};
