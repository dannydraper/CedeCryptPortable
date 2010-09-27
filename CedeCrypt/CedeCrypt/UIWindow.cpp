#include "UIWindow.h"

UIWindow::UIWindow ()
{
	g_Value = 0;
	g_ParentHWND = NULL;
	g_hWnd = NULL;
	g_lpszCaption = "";
	g_bgcolour = RGB (0, 0, 0);
	m_bStyleset = false;
	g_bWindowActive = true;
	g_iDelayedPaintCount = 0;
	m_iSelectedstyle = 0;
	g_bUsingSysColour = false;
}

UIWindow::~UIWindow ()
{
}

LRESULT WINAPI UIWindow::BaseProc (HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	UIWindow *pWnd; // Pointer to this objects instance of UIWindow
	// If the message is not an initial creation message then we must have already
	// stored the object pointer in the window user data, therefore retrieve the pointer!
	if (msg != WM_NCCREATE) {
		pWnd = (UIWindow *) GetWindowLongPtr (hWnd, GWLP_USERDATA);
	}
	switch (msg)
	{		
		case WM_NCCREATE:
		{
			// If this is the very first time this window object is created then we need to
			// store this objects instance pointer in the USERDATA section using SetWindowLongPtr
			// this is so that when future messages are received by this window, we will know which
			// object instance must process the messages.
			SetWindowLongPtr (hWnd, GWLP_USERDATA, (LONG_PTR) ((LPCREATESTRUCT)lParam)->lpCreateParams);
		}
		break;
		case WM_CREATE:
		{
			pWnd->OnCreate (hWnd);
		}
		break;
		case WM_PAINT:
		{
			pWnd->OnPaint (hWnd); // Process all child paint events.		
		}
		break;
		case WM_MOUSEMOVE:
		{
			pWnd->OnMouseMove (hWnd, LOWORD (lParam), HIWORD (lParam));
		}
		break;
		case WM_TIMER:
		{						
			pWnd->OnTimer (wParam); // Process all child Timer events.
		}
		break;
		case WM_LBUTTONDOWN:
		{
			pWnd->OnLButtonDown (hWnd);
		}
		break;
		case WM_MOUSEWHEEL:
		{
			pWnd->OnMouseWheel (hWnd, wParam, lParam);
		}
		break;
		case WM_LBUTTONUP:
		{
			pWnd->OnLButtonUp (hWnd);
		}
		break;
		case WM_LBUTTONDBLCLK:
		{
			pWnd->OnLButtonDblClick (hWnd, wParam, lParam);
		}
		break;
		case WM_NOTIFY:
		{
			pWnd->OnNotify (hWnd, wParam, lParam);
		}
		break;
		case WM_UISCROLL:
		{
			pWnd->OnUIScroll (hWnd, wParam, lParam);
		}
		break;
		case WM_COMMAND:
		{
			pWnd->OnCommand (hWnd, wParam, lParam);
		}
		break;
		case WM_UICOMMAND:
		{
			pWnd->OnUICommand (hWnd, wParam, lParam);
			pWnd->OnUICommand (hWnd, wParam);
		}
		break;
		case WM_HOTKEY:
		{			
			pWnd->OnHotKey (hWnd, wParam, lParam);
		}
		break;
		case WM_KEYUP:
		{
			pWnd->OnKeyUp (hWnd, wParam, lParam);
		}
		break;
		case WM_UIHIGHLIGHT:
		{
			pWnd->OnUIHighlight (hWnd, wParam);
		}
		break;
		case WM_UINOHIGHLIGHT:
		{
			pWnd->OnUINoHighlight (hWnd, wParam);
		}
		break;
		case WM_ACTIVATE:
		{
			// Check if the window was activated or deactived, we then set a global
			// flag to indicate the window state so that delayed painting will work properly.
			switch (LOWORD (wParam))
			{
				case WA_ACTIVE:
				{
					pWnd->g_bWindowActive = true;
				}
				break;
				case WA_CLICKACTIVE:
				{
					pWnd->g_bWindowActive = true;
				}
				break;
				case WA_INACTIVE:
				{
					pWnd->g_bWindowActive = false;
				}
				break;
			}
		}
		break;
		case WM_DESTROY:
		{
			pWnd->OnDestroy (hWnd);
		}
		break;		
		case WM_CLOSE:
		{
			pWnd->OnClose (hWnd);
		}
		break;
		case CRYPT_MSG:
		{
			pWnd->OnCryptEvent (hWnd, wParam, lParam);
		}
		break;		
		case WM_QUERYENDSESSION:
		{
			return pWnd->OnQueryEndSession (hWnd, lParam);
		}
		break;
		case WM_ENDSESSION:
		{
			return pWnd->OnEndSession (hWnd, wParam, lParam);
		}
		break;
		case WM_POWERBROADCAST:
		{
			return pWnd->OnPowerBroadcast (hWnd, wParam, lParam);
		}
		break;
		case WM_DROPFILES:
		{
			pWnd->OnDropFiles (hWnd, wParam, lParam);
		}
		break;
	}

	return DefWindowProc (hWnd, msg, wParam, lParam);
}

void UIWindow::SetValue (int iValue)
{
	g_Value = iValue;
}

void UIWindow::SetParentHWND (HWND hWnd)
{
	g_ParentHWND = hWnd;
}

bool UIWindow::IsWindowFocused ()
{
	return g_bWindowActive;
}

void UIWindow::FlashWindow ()
{
	FLASHWINFO flashinfo;
	flashinfo.cbSize = sizeof (FLASHWINFO);
	flashinfo.hwnd = g_hWnd;
	flashinfo.dwFlags = FLASHW_ALL | FLASHW_TIMER;
	flashinfo.uCount = 7;
	flashinfo.dwTimeout = 0;

	FlashWindowEx (&flashinfo);
}

HWND UIWindow::GetBaseHWND ()
{
	return g_hWnd;
}

void UIWindow::SetCaption (LPCSTR lpszCaption)
{
	g_lpszCaption = lpszCaption;
}

void UIWindow::SetCaption (LPCSTR lpszCaption, bool refresh)
{
	SetWindowText (g_hWnd, lpszCaption);
}

void UIWindow::SetBgSysColor (int iSysColor)
{
	g_bUsingSysColour = true;
	g_iChosenSysColour = iSysColor;
}


void UIWindow::SetBgColor (COLORREF bgcolour)
{
	g_bgcolour = bgcolour;
	g_bUsingSysColour = false;
}

void UIWindow::Show ()
{
	// Show the window
	ShowWindow (g_hWnd, SW_SHOW);
	UpdateWindow (g_hWnd);
}

void UIWindow::ShowEx (int nCmdShow)
{
	ShowWindow (g_hWnd, nCmdShow);
	UpdateWindow (g_hWnd);
}

void UIWindow::Restore ()
{
	SendMessage (g_hWnd, WM_SYSCOMMAND, SC_RESTORE, 0);
	SetFocus (g_hWnd);
}

void UIWindow::Minimise ()
{
	SendMessage (g_hWnd, WM_SYSCOMMAND, SC_MINIMIZE, 0);
}

void UIWindow::Hide ()
{
	// Hide the window
	ShowWindow (g_hWnd, SW_HIDE);
}

void UIWindow::SetProperties (int xPos, int yPos, int Width, int Height)
{
	MoveWindow (g_hWnd, xPos, yPos, Width, Height, true);
}

void UIWindow::ShowInt (int iInttoShow) {
	char szMsg[255];
	ZeroMemory (szMsg, 255);
	sprintf_s (szMsg, 255, "Value of int: %d", iInttoShow);
	MessageBox (NULL, szMsg, "ShowInt", MB_OK);
}

void UIWindow::SetAlwaysOnTop (bool bOntop) {
	
	RECT myRect;
	GetWindowRect (g_hWnd, &myRect);
	
	if (bOntop == true) {
		SetWindowPos (g_hWnd, HWND_TOPMOST, myRect.left, myRect.top, myRect.right-myRect.left, myRect.bottom-myRect.top, SWP_NOMOVE);
	} else {
		SetWindowPos (g_hWnd, HWND_NOTOPMOST, myRect.left, myRect.top, myRect.right-myRect.left, myRect.bottom-myRect.top, SWP_NOMOVE);
	}
}

void UIWindow::MoveUIWindow (int xPos, int yPos)
{	
	SetWindowPos (g_hWnd, HWND_TOPMOST, xPos, yPos, 0, 0, SWP_NOSIZE);
	
	// Update the global positions.
	g_xPos = xPos;
	g_yPos = yPos;
}

void UIWindow::ResizeWindow (int iWidth, int iHeight)
{
	SetWindowPos (g_hWnd, HWND_TOPMOST, 0, 0, iWidth, iHeight, SWP_NOMOVE);
}

void UIWindow::GetScreenResolution ()
{
	// Get the resolution of the screen, and store in global variables.
	HDC hScrDC;
	hScrDC = CreateDC("DISPLAY", NULL, NULL, NULL);
	if (hScrDC == NULL) {
		g_iScreenResX = 1024;
		g_iScreenResY = 768;
	} else {
		g_iScreenResX = GetDeviceCaps(hScrDC, HORZRES);
		g_iScreenResY = GetDeviceCaps(hScrDC, VERTRES);
		
		//g_iScreenResY = GetSystemMetrics(SM_CYSCREEN);
		//g_iScreenResX = GetSystemMetrics(SM_CXSCREEN);
	}
	DeleteDC (hScrDC);
}

void UIWindow::SetWindowPosition (unsigned int iPosition) {
	
	int newxpos;
	int newypos;
	int TrayHeight = 0;
	int TrayWidth = 0;
	HWND TrayWnd;
	RECT TrayRect;

	if (iPosition == FS_CENTER) {
		newxpos = ((g_iScreenResX / 2) - (g_iWidth /2));
		newypos = ((g_iScreenResY /2) - (g_iHeight /2));		
	}

	if (iPosition == FS_BOTTOMRIGHT) {
		// If we are going to position the window at the bottom right of the users
		// desktop, we need to take into account where the task bar window is. We then have
		// to position our window so that it is nicely on top of the task bar. This routine
		// also takes into account wherever the taskbar maybe on the users desktop.
		TrayWnd = FindWindow("Shell_TrayWnd", "");
		if (TrayWnd != NULL) {
			if (GetWindowRect (TrayWnd, &TrayRect) != 0) {
																		
				if (TrayRect.top > (g_iScreenResY-g_iHeight)) {
					TrayHeight = TrayRect.bottom - TrayRect.top;
				}

				if (TrayRect.left > (g_iScreenResX-g_iWidth)) {
					TrayWidth = TrayRect.right - TrayRect.left;
				}
			}
		}

		newxpos = g_iScreenResX - g_iWidth - TrayWidth-4;
		newypos = g_iScreenResY - g_iHeight - TrayHeight-4;
	}

	// Set the new positions globally for reference.
	g_xPos = newxpos;
	g_yPos = newypos;

	if (SetWindowPos (g_hWnd, HWND_TOP, newxpos, newypos, g_iWidth, g_iHeight, SWP_HIDEWINDOW) == 0) {
		//MessageBox (NULL, "Could not move window!", "Error", MB_ICONEXCLAMATION | MB_OK);
	}
}

void UIWindow::SetWindowStyle (unsigned int iStyle)
{
	m_bStyleset = true;
	m_iSelectedstyle = iStyle;
}

void UIWindow::CreateAppWindow (LPCSTR lpszClassName, int xPos, int yPos, int Width, int Height, bool bChild)
{

	WNDCLASSEX wc;
	HICON hAppIcon = LoadIcon (GetModuleHandle (NULL), MAKEINTRESOURCE (IDI_ICON));
	HICON hAppIconSmall = LoadIcon (GetModuleHandle (NULL), MAKEINTRESOURCE (IDI_ICONSMALL));

	wc.cbSize = sizeof (WNDCLASSEX);
	wc.style = CS_DBLCLKS;
	wc.lpfnWndProc = BaseProc;
	wc.cbClsExtra = 0L;
	wc.cbWndExtra = 0L;
	wc.hInstance = GetModuleHandle (NULL);
	wc.hIcon = hAppIcon;
	wc.hCursor = LoadCursor (NULL, IDC_ARROW);
	if (g_bUsingSysColour == true) {
		wc.hbrBackground = (HBRUSH) (GetSysColorBrush (g_iChosenSysColour));
	} else {
		wc.hbrBackground = (HBRUSH) (CreateSolidBrush (g_bgcolour));
	}
	wc.lpszMenuName = NULL;
	wc.lpszClassName = lpszClassName;
	wc.hIconSm = hAppIconSmall;

	if (RegisterClassEx (&wc) == 0) {
		MessageBox (NULL, "Window class registration failed!", lpszClassName, MB_OK);
	} 
	
	// Create the window passing a pointer to this window object through the last parameter lpParams
	// This has to be done because the BaseProc must be static, and static member functions don't have access to non static
	// functions or variables therefore we explicitly pass this objects instances pointer to the static baseproc which then
	// can access non static functions through the supplied *this pointer.
	//g_hWnd = CreateWindowEx (NULL, lpszClassName, g_lpszCaption, WS_POPUPWINDOW, xPos, yPos, Width, Height, g_ParentHWND, NULL, GetModuleHandle (NULL), (void *) this);
	
	// Check if we're asked to create a child window. The main different being the style we use and the fact that we have to specify a
	// parent HWND for a Child window. If this is a main window, then the Parent is NULL.
	
	if (m_bStyleset == true) {
		if (m_iSelectedstyle == FS_STYLESTANDARD) {
			if (bChild == true) {
				g_hWnd = CreateWindowEx (WS_EX_WINDOWEDGE | WS_EX_ACCEPTFILES, lpszClassName, g_lpszCaption, WS_MINIMIZEBOX| WS_CAPTION | WS_POPUPWINDOW, xPos, yPos, Width, Height, NULL, NULL, GetModuleHandle (NULL), (void *) this);
			} else {
				g_hWnd = CreateWindowEx (WS_EX_WINDOWEDGE | WS_EX_ACCEPTFILES, lpszClassName, g_lpszCaption, WS_MINIMIZEBOX| WS_CAPTION | WS_POPUPWINDOW, xPos, yPos, Width, Height, NULL, NULL, GetModuleHandle (NULL), (void *) this);
			}
		}
		if (m_iSelectedstyle == FS_STYLEBLANK) {
			g_hWnd = CreateWindowEx (WS_EX_TOOLWINDOW, lpszClassName, g_lpszCaption, WS_POPUPWINDOW, xPos, yPos, Width, Height, NULL, NULL, GetModuleHandle (NULL), (void *) this);
		}
	} else {	
		if (bChild == true) {
			g_hWnd = CreateWindowEx (NULL, lpszClassName, g_lpszCaption, WS_POPUP, xPos, yPos, Width, Height, NULL, NULL, GetModuleHandle (NULL), (void *) this);
		} else {
			g_hWnd = CreateWindowEx (NULL, lpszClassName, g_lpszCaption, WS_POPUPWINDOW, xPos, yPos, Width, Height, NULL, NULL, GetModuleHandle (NULL), (void *) this);
		}
	}
	
	m_rctWndRect.left = xPos;
	m_rctWndRect.right = xPos + Width;
	m_rctWndRect.top = yPos;
	m_rctWndRect.bottom = yPos + Height;

	g_iWidth = Width;
	g_iHeight = Height;
	// Get the Screen Resolution
	GetScreenResolution ();

	if (g_hWnd == NULL) {
		//DWORD dwErr = GetLastError ();
		//ShowInt (dwErr);
		MessageBox (NULL, "Window creation failed!", lpszClassName, MB_OK);
		return;
	}
}

HWND UIWindow::CreateGroupBox (char *szCaption, int xPos, int yPos, int Width, int Height, unsigned int iControlID)
{
	HWND hwndcontrol;

	HFONT hfDefault = (HFONT) GetStockObject (DEFAULT_GUI_FONT);

	hwndcontrol = CreateWindow ("button", szCaption, WS_CHILD | WS_VISIBLE | BS_GROUPBOX, xPos, yPos, Width, Height, g_hWnd, (HMENU) iControlID, GetModuleHandle (NULL), NULL);
	SendMessage (hwndcontrol, WM_SETFONT, (WPARAM) hfDefault, MAKELPARAM (FALSE, 0));

	return hwndcontrol;
}

HWND UIWindow::CreateRadioButton (char *szCaption, int xPos, int yPos, int Width, int Height, unsigned int iControlID)
{
	HWND hwndcontrol;

	HFONT hfDefault = (HFONT) GetStockObject (DEFAULT_GUI_FONT);

	hwndcontrol = CreateWindow ("button", szCaption, WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON, xPos, yPos, Width, Height, g_hWnd, (HMENU) iControlID, GetModuleHandle (NULL), NULL);
	SendMessage (hwndcontrol, WM_SETFONT, (WPARAM) hfDefault, MAKELPARAM (FALSE, 0));

	return hwndcontrol;
}

HWND UIWindow::CreateCheckBox (char *szCaption, int xPos, int yPos, int Width, int Height, unsigned int iControlID)
{
	HWND hwndcontrol;

	HFONT hfDefault = (HFONT) GetStockObject (DEFAULT_GUI_FONT);

	hwndcontrol = CreateWindow ("button", szCaption, WS_CHILD | WS_VISIBLE | BS_CHECKBOX, xPos, yPos, Width, Height, g_hWnd, (HMENU) iControlID, GetModuleHandle (NULL), NULL);
	SendMessage (hwndcontrol, WM_SETFONT, (WPARAM) hfDefault, MAKELPARAM (FALSE, 0));

	return hwndcontrol;
}

HWND UIWindow::CreateButton (char *szCaption, int xPos, int yPos, int Width, int Height, unsigned int iControlID)
{
	HWND hwndcontrol;

	HFONT hfDefault = (HFONT) GetStockObject (DEFAULT_GUI_FONT);

	hwndcontrol = CreateWindow ("button", szCaption, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, xPos, yPos, Width, Height, g_hWnd, (HMENU) iControlID, GetModuleHandle (NULL), NULL);
	SendMessage (hwndcontrol, WM_SETFONT, (WPARAM) hfDefault, MAKELPARAM (FALSE, 0));
	
	return hwndcontrol;
}

HWND UIWindow::CreateLabel (char *szCaption, int xPos, int yPos, int Width, int Height, unsigned int iControlID)
{
	HWND hwndcontrol;

	HFONT hfDefault = (HFONT) GetStockObject (DEFAULT_GUI_FONT);

	hwndcontrol = CreateWindow ("static", szCaption, WS_CHILD | WS_VISIBLE, xPos, yPos, Width, Height, g_hWnd, (HMENU) iControlID, GetModuleHandle (NULL), NULL);
	SendMessage (hwndcontrol, WM_SETFONT, (WPARAM) hfDefault, MAKELPARAM (FALSE, 0));

	return hwndcontrol;
}

HWND UIWindow::CreateTextBox (int xPos, int yPos, int Width, int Height, unsigned int iControlID)
{
	HWND hwndcontrol;

	HFONT hfDefault = (HFONT) GetStockObject (DEFAULT_GUI_FONT);

	hwndcontrol = CreateWindow ("edit", "", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL, xPos, yPos, Width, Height, g_hWnd, (HMENU) iControlID, GetModuleHandle (NULL), NULL);
	SendMessage (hwndcontrol, WM_SETFONT, (WPARAM) hfDefault, MAKELPARAM (FALSE, 0));

	return hwndcontrol;
}

void UIWindow::SetCheck (HWND hwndcontrol, bool bchecked)
{
	if (bchecked == true) {
		SendMessage (hwndcontrol, BM_SETCHECK, 1, 0);
	} else {
		SendMessage (hwndcontrol, BM_SETCHECK, 0, 0);
	}
	
}
