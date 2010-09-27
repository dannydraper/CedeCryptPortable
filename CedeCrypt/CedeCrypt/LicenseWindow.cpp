// This work is dedicated to the Lord our God. King of Heaven, Lord of Heaven's Armies.

#include "LicenseWindow.h"

LicenseWindow::LicenseWindow ()
{
	m_bUseDiagnostics = true;
	ZeroMemory (m_szClassname, SIZE_STRING);
	m_ID = 0;
	m_bInitialised = false;
	ZeroMemory (m_szLicense, SIZE_STRING);
}

LicenseWindow::~LicenseWindow ()
{

}

void LicenseWindow::SetDiagnostics (Diagnostics *pdiag)
{
	m_pdiag = pdiag;
}

void LicenseWindow::Initialise (HWND hWnd, unsigned int uID)
{	
	// Make the ID global
	m_ID = uID;
	m_parenthwnd = hWnd;
	//m_hwnd = hWnd;

	// Temporary string value for uID
	char szID[SIZE_STRING];
	ZeroMemory (szID, SIZE_STRING);

	SetParentHWND (hWnd);
	SetBgSysColor (COLOR_BTNFACE);
	SetCaption (TEXT ("License Key"));
	SetWindowStyle (FS_STYLESTANDARD);

	// Create the class name
	strcat_s (m_szClassname, SIZE_STRING, "CTLicenseWindowClass");
	sprintf_s (szID, SIZE_STRING, "%d", uID);
	strcat_s (m_szClassname, SIZE_STRING, szID);

	// Clear the multicontent control
	//m_mainconv.Clear ();

	if (m_bInitialised == false) {
		CreateAppWindow (m_szClassname, 70, 0, 568, 180, true);
		m_uihandler.SetWindowProperties (0, 0, 0, 0, RGB (230, 230, 240));
		m_bInitialised = true;
	}
	
	SetWindowPosition (FS_CENTER);
	Show ();
}

void LicenseWindow::OnDestroy (HWND hWnd)
{			
	Hide ();
}

void LicenseWindow::OnClose (HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	Hide ();
}

void LicenseWindow::SetWindowFocus ()
{
	SetFocus (m_hwnd);
}

void LicenseWindow::OnCreate (HWND hWnd)
{				
	// Make this window handle global
	m_hwnd = hWnd;

	// Default font
	HFONT hfDefault;
	hfDefault = (HFONT) GetStockObject (DEFAULT_GUI_FONT);

	// The license image
	m_header.SetBitmapResources (IDB_LICENSEIMAGE);
	m_header.SetBitmapProperties (0, 0, 128, 28);
	m_header.SetProperties (hWnd, CID_STATIC, 10, 10, 128, 28);
	m_uihandler.AddDirectControl (&m_header);


	// License label
	m_lbltitle = CreateWindow ("static", "Please enter / paste your license key here...", WS_CHILD | WS_VISIBLE, 10, 60, 472, 19, hWnd, (HMENU) CID_STATIC, GetModuleHandle (NULL), NULL);
	SendMessage (m_lbltitle, WM_SETFONT, (WPARAM) hfDefault, MAKELPARAM (FALSE, 0));	

	// The license text box
	m_txtlicense = CreateWindow ("edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL, 10, 81, 543, 19, hWnd, (HMENU) IDC_TXTLICENSE, GetModuleHandle (NULL), NULL);
	SendMessage (m_txtlicense, WM_SETFONT, (WPARAM) hfDefault, MAKELPARAM (FALSE, 0));

	// The cancel button
	m_btncancel = CreateWindow ("button", "Cancel", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 393, 113, 75, 22, hWnd, (HMENU) IDC_TXTLICCANCEL, GetModuleHandle (NULL), NULL);
	SendMessage (m_btncancel, WM_SETFONT, (WPARAM) hfDefault, MAKELPARAM (FALSE, 0));

	// The ok button
	m_btnok = CreateWindow ("button", "Ok", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 473, 113, 75, 22, hWnd, (HMENU) IDC_TXTLICOK, GetModuleHandle (NULL), NULL);
	SendMessage (m_btnok, WM_SETFONT, (WPARAM) hfDefault, MAKELPARAM (FALSE, 0));

}

void LicenseWindow::OutputInt (LPCSTR lpszText, int iValue)
{
	if (m_bUseDiagnostics == true) {
		m_pdiag->OutputInt (lpszText, iValue);
	}
}

void LicenseWindow::OutputText (LPCSTR lpszText)
{
	if (m_bUseDiagnostics == true) {
		m_pdiag->OutputText (lpszText);
	}
}

void LicenseWindow::OutputText (LPCSTR lpszName, LPCSTR lpszValue)
{
	if (m_bUseDiagnostics == true) {
		m_pdiag->OutputText (lpszName, lpszValue);
	}
}

char *LicenseWindow::GetEnteredLicense ()
{
	return m_szLicense;
}

void LicenseWindow::OnCommand (HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD (wParam)) {
		case IDC_TXTLICOK:
		{
			ZeroMemory (m_szLicense, SIZE_STRING);
			GetDlgItemText (m_hwnd, IDC_TXTLICENSE, m_szLicense, SIZE_STRING);

			if (strlen (m_szLicense) > 10) {
				
				PostMessage (m_parenthwnd, WM_UICOMMAND, CID_LICENSEWINDOWOK, 0);
				//Hide ();

			} else {
				MessageBox (m_hwnd, "Please enter a valid license key.", "License Error", MB_OK | MB_ICONEXCLAMATION);
			}
		}
		break;
		case IDC_TXTLICCANCEL:
		{
			PostMessage (m_parenthwnd, WM_UICOMMAND, CID_LICENSEWINDOWCANCEL, 0);
			Hide ();
		}
		break;
	}
}

void LicenseWindow::OnUIScroll (HWND hWnd, WPARAM wParam, LPARAM lParam)
{

}

void LicenseWindow::OnUICommand (HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
		
	}
}

void LicenseWindow::OnTimer (WPARAM wParam)
{	
	m_uihandler.NotifyTimer (wParam);	
}

void LicenseWindow::OnPaint (HWND hWnd)
{
	m_uihandler.PaintControls (hWnd);	
}

void LicenseWindow::OnMouseMove (HWND hWnd, int mouseXPos, int mouseYPos)
{	
	m_uihandler.NotifyMouseMove (mouseXPos, mouseYPos);
}

void LicenseWindow::OnLButtonDown (HWND hWnd)
{
	m_uihandler.NotifyMouseDown ();
}

void LicenseWindow::OnLButtonUp (HWND hWnd)
{
	m_uihandler.NotifyMouseUp ();
}