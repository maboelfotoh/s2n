CC = g++

s2n.so : s2n.cpp S2Daemon.cpp
	$(CC) -g -lpthread -Wall -O2 -fpic -shared -ldl -o s2n.so s2n.cpp S2Daemon.cpp CnC.cpp CmdProcessor.cpp CmdHandler.cpp User.cpp util.cpp

clean:
	$(RM) s2n.so *.o
