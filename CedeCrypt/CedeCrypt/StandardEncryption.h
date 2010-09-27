#pragma once
#include <stdio.h>
#include <windows.h>
#include <wincrypt.h>
#include "MemoryBuffer.h"
#include "DirScanner.h"
#include "resource.h"

#define SIZE_STRING		1024
#define SIZE_INTEGER	32
#define SIZE_NAME			64

#define PROVIDER	PROV_RSA_AES
//#define ENCRYPT_ALGORITHM CALG_AES


#define ENCRYPT_BLOCK_SIZE 16


// 112 bit Triple-DES
//#define ENCRYPT_ALGORITHM CALG_3DES
//#define KEYLENGTH  0x00A80000 // 168 bit 3DES

// 56 bit DES
//#define ENCRYPT_ALGORITHM CALG_DES
//#define KEYLENGTH  0x00380000 // 56 bit DES

// 256 bit AES
#define ENCRYPT_ALGORITHM CALG_AES_256
#define KEYLENGTH  0x01000000 // 256 bit key length



#define CRYPTRES_FAILED			100
#define CRYPTRES_NOTENCRYPTED	101
#define CRYPTRES_ENCRYPTED		102

class StandardEncryption {
	
	public:
		StandardEncryption ();
		~StandardEncryption ();		
		bool EncryptFile(PCHAR szSource, PCHAR szDestination, PCHAR szPassword, bool bEncrypt);
		bool EncryptFileEx (char *szSource, char *szDestination, char *szPassword, bool bEncrypt);
		bool EncryptFileEx2 (char *szSource, char *szDestination, char *szPassword, bool bEncrypt);
		bool EncryptFileEx3 (char *szSource, char *szDestination, char *szPassword, bool bEncrypt);
		void SetupBlankFile (char *szDestination);
		bool GetMD5Hash (char *szPassword, char *szOutbuf);

		bool EncryptBuffer (MemoryBuffer *memSource, PCHAR szPassword, bool bEncrypt);
		unsigned int IsFileEncrypted (char *szFilename);
		unsigned long GetFileSize (char *FileName);
		void SetAlgorithm (unsigned int iAlgID);

		void SetProgressHWND (HWND hwnd);

		void SetOutputHWND (HWND hwndOutput);
		void SetSpeedLabelHWND (HWND hwnd);
		void InitEncryptFile ();

		void OutputText (LPCSTR lpszText);
		void OutputInt (LPCSTR lpszText, int iValue);
		void MyHandleError(char *s);

		unsigned long long ltotalbytes;
		unsigned long long ltotalbytesprocessed;
		unsigned long m_lasttickcount;
		int m_ispeedtrigger;
		int m_iaveragetrigger;
		unsigned long m_addedspeed; // for average calculation
	private:
		bool m_bOutputwindowset;
		HWND m_hwnddiaglist;
		unsigned int m_lastprogressvalue;
		unsigned long m_lasttimereading;
		unsigned long m_lastbytesreading;

		char m_szLastaverage[SIZE_STRING];

		bool m_bmaxprogressredone;

		HWND m_hwndprogress;
		HWND m_hwndspeed;

		ALG_ID m_Currentalg;
		DWORD m_dwKeylength;

		

		DirScanner m_Dirscanner;

		unsigned long m_lMagicone;
		unsigned long m_lMagictwo;
		unsigned long m_lMagicthree;
		unsigned long m_lMagicfour;
};
