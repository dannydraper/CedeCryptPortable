#pragma once
#include <windows.h>
#include <malloc.h>
#include <stdio.h>
#include "Diagnostics.h"
#include "DynList.h"
#include "DynStringList.h"
#include "SingleFileInfo.h"


#define SCAN_COMPLETE		100

class DirScanner {
	
	public:
		DirScanner ();
		~DirScanner ();		
		void OutputInt (LPCSTR lpszText, int iValue);
		void OutputText (LPCSTR lpszText);
		void OutputText (LPCSTR lpszName, LPCSTR lpszValue);

		void SetDiagnostics (Diagnostics *pdiag);
		void GetFullFolderList (char *szSrc);

		//**************************************
		void ListAllRootFilesEx (char *szSourcePath, DynList *pPathList);
		void ListAllRootFiles (char *szSourcePath, DynList *pPathList);
		void ListAllFiles (char *szSourcePath, DynList *pPathList);
		void ListAllFilesEx (char *szRootpath, char *szSourcePath, DynList *pPathList);
		void ListAllCCRFiles (char *szSourcePath, DynList *pPathList);
		void ListAllNONCCRFiles (char *szSourcePath, DynList *pPathList);
		bool IsFileCCR (char *pszFilePath);
		bool IsFolder (char *szPath);
		bool FolderExists (char *szFolderpath);
		bool FileExists (char *szFilepath);
		unsigned long FileSize (char *szFilepath);
		void CreateFolderEx (char *szFolderpath);
		void GetPathOnly (char *szInpath, char *szOutpath, char *szOutfile, char *szSep);
		void ListAllFilesWithProgress (char *szSourcepath, DynList *pPathList);
		void SetFileInfoList (DynList *pfileinfolist);
		void AddFileInfo (char *szFilepath, char *szRootdir, unsigned long lFilesize, bool IsDir);
		unsigned long long GetFolderSize (char *szSourcePath);
		unsigned long long GetFullFolderSize (char *szSourcePath);

		unsigned long long m_lfoldersize;

		void SetHWND (HWND hWnd);
		
		static DirScanner *m_pdirscanner;

		static DWORD WINAPI ScanProc (PVOID pParam);
		void DoScanThread ();
		DynList *m_pdlscanresults;

		HWND m_dlghwnd;

		HWND m_hwnd;

		HWND m_lblhwnd;

		char m_szScansourcepath[SIZE_STRING];

		DynList *m_pdlfileinfolist;

	private:
		void ShowInt (int iInttoShow);
		void ListFiles (char *szSourcePath);
		char *GetNextDir (char *szPath, char *szFilter);
		char *ScanNextSubDir ();

		// Flag indicating if we're using diagnostics
		bool m_bUseDiagnostics;
		Diagnostics *m_pdiag;

		DynStringList m_dlDoneDirs;
		DynStringList m_dlCurPath;
		DynStringList m_dlDirs;

		char m_szCompleteDir[1024];
		char m_szPassDir[1024];
		char m_szCurrentDir[1024];
		char m_szInitialDir[255];


		static INT_PTR CALLBACK DirDlgProc (HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
		//************************************


};
