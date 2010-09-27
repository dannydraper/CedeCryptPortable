// This work is dedicated to the Lord our God. King of Heaven, Lord of Heaven's Armies.

#include "PasswordWindow.h"

PasswordWindow::PasswordWindow ()
{
	m_bUseDiagnostics = true;
	ZeroMemory (m_szClassname, SIZE_STRING);
	m_ID = 0;
	m_bSessionmode = false;
	m_bInitialised = false;
	m_bEncryptmode = false;
}

PasswordWindow::~PasswordWindow ()
{

}

void PasswordWindow::SetDiagnostics (Diagnostics *pdiag)
{
	m_pdiag = pdiag;
	m_mainconv.SetDiagnostics (pdiag);
}

void PasswordWindow::Initialise (HWND hWnd, unsigned int uID)
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
	SetCaption (TEXT ("Please enter a password"));
	SetWindowStyle (FS_STYLESTANDARD);

	// Create the class name
	strcat_s (m_szClassname, SIZE_STRING, "PASSWindowClass");
	sprintf_s (szID, SIZE_STRING, "%d", uID);
	strcat_s (m_szClassname, SIZE_STRING, szID);

	// Clear the multicontent control
	m_mainconv.Clear ();

	if (m_bInitialised == false) {
		m_bInitialised = true;
		CreateAppWindow (m_szClassname, 70, 0, 299, 220, true);
		m_uihandler.SetWindowProperties (0, 0, 299, 0, GetSysColor (COLOR_BTNFACE));
	}
	
	SetWindowPosition (FS_CENTER);
	SetAlwaysOnTop (true);
	Show ();
}

char *PasswordWindow::GetLastPassword ()
{
	return (char *) m_szPassword;
}

void PasswordWindow::OnDestroy (HWND hWnd)
{			
	Hide ();
}

void PasswordWindow::OnCreate (HWND hWnd)
{				
	// Make this window handle global
	m_hwnd = hWnd;
	g_hWnd = hWnd;
	
	m_header.SetBitmapResources (IDB_PASSHEADER);
	m_header.SetBitmapProperties (0, 0, 293, 33);
	m_header.SetProperties (hWnd, CID_HEADER, 0, 0, 293, 33);
	m_uihandler.AddDirectControl (&m_header);

	// The info rect background
	m_inforect.SetProperties (hWnd, CID_HEADER, 0, 33, 293, 24);
	m_uihandler.AddDirectControl (&m_inforect);

	// The info labels
	m_info1.SetTextProperties (TEXT ("Arial"), 70, 0, 0, 0, "Your password must be longer than 4 characters and can be up to", RGB (0, 0, 0));
	m_info1.SetProperties (hWnd, CID_HEADER, 3, 34, 280, 19);
	m_uihandler.AddDirectControl (&m_info1);

	m_info2.SetTextProperties (TEXT ("Arial"), 70, 0, 0, 0, "40 characters.", RGB (0, 0, 0));
	m_info2.SetProperties (hWnd, CID_HEADER, 3, 45, 280, 19);
	m_uihandler.AddDirectControl (&m_info2);

	// Password edit box
	HFONT hfDefault = (HFONT) GetStockObject (DEFAULT_GUI_FONT);
	m_hwndeditpass = CreateWindow ("edit", "", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_PASSWORD | ES_LEFT , 10, 70, 273, 19, hWnd, (HMENU) ID_EDITPASSWORD, GetModuleHandle (NULL), NULL);
	SendMessage (m_hwndeditpass, WM_SETFONT, (WPARAM) hfDefault, MAKELPARAM (FALSE, 0));

	m_lblpass2info = CreateLabel ("Please confirm password:", 10, 93, 273, 19, ID_LBLSTATIC);

	m_hwndeditpass2 = CreateWindow ("edit", "", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_PASSWORD | ES_LEFT , 10, 110, 273, 19, hWnd, (HMENU) ID_EDITPASSWORD2, GetModuleHandle (NULL), NULL);
	SendMessage (m_hwndeditpass2, WM_SETFONT, (WPARAM) hfDefault, MAKELPARAM (FALSE, 0));

	// The session information label
	m_hwndsessionlbl = CreateWindow ("static", "<Session Information>", WS_CHILD | WS_VISIBLE, 10, 135, 273, 40, hWnd, (HMENU) ID_LBLSESSIONINFO, GetModuleHandle (NULL), NULL);
	SendMessage (m_hwndsessionlbl, WM_SETFONT, (WPARAM) hfDefault, MAKELPARAM (FALSE, 0));
	ShowWindow (m_hwndsessionlbl, SW_HIDE);

	// The ok button
	m_hwndbtnpassok = CreateWindow ("button", "Ok", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 138, 140, 70, 23, hWnd, (HMENU) ID_BTNPASSOK, GetModuleHandle (NULL), NULL);
	SendMessage (m_hwndbtnpassok, WM_SETFONT, (WPARAM) hfDefault, MAKELPARAM (FALSE, 0));

	// The cancel button
	m_hwndbtnpasscancel = CreateWindow ("button", "Cancel", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 213, 140, 70, 23, hWnd, (HMENU) ID_BTNPASSCANCEL, GetModuleHandle (NULL), NULL);
	SendMessage (m_hwndbtnpasscancel, WM_SETFONT, (WPARAM) hfDefault, MAKELPARAM (FALSE, 0));

	SetFocus (m_hwndeditpass);

	lpfnOldWndProc = (FARPROC) SetWindowLongPtr (m_hwndeditpass, GWLP_WNDPROC, (DWORD) SubProc);
	lpfnOldWndProc2 = (FARPROC) SetWindowLongPtr (m_hwndeditpass2, GWLP_WNDPROC, (DWORD) SubProc2);

	ppwnd = (PasswordWindow *) this;
}

void PasswordWindow::SetSessionMode (bool bSessionmode, bool bEncrypt)
{
	m_bSessionmode = bSessionmode;

	if (bSessionmode == true) {

		if (m_bEncryptmode == true) {
			ResizeWindow (299, 230);
			MoveWindow (m_hwndbtnpasscancel, 213, 170, 70, 23, true);
			MoveWindow (m_hwndbtnpassok, 138, 170, 70, 23, true);
			MoveWindow (m_hwndsessionlbl,  10, 135, 273, 40, true);

			ShowWindow (m_hwndsessionlbl, SW_SHOW);

			ShowWindow (m_lblpass2info, SW_SHOW);
			ShowWindow (m_hwndeditpass2, SW_SHOW);

		} else {
			ResizeWindow (299, 190);
			MoveWindow (m_hwndbtnpasscancel, 213, 130, 70, 23, true);
			MoveWindow (m_hwndbtnpassok, 138, 130, 70, 23, true);
			MoveWindow (m_hwndsessionlbl,  10, 100, 273, 40, true);
			
			ShowWindow (m_hwndsessionlbl, SW_SHOW);

			ShowWindow (m_lblpass2info, SW_HIDE);
			ShowWindow (m_hwndeditpass2, SW_HIDE);
		}

		if (bEncrypt == true) {
			SetDlgItemText (m_hwnd, ID_LBLSESSIONINFO, "Please choose and enter a session password for your protected folders.");
		} else {
			SetDlgItemText (m_hwnd, ID_LBLSESSIONINFO, "Please enter your session password for your protected folders.");
		}

	} else {

		if (m_bEncryptmode == true) {
			ResizeWindow (299, 200);
			MoveWindow (m_hwndbtnpasscancel, 213, 140, 70, 23, true);
			MoveWindow (m_hwndbtnpassok, 138, 140, 70, 23, true);
			ShowWindow (m_hwndsessionlbl, SW_HIDE);

			ShowWindow (m_lblpass2info, SW_SHOW);
			ShowWindow (m_hwndeditpass2, SW_SHOW);

		} else {
			ResizeWindow (299, 170);
			MoveWindow (m_hwndbtnpasscancel, 213, 105, 70, 23, true);
			MoveWindow (m_hwndbtnpassok, 138, 105, 70, 23, true);
			ShowWindow (m_hwndsessionlbl, SW_HIDE);

			ShowWindow (m_lblpass2info, SW_HIDE);
			ShowWindow (m_hwndeditpass2, SW_HIDE);
		}
		
	}

	SetFocus (m_hwndeditpass);
}

void PasswordWindow::SetSecondFocus ()
{
	SetFocus (m_hwndeditpass2);
}

void PasswordWindow::SetEncryptMode (bool bEncrypt)
{
	m_bEncryptmode = bEncrypt;
}

bool PasswordWindow::GetSessionMode ()
{
	return m_bSessionmode;
}

void PasswordWindow::OutputInt (LPCSTR lpszText, int iValue)
{
	if (m_bUseDiagnostics == true) {
		m_pdiag->OutputInt (lpszText, iValue);
	}
}

void PasswordWindow::OutputText (LPCSTR lpszText)
{
	if (m_bUseDiagnostics == true) {
		m_pdiag->OutputText (lpszText);
	}
}

void PasswordWindow::OutputText (LPCSTR lpszName, LPCSTR lpszValue)
{
	if (m_bUseDiagnostics == true) {
		m_pdiag->OutputText (lpszName, lpszValue);
	}
}

void PasswordWindow::ParseUserInput ()
{
	// Every time a user presses a key on the user input text box
	// on the conversation window, we need to know what the last 2 characters
	// were so we can determine if the enter key was pressed. Which will
	// trigger the process chat input function.
	char szInput[SIZE_STRING];
	ZeroMemory (szInput, SIZE_STRING);
	
	GetDlgItemText (m_hwnd, ID_EDITPASSWORD, szInput, SIZE_STRING);

	BYTE bChar1;
	BYTE bChar2;
	
	memcpy (&bChar1, szInput+strlen(szInput)-2, 1);
	memcpy (&bChar2, szInput+strlen(szInput)-1, 1);
	
	if (bChar1 == 13 && bChar2 == 10) {
		//OutputText ("Enter was pressed.");
		//ProcessChatInput (szInput);

		//SetDlgItemText (m_hwnd, ID_EDITPASSWORD, "");
		PostMessage (m_hwnd, WM_COMMAND, ID_BTNPASSOK, 0);
	}
}

void PasswordWindow::ParseUserInput2 ()
{
	// Every time a user presses a key on the user input text box
	// on the conversation window, we need to know what the last 2 characters
	// were so we can determine if the enter key was pressed. Which will
	// trigger the process chat input function.
	char szInput[SIZE_STRING];
	ZeroMemory (szInput, SIZE_STRING);
	
	GetDlgItemText (m_hwnd, ID_EDITPASSWORD2, szInput, SIZE_STRING);

	BYTE bChar1;
	BYTE bChar2;
	
	memcpy (&bChar1, szInput+strlen(szInput)-2, 1);
	memcpy (&bChar2, szInput+strlen(szInput)-1, 1);
	
	if (bChar1 == 13 && bChar2 == 10) {
		//OutputText ("Enter was pressed.");
		//ProcessChatInput (szInput);

		//SetDlgItemText (m_hwnd, ID_EDITPASSWORD, "");
		PostMessage (m_hwnd, WM_COMMAND, ID_BTNPASSOK, 0);
	}
}

void PasswordWindow::CheckPassword ()
{
	ZeroMemory (m_szPassword, SIZE_NAME);
	GetDlgItemText (m_hwnd, ID_EDITPASSWORD, m_szPassword, SIZE_NAME);
	SetDlgItemText (m_hwnd, ID_EDITPASSWORD, "");
	

	char szPassword2[SIZE_NAME];
	ZeroMemory (szPassword2, SIZE_NAME);
	GetDlgItemText (m_hwnd, ID_EDITPASSWORD2, szPassword2, SIZE_NAME);
	SetDlgItemText (m_hwnd, ID_EDITPASSWORD2, "");

	if (strlen (m_szPassword) < 5 || strlen (m_szPassword) > 40) {
		MessageBox (m_hwnd, "Your password must be greater than 4 characters and less than 40 characters.", "Password Input", MB_OK | MB_ICONEXCLAMATION);
	} else {
		if (m_bEncryptmode == true) {
			
			if (strcmp (m_szPassword, szPassword2) == 0) {
				PostMessage (m_parenthwnd, WM_UICOMMAND, (WPARAM) CID_PASSWORDOK, 0);
				Hide ();				
			} else {
				MessageBox (m_hwnd, "The passwords you have entered do not match. Please retype your password.", "Password Input", MB_OK | MB_ICONEXCLAMATION);			
				SetFocus (m_hwndeditpass);
			}

		} else {
			PostMessage (m_parenthwnd, WM_UICOMMAND, (WPARAM) CID_PASSWORDOK, 0);
			Hide ();				
		}

		
	}
}

void PasswordWindow::ClearPassword ()
{
	ZeroMemory (m_szPassword, SIZE_NAME);
}

void PasswordWindow::OnCommand (HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	
	switch (LOWORD (wParam)) {
		case ID_BTNPASSOK:
		{
			CheckPassword ();
		}
		break;
		case ID_BTNPASSCANCEL:
		{
			ZeroMemory (m_szPassword, SIZE_NAME);
			PostMessage (m_parenthwnd, WM_UICOMMAND, (WPARAM) CID_PASSWORDCANCEL, 0);
			Hide ();

			//FARPROC lpfnOldWndProc;
			

		}
		break;

		case ID_EDITPASSWORD:
		{
			
			switch (HIWORD (wParam)) {
				case EN_CHANGE:
				{
					ParseUserInput ();
				}
				break;
			}
		}
		break;
		
		case ID_EDITPASSWORD2:
		{			
			switch (HIWORD (wParam)) {
				case EN_CHANGE:
				{
					ParseUserInput2 ();
				}
				break;
			}
		}
		break;
		case IDOK:
		{
			MessageBox (NULL, "Enter", "Info", MB_OK);
		}
		break;
	}
}


long FAR PASCAL PasswordWindow::SubProc(HWND hWnd, WORD wMessage, WORD wParam, LONG lParam)
{
	switch (wMessage)
    {	
		
        //case WM_GETDLGCODE:
		//{
		//	return (DLGC_WANTALLKEYS | CallWindowProc((WNDPROC) lpfnOldWndProc, hWnd, wMessage, wParam, lParam));	
		//}
		//break;
		
        case WM_CHAR:
		{
			//Process this message to avoid message beeps.
			if ((wParam == VK_RETURN) || (wParam == VK_TAB)) {
				return 0;
			} else {
				return (CallWindowProc((WNDPROC) lpfnOldWndProc, hWnd, wMessage, wParam, lParam));
			}
		}
        break;
		
        case WM_KEYDOWN:
		{			
			if ((wParam == VK_RETURN) || (wParam == VK_TAB)) {
				//PostMessage (ghDlg, WM_NEXTDLGCTL, 0, 0L);
				if (ppwnd->m_bEncryptmode == true) {
					ppwnd->SetSecondFocus ();
				} else {
					ppwnd->CheckPassword ();
				}				
				return FALSE;
			}
		}
		break;		
	}
	return (CallWindowProc((WNDPROC) lpfnOldWndProc, hWnd, wMessage, wParam, lParam)); 
}

long FAR PASCAL PasswordWindow::SubProc2(HWND hWnd, WORD wMessage, WORD wParam, LONG lParam)
{
	switch (wMessage)
    {	
		
        //case WM_GETDLGCODE:
		//{
		//	return (DLGC_WANTALLKEYS | CallWindowProc((WNDPROC) lpfnOldWndProc, hWnd, wMessage, wParam, lParam));	
		//}
		//break;
		
        case WM_CHAR:
		{
			//Process this message to avoid message beeps.
			if ((wParam == VK_RETURN) || (wParam == VK_TAB)) {
				return 0;
			} else {
				return (CallWindowProc((WNDPROC) lpfnOldWndProc2, hWnd, wMessage, wParam, lParam));
			}
		}
        break;
		
        case WM_KEYDOWN:
		{			
			if ((wParam == VK_RETURN) || (wParam == VK_TAB)) {
				//PostMessage (ghDlg, WM_NEXTDLGCTL, 0, 0L);
				ppwnd->CheckPassword ();
				return FALSE;
			}
		}
		break;		
	}
	return (CallWindowProc((WNDPROC) lpfnOldWndProc, hWnd, wMessage, wParam, lParam)); 
}

void PasswordWindow::OnUIScroll (HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	m_mainconv.NotifyUIScroll (hWnd, wParam, lParam);
}

void PasswordWindow::OnUICommand (HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{

	}
}

void PasswordWindow::OnTimer (WPARAM wParam)
{	
	m_uihandler.NotifyTimer (wParam);	
}

void PasswordWindow::OnPaint (HWND hWnd)
{
	m_uihandler.PaintControls (hWnd);	
}

void PasswordWindow::OnMouseMove (HWND hWnd, int mouseXPos, int mouseYPos)
{	
	m_uihandler.NotifyMouseMove (mouseXPos, mouseYPos);
}

void PasswordWindow::OnLButtonDown (HWND hWnd)
{
	m_uihandler.NotifyMouseDown ();
}

void PasswordWindow::OnLButtonUp (HWND hWnd)
{
	m_uihandler.NotifyMouseUp ();
}