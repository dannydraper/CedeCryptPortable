// This work is dedicated to the Lord our God. King of Heaven, Lord of Heaven's Armies.

#include "BaseWindowTemplate.h"

BaseWindowTemplate::BaseWindowTemplate ()
{
	m_bUseDiagnostics = true;
	ZeroMemory (m_szClassname, SIZE_STRING);
	m_ID = 0;
}

BaseWindowTemplate::~BaseWindowTemplate ()
{

}

void BaseWindowTemplate::SetDiagnostics (Diagnostics *pdiag)
{
	m_pdiag = pdiag;
	
}

void BaseWindowTemplate::Initialise (HWND hWnd, unsigned int uID)
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
	SetCaption (TEXT ("Standard Window"));
	SetWindowStyle (FS_STYLESTANDARD);

	// Create the class name
	strcat_s (m_szClassname, SIZE_STRING, "StandardWindowClass");
	sprintf_s (szID, SIZE_STRING, "%d", uID);
	strcat_s (m_szClassname, SIZE_STRING, szID);

	CreateAppWindow (m_szClassname, 70, 0, 400, 400, true);
	m_uihandler.SetWindowProperties (0, 0, 0, 0, RGB (230, 230, 240));
	SetWindowPosition (FS_CENTER);
	Show ();
}

void BaseWindowTemplate::OnDestroy (HWND hWnd)
{			
	Hide ();
}

void BaseWindowTemplate::OnClose (HWND hWnd)
{
	Hide ();
}

void BaseWindowTemplate::OnCreate (HWND hWnd)
{				
	// Make this window handle global
	m_hwnd = hWnd;
	g_hWnd = hWnd; // Need to do this otherwise control wrappers don't work.
	
}

void BaseWindowTemplate::OutputInt (LPCSTR lpszText, int iValue)
{
	if (m_bUseDiagnostics == true) {
		m_pdiag->OutputInt (lpszText, iValue);
	}
}

void BaseWindowTemplate::OutputText (LPCSTR lpszText)
{
	if (m_bUseDiagnostics == true) {
		m_pdiag->OutputText (lpszText);
	}
}

void BaseWindowTemplate::OutputText (LPCSTR lpszName, LPCSTR lpszValue)
{
	if (m_bUseDiagnostics == true) {
		m_pdiag->OutputText (lpszName, lpszValue);
	}
}

void BaseWindowTemplate::OnCommand (HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD (wParam)) {

	}
}

void BaseWindowTemplate::OnUIScroll (HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	
}

void BaseWindowTemplate::OnUICommand (HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
		
	}
}

void BaseWindowTemplate::OnTimer (WPARAM wParam)
{	
	m_uihandler.NotifyTimer (wParam);	
}

void BaseWindowTemplate::OnPaint (HWND hWnd)
{
	m_uihandler.PaintControls (hWnd);	
}

void BaseWindowTemplate::OnMouseMove (HWND hWnd, int mouseXPos, int mouseYPos)
{	
	m_uihandler.NotifyMouseMove (mouseXPos, mouseYPos);
}

void BaseWindowTemplate::OnLButtonDown (HWND hWnd)
{
	m_uihandler.NotifyMouseDown ();
}

void BaseWindowTemplate::OnLButtonUp (HWND hWnd)
{
	m_uihandler.NotifyMouseUp ();
}