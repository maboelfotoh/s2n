namespace SHostServer
{
	typedef int(*tCHostServer_GetClientNumFromAccountID)(void*, int);
	tCHostServer_GetClientNumFromAccountID pCHostServerGetClientNumFromAccountID = NULL;
	void** pHost;

	int GetClientNumFromAccountID(int accountID) {
		if(pCHostServerGetClientNumFromAccountID == NULL) {
			pCHostServerGetClientNumFromAccountID = (tCHostServer_GetClientNumFromAccountID)dlsym(RTLD_NEXT, "_ZN11CHostServer25GetClientNumFromAccountIDEi");
			//pHost = (void**)((uint64_t)dlsym(RTLD_NEXT, "g_pHost") - 0x62590);
			pHost = (void**)((uint8_t *)pCHostServerGetClientNumFromAccountID + 0x5e5748);
		}
		printf("pCHostServerGetClientNumFromAccountID %p\r\n", pCHostServerGetClientNumFromAccountID);
		printf("pHost %p\r\n", pHost);
		if(*pHost) {
			//void* pHostServerLoc = *(pHost + 0x080);
			//void* p1 = *(void**)(*pHost); //*(void**)pHostServerLoc;
			void* p1 = *pHost;
			void* p2 = *(void**)p1;
			void* pHostServer = *(void **)((uint8_t *)p2 + 0x080);
			printf("pHostServer %p\r\n", pHostServer);
			return pCHostServerGetClientNumFromAccountID(pHostServer, accountID);
		}
		return -1;
	}
};
