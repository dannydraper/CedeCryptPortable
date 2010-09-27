// This work is dedicated to the Lord our God. King of Heaven, Lord of Heaven's Armies.

#include "UTEOptionsWindow.h"

UTEOptionsWindow::UTEOptionsWindow ()
{
	m_bUseDiagnostics = true;
	ZeroMemory (m_szClassname, SIZE_STRING);
	m_ID = 0;
	m_bwindowinitialised = false;

	m_bctrlencenabled = false;
	m_baltencenabled = false;

	m_bctrldecenabled = false;
	m_baltdecenabled = false;

	m_buteenabled = false;
	m_bstatusshown = false;
	
	m_benckeylisten = false;
	m_bdeckeylisten = false;
}

UTEOptionsWindow::~UTEOptionsWindow ()
{

}

void UTEOptionsWindow::SetDiagnostics (Diagnostics *pdiag)
{
	m_pdiag = pdiag;

}

void UTEOptionsWindow::Initialise (HWND hWnd, unsigned int uID)
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
	SetCaption (TEXT ("Universal Text Encryption"));
	SetWindowStyle (FS_STYLESTANDARD);

	// Create the class name
	strcat_s (m_szClassname, SIZE_STRING, "CCRYPTUTEOptions");
	sprintf_s (szID, SIZE_STRING, "%d", uID);
	strcat_s (m_szClassname, SIZE_STRING, szID);

	if (m_bwindowinitialised == false) {
		CreateAppWindow (m_szClassname, 70, 0, 350, 250, true);
		m_uihandler.SetWindowProperties (0, 0, 350, 0, RGB (230, 230, 240));
		SetWindowPosition (FS_CENTER);
		m_bwindowinitialised = true;
	} 

	Show ();
}

void UTEOptionsWindow::OnDestroy (HWND hWnd)
{			
	Hide ();
}

void UTEOptionsWindow::OnCreate (HWND hWnd)
{				
	// Make this window handle global
	m_hwnd = hWnd;

	/*
	#define ID_UTEBTNOK						914
	#define ID_UTEBTNCANCEL					915
	#define ID_UTEBTNENABLE					916
	#define ID_UTELBLHOTENC					917
	#define ID_UTELBLHOTDEC					918
	#define ID_UTEBTNHOTENCCTRL				919
	#define ID_UTEBTNHOTDECCTRL				920
	#define ID_UTEBTNHOTENCALT				921
	#define ID_UTEBTNHOTDECALT				922
	#define ID_UTETXTHOTENC					923
	#define ID_UTETXTHOTDEC					924
	#define ID_UTEBTNHOTENCLISTEN			925
	#define ID_UTEBTNHOTDECLISTEN			926
	#define ID_UTELBLHOTKEYSTATUS			927
	*/

	HFONT hfDefault = (HFONT) GetStockObject (DEFAULT_GUI_FONT);

	// CHECKBOXES
	m_chkenable = CreateWindow ("button", "Enable Universal Text Encryption", WS_CHILD | WS_VISIBLE | BS_CHECKBOX, 25, 12, 190, 21, hWnd, (HMENU) ID_UTEBTNENABLE, GetModuleHandle (NULL), NULL);
	SendMessage (m_chkenable, WM_SETFONT, (WPARAM) hfDefault, MAKELPARAM (FALSE, 0));

	m_chkhotencctrl = CreateWindow ("button", "Ctrl", WS_CHILD | WS_VISIBLE | BS_CHECKBOX, 71, 58, 44, 21, hWnd, (HMENU) ID_UTEBTNHOTENCCTRL, GetModuleHandle (NULL), NULL);
	SendMessage (m_chkhotencctrl, WM_SETFONT, (WPARAM) hfDefault, MAKELPARAM (FALSE, 0));

	m_chkhotencalt = CreateWindow ("button", "Alt", WS_CHILD | WS_VISIBLE | BS_CHECKBOX, 121, 58, 44, 21, hWnd, (HMENU) ID_UTEBTNHOTENCALT, GetModuleHandle (NULL), NULL);
	SendMessage (m_chkhotencalt, WM_SETFONT, (WPARAM) hfDefault, MAKELPARAM (FALSE, 0));

	m_chkhotdecctrl = CreateWindow ("button", "Ctrl", WS_CHILD | WS_VISIBLE | BS_CHECKBOX, 71, 115, 44, 21, hWnd, (HMENU) ID_UTEBTNHOTDECCTRL, GetModuleHandle (NULL), NULL);
	SendMessage (m_chkhotdecctrl, WM_SETFONT, (WPARAM) hfDefault, MAKELPARAM (FALSE, 0));

	m_chkhotdecalt = CreateWindow ("button", "Alt", WS_CHILD | WS_VISIBLE | BS_CHECKBOX, 121, 115, 44, 21, hWnd, (HMENU) ID_UTEBTNHOTDECALT, GetModuleHandle (NULL), NULL);
	SendMessage (m_chkhotdecalt, WM_SETFONT, (WPARAM) hfDefault, MAKELPARAM (FALSE, 0));
	
	//LABELS
	m_lblhotenc = CreateWindow ("static", "Encryption Hot Key", WS_CHILD | WS_VISIBLE, 38, 39, 114, 13, hWnd, (HMENU) ID_UTELBLHOTENC, GetModuleHandle (NULL), NULL);
	SendMessage (m_lblhotenc, WM_SETFONT, (WPARAM) hfDefault, MAKELPARAM (FALSE, 0));

	m_lblhotdec = CreateWindow ("static", "Decryption Hot Key", WS_CHILD | WS_VISIBLE, 38, 92, 114, 13, hWnd, (HMENU) ID_UTELBLHOTDEC, GetModuleHandle (NULL), NULL);
	SendMessage (m_lblhotdec, WM_SETFONT, (WPARAM) hfDefault, MAKELPARAM (FALSE, 0));

	m_lblhotkeystatus = CreateWindow ("static", "Please press the key you wish to use...", WS_CHILD | WS_VISIBLE, 68, 149, 216, 19, hWnd, (HMENU) ID_UTELBLHOTKEYSTATUS, GetModuleHandle (NULL), NULL);
	SendMessage (m_lblhotkeystatus, WM_SETFONT, (WPARAM) hfDefault, MAKELPARAM (FALSE, 0));
	ShowWindow (m_lblhotkeystatus, SW_HIDE);

	// BUTTONS
	m_btnhotenclisten = CreateWindow ("button", "Set Hot Key", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 210, 57, 86, 22, hWnd, (HMENU) ID_UTEBTNHOTENCLISTEN, GetModuleHandle (NULL), NULL);
	SendMessage (m_btnhotenclisten, WM_SETFONT, (WPARAM) hfDefault, MAKELPARAM (FALSE, 0));

	m_btnhotdeclisten = CreateWindow ("button", "Set Hot Key", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 210, 114, 86, 22, hWnd, (HMENU) ID_UTEBTNHOTDECLISTEN, GetModuleHandle (NULL), NULL);
	SendMessage (m_btnhotdeclisten, WM_SETFONT, (WPARAM) hfDefault, MAKELPARAM (FALSE, 0));

	m_btnok = CreateWindow ("button", "Ok", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 177, 180, 68, 22, hWnd, (HMENU) ID_UTEBTNOK, GetModuleHandle (NULL), NULL);
	SendMessage (m_btnok, WM_SETFONT, (WPARAM) hfDefault, MAKELPARAM (FALSE, 0));

	m_btncancel = CreateWindow ("button", "Cancel", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 251, 180, 68, 22, hWnd, (HMENU) ID_UTEBTNCANCEL, GetModuleHandle (NULL), NULL);
	SendMessage (m_btncancel, WM_SETFONT, (WPARAM) hfDefault, MAKELPARAM (FALSE, 0));

	// TEXT BOXES
	m_txthotenc = CreateWindow ("edit", "", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY | ES_LEFT | ES_AUTOHSCROLL, 171, 58, 33, 20, hWnd, (HMENU) ID_UTETXTHOTENC, GetModuleHandle (NULL), NULL);
	SendMessage (m_txthotenc, WM_SETFONT, (WPARAM) hfDefault, MAKELPARAM (FALSE, 0));

	m_txthotdec = CreateWindow ("edit", "", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY | ES_LEFT | ES_AUTOHSCROLL, 171, 115, 33, 20, hWnd, (HMENU) ID_UTETXTHOTDEC, GetModuleHandle (NULL), NULL);
	SendMessage (m_txthotdec, WM_SETFONT, (WPARAM) hfDefault, MAKELPARAM (FALSE, 0));

}

void UTEOptionsWindow::SetHotKeyControlsEnabled (bool m_benabled)
{
	if (m_benabled == true) {
		EnableWindow (m_chkhotencctrl, true);
		EnableWindow (m_chkhotencalt, true);
		EnableWindow (m_chkhotdecctrl, true);
		EnableWindow (m_chkhotdecalt, true);
		EnableWindow (m_lblhotenc, true);
		EnableWindow (m_lblhotdec, true);
		EnableWindow (m_btnhotenclisten, true);
		EnableWindow (m_btnhotdeclisten, true);
		EnableWindow (m_txthotenc, true);
		EnableWindow (m_txthotdec, true);
	} else {
		EnableWindow (m_chkhotencctrl, false);
		EnableWindow (m_chkhotencalt, false);
		EnableWindow (m_chkhotdecctrl, false);
		EnableWindow (m_chkhotdecalt, false);
		EnableWindow (m_lblhotenc, false);
		EnableWindow (m_lblhotdec, false);
		EnableWindow (m_btnhotenclisten, false);
		EnableWindow (m_btnhotdeclisten, false);
		EnableWindow (m_txthotenc, false);
		EnableWindow (m_txthotdec, false);	
	}
}

void UTEOptionsWindow::SetFlashingStatus (bool m_benabled)
{
	if (m_benabled == true) {
		SetTimer (m_hwnd, IDT_STATUSFLASH, 500, NULL);
	} else {
		KillTimer (m_hwnd, IDT_STATUSFLASH);
		ShowWindow (m_lblhotkeystatus, SW_HIDE);
	}
}

void UTEOptionsWindow::OutputInt (LPCSTR lpszText, int iValue)
{
	if (m_bUseDiagnostics == true) {
		m_pdiag->OutputInt (lpszText, iValue);
	}
}

void UTEOptionsWindow::OutputText (LPCSTR lpszText)
{
	if (m_bUseDiagnostics == true) {
		m_pdiag->OutputText (lpszText);
	}
}

void UTEOptionsWindow::OutputText (LPCSTR lpszName, LPCSTR lpszValue)
{
	if (m_bUseDiagnostics == true) {
		m_pdiag->OutputText (lpszName, lpszValue);
	}
}

void UTEOptionsWindow::OnCommand (HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	/*
	#define ID_UTEBTNOK						914
	#define ID_UTEBTNCANCEL					915
	#define ID_UTEBTNENABLE					916
	#define ID_UTELBLHOTENC					917
	#define ID_UTELBLHOTDEC					918
	#define ID_UTEBTNHOTENCCTRL				919
	#define ID_UTEBTNHOTDECCTRL				920
	#define ID_UTEBTNHOTENCALT				921
	#define ID_UTEBTNHOTDECALT				922
	#define ID_UTETXTHOTENC					923
	#define ID_UTETXTHOTDEC					924
	#define ID_UTEBTNHOTENCLISTEN			925
	#define ID_UTEBTNHOTDECLISTEN			926
	#define ID_UTELBLHOTKEYSTATUS			927

	SendMessage (m_hwndoptfile, BM_SETCHECK, 1, 0);
	*/
	switch (LOWORD (wParam)) {
		case ID_UTEBTNENABLE:
		{
			if (m_buteenabled == false) {
				m_buteenabled = true;
				SetHotKeyControlsEnabled (true);
				SendMessage (m_chkenable, BM_SETCHECK, 1, 0);
			} else {
				m_buteenabled = false;
				SetHotKeyControlsEnabled (false);
				SendMessage (m_chkenable, BM_SETCHECK, 0, 0);
			}			
		}
		break;
		case ID_UTEBTNHOTENCCTRL:
		{
			if (m_bctrlencenabled == false) {
				m_bctrlencenabled = true;
				SendMessage (m_chkhotencctrl, BM_SETCHECK, 1, 0);
			} else {
				m_bctrlencenabled = false;
				SendMessage (m_chkhotencctrl, BM_SETCHECK, 0, 0);
			}
		}
		break;
		case ID_UTEBTNHOTENCALT:
		{
			if (m_baltencenabled == false) {
				m_baltencenabled = true;
				SendMessage (m_chkhotencalt, BM_SETCHECK, 1, 0);
			} else {
				m_baltencenabled = false;
				SendMessage (m_chkhotencalt, BM_SETCHECK, 0, 0);
			}
		}
		break;

		case ID_UTEBTNHOTDECCTRL:
		{
			if (m_bctrldecenabled == false) {
				m_bctrldecenabled = true;
				SendMessage (m_chkhotdecctrl, BM_SETCHECK, 1, 0);
			} else {
				m_bctrldecenabled = false;
				SendMessage (m_chkhotdecctrl, BM_SETCHECK, 0, 0);
			}
		}
		break;
		case ID_UTEBTNHOTDECALT:
		{
			if (m_baltdecenabled == false) {
				m_baltdecenabled = true;
				SendMessage (m_chkhotdecalt, BM_SETCHECK, 1, 0);
			} else {
				m_baltdecenabled = false;
				SendMessage (m_chkhotdecalt, BM_SETCHECK, 0, 0);
			}
		}
		break;

		case ID_UTEBTNHOTENCLISTEN:
		{
			SetFlashingStatus (true);
			m_benckeylisten = true;
			SetFocus (m_hwnd);
		}
		break;

		case ID_UTEBTNHOTDECLISTEN:
		{
			SetFlashingStatus (true);
			m_bdeckeylisten = true;
			SetFocus (m_hwnd);
		}
		break;
	}
}

void UTEOptionsWindow::OnKeyUp (HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	char szVirtualKey[SIZE_NAME];

	if (m_benckeylisten == true) {
		SetFlashingStatus (false);
		m_benckeylisten = false;
		
		
		ZeroMemory (szVirtualKey, SIZE_NAME);
		sprintf_s (szVirtualKey, SIZE_NAME, "%i", wParam);

		SetDlgItemText (m_hwnd, ID_UTETXTHOTENC, szVirtualKey);


	}

	if (m_bdeckeylisten == true) {
		SetFlashingStatus (false);

	}
}

void UTEOptionsWindow::OnUIScroll (HWND hWnd, WPARAM wParam, LPARAM lParam)
{

}

void UTEOptionsWindow::OnUICommand (HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
		
	}
}

void UTEOptionsWindow::OnTimer (WPARAM wParam)
{	
	m_uihandler.NotifyTimer (wParam);

	switch (wParam)
	{
		case IDT_STATUSFLASH:
		{
			if (m_bstatusshown == false) {
				m_bstatusshown = true;
				ShowWindow (m_lblhotkeystatus, SW_SHOW);
			} else {
				m_bstatusshown = false;
				ShowWindow (m_lblhotkeystatus, SW_HIDE);
			}
		}
	}
}

void UTEOptionsWindow::OnPaint (HWND hWnd)
{
	m_uihandler.PaintControls (hWnd);	
}

void UTEOptionsWindow::OnMouseMove (HWND hWnd, int mouseXPos, int mouseYPos)
{	
	m_uihandler.NotifyMouseMove (mouseXPos, mouseYPos);
}

void UTEOptionsWindow::OnLButtonDown (HWND hWnd)
{
	m_uihandler.NotifyMouseDown ();
}

void UTEOptionsWindow::OnLButtonUp (HWND hWnd)
{
	m_uihandler.NotifyMouseUp ();
}