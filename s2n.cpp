#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "Prerequisites.h"
#include "S2Daemon.h"

#ifdef ENABLE_DAEMON
typedef ssize_t(*tsendto)(int,const void*,size_t,int,const struct sockaddr*,socklen_t);
ssize_t sendto(int sockfd, const void* buf, size_t len, int flags, const struct sockaddr* dest_addr, socklen_t addrlen)
{
    static tsendto osendto = NULL;
    if(osendto == NULL)
    {
        osendto = (tsendto)dlsym(RTLD_NEXT, "sendto"); 
    }
    if(g_pDaemon != NULL)
    {
        len = g_pDaemon->OnSendPacket((uint8_t*)buf, len);
    }

    return osendto(sockfd, buf, len, flags, dest_addr, addrlen);
}

ssize_t recvfrom(int sockfd, void* buf, size_t len, int flags, struct sockaddr* src_addr, socklen_t* addrlen)
{
    ssize_t r = syscall(SYS_recvfrom, sockfd, buf, len, flags, src_addr, addrlen);

    if(g_pDaemon == NULL)
    {
        g_pDaemon = new S2Daemon();
        printf("Initialized S2Daemon.\r\n");
    }
    if(g_pDaemon != NULL && r != -1)
    {
        r = (ssize_t)g_pDaemon->OnReceivePacket((uint8_t*)buf, (size_t)r);
    }

    return r;
}

#endif

typedef struct malloc_chunk* mchunkptr;
#define INTERNAL_SIZE_T uint32_t
#define SIZE_SZ (sizeof(INTERNAL_SIZE_T))
struct malloc_chunk { INTERNAL_SIZE_T prev_size; INTERNAL_SIZE_T size; };
typedef struct malloc_chunk* mchunkptr;
#define mem2chunk(mem) ((mchunkptr)((char*)(mem) - 2*SIZE_SZ))
#define PREV_INUSE 0x1
#define IS_MMAPPED 0x2
#define NON_MAIN_AREA 0x4
#define SIZE_BITS (PREV_INUSE|IS_MMAPPED|NON_MAIN_AREA)
#define chunksize(p) ((p)->size & ~SIZE_BITS)
#define chunk_is_mmapped(p) ((p)->size & IS_MMAPPED)
typedef void(*tfree)(void*);
void free(void* ptr)
{
    static tfree ofree = NULL;
    if(ofree == NULL)
    {
        ofree = (tfree)dlsym(RTLD_NEXT, "free");
    }

    mchunkptr p = mem2chunk(ptr);
    if(ptr != NULL && chunk_is_mmapped(p))
    {
        uintptr_t block = (uintptr_t)p - p->prev_size;
        size_t total_size = p->prev_size + p->size;
        size_t pagesize = sysconf(_SC_PAGESIZE);
        if(__builtin_expect(((block | total_size) & (pagesize - 1)) != 0, 0))
        {
            FILE* f = fopen("antifree.log", "w");
            fprintf(f, "Preventing erroneous memory free (%p)\r\n", ptr);
            fclose(f);
            return;
        }
    }

    ofree(ptr);
}
