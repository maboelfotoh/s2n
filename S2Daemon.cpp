#include "S2Daemon.h"
#include "Savage.h"
#include "CmdProcessor.h"
#include "CnC.h"
#include "util.h"

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
    CmdProcessor::init();
    CnC::init();
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
//select build ability (human builder)...
//0b 00 00 00 03 80 16 c8 4b 75 00
	if(len < 10) return len;

	if(*(uint32_t *)buf == 0xf197de9a && len > 38) {
		if(!strncmp((char *)&buf[8], "S2_K2_CONNECT", 13)) {
			uint32_t connId = *(uint32_t *)&buf[5] & 0x0ffff;
			uint32_t accountId = *(uint32_t *)&buf[34];
			if(CmdProcessor::disableBuildConnIdSet.find(connId) != CmdProcessor::disableBuildConnIdSet.end())
				CmdProcessor::disableBuildConnIdSet.erase(connId);
			if(CmdProcessor::disableBuildAccountIdSet.find(accountId) 
				!= CmdProcessor::disableBuildAccountIdSet.end())
				CmdProcessor::disableBuildConnIdSet.insert(std::pair<uint32_t, uint32_t>(connId, connId));

			if(CmdProcessor::userMap.find(connId) != CmdProcessor::userMap.end())
				CmdProcessor::userMap.erase(connId);
			CmdProcessor::userMap.insert(std::pair<uint32_t, User>(connId, new User()));

			return len;
		}
	}
	uint32_t connId = *(uint32_t *)&buf[5] & 0x0ffff;
	if(buf[4] == 3 && buf[7] == 0x0c8) {

		// spam check (All/Team/Squad Chat)
		if(buf[8] > 2 && buf[8] < 6) {
			//spam check
			if(len > 100) {
				User user = CmdProcessor::userMap.find(connId);
				if(!user.addChatMsg(buf)) {
					//duplicate message, kick user
					Savage::Execute("kick user \"reason\"");
					return 0;
				}
			}

		}

		switch(buf[8]) {
			case 3:
			//AllChat packet

			//limit game AllChat message length; game server cannot properly handle messages larger than 1250 bytes
			if(len > 250) {
				buf[250] = 0;
				return 251;
			}
			break;

			case 4:
			//TeamChat packet

			case 5:
			//SquadChat packet

			case 75: //0x4b
			//Builder build ability selected
			if(CmdProcessor::disableBuildConnIdSet.find(connId) == CmdProcessor::disableBuildConnIdSet.end())
				return len;
			//Connection Id found on blacklist, disable build ability
			buf[9] = 0;
			return len;

			case 94: //0x5e
			//Call vote

			switch(buf[9]) {
				case 2:
				//shuffle
				break;
				case 3:
				//next map
				break;
			}
			break;
			
		}
	}


/*
    if(len != 13) return len;
    if(buf[4] != 3 || buf[7] != 0x0c8 || buf[8] != 0x5e) return len;
printf("Received next map vote packet!!!!\n");
    buf[9] = 0;
    return len;
*/
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
