// This work is dedicated to the Lord our God. King of Heaven, Lord of Heaven's Armies.

#include "PFolderRequestWindow.h"

PFolderRequestWindow::PFolderRequestWindow ()
{
	m_bUseDiagnostics = true;
	ZeroMemory (m_szClassname, SIZE_STRING);
	m_ID = 0;
	m_bdoaction = false;
	m_bstartupmode = false;
}

PFolderRequestWindow::~PFolderRequestWindow ()
{

}

void PFolderRequestWindow::SetDiagnostics (Diagnostics *pdiag)
{
	m_pdiag = pdiag;
	
}

void PFolderRequestWindow::Initialise (HWND hWnd, unsigned int uID)
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
	SetCaption (TEXT ("Protected Folders Request"));
	SetWindowStyle (FS_STYLESTANDARD);

	// Create the class name
	strcat_s (m_szClassname, SIZE_STRING, "StandardWindowClass");
	sprintf_s (szID, SIZE_STRING, "%d", uID);
	strcat_s (m_szClassname, SIZE_STRING, szID);

	CreateAppWindow (m_szClassname, 70, 0, 400, 160, true);
	m_uihandler.SetWindowProperties (0, 0, 0, 0, RGB (230, 230, 240));
	SetWindowPosition (FS_CENTER);
	Show ();
	SetAlwaysOnTop (true);
}

void PFolderRequestWindow::SetStartupMode (bool bstartupmode)
{
	m_bstartupmode = bstartupmode;
}

bool PFolderRequestWindow::GetStartMode ()
{
	return m_bstartupmode;
}

void PFolderRequestWindow::SetAnswered (bool banswered)
{
	m_banswered = banswered;
}

bool PFolderRequestWindow::GetAnswered ()
{
	return m_banswered;
}

void PFolderRequestWindow::OnDestroy (HWND hWnd)
{			
	Hide ();
}

void PFolderRequestWindow::OnClose (HWND hWnd)
{
	Hide ();
}

void PFolderRequestWindow::OnCreate (HWND hWnd)
{				
	// Make this window handle global
	m_hwnd = hWnd;
	g_hWnd = hWnd;

	m_optiongroup = CreateGroupBox ("Your protected folders are currently encrypted", 15, 15, 365, 80, ID_PFOLDERREQUESTGROUP);
	m_optdonothing = CreateRadioButton ("Keep files && folders encrypted", 35, 40, 339, 20, ID_OPTDONOTHING);
	m_optdoaction = CreateRadioButton ("Decrypt files && folders now", 35, 63, 250, 20, ID_OPTDOACTION);
	m_btnrequestok = CreateButton ("Ok", 311, 102, 70, 24, ID_PFOLDERREQUESTOK);

	ReadRegistrySettings (); // Read the last selection made by the user
	//SetStartupMessage (false);

}

void PFolderRequestWindow::ReadRegistrySettings ()
{
	// Read the last action setting from the registry. This is a small feature
	// which remembers the last selection made by the user to save them clicking
	// twice if they regularly perform the same selection.

	if (m_registry.DoesSettingExist (PFRLASTACTION) == true) {
		if (strcmp (m_registry.ReadStringSetting (PFRLASTACTION), "yes") == 0) {
			m_bdoaction = true;
			SetCheck (m_optdoaction, true);
			SetCheck (m_optdonothing, false);
		} else {
			m_bdoaction = false;
			SetCheck (m_optdoaction, false);
			SetCheck (m_optdonothing, true);
		}
	}
}

void PFolderRequestWindow::SetStartupMessage (bool m_bUsestartupmessage)
{
	if (m_bUsestartupmessage == true) {
		SetDlgItemText (m_hwnd, ID_PFOLDERREQUESTGROUP, "Your protected folders are currently encrypted, would you like to:");
		SetDlgItemText (m_hwnd, ID_OPTDONOTHING, "Keep files && folders encrypted. (Take no action)");
		SetDlgItemText (m_hwnd, ID_OPTDOACTION, "Decrypt files && folders now");
	} else {
		SetDlgItemText (m_hwnd, ID_PFOLDERREQUESTGROUP, "Your protected folders are currently decrypted, would you like to:");
		SetDlgItemText (m_hwnd, ID_OPTDONOTHING, "Keep files && folders decrypted. (Take no action - not recommended)");
		SetDlgItemText (m_hwnd, ID_OPTDOACTION, "Encrypt files && folders now (Recommended)");	
	}
}

void PFolderRequestWindow::OutputInt (LPCSTR lpszText, int iValue)
{
	if (m_bUseDiagnostics == true) {
		m_pdiag->OutputInt (lpszText, iValue);
	}
}

void PFolderRequestWindow::OutputText (LPCSTR lpszText)
{
	if (m_bUseDiagnostics == true) {
		m_pdiag->OutputText (lpszText);
	}
}

void PFolderRequestWindow::OutputText (LPCSTR lpszName, LPCSTR lpszValue)
{
	if (m_bUseDiagnostics == true) {
		m_pdiag->OutputText (lpszName, lpszValue);
	}
}

void PFolderRequestWindow::OnCommand (HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD (wParam)) {
		case ID_PFOLDERREQUESTOK:
		{
			if (m_bdoaction == true) {
				PostMessage (m_parenthwnd, WM_UICOMMAND, CID_PFOLDERREQUESTOK, 1);
			} else {
				PostMessage (m_parenthwnd, WM_UICOMMAND, CID_PFOLDERREQUESTOK, 0);
			}			
			Hide ();
		}
		break;
		case ID_OPTDOACTION:
		{
			m_bdoaction = true;
			SetCheck (m_optdoaction, true);
			SetCheck (m_optdonothing, false);
			m_registry.WriteStringSetting (PFRLASTACTION, "yes"); // remember the last selection
		}
		break;
		case ID_OPTDONOTHING:
		{
			m_bdoaction = false;
			SetCheck (m_optdoaction, false);
			SetCheck (m_optdonothing, true);
			m_registry.WriteStringSetting (PFRLASTACTION, "no"); // remember the last selection
		}
		break;
	}
}

void PFolderRequestWindow::OnUIScroll (HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	
}

void PFolderRequestWindow::OnUICommand (HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
		
	}
}

void PFolderRequestWindow::OnTimer (WPARAM wParam)
{	
	m_uihandler.NotifyTimer (wParam);	
}

void PFolderRequestWindow::OnPaint (HWND hWnd)
{
	m_uihandler.PaintControls (hWnd);	
}

void PFolderRequestWindow::OnMouseMove (HWND hWnd, int mouseXPos, int mouseYPos)
{	
	m_uihandler.NotifyMouseMove (mouseXPos, mouseYPos);
}

void PFolderRequestWindow::OnLButtonDown (HWND hWnd)
{
	m_uihandler.NotifyMouseDown ();
}

void PFolderRequestWindow::OnLButtonUp (HWND hWnd)
{
	m_uihandler.NotifyMouseUp ();
}