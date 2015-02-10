#include "Connection.h"

string HttpResp = "HTTP/1.1 200 OK\r\n\r\n";

Connection::Connection(int sockfd) {
    this.sockfd = sockfd;
    bzero(buffer, 256);

    filter_map filters;
    filters[onDisableBuild] = new std::regex("\\b(GET /disableBuild?id=)([^\r]+)");
}

Connection::start() {
    pthread_t connThread;
    int i1;
    i1 = pthread_create(&connThread, NULL, Connection::loop(), (void*) "connThread");
}

Connection::loop() {
     int n;
     while(true) {
         n = read(newsockfd,buffer,255);
         if (n < 0) error("ERROR reading from socket");
         processMsg(buffer, n);
     }
}

Connection::processMsg(char* buf, int len) {
    std::smatch match;
    for(filter_iterator iter = filter_map.begin(); iter != filter_map.end(); ++iter) {
        // function = iterator->first = key
        // regex = iterator->second = value
        std::regex re = iter->second;
        if (std::regex_search(buf, match, iter->second)) {
            // The first sub_match is the whole string; the next
            // sub_match is the first parenthesized expression.
            if (match.size() == 0) continue;
            string args[] = new string[match.size()];
            for(int i = 0; i < match.size(); ++i) {
                args[i] = match[i].str();
            }
            iterator->first(match.size(), args);
            break;
        }
    }
}

Connection::write(char* buffer, int length) {
    write(sockfd, buffer, length);
}

Connection::~Connection() {
     close(sockfd);
}
