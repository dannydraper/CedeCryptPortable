#pragma once
#include <windows.h>
#include "resource.h"
#include "MemoryBuffer.h"
#include "InternetHandler.h"
#include "DynList.h"
#include "Wininet.h"
#include "Diagnostics.h"

class AppUpdater
{
	public:
		AppUpdater ();
		~AppUpdater ();
		void SetDiagnostics (Diagnostics *pdiag);

		bool IsUpdateCheckPossible ();
		bool CheckForUpdate ();
		bool DeserialiseVersionInfo (MemoryBuffer *memFile);
		bool IsNewVersionAvailable ();
		bool ShowNewVersionInfo ();
		bool UpdateFromInternet ();

		void OutputInt (LPCSTR lpszText, int iValue);
		void OutputText (LPCSTR lpszText);
		void OutputText (LPCSTR lpszName, LPCSTR lpszValue);

	private:
		void GetPathOnly (char *szInpath, char *szOutpath, char *szOutfile, char *szSep);
		void GetProgramFilePaths ();
		bool FileExists (char *FileName);
		bool UpdateSingleFile (char *szLocalfilename, char *szWebfile);

		Diagnostics *m_pdiag;
		bool m_bUseDiagnostics;

		char m_szWebsite[SIZE_STRING];
		char m_szVersioninfo[SIZE_STRING];

		int m_iVersioninfomax;
		int m_iCurrentversion;

		InternetHandler m_inet;

		// Version on the web
		int m_iWebversion;
		int m_iNumupdatefiles;

		char m_szLatestupdates[SIZE_STRING];

		char m_szLocalfile1[SIZE_STRING];
		char m_szWebfile1[SIZE_STRING];

		char m_szLocalfile2[SIZE_STRING];
		char m_szWebfile2[SIZE_STRING];

		char m_szLocalfile3[SIZE_STRING];
		char m_szWebfile3[SIZE_STRING];

		char m_szLocalfile4[SIZE_STRING];
		char m_szWebfile4[SIZE_STRING];


		// Current file paths
		// First the main executable paths
		char m_szCurexepathfull[SIZE_STRING];
		char m_szCurexepathonly[SIZE_STRING];
		char m_szCuroldpathfull[SIZE_STRING];
		char m_szCurfileonly[SIZE_STRING];
		char m_szCurnameonly[SIZE_STRING];
		char m_szCurextonly[SIZE_STRING];

		// Now the system executable paths
		// - the one in C:\Windows

};