#ifndef S2DAEMON_H
#define S2DAEMON_H

#include "Prerequisites.h"

class S2Daemon
{
public:
    S2Daemon(void);
    ~S2Daemon(void);

    size_t OnSendPacket(uint8_t* pkt, size_t len);
    size_t OnReceivePacket(uint8_t* pkt, size_t len);
};

extern S2Daemon* g_pDaemon;

#endif
