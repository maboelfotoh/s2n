#ifndef SAVAGE_H
#define SAVAGE_H

#include "Prerequisites.h"
#include "CHostServer.h"

namespace Savage
{
    typedef void(*tCConsole_Execute)(void* pConsole, const std::string&);
    tCConsole_Execute pConsoleExecute;
    void** pConsole;
    void Execute(const std::string& cmd)
    {
        if(pConsoleExecute == NULL)
        {
            pConsoleExecute = (tCConsole_Execute)dlsym(RTLD_NEXT, "_ZN8CConsole7ExecuteERKSs");
            pConsole = (void**)((uint64_t)dlsym(RTLD_NEXT, "g_pConsole") - 0x10);
            printf("pConsoleExecute %p\r\n", pConsoleExecute);
            printf("pConsole %p\r\n", pConsole);
        }
        if(*pConsole)
        {
            pConsoleExecute(*pConsole, cmd);
        }
    }

    /* Sequence ID for "non-reliable" packets */
    const uint32_t kS2Magic = 0xF197DE9A;

    class Packet
    {
    public:
        Packet(uint8_t* buf, size_t len)
        {
            m_Buffer = buf;
            m_Length = len;
            m_Pos = m_LastMove = 0;
        } 
        ~Packet(void)
        { }

        size_t Length(void) { return m_Length; }
        void Advance(int amount)
        {
            uint32_t newPos = max(0, min(m_Length, m_Pos + amount));
            m_LastMove = (int)(newPos - m_Pos);
            m_Pos = newPos;
        }
        void Back(void)
        {
            m_Pos -= m_LastMove;
            m_LastMove = 0;
        }
        void ResetPos(void)
        {
            m_Pos = 0;
            m_LastMove = 0;
        }

        template<typename T>
        T Read(void)
        {
            assert(m_Pos + sizeof(T) <= m_Length);
            T data = *(T*)&m_Buffer[m_Pos];
            Advance(sizeof(T));
            return data;
        }
        uint32_t ReadDWord(void)
        { return Read<uint32_t>(); }
        uint16_t ReadWord(void)
        { return Read<uint16_t>(); }
        uint8_t  ReadByte(void)
        { return Read<uint8_t>(); }
        uint32_t ReadString(char* buffer, uint32_t maxlen)
        {
            uint32_t i = 0;
            do
            {
                buffer[i++] = (char)ReadByte();
            } while(buffer[i-1] != '\0' && i < maxlen);
            return i;
        }
        uint32_t ReadPackedFields(uint8_t* out, uint32_t outlen)
        {
            memset(out, 0, outlen);
            uint8_t b = ReadByte();
            if((b&1) == 0)
            {
                return 0;
            }
            out[0] |= 0x2;
            uint32_t currentBit = 1;
            bool last = false;
            for(uint32_t i = 2; i != (1 << 7); i=i*2)
            {
                if((b&i) == 1)
                {
                    if(last)
                    {
                        printf("Don't know what to do in this case (11)\r\n");
                    }
                    last = true;
                } else
                {
                    currentBit = currentBit * 2;
                    currentBit += last ? 1 : 0;
                    out[currentBit/8] |= currentBit%8;
                    last = false;
                }
            }
            return 0;
        }
        uint32_t WritePackedFields(uint8_t* data, uint32_t len)
        {
            return 0;
        }
    private:
        uint32_t m_Pos;
        uint8_t* m_Buffer;
        size_t   m_Length;
        int      m_LastMove;        
    };

    class S2Packet : public Packet
    {
    public:
        S2Packet(uint8_t* buf, size_t len) : Packet(buf, len)
        {
            m_SequenceId = ReadDWord();
            m_Type = ReadByte();
            m_SenderId = ReadWord();
            m_CmdId = ReadByte();
        }

        uint32_t SeqId()    { return m_SequenceId; }
        uint8_t  Type()     { return m_Type; }
        uint16_t SenderId() { return m_SenderId; }
        uint8_t  CmdId()    { return m_CmdId; }

    private:
        uint32_t m_SequenceId;
        uint8_t  m_Type;
        uint16_t m_SenderId;
        uint8_t  m_CmdId;
    };
    
    class S2ClientSnapshot : public S2Packet
    {
    public:
        S2ClientSnapshot(uint8_t* buf, size_t len) : S2Packet(buf, len)
        {
            m_FrameNumber = ReadDWord();
            m_Timestamp = ReadDWord();

            /* selected weapon byte */
            m_Input[0] = ReadByte();
            
            /* bit 1 = attack, bit 2 = dodge, bit 3 = block, bit 4 = W, bit 5 = S, bit 6 = A, bit 7 = D, bit 8 = ??? */
            m_Input[1] = ReadByte();
            
            /* more inputs, jump etc ... */
            m_Input[2] = ReadByte();
            m_Input[3] = ReadByte();

            /* haven't seen it not just equal 0xFF, probably related to typevector, idk */
            m_Unknown = ReadByte();

            /* bit 1 = pitch 4bytes/float, bit 2 = ??, bit 3 = yaw 4bytes/float, bit 6 = selected entity index 4bytes/uint, bit 7 = impulse (e.g. savage skill 4 or 5) 1byte */
            m_Flags = ReadByte();
        }

    private:
        uint32_t m_FrameNumber;
        uint32_t m_Timestamp;
        uint8_t  m_Input[4];
        uint8_t  m_Unknown;
        uint8_t  m_Flags;
    };

    enum ePacketType
    {
        ekDef = 0x01,
        ekRel = 0x03,
        ekAck = 0x05
    };
    enum eCommandId
    {
        ekConnect = 0xC0,
        ekNetSets = 0xC1,
        ekStateReq = 0xC2,
        ekDisconnect = 0xC3,
        ekMapReq = 0xC4,
        ekReady = 0xC5,
        ekClientSnapshot = 0xC7,
        ekGameData = 0xC8
    };
    enum eGameDataIds
    {
        ekGameDataRequestUnit = 0x01,
        ekGameDataRequestTeam = 0x02,
        ekGameDataChatAll = 0x03,
        ekGameDataChatTeam = 0x04,
        ekGameDataChatSquad = 0x05,
        ekGameDataSpendPoint = 0x07,
        ekGameDataPlaceBuilding = 0x08,
        ekGameDataOrderRepair = 0x0A,
        ekGameDataRequestSpawn = 0x0B,
        ekGameDataRequestPurchase = 0x0D,
        ekGameDataRequestCommand = 0x14,
        ekGameDataRequestSpawnAt = 0x16,
        ekGameDataRequestPersistant = 0x18,
        ekGameDataDrawOnMinimap = 0x1C,
        ekGameDataCommanderResign = 0x20,
        ekGameDataOfficerClearOrders = 0x25,
        ekGameDataOfficerRallySquad = 0x26,
        ekGameDataVoiceCommand = 0x36,
        ekGameDataExitBuild = 0x37,
        ekGameDataRequestDonate = 0x3D,
        ekGameDataSelectBuilding = 0x4B
    };
};

#endif
