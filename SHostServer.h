namespace SHostServer
{
	typedef int(*tCHostServer_GetClientNumFromAccountID)(void*, int);
	tCHostServer_GetClientNumFromAccountID pCHostServerGetClientNumFromAccountID = NULL;
	void** pHost = (void**)((uint64_t)dlsym(RTLD_NEXT, "g_pHost") - 0x62590);

	int GetClientNumFromAccountID(int accountID) {
		if(pCHostServerGetClientNumFromAccountID == NULL) {
			pCHostServerGetClientNumFromAccountID = (tCHostServer_GetClientNumFromAccountID)dlsym(RTLD_NEXT, "_ZN11CHostServer25GetClientNumFromAccountIDEi");
		}
		printf("pCHostServerGetClientNumFromAccountID %p\r\n", pCHostServerGetClientNumFromAccountID);
		if(*pHost) {
			void* pHostServerLoc = *(pHost + 0x080);
			void* pHostServer = *(void**)pHostServerLoc;
			printf("pHostServer %p\r\n", pHostServer);
			return pCHostServerGetClientNumFromAccountID(pHostServer, accountID);
		}
		return -1;
	}
};
