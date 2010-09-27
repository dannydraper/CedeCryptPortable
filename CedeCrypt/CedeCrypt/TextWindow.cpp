// This work is dedicated to the Lord our God. King of Heaven, Lord of Heaven's Armies.

#include "TextWindow.h"

TextWindow::TextWindow ()
{
	m_bUseDiagnostics = true;
	ZeroMemory (m_szClassname, SIZE_STRING);
	m_ID = 0;
	m_binitialised = false;
}

TextWindow::~TextWindow ()
{

}

void TextWindow::SetDiagnostics (Diagnostics *pdiag)
{
	m_pdiag = pdiag;

}

void TextWindow::Initialise (HWND hWnd, unsigned int uID)
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
	SetCaption (TEXT ("Decrypted Text"));
	SetWindowStyle (FS_STYLESTANDARD);

	// Create the class name
	strcat_s (m_szClassname, SIZE_STRING, "TextWindowClass");
	sprintf_s (szID, SIZE_STRING, "%d", uID);
	strcat_s (m_szClassname, SIZE_STRING, szID);

	if (m_binitialised == false) {
		CreateAppWindow (m_szClassname, 70, 0, 400, 550, true);
		m_uihandler.SetWindowProperties (0, 0, 300, 0, RGB (230, 230, 240));
		SetWindowPosition (FS_CENTER);
		m_binitialised = true;
	}

	Show ();
}

void TextWindow::OnDestroy (HWND hWnd)
{			
	Hide ();
}

void TextWindow::OnCreate (HWND hWnd)
{				
	// Make this window handle global
	m_hwnd = hWnd;

	// The default font
	HFONT hfDefault = (HFONT) GetStockObject (DEFAULT_GUI_FONT);

	// The close button
	m_hwndbtnclose = CreateWindow ("button", "Close", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 315, 485, 70, 23, hWnd, (HMENU) ID_BTNTEXTCLOSE, GetModuleHandle (NULL), NULL);
	SendMessage (m_hwndbtnclose, WM_SETFONT, (WPARAM) hfDefault, MAKELPARAM (FALSE, 0));

	// The decrypted text box
	m_hwndtextdecrypt = CreateWindow ("edit", "", WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | ES_MULTILINE | ES_LEFT | ES_AUTOVSCROLL, 7, 10, 380, 465, hWnd, (HMENU) ID_TXTDECRYPTED, GetModuleHandle (NULL), NULL);
	SendMessage (m_hwndtextdecrypt, WM_SETFONT, (WPARAM) hfDefault, MAKELPARAM (FALSE, 0));
}

void TextWindow::SetText (char *szText)
{
	SetDlgItemText (m_hwnd, ID_TXTDECRYPTED, szText);
}

void TextWindow::OutputInt (LPCSTR lpszText, int iValue)
{
	if (m_bUseDiagnostics == true) {
		m_pdiag->OutputInt (lpszText, iValue);
	}
}

void TextWindow::OutputText (LPCSTR lpszText)
{
	if (m_bUseDiagnostics == true) {
		m_pdiag->OutputText (lpszText);
	}
}

void TextWindow::OutputText (LPCSTR lpszName, LPCSTR lpszValue)
{
	if (m_bUseDiagnostics == true) {
		m_pdiag->OutputText (lpszName, lpszValue);
	}
}

void TextWindow::OnCommand (HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD (wParam)) {
		case ID_BTNTEXTCLOSE:
		{
			Hide ();
		}
		break;
	}
}

void TextWindow::OnUIScroll (HWND hWnd, WPARAM wParam, LPARAM lParam)
{

}

void TextWindow::OnUICommand (HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
		
	}
}

void TextWindow::OnTimer (WPARAM wParam)
{	
	m_uihandler.NotifyTimer (wParam);	
}

void TextWindow::OnPaint (HWND hWnd)
{
	m_uihandler.PaintControls (hWnd);	
}

void TextWindow::OnMouseMove (HWND hWnd, int mouseXPos, int mouseYPos)
{	
	m_uihandler.NotifyMouseMove (mouseXPos, mouseYPos);
}

void TextWindow::OnLButtonDown (HWND hWnd)
{
	m_uihandler.NotifyMouseDown ();
}

void TextWindow::OnLButtonUp (HWND hWnd)
{
	m_uihandler.NotifyMouseUp ();
}