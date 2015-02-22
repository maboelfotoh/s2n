namespace CHostServer
{
	typedef int(*tCHostServer_GetClientNumFromAccountID)(int);
	tCHostServer_GetClientNumFromAccountID pCHostServerGetClientNumFromAccountID;

	int GetClientNumFromAccountID(int accountID) {
		if(pCHostServerGetClientNumFromAccountID == NULL) {
			pCHostServerGetClientNumFromAccountID = (tCHostServer_GetClientNumFromAccountID)dlsym(RTLD_NEXT, "_ZN11CHostServer25GetClientNumFromAccountIDEi");
		}
		return pCHostServerGetClientNumFromAccoutID(accountID);
	}
};
