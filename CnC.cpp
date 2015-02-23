#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <iostream>
#include <vector>
#include "CnC.h"
#include "CmdProcessor.h"

void error(const char *msg) {
    //perror(msg);
    printf(msg);
    exit(1);
}

void* listenTask(void *argument) {
	printf("CnC: TCP server thread stared\r\n");
      char* msg;
      msg = (char*)argument;
      std::cout<<msg<<std::endl;

     int sockfd, newsockfd, portno = 57531;
	int NumRetries = 3;
     socklen_t clilen;
     char buffer[256];
     struct sockaddr_in serv_addr, cli_addr;
     int n;
     //std::vector<Connection*> conns;
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
	printf("CnC: binding to port %d\n", portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) {
		int i = 0;
		for(; i < NumRetries; ++i) {
			++portno;
			serv_addr.sin_port = htons(portno);
			if(0 <= bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)))
				break;
		}
		if(i == NumRetries) {
		printf("CnC: error on binding\r\n");
              error("ERROR on binding");
		}
	}
     while(true) {
	printf("CnC: listening for connection\r\n");
         listen(sockfd,5);
         clilen = sizeof(cli_addr);
         newsockfd = accept(sockfd, 
                 (struct sockaddr *) &cli_addr, 
                 &clilen);
         if (newsockfd < 0) 
             //error("ERROR on accept");
             continue;
         //conns.push_back(new Connection(newsockfd));
         bzero(buffer,256);
         n = read(newsockfd,buffer,255);
         if (n < 0) //error("ERROR reading from socket");
             continue;
         //printf("Here is the message: %s\n",buffer);
         CmdProcessor::process(buffer, n);
         n = write(newsockfd, "OK", 2);
         //if (n < 0) error("ERROR writing to socket");
         close(newsockfd);
     }
     close(sockfd);
     return NULL;
}

void CnC::init() {
    pthread_t listenThread;
    int i1;
	printf("CnC: launching TCP server thread\r\n");
    i1 = pthread_create( &listenThread, NULL, listenTask, (void*) "listenThread");
    //pthread_join(listenThread, NULL);
}
