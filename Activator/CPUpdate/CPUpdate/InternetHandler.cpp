#include "InternetHandler.h"

InternetHandler::InternetHandler ()
{

}

InternetHandler::~InternetHandler ()
{

}

bool InternetHandler::DoesWebConnectionExist ()
{
	
	if (InternetCheckConnection ("http://www.cedesoft.com", FLAG_ICC_FORCE_CONNECTION, 0) == TRUE) {
		return true;
	} else {
		return false;
	}
	
	//return false;
}

bool InternetHandler::InternetFiletoMemoryBuffer (char *szUrl, MemoryBuffer *memOut, unsigned int maxsize)
{
	
	DWORD dwBytesread = 0;
	MemoryBuffer memTemp;

	HINTERNET myInternet = InternetOpen ("CedeCrypt", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, NULL);

	if (myInternet != NULL) {
		
		HINTERNET myUrl = InternetOpenUrl (myInternet, szUrl, NULL, NULL, INTERNET_FLAG_RELOAD, NULL);

		if (myUrl != NULL) {

			memTemp.SetSize (maxsize);

			if (InternetReadFile (myUrl, memTemp.GetBuffer (), maxsize, &dwBytesread) == false) {
				InternetCloseHandle (myInternet);
				memTemp.Clear ();
				return false;
			} else {
				if (dwBytesread > 0) {
					memOut->SetSize (dwBytesread);
					memOut->Write (memTemp.GetBuffer (), 0, dwBytesread);
					memTemp.Clear ();
					InternetCloseHandle (myInternet);
					return true;
				} else {
					InternetCloseHandle (myInternet);
					memTemp.Clear ();
					return false;
				}
			}
		} else {
			InternetCloseHandle (myInternet);
			return false;
		}
	} else {
		return false;
	}
	

	//return false;
}