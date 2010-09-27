// This work is dedicated to the Lord our God. King of Heaven, Lord of Heaven's Armies.

#include "Diagnostics.h"

Diagnostics::Diagnostics ()
{
	m_bUsingdiagnostics = false;
}

Diagnostics::~Diagnostics ()
{
}

void Diagnostics::Initialise (HWND hWnd, LPSTR lpCmdLine)
{	
	SetParentHWND (hWnd);
	SetBgColor (RGB (200, 200, 200));
	SetParentHWND (hWnd);
	SetCaption (TEXT ("CedeCrypt Diagnostics"));
	SetWindowStyle (FS_STYLESTANDARD);
	CreateAppWindow ("COMMDiagnosticsClass", 30, 30, 1000, 350, true);
	//m_uihandler.SetWindowProperties (0, 70, 40, 443, RGB (200, 200, 200));		
	//SetWindowPosition (FS_CENTER);
	ParseCommandLine (lpCmdLine);
	//Show ();
}


void Diagnostics::ParseCommandLine (char *pszCmdline)
{
	//MessageBox (NULL, "Hello", "Info", MB_OK);
	//MessageBox (NULL, pszCmdline, "Command Line", MB_OK);

	char szAction[SIZE_STRING];
	ZeroMemory (szAction, SIZE_STRING);

	char szPath[SIZE_STRING];
	ZeroMemory (szPath, SIZE_STRING);

	bool bDiagmode = false;

	strncpy_s (szAction, SIZE_STRING, pszCmdline, 5);

	if (strcmp (szAction, "/diag") == 0) {
		
		OutputInt ("CmdLine length: ", strlen (pszCmdline));
		OutputText ("Time to enter diagnostic mode...");
		bDiagmode = true;

		Show ();

	}
}

void Diagnostics::OnCreate (HWND hWnd)
{			
	
	m_header.SetBitmapResources (IDB_DIAGHEADER);
	m_header.SetBitmapProperties (0, 0, 591, 41);
	m_header.SetProperties (hWnd, CID_HEADER, 1, 1, 591, 41);
	m_uihandler.AddDirectControl (&m_header);
	
	HFONT hfDefault = (HFONT) GetStockObject (DEFAULT_GUI_FONT);
	m_hwnddiaglist = CreateWindow ("listbox", NULL, WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL, 1, 43, 991, 283, hWnd, (HMENU) ID_DIAGLIST, GetModuleHandle (NULL), NULL) ;
	SendMessage (m_hwnddiaglist, WM_SETFONT, (WPARAM) hfDefault, MAKELPARAM (FALSE, 0));
	OutputInt ("Diagnostics Ready: ", 0);
}

void Diagnostics::OutputInt (LPCSTR lpszText, int iValue)
{
	if (m_bUsingdiagnostics == false) {
		return;
	}

	char szInteger[SIZE_INTEGER];
	ZeroMemory (szInteger, SIZE_INTEGER);
	sprintf_s (szInteger, SIZE_INTEGER, "%d", iValue);

	char szText[SIZE_STRING];
	ZeroMemory (szText, SIZE_STRING);
	strcpy_s (szText, SIZE_STRING, lpszText);

	strcat_s (szText, SIZE_STRING, szInteger);

	SendMessage (m_hwnddiaglist, LB_ADDSTRING, 0, (LPARAM) (LPCTSTR) &szText);

	int lCount = SendMessage (m_hwnddiaglist, LB_GETCOUNT, 0, 0);
	SendMessage (m_hwnddiaglist, LB_SETCURSEL, lCount-1, 0);
}

void Diagnostics::OutputText (LPCSTR lpszText)
{
	if (m_bUsingdiagnostics == false) {
		return;
	}


	char szText[SIZE_STRING];
	ZeroMemory (szText, SIZE_STRING);

	if (strlen (lpszText) < SIZE_STRING) {

		strcpy_s (szText, SIZE_STRING, lpszText);
		SendMessage (m_hwnddiaglist, LB_ADDSTRING, 0, (LPARAM) (LPCTSTR) &szText);

		int lCount = SendMessage (m_hwnddiaglist, LB_GETCOUNT, 0, 0);
		SendMessage (m_hwnddiaglist, LB_SETCURSEL, lCount-1, 0);
	}
}

void Diagnostics::OutputText (LPCSTR lpszName, LPCSTR lpszValue)
{
	if (m_bUsingdiagnostics == false) {
		return;
	}

	char szText[SIZE_STRING*2];
	ZeroMemory (szText, SIZE_STRING*2);	

	if (strlen (lpszValue) < SIZE_STRING) {

		strcat_s (szText, SIZE_STRING*2, lpszName);
		strcat_s (szText, SIZE_STRING*2, lpszValue);

		SendMessage (m_hwnddiaglist, LB_ADDSTRING, 0, (LPARAM) (LPCTSTR) &szText);

		int lCount = SendMessage (m_hwnddiaglist, LB_GETCOUNT, 0, 0);
		SendMessage (m_hwnddiaglist, LB_SETCURSEL, lCount-1, 0);
	}
}

void Diagnostics::OnCommand (HWND hWnd, WPARAM wParam, LPARAM lParam)
{

}

void Diagnostics::OnUICommand (HWND hWnd, WPARAM wParam, LPARAM lParam)
{

}

void Diagnostics::OnTimer (WPARAM wParam)
{	
	m_uihandler.NotifyTimer (wParam);
}

void Diagnostics::OnPaint (HWND hWnd)
{
	m_uihandler.PaintControls (hWnd);	
}

void Diagnostics::OnMouseMove (HWND hWnd, int mouseXPos, int mouseYPos)
{	
	m_uihandler.NotifyMouseMove (mouseXPos, mouseYPos);
}

void Diagnostics::OnLButtonDown (HWND hWnd)
{
	m_uihandler.NotifyMouseDown ();
}

void Diagnostics::OnLButtonUp (HWND hWnd)
{
	m_uihandler.NotifyMouseUp ();
}