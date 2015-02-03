#include "S2Daemon.h"
#include "Savage.h"

S2Daemon* g_pDaemon = NULL;

static char* makeBitString(uint8_t value, char* buffer)
{
    for(int i = 0; i < 8; i++)
    {
        buffer[7-i] = (value>>i & 1) ? '1' : '0';
    }
    buffer[8] = '\0';
    return buffer;
}

static void dump(uint8_t* buf, size_t len)
{
    static const size_t kBytesPerRow = 16;

    for(size_t i = 0; i < len; i += kBytesPerRow)
    {
        for(size_t j = 0; j < kBytesPerRow && (i+j) < len; j++)
        {
            printf("%02X ", buf[i+j]);
        }
        printf("  ");
        for(size_t j = 0; j < kBytesPerRow && (i+j) < len; j++)
        {
            printf("%c", buf[i+j] >= ' ' && buf[i+j] <= '~' ? buf[i+j] : '.');
        }

        printf("\r\n");
    }
}

S2Daemon::S2Daemon(void)
{
    Savage::Execute("S2Daemon init: Echo hi!");
}
S2Daemon::~S2Daemon(void)
{
}

size_t S2Daemon::OnSendPacket(uint8_t* buf, size_t len)
{
/*
    if(len >= 8)
    {
        Savage::S2Packet pkt(buf, len);
        if(pkt.CmdId() == 0x5B)
        {
            printf("Sent cmd 0x%02X LEN = %zu;\r\n", pkt.CmdId(), pkt.Length());
            char bits[17] = {};
            makeBitString(buf[30], bits);
            makeBitString(buf[31], &bits[8]);
            printf("CTRL %s\r\n", bits);
            dump(&buf[8], len-8);
            printf("-----\r\n");
        }
    }
*/
    return len;
}

size_t S2Daemon::OnReceivePacket(uint8_t* buf, size_t len)
{
    /*
call next map vote command...
10 00 00 00
03
1e 5f
c8 5e
03 00 00 00

F3 on a next map vote..
09 00 00 00
03
1e 5f
c8 5f
*/
//printf("Received packet!!!!!!!!\n");
    if(len != 13) return len;
    if(buf[4] != 3 || buf[7] != 0x0c8 || buf[8] != 0x5e) return len;
printf("Received next map vote packet!!!!\n");
    buf[9] = 0;
    return len;
/*
    if(len >= 8)
    {
        Savage::S2Packet pkt(buf, len);
        switch(pkt.CmdId())
        {
            case Savage::ekClientSnapshot:
            {
                //printf("Received client snapshot 0x%02X\r\n", pkt.CmdId());
                //dump(buf, len);
                //printf("-----\r\n");
                
            } break;
            case Savage::ekGameData:
            {
                //printf("GAMEDATA PACKET\r\n-----\r\n");
                //dump(buf+7, len-7);
                //printf("-----\r\n");
            } break;
        }

        return pkt.Length();
    }
*/

    //if(pkt.Length() >= 8)// && pkt.ReadDWord() == Savage::kS2Magic)
    //{
    //    uint32_t seq = pkt.ReadDWord();
    //    pkt.ReadByte(); /* Packet type */
    //    uint8_t uid = pkt.ReadWord();
    //    uint8_t cmd = pkt.ReadByte();
    //    //if(cmd != Savage::ekClientSnapshot)
    //    //{
    //        printf("<< cmd 0x%02X\r\n", cmd);
    //        dump(buf, len);
    //        printf("-----\r\n");
    //    //}
    //    switch(cmd)
    //    {
    //        case Savage::ekClientSnapshot:
    //        {
    //            //printf("Received client snapshot from %X...\r\n", uid);
    //        } break;
    //        case Savage::ekGameData:
    //        {
    //            printf("GAMEDATA PACKET\r\n-----\r\n");
    //            dump(buf, len);
    //            printf("-----\r\n");
    //        } break;
    //        case Savage::ekConnect:
    //        {
    //            printf("CONNECT PACKET\r\n-----\r\n");
    //            dump(buf, len);
    //            printf("-----\r\n");
    //        } break;
    //    };
    //}

    return len;
}
