#include "RegistryHandler.h"

RegistryHandler::RegistryHandler ()
{
	ZeroMemory (m_szReadstring, SIZE_STRING);

	ZeroMemory (m_szSettinglocation, SIZE_STRING);
	strcpy_s (m_szSettinglocation, SIZE_STRING, "Software\\CedeSoft\\CedeCrypt");
}

RegistryHandler::~RegistryHandler ()
{
	
}

bool RegistryHandler::DoesValueExist (LPCSTR szHive, LPCSTR szValue, DWORD dwRegType) {
	HKEY hParentKey;
	long lRes = 0;
	char szRegValue[1024];
	DWORD size =  1024;
	ZeroMemory (szRegValue, 1024);
		
	lRes = RegOpenKeyEx (HKEY_LOCAL_MACHINE, szHive, 0, KEY_ALL_ACCESS, &hParentKey);
	
	if (lRes == ERROR_SUCCESS) {	
		// Reset size and buffer for reg read
		size = 1024;
		ZeroMemory (szRegValue, 1024);
		
		lRes = RegQueryValueEx(hParentKey, szValue, NULL, &dwRegType, (unsigned char*) szRegValue, &size);
		if (lRes == ERROR_SUCCESS) {
			return true;
		} else {
			return false;
		}
	} else {
		return false;
	}
}

bool RegistryHandler::DoesValueExistCU (LPCSTR szHive, LPCSTR szValue, DWORD dwRegType) {
	HKEY hParentKey;
	long lRes = 0;
	char szRegValue[1024];
	DWORD size =  1024;
	ZeroMemory (szRegValue, 1024);
		
	lRes = RegOpenKeyEx (HKEY_CURRENT_USER, szHive, 0, KEY_ALL_ACCESS, &hParentKey);
	
	if (lRes == ERROR_SUCCESS) {	
		// Reset size and buffer for reg read
		size = 1024;
		ZeroMemory (szRegValue, 1024);
		
		lRes = RegQueryValueEx(hParentKey, szValue, NULL, &dwRegType, (unsigned char*) szRegValue, &size);
		if (lRes == ERROR_SUCCESS) {
			return true;
		} else {
			return false;
		}
	} else {
		return false;
	}
}

void RegistryHandler::WriteString (LPCSTR szHive, LPCSTR szKey, LPCSTR szValue) {
	// Write a REG_SZ value to the registry.
	HKEY hParentKey;
	SECURITY_ATTRIBUTES sa = {sizeof(sa), 0, 1};
	DWORD state = 0;
	long lRes = 0;
	
	RegCreateKeyEx (HKEY_LOCAL_MACHINE, szHive, 0, (LPTSTR) NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, &sa, &hParentKey, &state);
	
	if( state == 2) /* opened existing key */
	{
		RegCloseKey (hParentKey);
		RegOpenKeyEx (HKEY_LOCAL_MACHINE, szHive, 0, KEY_WRITE, &hParentKey);
		RegSetValueEx(hParentKey, szKey, 0, REG_SZ, (BYTE *) szValue, strlen (szValue));
	} else {
		/* save them to the registry */
		RegSetValueEx(hParentKey, szKey, 0, REG_SZ, (BYTE *) szValue, strlen (szValue));
	}
}

void RegistryHandler::WriteStringCU (LPCSTR szHive, LPCSTR szKey, LPCSTR szValue) {
	// Write a REG_SZ value to the registry.
	HKEY hParentKey;
	SECURITY_ATTRIBUTES sa = {sizeof(sa), 0, 1};
	DWORD state = 0;
	long lRes = 0;
	
	RegCreateKeyEx (HKEY_CURRENT_USER, szHive, 0, (LPTSTR) NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, &sa, &hParentKey, &state);
	
	if( state == 2) /* opened existing key */
	{
		RegCloseKey (hParentKey);
		RegOpenKeyEx (HKEY_CURRENT_USER, szHive, 0, KEY_WRITE, &hParentKey);
		RegSetValueEx(hParentKey, szKey, 0, REG_SZ, (BYTE *) szValue, strlen (szValue));
	} else {
		/* save them to the registry */
		RegSetValueEx(hParentKey, szKey, 0, REG_SZ, (BYTE *) szValue, strlen (szValue));
	}
}

void RegistryHandler::WriteStringSetting (const char szSettingname[], char *szSettingValue)
{
	WriteStringCU (m_szSettinglocation, szSettingname, szSettingValue);
}

LPCSTR RegistryHandler::ReadStringSetting (const char szSettingname[])
{
	return ReadStringCU (m_szSettinglocation, szSettingname);
}

bool RegistryHandler::DoesSettingExist (const char szSettingname[])
{
	return DoesValueExistCU (m_szSettinglocation, szSettingname, REG_SZ);
}

void RegistryHandler::WriteStringCR (LPCSTR szHive, LPCSTR szKey, LPCSTR szValue) {
	// Write a REG_SZ value to the registry.
	HKEY hParentKey;
	SECURITY_ATTRIBUTES sa = {sizeof(sa), 0, 1};
	DWORD state = 0;
	long lRes = 0;
	
	RegCreateKeyEx (HKEY_CLASSES_ROOT, szHive, 0, (LPTSTR) NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, &sa, &hParentKey, &state);
	
	if( state == 2) /* opened existing key */
	{
		RegCloseKey (hParentKey);
		RegOpenKeyEx (HKEY_LOCAL_MACHINE, szHive, 0, KEY_WRITE, &hParentKey);
		RegSetValueEx(hParentKey, szKey, 0, REG_SZ, (BYTE *) szValue, strlen (szValue));
	} else {
		/* save them to the registry */
		RegSetValueEx(hParentKey, szKey, 0, REG_SZ, (BYTE *) szValue, strlen (szValue));
	}
}

LPCSTR RegistryHandler::ReadString (LPCSTR szHive, LPCSTR szKey) {
	// Function to read a REG_SZ value from the registry.
	HKEY hParentKey;
	
	ZeroMemory (m_szReadstring, SIZE_STRING);
	SECURITY_ATTRIBUTES sa = {sizeof(sa), 0, 1};
	DWORD size =  0, state = 0, type = REG_SZ;
	long lRes = 0;
	//DWORD dwErr = 0;
	RegCreateKeyEx (HKEY_LOCAL_MACHINE, szHive, 0, (LPTSTR) NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, &sa, &hParentKey, &state);
	
	if( state == 2) /* opened existing key */
	{
		RegCloseKey (hParentKey);
		RegOpenKeyEx (HKEY_LOCAL_MACHINE, szHive, 0, KEY_READ, &hParentKey);
		size = SIZE_STRING;
		lRes = RegQueryValueEx(hParentKey, szKey, NULL, &type, (unsigned char*) m_szReadstring, &size);
		if (lRes == ERROR_SUCCESS) {
			//MessageBox (NULL, "Reg function succeeded.", "Info", MB_OK);
		} else {
			if (lRes == 2L) {
				//MessageBox (NULL, "Error is 2L", "Info", MB_OK);
			} else {
				//MessageBox (NULL, "Reg function failed. Not 2L", "Info", MB_OK);
			}
		}
		if (lRes != 0) {
			RegCloseKey (hParentKey);
		} else {
			RegCloseKey (hParentKey);
		}
	} 
	return m_szReadstring;
}

LPCSTR RegistryHandler::ReadStringCU (LPCSTR szHive, LPCSTR szKey) {
	// Function to read a REG_SZ value from the registry.
	HKEY hParentKey;
	
	ZeroMemory (m_szReadstring, SIZE_STRING);
	SECURITY_ATTRIBUTES sa = {sizeof(sa), 0, 1};
	DWORD size =  0, state = 0, type = REG_SZ;
	long lRes = 0;
	//DWORD dwErr = 0;
	RegCreateKeyEx (HKEY_CURRENT_USER, szHive, 0, (LPTSTR) NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, &sa, &hParentKey, &state);
	
	if( state == 2) /* opened existing key */
	{
		RegCloseKey (hParentKey);
		RegOpenKeyEx (HKEY_CURRENT_USER, szHive, 0, KEY_READ, &hParentKey);
		size = SIZE_STRING;
		lRes = RegQueryValueEx(hParentKey, szKey, NULL, &type, (unsigned char*) m_szReadstring, &size);
		if (lRes == ERROR_SUCCESS) {
			//MessageBox (NULL, "Reg function succeeded.", "Info", MB_OK);
		} else {
			if (lRes == 2L) {
				//MessageBox (NULL, "Error is 2L", "Info", MB_OK);
			} else {
				//MessageBox (NULL, "Reg function failed. Not 2L", "Info", MB_OK);
			}
		}
		if (lRes != 0) {
			RegCloseKey (hParentKey);
		} else {
			RegCloseKey (hParentKey);
		}
	} 
	return m_szReadstring;
}

void RegistryHandler::WriteDword (LPCSTR szHive, LPCSTR szKey, DWORD dwValue) {
	// Write a REG_DWORD value to the registry
	HKEY hParentKey;
	SECURITY_ATTRIBUTES sa = {sizeof(sa), 0, 1};
	DWORD state = 0;
	long lRes = 0;
	
	RegCreateKeyEx (HKEY_LOCAL_MACHINE, szHive, 0, (LPTSTR) NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, &sa, &hParentKey, &state);
	
	if( state == 2) /* opened existing key */
	{
		RegCloseKey (hParentKey);
		RegOpenKeyEx (HKEY_LOCAL_MACHINE, szHive, 0, KEY_WRITE, &hParentKey);
		RegSetValueEx(hParentKey, szKey, 0, REG_DWORD, (BYTE *) &dwValue, sizeof (dwValue));
	} else {
		/* save them to the registry */
		RegSetValueEx(hParentKey, szKey, 0, REG_DWORD, (BYTE *) &dwValue, sizeof (dwValue));
	}
}

void RegistryHandler::WriteDwordEx (LPCSTR szKey, DWORD dwValue) {
	WriteDword ("Software\\CedeSoft\\CommunicatorClient", szKey, dwValue);
}

DWORD RegistryHandler::ReadDword (LPCSTR szHive, LPCSTR szKey) {
	// Function to read a REG_DWORD value from the registry.
	HKEY hParentKey;
	DWORD dwRegValue = 0;
	SECURITY_ATTRIBUTES sa = {sizeof(sa), 0, 1};
	DWORD size =  0, state = 0, type = REG_DWORD;
	long lRes = 0;
	
	RegCreateKeyEx (HKEY_LOCAL_MACHINE, szHive, 0, (LPTSTR) NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, &sa, &hParentKey, &state);
	
	if( state == 2) /* opened existing key */
	{
		RegCloseKey (hParentKey);
		RegOpenKeyEx (HKEY_LOCAL_MACHINE, szHive, 0, KEY_READ, &hParentKey);
		size = sizeof (DWORD);
		lRes = RegQueryValueEx(hParentKey, szKey, NULL, &type, (unsigned char*) &dwRegValue, &size);
		if (lRes != 0) {
			RegCloseKey (hParentKey);
		} else {
			RegCloseKey (hParentKey);
		}
	} 
	return dwRegValue;
}

DWORD RegistryHandler::ReadDwordEx (LPCSTR szKey)
{
	return ReadDword ("Software\\CedeSoft\\CommunicatorClient", szKey);
}

bool RegistryHandler::DoesKeyExist (LPCSTR szHive) {
	HKEY hParentKey;
	long lRes = 0;
		
	lRes = RegOpenKeyEx (HKEY_LOCAL_MACHINE, szHive, 0, KEY_ALL_ACCESS, &hParentKey);
	
	if (lRes == ERROR_SUCCESS) {	
		return true;
	} else {
		return false;
	}
}

void RegistryHandler::WriteBinary (LPCSTR szHive, LPCSTR szKey, MemoryBuffer *memBuffer)
{
	// Write a REG_BINARY value to the registry.
	HKEY hParentKey;
	SECURITY_ATTRIBUTES sa = {sizeof(sa), 0, 1};
	DWORD state = 0;
	long lRes = 0;
	
	RegCreateKeyEx (HKEY_LOCAL_MACHINE, szHive, 0, (LPTSTR) NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, &sa, &hParentKey, &state);
	
	if( state == 2) /* opened existing key */
	{
		RegCloseKey (hParentKey);
		RegOpenKeyEx (HKEY_LOCAL_MACHINE, szHive, 0, KEY_WRITE, &hParentKey);
		RegSetValueEx(hParentKey, szKey, 0, REG_BINARY, (BYTE *) memBuffer->GetBuffer (), memBuffer->GetSize ());
	} else {
		/* save them to the registry */
		RegSetValueEx(hParentKey, szKey, 0, REG_BINARY, (BYTE *) memBuffer->GetBuffer (), memBuffer->GetSize ());
	}
}

void RegistryHandler::ReadBinary (LPCSTR szHive, LPCSTR szKey, MemoryBuffer *memBuffer, unsigned int iSize)
{
	// Function to read a REG_BINARY value from the registry.
	HKEY hParentKey;

	SECURITY_ATTRIBUTES sa = {sizeof(sa), 0, 1};
	DWORD size =  0, state = 0, type = REG_BINARY;
	long lRes = 0;
	//DWORD dwErr = 0;
	RegCreateKeyEx (HKEY_LOCAL_MACHINE, szHive, 0, (LPTSTR) NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, &sa, &hParentKey, &state);
	
	if( state == 2) /* opened existing key */
	{
		RegCloseKey (hParentKey);
		RegOpenKeyEx (HKEY_LOCAL_MACHINE, szHive, 0, KEY_READ, &hParentKey);
		size = iSize;
		memBuffer->SetSize (iSize);

		lRes = RegQueryValueEx(hParentKey, szKey, NULL, &type, (BYTE *) memBuffer->GetBuffer (), &size);
		if (lRes == ERROR_SUCCESS) {
			//MessageBox (NULL, "Reg function succeeded.", "Info", MB_OK);
		} else {
			if (lRes == 2L) {
				//MessageBox (NULL, "Error is 2L", "Info", MB_OK);
			} else {
				//MessageBox (NULL, "Reg function failed. Not 2L", "Info", MB_OK);
			}
		}
		if (lRes != 0) {
			RegCloseKey (hParentKey);
		} else {
			RegCloseKey (hParentKey);
		}
	} 
}

