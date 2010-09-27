// This work is dedicated to the Lord our God. King of Heaven, Lord of Heaven's Armies.

#include "EncryptWindow.h"

EncryptWindow::EncryptWindow ()
{
	m_bUseDiagnostics = true;
	ZeroMemory (m_szClassname, SIZE_STRING);
	m_ID = 0;
	m_binitialised = false;
}

EncryptWindow::~EncryptWindow ()
{

}

void EncryptWindow::SetDiagnostics (Diagnostics *pdiag)
{
	m_pdiag = pdiag;
	
}

void EncryptWindow::Initialise (HWND hWnd, unsigned int uID)
{	
	// Make the ID global
	m_ID = uID;
	m_parenthwnd = hWnd;
	//m_hwnd = hWnd;

	// Temporary string value for uID
	char szID[SIZE_STRING];
	ZeroMemory (szID, SIZE_STRING);

	SetParentHWND (hWnd);
	SetBgColor (GetSysColor (COLOR_BTNFACE));
	SetCaption (TEXT ("Progress"));
	SetWindowStyle (FS_STYLESTANDARD);

	// Create the class name
	strcat_s (m_szClassname, SIZE_STRING, "ProgressClass");
	sprintf_s (szID, SIZE_STRING, "%d", uID);
	strcat_s (m_szClassname, SIZE_STRING, szID);

	if (m_binitialised == false) {
		CreateAppWindow (m_szClassname, 70, 0, 600, 80, true);
		m_uihandler.SetWindowProperties (0, 0, 0, 0, RGB (230, 230, 240));
		SetWindowPosition (FS_CENTER);
		m_binitialised = true;
	}
	
	//Show ();
}

void EncryptWindow::SetPassword (char *szPassword)
{
	ZeroMemory (m_szPassword, SIZE_STRING);
	strcpy_s (m_szPassword, SIZE_STRING, szPassword);
}

void EncryptWindow::GetPathOnly (char *szInpath, char *szOutpath, char *szOutfile, char *szSep)
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

void EncryptWindow::StripStartPath (char *szStartpath, char *szFullpath, char *szOutpath)
{

	char szTemp[SIZE_STRING];
	ZeroMemory (szTemp, SIZE_STRING);

	strncpy_s (szTemp, SIZE_STRING, szFullpath, strlen (szStartpath));



	if (strcmp (szStartpath, szTemp) == 0) {
		ZeroMemory (szOutpath, SIZE_STRING);
		strcpy_s (szOutpath, SIZE_STRING, szFullpath+strlen (szStartpath));
	}

}

void EncryptWindow::OnDestroy (HWND hWnd)
{			
	Hide ();
}

void EncryptWindow::OnClose (HWND hWnd)
{
	Hide ();
}

void EncryptWindow::OnCreate (HWND hWnd)
{				
	// Make this window handle global
	m_hwnd = hWnd;
	g_hWnd = hWnd; // Need to do this otherwise control wrappers don't work.
	

	m_hwndprogress  = CreateWindowEx(0L, PROGRESS_CLASS, "", WS_CHILD | WS_VISIBLE, 22, 15, 550, 22, hWnd, (HMENU) ID_ENCPROGRESS, GetModuleHandle (NULL), NULL);

	SendMessage(m_hwndprogress, PBM_SETRANGE, 0L, MAKELONG (0, 100));
	SendMessage (m_hwndprogress, PBM_SETSTEP, 1, 0L);
	//SendMessage (m_hwndprogress, PBM_SETPOS, 0, 0L);
}

void EncryptWindow::ClearFileInfo ()
{
	m_dlFileinfolist.Clear ();
}

void EncryptWindow::AddFileInfo (char *szFilepath, char *szRootdir, bool IsDir)
{
	SingleFileInfo fiCurrent;

	ZeroMemory (fiCurrent.szFilepath, SIZE_STRING);
	strcpy_s (fiCurrent.szFilepath, SIZE_STRING, szFilepath);

	ZeroMemory (fiCurrent.szRootdir, SIZE_STRING);
	strcpy_s (fiCurrent.szRootdir, SIZE_STRING, szRootdir);

	fiCurrent.bIsDir = IsDir;

	m_dlFileinfolist.AddItem (&fiCurrent, sizeof (SingleFileInfo), false);

	//m_dlFileinfolist.AddItem ((SingleFileInfo *) pinfo, sizeof (SingleFileInfo), false);
}

void EncryptWindow::AddFileInfo (char *szFilepath, bool IsDir)
{
	SingleFileInfo fiCurrent;

	ZeroMemory (fiCurrent.szFilepath, SIZE_STRING);
	strcpy_s (fiCurrent.szFilepath, SIZE_STRING, szFilepath);

	fiCurrent.bIsDir = IsDir;

	m_dlFileinfolist.AddItem (&fiCurrent, sizeof (SingleFileInfo), false);
	//m_dlFileinfolist.AddItem ((SingleFileInfo *) pinfo, sizeof (SingleFileInfo), false);
}

void EncryptWindow::DoSomething ()
{
	for (int a=0;a<100;a+=5) {
		Sleep (500);
		SendMessage (m_hwndprogress, PBM_SETPOS, a, 0L);	
	}
	
}

void EncryptWindow::DoDecryption ()
{
	SingleFileInfo *pinfo;

	int a = 0;
	int errorcount = 0;
	char szDestpath[SIZE_STRING];

	char szFileonly[SIZE_STRING];
	char szPathonly[SIZE_STRING];
	char szTemp[SIZE_STRING];

	char szTempdir[SIZE_STRING];
	ZeroMemory (szTempdir, SIZE_STRING);
	//strcpy_s (szTempdir, SIZE_STRING, "C:\\Temp\\TempEnc");
	GetEnvironmentVariable ("Temp", szTempdir, SIZE_STRING);
	strcat_s (szTempdir, SIZE_STRING, "\\CedeCrypt.tmp");

	Show ();
	SendMessage(m_hwndprogress, PBM_SETRANGE, 0L, MAKELONG (0, m_dlFileinfolist.GetNumItems ()));

	for (a=0;a<m_dlFileinfolist.GetNumItems ();a++) {

		pinfo = (SingleFileInfo *) m_dlFileinfolist.GetItem (a);

		if (pinfo->bIsDir == true) {
			
			ZeroMemory (szDestpath, SIZE_STRING);
			strcpy_s (szDestpath, SIZE_STRING, szTempdir);
			strcat_s (szDestpath, SIZE_STRING, "\\");

			ZeroMemory (szFileonly, SIZE_STRING);
			ZeroMemory (szPathonly, SIZE_STRING);
			
			GetPathOnly (pinfo->szRootdir, szPathonly, szFileonly, "\\");

			ZeroMemory (szTemp, SIZE_STRING);
			StripStartPath (pinfo->szRootdir, pinfo->szFilepath, szTemp);

			strcat_s (szDestpath, SIZE_STRING, szFileonly);
			strcat_s (szDestpath, SIZE_STRING, szTemp);
			
			//m_pdiag->OutputText ("Encrypt Source: ", pinfo->szFilepath);
			//m_pdiag->OutputText ("BlankFile Dest: ", szDestpath);

			if (m_enc.EncryptFileEx3 (pinfo->szFilepath, szDestpath, m_szPassword, false) == false) {
				errorcount++;
			}
		} else {

			ZeroMemory (szDestpath, SIZE_STRING);
			strcpy_s (szDestpath, SIZE_STRING, szTempdir);
			strcat_s (szDestpath, SIZE_STRING, "\\");

			ZeroMemory (szFileonly, SIZE_STRING);
			ZeroMemory (szPathonly, SIZE_STRING);

			GetPathOnly (pinfo->szFilepath, szPathonly, szFileonly, "\\");
			
			strcat_s (szDestpath, SIZE_STRING, szFileonly);

			//m_pdiag->OutputText ("BlankFile Dest: ", szDestpath);

			if (m_enc.EncryptFileEx3 (pinfo->szFilepath, szDestpath, m_szPassword, false) == false) {
				errorcount++;
			}
		}

		SendMessage (m_hwndprogress, PBM_SETPOS, a, 0L);
	}


	m_idecrypterrorcount = errorcount;
	Hide ();
}

void EncryptWindow::DoBlankSetup ()
{
	SingleFileInfo *pinfo;

	int a = 0;
	char szDestpath[SIZE_STRING];

	char szFileonly[SIZE_STRING];
	char szPathonly[SIZE_STRING];
	char szTemp[SIZE_STRING];

	char szTempdir[SIZE_STRING];
	ZeroMemory (szTempdir, SIZE_STRING);
	//strcpy_s (szTempdir, SIZE_STRING, "C:\\Temp\\TempEnc");

	GetEnvironmentVariable ("Temp", szTempdir, SIZE_STRING);
	strcat_s (szTempdir, SIZE_STRING, "\\CedeCrypt.tmp");
	

	for (a=0;a<m_dlFileinfolist.GetNumItems ();a++) {

		pinfo = (SingleFileInfo *) m_dlFileinfolist.GetItem (a);

		if (pinfo->bIsDir == true) {
			
			ZeroMemory (szDestpath, SIZE_STRING);
			strcpy_s (szDestpath, SIZE_STRING, szTempdir);
			strcat_s (szDestpath, SIZE_STRING, "\\");

			ZeroMemory (szFileonly, SIZE_STRING);
			ZeroMemory (szPathonly, SIZE_STRING);
			
			GetPathOnly (pinfo->szRootdir, szPathonly, szFileonly, "\\");

			ZeroMemory (szTemp, SIZE_STRING);
			StripStartPath (pinfo->szRootdir, pinfo->szFilepath, szTemp);

			strcat_s (szDestpath, SIZE_STRING, szFileonly);
			strcat_s (szDestpath, SIZE_STRING, szTemp);
			
			//m_pdiag->OutputText ("Encrypt Source: ", pinfo->szFilepath);
			m_pdiag->OutputText ("BlankFile Dest: ", szDestpath);

			m_enc.SetupBlankFile (szDestpath);
		} else {

			ZeroMemory (szDestpath, SIZE_STRING);
			strcpy_s (szDestpath, SIZE_STRING, szTempdir);
			strcat_s (szDestpath, SIZE_STRING, "\\");

			ZeroMemory (szFileonly, SIZE_STRING);
			ZeroMemory (szPathonly, SIZE_STRING);

			GetPathOnly (pinfo->szFilepath, szPathonly, szFileonly, "\\");
			
			strcat_s (szDestpath, SIZE_STRING, szFileonly);

			m_pdiag->OutputText ("BlankFile Dest: ", szDestpath);

			m_enc.SetupBlankFile (szDestpath);
		}
	}
}

void EncryptWindow::DoEncryption (char *szVaultpath)
{
	// Goes through each of the file info objects in the added
	// dynlist and encrypts each file one by one, placing the encrypted
	// file in the destination vault path. Which will be in the "EncryptedVault" directory
	// which resides at the same location as this executable.

	SingleFileInfo *pinfo;

	int a = 0;
	char szDestpath[SIZE_STRING];

	char szFileonly[SIZE_STRING];
	char szPathonly[SIZE_STRING];
	char szTemp[SIZE_STRING];

	Show ();
	SendMessage(m_hwndprogress, PBM_SETRANGE, 0L, MAKELONG (0, m_dlFileinfolist.GetNumItems ()));

	for (a=0;a<m_dlFileinfolist.GetNumItems ();a++) {
		pinfo = (SingleFileInfo *) m_dlFileinfolist.GetItem (a);

		if (pinfo->bIsDir == true) {

			ZeroMemory (szDestpath, SIZE_STRING);
			strcpy_s (szDestpath, SIZE_STRING, szVaultpath);
			strcat_s (szDestpath, SIZE_STRING, "\\");

			ZeroMemory (szFileonly, SIZE_STRING);
			ZeroMemory (szPathonly, SIZE_STRING);
			
			GetPathOnly (pinfo->szRootdir, szPathonly, szFileonly, "\\");

			ZeroMemory (szTemp, SIZE_STRING);

			StripStartPath (pinfo->szRootdir, pinfo->szFilepath, szTemp);

			strcat_s (szDestpath, SIZE_STRING, szFileonly);
			//strcat_s (szDestpath, SIZE_STRING, "\\");
			strcat_s (szDestpath, SIZE_STRING, szTemp);

			m_enc.EncryptFileEx3 (pinfo->szFilepath, szDestpath, m_szPassword, true);
			
			m_pdiag->OutputText ("Encrypt Source: ", pinfo->szFilepath);
			m_pdiag->OutputText ("Encrypt Dest: ", szDestpath);


		} else {
			ZeroMemory (szDestpath, SIZE_STRING);
			strcpy_s (szDestpath, SIZE_STRING, szVaultpath);
			strcat_s (szDestpath, SIZE_STRING, "\\");

			ZeroMemory (szFileonly, SIZE_STRING);
			ZeroMemory (szPathonly, SIZE_STRING);

			GetPathOnly (pinfo->szFilepath, szPathonly, szFileonly, "\\");

			strcat_s (szDestpath, SIZE_STRING, szFileonly);

			m_pdiag->OutputText ("Encrypt Source: ", pinfo->szFilepath);
			m_pdiag->OutputText ("Encrypt Dest: ", szDestpath);

			m_enc.EncryptFileEx3 (pinfo->szFilepath, szDestpath, m_szPassword, true);
		}

		SendMessage (m_hwndprogress, PBM_SETPOS, a, 0L);
	}

	Hide ();
}

void EncryptWindow::OutputInt (LPCSTR lpszText, int iValue)
{
	if (m_bUseDiagnostics == true) {
		m_pdiag->OutputInt (lpszText, iValue);
	}
}

void EncryptWindow::OutputText (LPCSTR lpszText)
{
	if (m_bUseDiagnostics == true) {
		m_pdiag->OutputText (lpszText);
	}
}

void EncryptWindow::OutputText (LPCSTR lpszName, LPCSTR lpszValue)
{
	if (m_bUseDiagnostics == true) {
		m_pdiag->OutputText (lpszName, lpszValue);
	}
}

void EncryptWindow::OnCommand (HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD (wParam)) {

	}
}

void EncryptWindow::OnUIScroll (HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	
}

void EncryptWindow::OnUICommand (HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
		
	}
}

void EncryptWindow::OnTimer (WPARAM wParam)
{	
	m_uihandler.NotifyTimer (wParam);	
}

void EncryptWindow::OnPaint (HWND hWnd)
{
	m_uihandler.PaintControls (hWnd);	
}

void EncryptWindow::OnMouseMove (HWND hWnd, int mouseXPos, int mouseYPos)
{	
	m_uihandler.NotifyMouseMove (mouseXPos, mouseYPos);
}

void EncryptWindow::OnLButtonDown (HWND hWnd)
{
	m_uihandler.NotifyMouseDown ();
}

void EncryptWindow::OnLButtonUp (HWND hWnd)
{
	m_uihandler.NotifyMouseUp ();
}