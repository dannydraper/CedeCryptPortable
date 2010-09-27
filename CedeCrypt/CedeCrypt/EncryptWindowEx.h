#pragma once
#include <windows.h>
#include "Diagnostics.h"
#include "StandardEncryption.h"
#include "SingleFileInfo.h"
#include "resource.h"

#define DECRYPT_COMPLETE		100
#define ENCRYPT_COMPLETE		200

class EncryptWindowEx
{
	public:
		EncryptWindowEx ();
		~EncryptWindowEx ();	
		
		void DoEncryption (char *szVaultpath);
		void DoDecryption ();

		void SetDiagnostics (Diagnostics *pdiag);
		void SetHWND (HWND hWnd);

		void ClearFileInfo ();
		void AddFileInfo (char *szFilepath, char *szRootdir, unsigned long lFilesize, bool IsDir);
		void AddFileInfo (char *szFilepath, unsigned long lFilesize, bool IsDir);
		void GetPathOnly (char *szInpath, char *szOutpath, char *szOutfile, char *szSep);
		void StripStartPath (char *szStartpath, char *szFullpath, char *szOutpath);
		unsigned long long GetTotalBytes ();
		DynList *GetFileInfoList ();

		void DoBlankSetup ();
		
		
		void SetPassword (char *szPassword);
		
		int m_idecrypterrorcount;

		static EncryptWindowEx *m_pwndenc;

		// DynList storing all of the File Infos
		// which have been added to encrypt
		DynList m_dlFileinfolist;


		Diagnostics *m_pdiag;
		char m_szPassword[SIZE_STRING];

		StandardEncryption m_enc;

		HWND m_hwndprogress;


		static DWORD WINAPI DecryptionProc (PVOID pParam);
		void DoDecryptionThread ();

		static DWORD WINAPI EncryptionProc (PVOID pParam);
		void DoEncryptionThread ();

		bool m_bEncryptmode;

		HWND m_hwnd;
		HWND m_dlghwnd;
		HWND m_lblspeed;
		HWND m_lblfile;
		HWND m_lbloperation;

		HBITMAP m_hbmBanner;
		bool m_bBannerloaded;

		char m_szVaultpath[SIZE_STRING];
	private:
		
		
		static INT_PTR CALLBACK ProgressDlgProc (HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
		///////////////// METHOD DEFINITIONS ///////////////////

};