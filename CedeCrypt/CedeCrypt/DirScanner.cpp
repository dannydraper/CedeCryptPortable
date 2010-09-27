#include "DirScanner.h"

DirScanner::DirScanner ()
{
	m_bUseDiagnostics = false;
}

DirScanner::~DirScanner ()
{
	
}

void DirScanner::ShowInt (int iInttoShow) {
	char szMsg[255];
	ZeroMemory (szMsg, 255);
	sprintf_s (szMsg, 255, "Value of int: %d", iInttoShow);
	MessageBox (NULL, szMsg, "ShowInt", MB_OK);
}

void DirScanner::SetHWND (HWND hWnd)
{
	//m_pwndenc = (EncryptWindowEx *) this;
	m_pdirscanner = (DirScanner *) this;
	m_hwnd = hWnd;
}

void DirScanner::SetDiagnostics (Diagnostics *pdiag)
{
	m_pdiag = pdiag;
}

// Given a source and dest path, copy the files to the dest that do not exist, but are present
// on the source.
void DirScanner::ListFiles (char *szSourcePath) {
	struct _finddata_t c_file;
	long hFile;

	char szSource[255];
	char szFilter[255];
	char szFullPath[1024];
	
	BOOL bExists;
	DynStringList dlSourceFiles;
	
	ZeroMemory (szSource, 255);
	ZeroMemory (szFilter, 255);
	ZeroMemory (szFullPath, 1024);
	
	strcpy (szSource, szSourcePath);
	strcpy (szFilter, "\\*.*");
	
	strcat (szFullPath, szSource);
	strcat (szFullPath, szFilter);
	
	OutputText ("SourcePath: ", szSourcePath);

	// Build the list of files in the source path
	if( (hFile = _findfirst(szFullPath, &c_file )) == -1L )
		OutputText ("No Files.");
	else
	{
		if ((c_file.attrib & _A_SUBDIR) != 16) {
			dlSourceFiles.AddItem (c_file.name);
			OutputText (c_file.name);
		}
		while( _findnext(hFile, &c_file ) == 0 )
		{
			if ((c_file.attrib & _A_SUBDIR) != 16) {
				dlSourceFiles.AddItem (c_file.name);
				OutputText (c_file.name);
			}
		}
		_findclose( hFile );
	}
	
	if (dlSourceFiles.GetNumItems () > 0) {
		dlSourceFiles.Clear ();
	}
}

void DirScanner::CreateFolderEx (char *szFolderpath)
{
	// Function to create every folder in a given path

	int c = 0;
	char szCurchar[SIZE_NAME];
	char szCurpath[SIZE_STRING];

	for (c=0;c<strlen (szFolderpath);c++) {
		ZeroMemory (szCurchar, SIZE_NAME);
		strncpy_s (szCurchar, SIZE_NAME, szFolderpath+c, 1);

		if (strcmp (szCurchar, "\\") == 0) {
			ZeroMemory (szCurpath, SIZE_STRING);
			strncpy_s (szCurpath, SIZE_STRING, szFolderpath, c);

			if (FolderExists (szCurpath) == false) {
				_mkdir (szCurpath);
			}

			//OutputText ("Subpath: ", szCurpath);
		}
	}

	if (FolderExists (szFolderpath) == false) {
		_mkdir (szFolderpath);
	}

	//OutputText ("Subpath: ", szFolderpath);

}

void DirScanner::GetPathOnly (char *szInpath, char *szOutpath, char *szOutfile, char *szSep)
{
	char szCurchar[SIZE_NAME];
	int seploc = 0;

	for (int c=strlen(szInpath);c>0;c--) {
		
		ZeroMemory (szCurchar, SIZE_NAME);
		strncpy_s (szCurchar, SIZE_NAME, szInpath+c, 1);

		if (strcmp (szCurchar, szSep) == 0) {
			seploc = c+1;
			break;
		}
	}

	strncpy_s (szOutpath, SIZE_STRING, szInpath, seploc);
	strncpy_s (szOutfile, SIZE_STRING, szInpath+seploc, strlen (szInpath)-seploc);
}

// Directory recursion function. Get the next unscanned dir given by szPath
char *DirScanner::GetNextDir (char *szPath, char *szFilter) {
	
	struct _finddata_t c_file;
	long hFile;
	
	char szFullPath[1024];
	ZeroMemory (szFullPath, 1024);
	
	char szDirPath[1024];
	ZeroMemory (szDirPath, 1024);
	
	strcat (szFullPath, szPath);
	strcat (szFullPath, szFilter);
	
	if( (hFile = _findfirst(szFullPath, &c_file )) == -1L )
		return "";
	else
	{
		if ((c_file.attrib & _A_SUBDIR) == 16) {
			strcat (szDirPath, szPath);
			strcat (szDirPath, "\\");
			strcat (szDirPath, c_file.name);
			
			if (m_dlDoneDirs.DoesExist (szDirPath) == FALSE) {
				m_dlDoneDirs.AddItem (szDirPath);
				return c_file.name;
			}
		}
		
		while( _findnext(hFile, &c_file ) == 0 )
		{
			if ((c_file.attrib & _A_SUBDIR) == 16) {
				ZeroMemory (szDirPath, 1024);
				strcat (szDirPath, szPath);
				strcat (szDirPath, "\\");
				strcat (szDirPath, c_file.name);
				
				if (m_dlDoneDirs.DoesExist (szDirPath) == FALSE) {
					m_dlDoneDirs.AddItem (szDirPath);
					return c_file.name;
				} 
				
			}
		}

		_findclose( hFile );
	}
	return "";
}

char *DirScanner::ScanNextSubDir () {
	int p;
	char szRetstring[1024];
	ZeroMemory (szRetstring, 1024);
	
	ZeroMemory (m_szCompleteDir, 1024);

	for (p=0;p<m_dlCurPath.GetNumItems ();p++) {
		strcat (m_szCompleteDir, m_dlCurPath.GetItem (p));
		strcat (m_szCompleteDir, "\\");
	}
	
	strcpy (szRetstring, m_szCompleteDir);
	
	ZeroMemory (m_szPassDir, 1024);
	strcpy (m_szPassDir, m_szCompleteDir);
	
	ZeroMemory (m_szCurrentDir, 1024);
	strcpy (m_szCurrentDir,GetNextDir (m_szPassDir, "*.*"));
	while (strcmp (m_szCurrentDir, ".") == 0 || strcmp (m_szCurrentDir, "..") == 0) {
		ZeroMemory (m_szCurrentDir, 1024);
		strcpy (m_szCurrentDir,GetNextDir (m_szPassDir, "*.*"));
	}
	
	if (strcmp (m_szCurrentDir, "") != 0) {	
		m_dlCurPath.AddItem (m_szCurrentDir);
	} else {
		if (m_dlCurPath.GetNumItems () > 0) {
			m_dlCurPath.RemoveLastItem ();
			ZeroMemory (m_szCurrentDir, 1024);
			ZeroMemory (m_szCompleteDir, 1024);
		}
	}
	
	return szRetstring;
}

void DirScanner::GetFullFolderList (char *szSrc) {
	int count = 0;
	ZeroMemory (m_szInitialDir, 255);
	ZeroMemory (m_szCurrentDir, 1024);
	ZeroMemory (m_szCompleteDir, 1024);
	ZeroMemory (m_szPassDir, 1024);
		
	strcpy (m_szInitialDir, szSrc);
	m_dlCurPath.AddItem (m_szInitialDir);
	
	char szSubDir[1024];
	ZeroMemory (szSubDir, 1024);
	
	char szMsg[92];
	ZeroMemory (szMsg, 92);

	strcpy (szSubDir, ScanNextSubDir ());
	
	
	while (strcmp (szSubDir, "") != 0) {
		
		if (m_dlDirs.DoesExist (szSubDir) == FALSE) {
												
			// Synchronise from Source to Dest
			ListFiles (szSubDir);
			m_dlDirs.AddItem (szSubDir);
		}
		
		strcpy (szSubDir, ScanNextSubDir ());
		count ++;

		if (count > 1000) {
			return;
		}
	}
	
	//OutputText (szSubDir);

	m_dlDoneDirs.Clear ();
	m_dlCurPath.Clear ();
	m_dlDirs.Clear ();
}

bool DirScanner::FolderExists (char *szFolderpath)
{
	struct _finddata_t c_file;
	long hFile;
	
	if( (hFile = _findfirst(szFolderpath, &c_file )) == -1L )
		OutputText ("No Files.");		
	else
	{
		_findclose( hFile );
		return true;				
	}

	return false;
}

bool DirScanner::FileExists (char *szFilepath)
{
	struct _finddata_t c_file;
	long hFile;
	
	if( (hFile = _findfirst(szFilepath, &c_file )) == -1L )
		OutputText ("No Files.");		
	else
	{
		_findclose( hFile );
		return true;				
	}

	return false;
}

unsigned long DirScanner::FileSize (char *szFilepath)
{
	struct _finddata_t c_file;
	long hFile;
	unsigned long lsize = 0;
	
	if( (hFile = _findfirst(szFilepath, &c_file )) == -1L )
		OutputText ("No Files.");		
	else
	{
		lsize = c_file.size;
		_findclose( hFile );
		return lsize;				
	}

	return lsize;
}

bool DirScanner::IsFolder (char *szPath)
{
	struct _finddata_t c_file;
	long hFile;
	
	if( (hFile = _findfirst(szPath, &c_file )) == -1L )
		OutputText ("No Files.");
	else
	{
		if ((c_file.attrib & _A_SUBDIR) != 16) {
			_findclose( hFile );
			return false;
		} else {
			_findclose( hFile );
			return true;
		}

		_findclose( hFile );
	}

	return false;
}

DWORD WINAPI DirScanner::ScanProc (PVOID pParam) {

	int r = 0;
	SingleFileInfo *psubinfo;

	char szAddmsg[SIZE_STRING];

	m_pdirscanner->ListAllFilesEx  (m_pdirscanner->m_szScansourcepath, m_pdirscanner->m_szScansourcepath, m_pdirscanner->m_pdlscanresults);

	//for (r=0;r<m_pdirscanner->m_pdlscanresults->GetNumItems ();r++) {
				
	//	psubinfo = (SingleFileInfo *) m_pdirscanner->m_pdlscanresults->GetItem (r);

		//ZeroMemory (szCurfilepath, SIZE_STRING);
		//strcpy_s (szCurfilepath, SIZE_STRING, (char *) dlAllfiles.GetItem (r));

		//m_pdiag->OutputText ("ScannedFile: ", szCurfilepath);
		
		// Add the item to the encryption window list
//		m_pdirscanner->AddFileInfo (psubinfo->szFilepath, m_pdirscanner->m_szScansourcepath, psubinfo->lFilesize, true);

//		ZeroMemory (szAddmsg, SIZE_STRING);
//		sprintf_s (szAddmsg, "Processing File %i of %i...", r, m_pdirscanner->m_pdlscanresults->GetNumItems ());

//		SetDlgItemText (m_pdirscanner->m_dlghwnd, ID_FOLDERLABEL, szAddmsg);
//	}

	PostMessage (m_pdirscanner->m_dlghwnd, SCAN_COMPLETE, 0, 0);

	return 0;
}

void DirScanner::DoScanThread ()
{
	// MULTI-THREADED FUNCTION
	// Spawns the first encryption thread.
	HANDLE hThread;
	DWORD dwThreadID;

	hThread = CreateThread (NULL, 0, ScanProc, NULL, 0, &dwThreadID);
}


INT_PTR CALLBACK DirScanner::DirDlgProc (HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch (Message)
	{

		case SCAN_COMPLETE:
		{
			//EndDialog (hWnd, IDOK);
			SetTimer (hWnd, IDT_DLGWAITCHECK, 500, NULL);
		}
		break;

		case WM_INITDIALOG:
		{
			m_pdirscanner->m_dlghwnd = hWnd;

			HFONT hfDefault = (HFONT) GetStockObject (DEFAULT_GUI_FONT);

			m_pdirscanner->m_lblhwnd = CreateWindow ("static", "Scanning Folder structure...", WS_CHILD | WS_VISIBLE, 22, 20, 550, 51, hWnd, (HMENU) ID_FOLDERLABEL, GetModuleHandle (NULL), NULL);
			SendMessage (m_pdirscanner->m_lblhwnd, WM_SETFONT, (WPARAM) hfDefault, MAKELPARAM (FALSE, 0));

			//m_pwndenc->m_lblspeed = CreateWindow ("static", "Calculating speed...", WS_CHILD | WS_VISIBLE, 22, 20, 550, 21, hWnd, (HMENU) ID_LBLSPEED, GetModuleHandle (NULL), NULL);
			//SendMessage (m_pwndenc->m_lblspeed, WM_SETFONT, (WPARAM) hfDefault, MAKELPARAM (FALSE, 0));

			//m_pwndenc->m_lblfile = CreateWindow ("static", "Working...", WS_CHILD | WS_VISIBLE, 22, 40, 550, 21, hWnd, (HMENU) ID_LBLFILE, GetModuleHandle (NULL), NULL);
			//SendMessage (m_pwndenc->m_lblfile, WM_SETFONT, (WPARAM) hfDefault, MAKELPARAM (FALSE, 0));

			//m_pwndenc->m_lbloperation = CreateWindow ("static", "Cipher in progress...", WS_CHILD | WS_VISIBLE, 22, 60, 550, 21, hWnd, (HMENU) ID_LBLOPERATION, GetModuleHandle (NULL), NULL);
			//SendMessage (m_pwndenc->m_lbloperation, WM_SETFONT, (WPARAM) hfDefault, MAKELPARAM (FALSE, 0));

			//SetTimer (hWnd, IDT_DLGWAITCHECK, 500, NULL);

			m_pdirscanner->DoScanThread ();
		}
		break;
		case WM_TIMER:
		{
			switch (wParam)
			{
				case IDT_DLGWAITCHECK:
				{					
					EndDialog (hWnd, IDOK);					
				}
				break;
			}
			break;
		}
		break;
		case WM_COMMAND:
		{
			switch (LOWORD (wParam))
			{
				case IDOK:
				{
				}
				break;
				case IDCANCEL:
				{
					EndDialog (hWnd, IDCANCEL);
				}
				break;
			}
			break;
		}
		break;
		default:
			return FALSE;
	}
	return TRUE;
}

void DirScanner::AddFileInfo (char *szFilepath, char *szRootdir, unsigned long lFilesize, bool IsDir)
{
	SingleFileInfo fiCurrent;

	ZeroMemory (fiCurrent.szFilepath, SIZE_STRING);
	strcpy_s (fiCurrent.szFilepath, SIZE_STRING, szFilepath);

	ZeroMemory (fiCurrent.szRootdir, SIZE_STRING);
	strcpy_s (fiCurrent.szRootdir, SIZE_STRING, szRootdir);

	fiCurrent.lFilesize = lFilesize;

	fiCurrent.bIsDir = IsDir;

	m_pdlfileinfolist->AddItem (&fiCurrent, sizeof (SingleFileInfo), false);

	//m_dlFileinfolist.AddItem ((SingleFileInfo *) pinfo, sizeof (SingleFileInfo), false);
}

void DirScanner::SetFileInfoList (DynList *pfileinfolist)
{
	m_pdlfileinfolist = pfileinfolist;
}

void DirScanner::ListAllFilesWithProgress (char *szSourcepath, DynList *pPathList)
{
	int iRet = 0;

	ZeroMemory (m_szScansourcepath, SIZE_STRING);
	strcpy_s (m_szScansourcepath, SIZE_STRING, szSourcepath);

	m_pdlscanresults = pPathList;  // Set the output dynlist pointer

	iRet = DialogBox (GetModuleHandle (NULL), MAKEINTRESOURCE (IDD_DIRDIALOG), m_hwnd, DirDlgProc);
	
}

void DirScanner::ListAllFiles (char *szSourcePath, DynList *pPathList) {
	struct _finddata_t c_file;
	long hFile;

	char szSource[SIZE_STRING];
	char szFilter[SIZE_STRING];
	char szFullPath[SIZE_STRING];
	char szDirectoryPath[SIZE_STRING];
	CHAR szFullFilePath[SIZE_STRING];

	//BOOL bExists;
	//DynStringList dlSourceFiles;
	//pPathList->Clear ();

	ZeroMemory (szSource, SIZE_STRING);
	ZeroMemory (szFilter, SIZE_STRING);
	ZeroMemory (szFullPath, SIZE_STRING);
	
	if (strncmp (szSourcePath+strlen(szSourcePath)-1, "\\", 1) == 0) {
		strncpy_s (szSource, SIZE_STRING, szSourcePath, strlen(szSourcePath)-1);
	} else {
		strcpy_s (szSource, SIZE_STRING, szSourcePath);
	}

	strcpy (szFilter, "\\*.*");
	
	strcat (szFullPath, szSource);
	strcat (szFullPath, szFilter);
	
	//OutputText ("SourcePath: ", szSourcePath);

	// Build the list of files in the source path
	if( (hFile = _findfirst(szFullPath, &c_file )) == -1L )
		OutputText ("No Files.");
	else
	{
		if ((c_file.attrib & _A_SUBDIR) != 16) {
			//dlSourceFiles.AddItem (c_file.name);
			//OutputText (c_file.name, ": AFILE" );

			ZeroMemory (szFullFilePath, SIZE_STRING);
			strcpy_s (szFullFilePath, SIZE_STRING, szSourcePath);
			strcat_s (szFullFilePath, SIZE_STRING, "\\");
			strcat_s (szFullFilePath, SIZE_STRING, c_file.name);
			//OutputText (szFullFilePath);

			SingleFileInfo fi1;			
			strcpy_s (fi1.szName, SIZE_STRING, c_file.name);
			strcpy_s (fi1.szFilepath, SIZE_STRING, szFullFilePath);
			fi1.lFilesize = c_file.size;	

			pPathList->AddItem (&fi1, sizeof (SingleFileInfo), false);			

		} else {
			if (strcmp (c_file.name, ".") != 0 && strcmp (c_file.name, "..") != 0) {
				//OutputText (c_file.name, ": ADIRECTORY");

				ZeroMemory (szDirectoryPath, SIZE_STRING);
				strcpy_s (szDirectoryPath, SIZE_STRING, szSourcePath);
				strcat_s (szDirectoryPath, SIZE_STRING, "\\");
				strcat_s (szDirectoryPath, SIZE_STRING, c_file.name);
				
				SetDlgItemText (m_pdirscanner->m_dlghwnd, ID_FOLDERLABEL, szDirectoryPath);
				
				ListAllFiles (szDirectoryPath, pPathList);
			}
		}
		while( _findnext(hFile, &c_file ) == 0 )
		{
			if ((c_file.attrib & _A_SUBDIR) != 16) {			
				//OutputText (c_file.name, ": AFILE");

				ZeroMemory (szFullFilePath, SIZE_STRING);
				strcpy_s (szFullFilePath, SIZE_STRING, szSourcePath);
				strcat_s (szFullFilePath, SIZE_STRING, "\\");
				strcat_s (szFullFilePath, SIZE_STRING, c_file.name);
				//OutputText (szFullFilePath);

				SingleFileInfo fi2;			
				strcpy_s (fi2.szName, SIZE_STRING, c_file.name);
				strcpy_s (fi2.szFilepath, SIZE_STRING, szFullFilePath);
				fi2.lFilesize = c_file.size;

				pPathList->AddItem (&fi2, sizeof (SingleFileInfo), false);
			} else {
				if (strcmp (c_file.name, ".") != 0 && strcmp (c_file.name, "..") != 0) {
					//OutputText (c_file.name, ": ADIRECTORY");

					ZeroMemory (szDirectoryPath, SIZE_STRING);
					strcpy_s (szDirectoryPath, SIZE_STRING, szSourcePath);
					strcat_s (szDirectoryPath, SIZE_STRING, "\\");
					strcat_s (szDirectoryPath, SIZE_STRING, c_file.name);

					SetDlgItemText (m_pdirscanner->m_dlghwnd, ID_FOLDERLABEL, szDirectoryPath);

					ListAllFiles (szDirectoryPath, pPathList);
				}
			}
		}
		_findclose( hFile );
	}
	
	//if (dlSourceFiles.GetNumItems () > 0) {
	//	dlSourceFiles.Clear ();
	//}
}

unsigned long long DirScanner::GetFullFolderSize (char *szSourcePath) {
	m_lfoldersize = 0;
	GetFolderSize (szSourcePath);

	return m_lfoldersize;
}

unsigned long long DirScanner::GetFolderSize (char *szSourcePath) {
	struct _finddata_t c_file;
	long hFile;

	char szSource[SIZE_STRING];
	char szFilter[SIZE_STRING];
	char szFullPath[SIZE_STRING];
	char szDirectoryPath[SIZE_STRING];
	CHAR szFullFilePath[SIZE_STRING];

	//BOOL bExists;
	//DynStringList dlSourceFiles;
	//pPathList->Clear ();

	ZeroMemory (szSource, SIZE_STRING);
	ZeroMemory (szFilter, SIZE_STRING);
	ZeroMemory (szFullPath, SIZE_STRING);
	
	if (strncmp (szSourcePath+strlen(szSourcePath)-1, "\\", 1) == 0) {
		strncpy_s (szSource, SIZE_STRING, szSourcePath, strlen(szSourcePath)-1);
	} else {
		strcpy_s (szSource, SIZE_STRING, szSourcePath);
	}

	strcpy (szFilter, "\\*.*");
	
	strcat (szFullPath, szSource);
	strcat (szFullPath, szFilter);
	
	//OutputText ("SourcePath: ", szSourcePath);

	// Build the list of files in the source path
	if( (hFile = _findfirst(szFullPath, &c_file )) == -1L )
		OutputText ("No Files.");
	else
	{
		if ((c_file.attrib & _A_SUBDIR) != 16) {
			m_lfoldersize += c_file.size;

		} else {
			if (strcmp (c_file.name, ".") != 0 && strcmp (c_file.name, "..") != 0) {
				//OutputText (c_file.name, ": ADIRECTORY");

				ZeroMemory (szDirectoryPath, SIZE_STRING);
				strcpy_s (szDirectoryPath, SIZE_STRING, szSourcePath);
				strcat_s (szDirectoryPath, SIZE_STRING, "\\");
				strcat_s (szDirectoryPath, SIZE_STRING, c_file.name);
											
				GetFolderSize (szDirectoryPath);
			}
		}
		while( _findnext(hFile, &c_file ) == 0 )
		{
			if ((c_file.attrib & _A_SUBDIR) != 16) {			
				m_lfoldersize += c_file.size;

			} else {
				if (strcmp (c_file.name, ".") != 0 && strcmp (c_file.name, "..") != 0) {
					//OutputText (c_file.name, ": ADIRECTORY");

					ZeroMemory (szDirectoryPath, SIZE_STRING);
					strcpy_s (szDirectoryPath, SIZE_STRING, szSourcePath);
					strcat_s (szDirectoryPath, SIZE_STRING, "\\");
					strcat_s (szDirectoryPath, SIZE_STRING, c_file.name);
				
					GetFolderSize (szDirectoryPath);
				}
			}
		}
		_findclose( hFile );
	}
	return m_lfoldersize;
}


void DirScanner::ListAllFilesEx (char *szRootpath, char *szSourcePath, DynList *pPathList) {
	struct _finddata_t c_file;
	long hFile;

	char szSource[SIZE_STRING];
	char szFilter[SIZE_STRING];
	char szFullPath[SIZE_STRING];
	char szDirectoryPath[SIZE_STRING];
	CHAR szFullFilePath[SIZE_STRING];

	//BOOL bExists;
	//DynStringList dlSourceFiles;
	//pPathList->Clear ();

	ZeroMemory (szSource, SIZE_STRING);
	ZeroMemory (szFilter, SIZE_STRING);
	ZeroMemory (szFullPath, SIZE_STRING);
	
	if (strncmp (szSourcePath+strlen(szSourcePath)-1, "\\", 1) == 0) {
		strncpy_s (szSource, SIZE_STRING, szSourcePath, strlen(szSourcePath)-1);
	} else {
		strcpy_s (szSource, SIZE_STRING, szSourcePath);
	}

	strcpy (szFilter, "\\*.*");
	
	strcat (szFullPath, szSource);
	strcat (szFullPath, szFilter);
	
	//OutputText ("SourcePath: ", szSourcePath);

	// Build the list of files in the source path
	if( (hFile = _findfirst(szFullPath, &c_file )) == -1L )
		OutputText ("No Files.");
	else
	{
		if ((c_file.attrib & _A_SUBDIR) != 16) {
			//dlSourceFiles.AddItem (c_file.name);
			//OutputText (c_file.name, ": AFILE" );

			ZeroMemory (szFullFilePath, SIZE_STRING);
			strcpy_s (szFullFilePath, SIZE_STRING, szSourcePath);
			strcat_s (szFullFilePath, SIZE_STRING, "\\");
			strcat_s (szFullFilePath, SIZE_STRING, c_file.name);
			//OutputText (szFullFilePath);

			SingleFileInfo fi1;			
			strcpy_s (fi1.szName, SIZE_STRING, c_file.name);
			strcpy_s (fi1.szFilepath, SIZE_STRING, szFullFilePath);
			fi1.lFilesize = c_file.size;	
			strcpy_s (fi1.szRootdir, SIZE_STRING, szRootpath);

			fi1.bIsDir = true;

			SetDlgItemText (m_pdirscanner->m_dlghwnd, ID_FOLDERLABEL, szFullFilePath);

			pPathList->AddItem (&fi1, sizeof (SingleFileInfo), false);			

		} else {
			if (strcmp (c_file.name, ".") != 0 && strcmp (c_file.name, "..") != 0) {
				//OutputText (c_file.name, ": ADIRECTORY");

				ZeroMemory (szDirectoryPath, SIZE_STRING);
				strcpy_s (szDirectoryPath, SIZE_STRING, szSourcePath);
				strcat_s (szDirectoryPath, SIZE_STRING, "\\");
				strcat_s (szDirectoryPath, SIZE_STRING, c_file.name);
				
				SetDlgItemText (m_pdirscanner->m_dlghwnd, ID_FOLDERLABEL, szDirectoryPath);
				
				ListAllFilesEx (szRootpath, szDirectoryPath, pPathList);
			}
		}
		while( _findnext(hFile, &c_file ) == 0 )
		{
			if ((c_file.attrib & _A_SUBDIR) != 16) {			
				//OutputText (c_file.name, ": AFILE");

				ZeroMemory (szFullFilePath, SIZE_STRING);
				strcpy_s (szFullFilePath, SIZE_STRING, szSourcePath);
				strcat_s (szFullFilePath, SIZE_STRING, "\\");
				strcat_s (szFullFilePath, SIZE_STRING, c_file.name);
				//OutputText (szFullFilePath);

				SingleFileInfo fi2;			
				strcpy_s (fi2.szName, SIZE_STRING, c_file.name);
				strcpy_s (fi2.szFilepath, SIZE_STRING, szFullFilePath);
				fi2.lFilesize = c_file.size;
				strcpy_s (fi2.szRootdir, SIZE_STRING, szRootpath);

				fi2.bIsDir = true;

				SetDlgItemText (m_pdirscanner->m_dlghwnd, ID_FOLDERLABEL, szFullFilePath);

				pPathList->AddItem (&fi2, sizeof (SingleFileInfo), false);
			} else {
				if (strcmp (c_file.name, ".") != 0 && strcmp (c_file.name, "..") != 0) {
					//OutputText (c_file.name, ": ADIRECTORY");

					ZeroMemory (szDirectoryPath, SIZE_STRING);
					strcpy_s (szDirectoryPath, SIZE_STRING, szSourcePath);
					strcat_s (szDirectoryPath, SIZE_STRING, "\\");
					strcat_s (szDirectoryPath, SIZE_STRING, c_file.name);

					SetDlgItemText (m_pdirscanner->m_dlghwnd, ID_FOLDERLABEL, szDirectoryPath);

					ListAllFilesEx (szRootpath, szDirectoryPath, pPathList);
				}
			}
		}
		_findclose( hFile );
	}
	
	//if (dlSourceFiles.GetNumItems () > 0) {
	//	dlSourceFiles.Clear ();
	//}
}

void DirScanner::ListAllRootFilesEx (char *szSourcePath, DynList *pPathList) {
	struct _finddata_t c_file;
	long hFile;
	double fSize = 0;

	char szSource[SIZE_STRING];
	char szFilter[SIZE_STRING];
	char szFullPath[SIZE_STRING];
	char szDirectoryPath[SIZE_STRING];
	CHAR szFullFilePath[SIZE_STRING];

	//BOOL bExists;
	//DynStringList dlSourceFiles;
	//pPathList->Clear ();

	ZeroMemory (szSource, SIZE_STRING);
	ZeroMemory (szFilter, SIZE_STRING);
	ZeroMemory (szFullPath, SIZE_STRING);
	

	if (strncmp (szSourcePath+strlen(szSourcePath)-1, "\\", 1) == 0) {
		strncpy_s (szSource, SIZE_STRING, szSourcePath, strlen(szSourcePath)-1);
	} else {
		strcpy_s (szSource, SIZE_STRING, szSourcePath);
	}

	strcpy (szFilter, "\\*.*");
	
	strcat (szFullPath, szSource);
	strcat (szFullPath, szFilter);
	
	//OutputText ("SourcePath: ", szSourcePath);

	// Build the list of files in the source path
	if( (hFile = _findfirst(szFullPath, &c_file )) == -1L )
		OutputText ("No Files.");
	else
	{
		if ((c_file.attrib & _A_SUBDIR) == 16) {
			//dlSourceFiles.AddItem (c_file.name);
			//OutputText (c_file.name, ": AFILE" );
			if (strcmp (c_file.name, ".") != 0 && strcmp (c_file.name, "..") != 0) {
				ZeroMemory (szFullFilePath, SIZE_STRING);
				strcpy_s (szFullFilePath, SIZE_STRING, szSourcePath);
				strcat_s (szFullFilePath, SIZE_STRING, "\\");
				strcat_s (szFullFilePath, SIZE_STRING, c_file.name);
				//OutputText (szFullFilePath);

				SingleFileInfo fi1;			
				strcpy_s (fi1.szName, SIZE_STRING, c_file.name);
				strcpy_s (fi1.szFilepath, SIZE_STRING, szFullFilePath);
				fi1.lFilesize = c_file.size;			

				fSize = (double) c_file.size / 1000;
				sprintf_s (fi1.szFilesize, SIZE_NAME, "%.2f KB", fSize);

				//ltoa (c_file.size, fi1.szFilesize, 10);

				fi1.bIsDir = true;

				pPathList->AddItem (&fi1, sizeof (SingleFileInfo), false);
			}

		} 

		while( _findnext(hFile, &c_file ) == 0 )
		{
			if ((c_file.attrib & _A_SUBDIR) == 16) {			
				//OutputText (c_file.name, ": AFILE");

				if (strcmp (c_file.name, ".") != 0 && strcmp (c_file.name, "..") != 0) {				
					ZeroMemory (szFullFilePath, SIZE_STRING);
					strcpy_s (szFullFilePath, SIZE_STRING, szSourcePath);
					strcat_s (szFullFilePath, SIZE_STRING, "\\");
					strcat_s (szFullFilePath, SIZE_STRING, c_file.name);
					//OutputText (szFullFilePath);

					SingleFileInfo fi3;
					strcpy_s (fi3.szName, SIZE_STRING, c_file.name);
					strcpy_s (fi3.szFilepath, SIZE_STRING, szFullFilePath);
					fi3.lFilesize = c_file.size;

					fSize = (double) c_file.size / 1000;
					sprintf_s (fi3.szFilesize, SIZE_NAME, "%.2f KB", fSize);

					//ltoa (c_file.size, fi3.szFilesize, 10);

					fi3.bIsDir = true;

					pPathList->AddItem (&fi3, sizeof (SingleFileInfo), false);				
				}
			} 
		}
		_findclose( hFile );
	}
	

	if( (hFile = _findfirst(szFullPath, &c_file )) == -1L )
		OutputText ("No Files.");
	else
	{
		if ((c_file.attrib & _A_SUBDIR) != 16) {
			//dlSourceFiles.AddItem (c_file.name);
			//OutputText (c_file.name, ": AFILE" );

			ZeroMemory (szFullFilePath, SIZE_STRING);
			strcpy_s (szFullFilePath, SIZE_STRING, szSourcePath);
			strcat_s (szFullFilePath, SIZE_STRING, "\\");
			strcat_s (szFullFilePath, SIZE_STRING, c_file.name);
			//OutputText (szFullFilePath);

			SingleFileInfo fi2;			
			strcpy_s (fi2.szName, SIZE_STRING, c_file.name);
			strcpy_s (fi2.szFilepath, SIZE_STRING, szFullFilePath);
			fi2.lFilesize = c_file.size;			

			fSize = (double) c_file.size / 1000;
			sprintf_s (fi2.szFilesize, SIZE_NAME, "%.2f KB", fSize);

			//ltoa (c_file.size, fi1.szFilesize, 10);

			fi2.bIsDir = false;

			pPathList->AddItem (&fi2, sizeof (SingleFileInfo), false);

		} 

		while( _findnext(hFile, &c_file ) == 0 )
		{
			if ((c_file.attrib & _A_SUBDIR) != 16) {			
				//OutputText (c_file.name, ": AFILE");

				ZeroMemory (szFullFilePath, SIZE_STRING);
				strcpy_s (szFullFilePath, SIZE_STRING, szSourcePath);
				strcat_s (szFullFilePath, SIZE_STRING, "\\");
				strcat_s (szFullFilePath, SIZE_STRING, c_file.name);
				//OutputText (szFullFilePath);

				SingleFileInfo fi4;
				strcpy_s (fi4.szName, SIZE_STRING, c_file.name);
				strcpy_s (fi4.szFilepath, SIZE_STRING, szFullFilePath);
				fi4.lFilesize = c_file.size;

				fSize = (double) c_file.size / 1000;
				sprintf_s (fi4.szFilesize, SIZE_NAME, "%.2f KB", fSize);

				//ltoa (c_file.size, fi3.szFilesize, 10);

				fi4.bIsDir = false;

				pPathList->AddItem (&fi4, sizeof (SingleFileInfo), false);				
			} 
		}
		_findclose( hFile );
	}

	//if (dlSourceFiles.GetNumItems () > 0) {
	//	dlSourceFiles.Clear ();
	//}
}

void DirScanner::ListAllRootFiles (char *szSourcePath, DynList *pPathList) {
	struct _finddata_t c_file;
	long hFile;
	double fSize = 0;

	char szSource[SIZE_STRING];
	char szFilter[SIZE_STRING];
	char szFullPath[SIZE_STRING];
	char szDirectoryPath[SIZE_STRING];
	CHAR szFullFilePath[SIZE_STRING];

	//BOOL bExists;
	//DynStringList dlSourceFiles;
	//pPathList->Clear ();

	ZeroMemory (szSource, SIZE_STRING);
	ZeroMemory (szFilter, SIZE_STRING);
	ZeroMemory (szFullPath, SIZE_STRING);
	

	if (strncmp (szSourcePath+strlen(szSourcePath)-1, "\\", 1) == 0) {
		strncpy_s (szSource, SIZE_STRING, szSourcePath, strlen(szSourcePath)-1);
	} else {
		strcpy_s (szSource, SIZE_STRING, szSourcePath);
	}

	strcpy (szFilter, "\\*.*");
	
	strcat (szFullPath, szSource);
	strcat (szFullPath, szFilter);
	
	//OutputText ("SourcePath: ", szSourcePath);

	// Build the list of files in the source path
	if( (hFile = _findfirst(szFullPath, &c_file )) == -1L )
		OutputText ("No Files.");
	else
	{
		if ((c_file.attrib & _A_SUBDIR) != 16) {
			//dlSourceFiles.AddItem (c_file.name);
			//OutputText (c_file.name, ": AFILE" );

			ZeroMemory (szFullFilePath, SIZE_STRING);
			strcpy_s (szFullFilePath, SIZE_STRING, szSourcePath);
			strcat_s (szFullFilePath, SIZE_STRING, "\\");
			strcat_s (szFullFilePath, SIZE_STRING, c_file.name);
			//OutputText (szFullFilePath);

			SingleFileInfo fi1;			
			strcpy_s (fi1.szName, SIZE_STRING, c_file.name);
			strcpy_s (fi1.szFilepath, SIZE_STRING, szFullFilePath);
			fi1.lFilesize = c_file.size;			

			fSize = (double) c_file.size / 1000;
			sprintf_s (fi1.szFilesize, SIZE_NAME, "%.2f KB", fSize);

			//ltoa (c_file.size, fi1.szFilesize, 10);

			fi1.bIsDir = false;

			pPathList->AddItem (&fi1, sizeof (SingleFileInfo), false);

		} else {
			if (strcmp (c_file.name, ".") != 0 && strcmp (c_file.name, "..") != 0) {
				//OutputText (c_file.name, ": ADIRECTORY");

				ZeroMemory (szDirectoryPath, SIZE_STRING);
				strcpy_s (szDirectoryPath, SIZE_STRING, szSourcePath);
				strcat_s (szDirectoryPath, SIZE_STRING, "\\");
				strcat_s (szDirectoryPath, SIZE_STRING, c_file.name);


				SingleFileInfo fi2;
				strcpy_s (fi2.szName, SIZE_STRING, c_file.name);
				strcpy_s (fi2.szFilepath, SIZE_STRING, szDirectoryPath);
				fi2.lFilesize = c_file.size;

				fSize = (double) c_file.size / 1000;
				sprintf_s (fi2.szFilesize, SIZE_NAME, "%.2f KB", fSize);

				//ltoa (c_file.size, fi2.szFilesize, 10);

				fi2.bIsDir = true;

				pPathList->AddItem (&fi2, sizeof (SingleFileInfo), false);				
				
			}
		}
		while( _findnext(hFile, &c_file ) == 0 )
		{
			if ((c_file.attrib & _A_SUBDIR) != 16) {			
				//OutputText (c_file.name, ": AFILE");

				ZeroMemory (szFullFilePath, SIZE_STRING);
				strcpy_s (szFullFilePath, SIZE_STRING, szSourcePath);
				strcat_s (szFullFilePath, SIZE_STRING, "\\");
				strcat_s (szFullFilePath, SIZE_STRING, c_file.name);
				//OutputText (szFullFilePath);

				SingleFileInfo fi3;
				strcpy_s (fi3.szName, SIZE_STRING, c_file.name);
				strcpy_s (fi3.szFilepath, SIZE_STRING, szFullFilePath);
				fi3.lFilesize = c_file.size;

				fSize = (double) c_file.size / 1000;
				sprintf_s (fi3.szFilesize, SIZE_NAME, "%.2f KB", fSize);

				//ltoa (c_file.size, fi3.szFilesize, 10);

				fi3.bIsDir = false;

				pPathList->AddItem (&fi3, sizeof (SingleFileInfo), false);				
			} else {
				if (strcmp (c_file.name, ".") != 0 && strcmp (c_file.name, "..") != 0) {
					//OutputText (c_file.name, ": ADIRECTORY");

					ZeroMemory (szDirectoryPath, SIZE_STRING);
					strcpy_s (szDirectoryPath, SIZE_STRING, szSourcePath);
					strcat_s (szDirectoryPath, SIZE_STRING, "\\");
					strcat_s (szDirectoryPath, SIZE_STRING, c_file.name);
					//ListAllFiles (szDirectoryPath, pPathList);

					SingleFileInfo fi4;
					strcpy_s (fi4.szName, SIZE_STRING, c_file.name);
					strcpy_s (fi4.szFilepath, SIZE_STRING, szDirectoryPath);
					fi4.lFilesize = c_file.size;
					
					fSize = (double) c_file.size / 1000;
					sprintf_s (fi4.szFilesize, SIZE_NAME, "%.2f KB", fSize);

					//ltoa (c_file.size, fi4.szFilesize, 10);
					
					fi4.bIsDir = true;

					pPathList->AddItem (&fi4, sizeof (SingleFileInfo), false);					
				}
			}
		}
		_findclose( hFile );
	}
	
	//if (dlSourceFiles.GetNumItems () > 0) {
	//	dlSourceFiles.Clear ();
	//}
}

bool DirScanner::IsFileCCR (char *pszFilePath)
{
	char szFullPath[SIZE_STRING];
	ZeroMemory (szFullPath, SIZE_STRING);
	strcpy_s (szFullPath, SIZE_STRING, pszFilePath);

	char szTmp[SIZE_NAME];
	char szExtension[SIZE_NAME];

	int a = 0;
	int exloc = 0;
	bool bexfound = false;

	for (a=strlen(szFullPath);a>0;a--) {
		ZeroMemory (szTmp, SIZE_NAME);
		strncpy_s (szTmp, SIZE_NAME, szFullPath+a, 1);
		
		if (strcmp (szTmp, ".") == 0) {
			exloc = a;
			bexfound = true;
			break;
		}
	}

	if (bexfound == true) {
		ZeroMemory (szExtension, SIZE_NAME);
		strcpy_s (szExtension, SIZE_NAME, szFullPath+exloc);

		if (strcmp (szExtension, ".CCR") == 0 || strcmp (szExtension, ".ccr") == 0) {
			//OutputText ("Extension: ", szExtension);
			return true;
		} else {
			//OutputText ("No Extension: ", szExtension);
			return false;
		}
	} else {
		return false;
	}

	return false;
}

void DirScanner::ListAllCCRFiles (char *szSourcePath, DynList *pPathList) {

	struct _finddata_t c_file;
	long hFile;

	char szSource[SIZE_STRING];
	char szFilter[SIZE_STRING];
	char szFullPath[SIZE_STRING];
	char szDirectoryPath[SIZE_STRING];
	CHAR szFullFilePath[SIZE_STRING];

	//BOOL bExists;
	//DynStringList dlSourceFiles;
	//pPathList->Clear ();

	ZeroMemory (szSource, SIZE_STRING);
	ZeroMemory (szFilter, SIZE_STRING);
	ZeroMemory (szFullPath, SIZE_STRING);
	
	if (strncmp (szSourcePath+strlen(szSourcePath)-1, "\\", 1) == 0) {
		strncpy_s (szSource, SIZE_STRING, szSourcePath, strlen(szSourcePath)-1);
	} else {
		strcpy_s (szSource, SIZE_STRING, szSourcePath);
	}

	strcpy (szFilter, "\\*.*");
	
	strcat (szFullPath, szSource);
	strcat (szFullPath, szFilter);
	
	//OutputText ("SourcePath: ", szSourcePath);

	// Build the list of files in the source path
	if( (hFile = _findfirst(szFullPath, &c_file )) == -1L )
		OutputText ("No Files.");
	else
	{
		if ((c_file.attrib & _A_SUBDIR) != 16) {
			//dlSourceFiles.AddItem (c_file.name);
			//OutputText (c_file.name, ": AFILE" );

			ZeroMemory (szFullFilePath, SIZE_STRING);
			strcpy_s (szFullFilePath, SIZE_STRING, szSourcePath);
			strcat_s (szFullFilePath, SIZE_STRING, "\\");
			strcat_s (szFullFilePath, SIZE_STRING, c_file.name);
			
			if (IsFileCCR (szFullFilePath) == true) {
				//OutputText (szFullFilePath);
				pPathList->AddItem (szFullFilePath, strlen (szFullFilePath), false);			
			} else {
				//OutputText ("NOT CCR: ", szFullFilePath);
			}
		} else {
			if (strcmp (c_file.name, ".") != 0 && strcmp (c_file.name, "..") != 0) {
				//OutputText (c_file.name, ": ADIRECTORY");

				ZeroMemory (szDirectoryPath, SIZE_STRING);
				strcpy_s (szDirectoryPath, SIZE_STRING, szSourcePath);
				strcat_s (szDirectoryPath, SIZE_STRING, "\\");
				strcat_s (szDirectoryPath, SIZE_STRING, c_file.name);
				ListAllCCRFiles (szDirectoryPath, pPathList);
			}
		}
		while( _findnext(hFile, &c_file ) == 0 )
		{
			if ((c_file.attrib & _A_SUBDIR) != 16) {			
				//OutputText (c_file.name, ": AFILE");

				ZeroMemory (szFullFilePath, SIZE_STRING);
				strcpy_s (szFullFilePath, SIZE_STRING, szSourcePath);
				strcat_s (szFullFilePath, SIZE_STRING, "\\");
				strcat_s (szFullFilePath, SIZE_STRING, c_file.name);
				
				if (IsFileCCR (szFullFilePath) == true) {
					//OutputText (szFullFilePath);
					pPathList->AddItem (szFullFilePath, strlen (szFullFilePath), false);
				} else {
					//OutputText ("NOT CCR: ", szFullFilePath);
				}
				
			} else {
				if (strcmp (c_file.name, ".") != 0 && strcmp (c_file.name, "..") != 0) {
					//OutputText (c_file.name, ": ADIRECTORY");

					ZeroMemory (szDirectoryPath, SIZE_STRING);
					strcpy_s (szDirectoryPath, SIZE_STRING, szSourcePath);
					strcat_s (szDirectoryPath, SIZE_STRING, "\\");
					strcat_s (szDirectoryPath, SIZE_STRING, c_file.name);
					ListAllCCRFiles (szDirectoryPath, pPathList);
				}
			}
		}
		_findclose( hFile );
	}
	
	//if (dlSourceFiles.GetNumItems () > 0) {
	//	dlSourceFiles.Clear ();
	//}
}

void DirScanner::ListAllNONCCRFiles (char *szSourcePath, DynList *pPathList) {

	struct _finddata_t c_file;
	long hFile;

	char szSource[SIZE_STRING];
	char szFilter[SIZE_STRING];
	char szFullPath[SIZE_STRING];
	char szDirectoryPath[SIZE_STRING];
	CHAR szFullFilePath[SIZE_STRING];

	//BOOL bExists;
	//DynStringList dlSourceFiles;
	//pPathList->Clear ();

	ZeroMemory (szSource, SIZE_STRING);
	ZeroMemory (szFilter, SIZE_STRING);
	ZeroMemory (szFullPath, SIZE_STRING);
	
	if (strncmp (szSourcePath+strlen(szSourcePath)-1, "\\", 1) == 0) {
		strncpy_s (szSource, SIZE_STRING, szSourcePath, strlen(szSourcePath)-1);
	} else {
		strcpy_s (szSource, SIZE_STRING, szSourcePath);
	}

	strcpy (szFilter, "\\*.*");
	
	strcat (szFullPath, szSource);
	strcat (szFullPath, szFilter);
	
	//OutputText ("SourcePath: ", szSourcePath);

	// Build the list of files in the source path
	if( (hFile = _findfirst(szFullPath, &c_file )) == -1L )
		OutputText ("No Files.");
	else
	{
		if ((c_file.attrib & _A_SUBDIR) != 16) {
			//dlSourceFiles.AddItem (c_file.name);
			//OutputText (c_file.name, ": AFILE" );

			ZeroMemory (szFullFilePath, SIZE_STRING);
			strcpy_s (szFullFilePath, SIZE_STRING, szSourcePath);
			strcat_s (szFullFilePath, SIZE_STRING, "\\");
			strcat_s (szFullFilePath, SIZE_STRING, c_file.name);
			
			if (IsFileCCR (szFullFilePath) == false) {
				//OutputText (szFullFilePath);
				pPathList->AddItem (szFullFilePath, strlen (szFullFilePath), false);			
			} else {
				//OutputText ("NOT CCR: ", szFullFilePath);
			}
		} else {
			if (strcmp (c_file.name, ".") != 0 && strcmp (c_file.name, "..") != 0) {
				//OutputText (c_file.name, ": ADIRECTORY");

				ZeroMemory (szDirectoryPath, SIZE_STRING);
				strcpy_s (szDirectoryPath, SIZE_STRING, szSourcePath);
				strcat_s (szDirectoryPath, SIZE_STRING, "\\");
				strcat_s (szDirectoryPath, SIZE_STRING, c_file.name);
				ListAllNONCCRFiles (szDirectoryPath, pPathList);
			}
		}
		while( _findnext(hFile, &c_file ) == 0 )
		{
			if ((c_file.attrib & _A_SUBDIR) != 16) {			
				//OutputText (c_file.name, ": AFILE");

				ZeroMemory (szFullFilePath, SIZE_STRING);
				strcpy_s (szFullFilePath, SIZE_STRING, szSourcePath);
				strcat_s (szFullFilePath, SIZE_STRING, "\\");
				strcat_s (szFullFilePath, SIZE_STRING, c_file.name);
				
				if (IsFileCCR (szFullFilePath) == false) {
					//OutputText (szFullFilePath);
					pPathList->AddItem (szFullFilePath, strlen (szFullFilePath), false);
				} else {
					//OutputText ("NOT CCR: ", szFullFilePath);
				}
				
			} else {
				if (strcmp (c_file.name, ".") != 0 && strcmp (c_file.name, "..") != 0) {
					//OutputText (c_file.name, ": ADIRECTORY");

					ZeroMemory (szDirectoryPath, SIZE_STRING);
					strcpy_s (szDirectoryPath, SIZE_STRING, szSourcePath);
					strcat_s (szDirectoryPath, SIZE_STRING, "\\");
					strcat_s (szDirectoryPath, SIZE_STRING, c_file.name);
					ListAllNONCCRFiles (szDirectoryPath, pPathList);
				}
			}
		}
		_findclose( hFile );
	}
	
	//if (dlSourceFiles.GetNumItems () > 0) {
	//	dlSourceFiles.Clear ();
	//}
}

void DirScanner::OutputInt (LPCSTR lpszText, int iValue)
{
	if (m_bUseDiagnostics == true) {
		m_pdiag->OutputInt (lpszText, iValue);
	}
}

void DirScanner::OutputText (LPCSTR lpszText)
{
	if (m_bUseDiagnostics == true) {
		m_pdiag->OutputText (lpszText);
	}
}

void DirScanner::OutputText (LPCSTR lpszName, LPCSTR lpszValue)
{
	if (m_bUseDiagnostics == true) {
		m_pdiag->OutputText (lpszName, lpszValue);
	}
}