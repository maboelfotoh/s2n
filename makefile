CC = g++

s2n.so : s2n.cpp S2Daemon.cpp
	$(CC) -Wall -O2 -fpic -shared -ldl -o s2n.so s2n.cpp S2Daemon.cpp

clean:
	$(RM) s2n.so *.o
