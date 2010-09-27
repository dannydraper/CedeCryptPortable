// This work is dedicated to the Lord our God. King of Heaven, Lord of Heaven's Armies.

#include "ProtectedFoldersWindow.h"

ProtectedFoldersWindow::ProtectedFoldersWindow ()
{
	m_bUseDiagnostics = true;
	ZeroMemory (m_szClassname, SIZE_STRING);
	m_ID = 0;
}

ProtectedFoldersWindow::~ProtectedFoldersWindow ()
{

}

void ProtectedFoldersWindow::SetDiagnostics (Diagnostics *pdiag)
{
	m_pdiag = pdiag;
	m_mainconv.SetDiagnostics (pdiag);
}

void ProtectedFoldersWindow::Initialise (HWND hWnd, unsigned int uID)
{	
	// Make the ID global
	m_ID = uID;
	m_parenthwnd = hWnd;
	//m_hwnd = hWnd;

	// Temporary string value for uID
	char szID[SIZE_STRING];
	ZeroMemory (szID, SIZE_STRING);

	SetParentHWND (hWnd);
	SetBgColor (RGB (237, 237, 237));
	SetCaption (TEXT ("Protected Folders"));
	SetWindowStyle (FS_STYLESTANDARD);

	// Create the class name
	strcat_s (m_szClassname, SIZE_STRING, "PFoldersWindowClass");
	sprintf_s (szID, SIZE_STRING, "%d", uID);
	strcat_s (m_szClassname, SIZE_STRING, szID);

	// Clear the multicontent control
	m_mainconv.Clear ();

	CreateAppWindow (m_szClassname, 70, 0, 600, 305, true);
	m_uihandler.SetWindowProperties (0, 0, 10, 0, RGB (237, 237, 237));
	SetWindowPosition (FS_CENTER);
	SetAlwaysOnTop (true);
	//Show ();
}

void ProtectedFoldersWindow::OnDestroy (HWND hWnd)
{			
	Hide ();
}

void ProtectedFoldersWindow::OnCreate (HWND hWnd)
{				
	// Make this window handle global
	m_hwnd = hWnd;

	// Default font used by window controls
	HFONT hfDefault;
	hfDefault = (HFONT) GetStockObject (DEFAULT_GUI_FONT);

	// A little title label
	m_lblheader = CreateWindow ("static", "Folders added here will be automatically Encrypted when your computer is shutdown or put in hibernation/standby. When your computer is started up, these folders will then be automatically decrypted allowing you to work on them normally.", WS_CHILD | WS_VISIBLE, 12, 7, 570, 27, hWnd, (HMENU) ID_LBLSTATIC, GetModuleHandle (NULL), NULL);
	SendMessage (m_lblheader, WM_SETFONT, (WPARAM) hfDefault, MAKELPARAM (FALSE, 0));

	// Listbox with the list of protected folder paths
	m_lstfolders = CreateWindow ("listbox", NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | WS_BORDER | LBS_NOTIFY, 12, 40, 570, 200, hWnd, (HMENU) ID_LSTFOLDERS, GetModuleHandle (NULL), NULL);
	SendMessage (m_lstfolders, WM_SETFONT, (WPARAM) hfDefault, MAKELPARAM (FALSE, 0));

	// Add Folder button
	m_btnaddfolder = CreateWindow ("button", "Add Folder", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 12, 235, 90, 32, hWnd, (HMENU) ID_BTNADDFOLDER, GetModuleHandle (NULL), NULL);
	SendMessage (m_btnaddfolder, WM_SETFONT, (WPARAM) hfDefault, MAKELPARAM (FALSE, 0));

	m_btnremfolder = CreateWindow ("button", "Remove Folder", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 112, 235, 90, 32, hWnd, (HMENU) ID_BTNREMFOLDER, GetModuleHandle (NULL), NULL);
	SendMessage (m_btnremfolder, WM_SETFONT, (WPARAM) hfDefault, MAKELPARAM (FALSE, 0));

	m_btnok = CreateWindow ("button", "Ok", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 493, 235, 90, 32, hWnd, (HMENU) ID_PFOLDEROK, GetModuleHandle (NULL), NULL);
	SendMessage (m_btnok, WM_SETFONT, (WPARAM) hfDefault, MAKELPARAM (FALSE, 0));

}

bool ProtectedFoldersWindow::FileExists (char *FileName)
{
    FILE* fp = NULL;

    //will not work if you do not have read permissions
    //to the file, but if you don't have read, it
    //may as well not exist to begin with.

    fp = fopen( FileName, "rb" );
    if( fp != NULL )
    {
        fclose( fp );
        return true;
    }

    return false;
}


bool ProtectedFoldersWindow::PrepareListPath ()
{
	// This function retrieves and sets up the directory necessary
	// under application data for temporarily storing the list of files
	// that have been selected for encryption.
	int iRes = 0;
	char szAppData[SIZE_STRING];
	char szCompanyAppData[SIZE_STRING];
	char szProgramAppData[SIZE_STRING];
	char szInfoAppData[SIZE_STRING];

	ZeroMemory (szAppData, SIZE_STRING);
	ZeroMemory (szCompanyAppData, SIZE_STRING);
	ZeroMemory (szProgramAppData, SIZE_STRING);
	ZeroMemory (szInfoAppData, SIZE_STRING);

	if (GetEnvironmentVariable ("APPDATA", szAppData, SIZE_STRING) == 0) {
		// retrieving environment variable failed.
		//m_diag.OutputText ("Failed to retrieve APPDATA environment variable!");		
		return false;
	} 

	strcpy_s (szCompanyAppData, SIZE_STRING, szAppData);
	strcat_s (szCompanyAppData, SIZE_STRING, "\\CedeSoft");

	strcpy_s (szProgramAppData, SIZE_STRING, szCompanyAppData);
	strcat_s (szProgramAppData, SIZE_STRING, "\\CedeCrypt");

	strcpy_s (szInfoAppData, SIZE_STRING, szProgramAppData);
	strcat_s (szInfoAppData, SIZE_STRING, "\\PFolders.dat");

	iRes = _mkdir (szCompanyAppData);
	iRes = _mkdir (szProgramAppData);

	// Set our member variable which is used by the other functions
	ZeroMemory (m_szListfile, SIZE_STRING);
	strcpy_s (m_szListfile, SIZE_STRING, szInfoAppData);
	
	return true;
}

void ProtectedFoldersWindow::GetListFilePath (char *szDestpath)
{
	PrepareListPath ();

	ZeroMemory (szDestpath, SIZE_STRING);
	strcpy_s (szDestpath, SIZE_STRING, m_szListfile);
}

bool ProtectedFoldersWindow::BrowseForFolder (LPCTSTR szTitle, char *szOutPath) {
	CoInitialize (NULL);
	BROWSEINFO bi = { 0 };
	bi.lpszTitle = szTitle;
	bi.ulFlags = BIF_USENEWUI;
	LPITEMIDLIST pidl = SHBrowseForFolder ( &bi );
	if ( pidl != 0 ) {
		// get the name of the folder
		SHGetPathFromIDList (pidl, szOutPath);
		// free memory used
		IMalloc * imalloc = 0;
		if ( SUCCEEDED( SHGetMalloc ( &imalloc )) ) {
			imalloc->Free ( pidl );
			imalloc->Release ( );
		}
		return true;
	}
	return false;
}

void ProtectedFoldersWindow::SaveList ()
{
	int lCount = SendMessage (m_lstfolders, LB_GETCOUNT, 0, 0);
	int l = 0;
	char szCurrentitem[SIZE_STRING];
	int iCursize = 0;
	unsigned long lTotalsize = 0;
	MemoryBuffer memList;

	// First get the size of each item
	for (l=0;l<lCount;l++) {
		ZeroMemory (szCurrentitem, SIZE_STRING);

		SendMessage (m_lstfolders, LB_GETTEXT, (WPARAM) l, (LPARAM) (LPTSTR) szCurrentitem);

		lTotalsize+=strlen (szCurrentitem);

		//MessageBox (NULL, szCurrentitem, "CurrentItem", MB_OK);
	}

	// Now set the size of the memorybuffer
	memList.SetSize ((sizeof (int)*lCount)+sizeof(int)+lTotalsize);

	// Add the item count to the file
	memList.Append (&lCount, sizeof (int));

	// Now go through the list and save the file
	for (l=0;l<lCount;l++) {
		ZeroMemory (szCurrentitem, SIZE_STRING);

		SendMessage (m_lstfolders, LB_GETTEXT, (WPARAM) l, (LPARAM) (LPTSTR) szCurrentitem);

		iCursize = strlen (szCurrentitem);

		memList.Append (&iCursize, sizeof (int));
		memList.Append ((char *) szCurrentitem, iCursize);
		
		//MessageBox (NULL, szCurrentitem, "CurrentItem", MB_OK);
	}

	// Now make sure the path is prepared
	PrepareListPath ();

	memList.SaveToFile (m_szListfile);
}

void ProtectedFoldersWindow::LoadList ()
{
	MemoryBuffer memList;
	int iCount = 0;
	int iCurlen = 0;
	char szCuritem[SIZE_STRING];
	int c = 0;
	unsigned long lPointer = 0;

	// First prepare the path to the list file
	PrepareListPath ();

	if (FileExists (m_szListfile) == true) {
		
		if (memList.ReadFromFile (m_szListfile) == true) {
			
			// Get the number of items in the list
			memcpy (&iCount, (BYTE *) memList.GetBuffer ()+lPointer, sizeof (int));
			lPointer+=sizeof (int);

			for (c=0;c<iCount;c++) {
				
				memcpy (&iCurlen, (BYTE *) memList.GetBuffer ()+lPointer, sizeof (int));
				lPointer+=sizeof (int);

				ZeroMemory (szCuritem, SIZE_STRING);
				memcpy (szCuritem, (char *) memList.GetBuffer ()+lPointer, iCurlen);
				lPointer+=iCurlen;
	
				// Add the current item to the list of items
				SendMessage (m_lstfolders, LB_ADDSTRING, 0, (LPARAM) (LPCTSTR) szCuritem);
			}

		}
	}
}

void ProtectedFoldersWindow::OutputInt (LPCSTR lpszText, int iValue)
{
	if (m_bUseDiagnostics == true) {
		m_pdiag->OutputInt (lpszText, iValue);
	}
}

void ProtectedFoldersWindow::OutputText (LPCSTR lpszText)
{
	if (m_bUseDiagnostics == true) {
		m_pdiag->OutputText (lpszText);
	}
}

void ProtectedFoldersWindow::OutputText (LPCSTR lpszName, LPCSTR lpszValue)
{
	if (m_bUseDiagnostics == true) {
		m_pdiag->OutputText (lpszName, lpszValue);
	}
}

void ProtectedFoldersWindow::OnCommand (HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD (wParam)) {
		case ID_PFOLDEROK:
		{
			Hide ();
		}
		break;
		case ID_BTNADDFOLDER:
		{
			SetAlwaysOnTop (false);
			char szSelectedFolder[SIZE_STRING];
			ZeroMemory (szSelectedFolder, SIZE_STRING);

			BrowseForFolder ("Select a folder to add...", szSelectedFolder);

			if (strlen (szSelectedFolder) > 1) {
				SendMessage (m_lstfolders, LB_ADDSTRING, 0, (LPARAM) (LPCTSTR) szSelectedFolder);

				SaveList ();
			}

			SetAlwaysOnTop (true);
		}
		break;
		case ID_BTNREMFOLDER:
		{
			int lIndex = SendMessage (m_lstfolders, LB_GETCURSEL, 0, 0);
			int lCount = SendMessage (m_lstfolders, LB_GETSELCOUNT, 0, 0);

			//if (lCount > 0) {
				SendMessage (m_lstfolders, LB_DELETESTRING, (WPARAM) lIndex, 0);
				SaveList ();
			//}
		}
		break;
	}
}

void ProtectedFoldersWindow::OnUIScroll (HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	m_mainconv.NotifyUIScroll (hWnd, wParam, lParam);
}

void ProtectedFoldersWindow::OnUICommand (HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
		
	}
}

void ProtectedFoldersWindow::OnTimer (WPARAM wParam)
{	
	m_uihandler.NotifyTimer (wParam);	
}

void ProtectedFoldersWindow::OnPaint (HWND hWnd)
{
	m_uihandler.PaintControls (hWnd);	
}

void ProtectedFoldersWindow::OnMouseMove (HWND hWnd, int mouseXPos, int mouseYPos)
{	
	m_uihandler.NotifyMouseMove (mouseXPos, mouseYPos);
}

void ProtectedFoldersWindow::OnLButtonDown (HWND hWnd)
{
	m_uihandler.NotifyMouseDown ();
}

void ProtectedFoldersWindow::OnLButtonUp (HWND hWnd)
{
	m_uihandler.NotifyMouseUp ();
}