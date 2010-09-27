#pragma once
#include <windows.h>
#include <malloc.h>
#include <stdio.h>
#include "MemoryBuffer.h"
#include "resource.h"

class RegistryHandler {
	
	public:
		RegistryHandler ();
		~RegistryHandler ();		
		void WriteString (LPCSTR szHive, LPCSTR szKey, LPCSTR szValue);
		void WriteStringCR (LPCSTR szHive, LPCSTR szKey, LPCSTR szValue);
		void WriteStringCU (LPCSTR szHive, LPCSTR szKey, LPCSTR szValue);

		LPCSTR ReadString (LPCSTR szHive, LPCSTR szKey);
		LPCSTR ReadStringCU (LPCSTR szHive, LPCSTR szKey);
		void WriteDword (LPCSTR szHive, LPCSTR szKey, DWORD dwValue);
		DWORD ReadDword (LPCSTR szHive, LPCSTR szKey);
		bool DoesValueExist (LPCSTR szHive, LPCSTR szValue, DWORD dwRegType);
		bool DoesValueExistCU (LPCSTR szHive, LPCSTR szValue, DWORD dwRegType);

		void WriteDwordEx (LPCSTR szKey, DWORD dwValue);
		DWORD ReadDwordEx (LPCSTR szKey);
		bool DoesKeyExist (LPCSTR szHive);		
		void WriteBinary (LPCSTR szHive, LPCSTR szKey, MemoryBuffer *memBuffer);
		void ReadBinary (LPCSTR szHive, LPCSTR szKey, MemoryBuffer *memBuffer, unsigned int iSize);
		void WriteStringSetting (const char szSettingname[], char *szSettingValue);
		LPCSTR ReadStringSetting (const char szSettingname[]);
		bool DoesSettingExist (const char szSettingname[]);

	private:

		
		// Storage for last read string
		char m_szReadstring[SIZE_STRING];
		char m_szSettinglocation[SIZE_STRING];

};