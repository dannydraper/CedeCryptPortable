// This work is dedicated to the Lord our God. King of Heaven, Lord of Heaven's Armies.

#include "AboutWindow.h"

AboutWindow::AboutWindow ()
{
	m_bUseDiagnostics = true;
	ZeroMemory (m_szClassname, SIZE_STRING);
	m_ID = 0;
}

AboutWindow::~AboutWindow ()
{

}

void AboutWindow::SetDiagnostics (Diagnostics *pdiag)
{
	m_pdiag = pdiag;	
}

void AboutWindow::SetDistributionName (char *szName)
{
	ZeroMemory (m_szDistributionName, SIZE_STRING);
	strcpy_s (m_szDistributionName, SIZE_STRING, szName);
}

void AboutWindow::Initialise (HWND hWnd, unsigned int uID)
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
	SetCaption (TEXT ("About"));
	SetWindowStyle (FS_STYLESTANDARD);

	// Create the class name
	strcat_s (m_szClassname, SIZE_STRING, "ABOUTWindowClass");
	sprintf_s (szID, SIZE_STRING, "%d", uID);
	strcat_s (m_szClassname, SIZE_STRING, szID);


	CreateAppWindow (m_szClassname, 70, 0, 397, 200, true);
	m_uihandler.SetWindowProperties (0, 0, 299, 0, GetSysColor (COLOR_BTNFACE));
	SetWindowPosition (FS_CENTER);
	Show ();
}

void AboutWindow::OnDestroy (HWND hWnd)
{			
	Hide ();
}

void AboutWindow::OnCreate (HWND hWnd)
{				
	// Make this window handle global
	m_hwnd = hWnd;
	
	m_header.SetBitmapResources (IDB_ABOUTHEADER);
	m_header.SetBitmapProperties (0, 0, 392, 138);
	m_header.SetProperties (hWnd, CID_HEADER, 0, 0, 392, 138);
	m_uihandler.AddDirectControl (&m_header);

	// The ok button
	HFONT hfDefault = (HFONT) GetStockObject (DEFAULT_GUI_FONT);
	m_hwndbtnok = CreateWindow ("button", "Ok", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 315, 145, 70, 23, hWnd, (HMENU) ID_BTNABOUTOK, GetModuleHandle (NULL), NULL);
	SendMessage (m_hwndbtnok, WM_SETFONT, (WPARAM) hfDefault, MAKELPARAM (FALSE, 0));
	
	m_distname.SetProperties (m_hwnd, 9876, 10, 117, 390, 25);
	m_distname.SetTextProperties ("Arial", 90, 0, 0, 0, m_szDistributionName, RGB (255, 255, 255));
	m_uihandler.AddDirectControl (&m_distname);
}

void AboutWindow::OutputInt (LPCSTR lpszText, int iValue)
{
	if (m_bUseDiagnostics == true) {
		m_pdiag->OutputInt (lpszText, iValue);
	}
}

void AboutWindow::OutputText (LPCSTR lpszText)
{
	if (m_bUseDiagnostics == true) {
		m_pdiag->OutputText (lpszText);
	}
}

void AboutWindow::OutputText (LPCSTR lpszName, LPCSTR lpszValue)
{
	if (m_bUseDiagnostics == true) {
		m_pdiag->OutputText (lpszName, lpszValue);
	}
}

void AboutWindow::OnCommand (HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD (wParam)) {
		case ID_BTNABOUTOK:
		{
			Hide ();
		}
		break;		
	}
}

void AboutWindow::OnUIScroll (HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	
}

void AboutWindow::OnUICommand (HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{

	}
}

void AboutWindow::OnTimer (WPARAM wParam)
{	
	m_uihandler.NotifyTimer (wParam);	
}

void AboutWindow::OnPaint (HWND hWnd)
{
	m_uihandler.PaintControls (hWnd);	
}

void AboutWindow::OnMouseMove (HWND hWnd, int mouseXPos, int mouseYPos)
{	
	m_uihandler.NotifyMouseMove (mouseXPos, mouseYPos);
}

void AboutWindow::OnLButtonDown (HWND hWnd)
{
	m_uihandler.NotifyMouseDown ();
}

void AboutWindow::OnLButtonUp (HWND hWnd)
{
	m_uihandler.NotifyMouseUp ();
}