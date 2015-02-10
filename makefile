CC = g++

s2n.so : s2n.cpp S2Daemon.cpp
	$(CC) -lpthread -Wall -O2 -fpic -shared -ldl -o s2n.so s2n.cpp S2Daemon.cpp CnC.cpp CmdProcessor.cpp CmdHandler.cpp

clean:
	$(RM) s2n.so *.o
