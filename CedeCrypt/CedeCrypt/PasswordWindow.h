#pragma once
#include <windows.h>
#include <io.h>
#include <commctrl.h>
#include "UIWindow.h"
#include "Diagnostics.h"
#include "UIHandler.h"
#include "UIBanner.h"
#include "MultiContent.h"
#include "UIRect.h"
#include "UIPicButton.h"


class PasswordWindow : public UIWindow
{
	public:
		PasswordWindow ();
		~PasswordWindow ();
		void SetDiagnostics (Diagnostics *pdiag);
		
		void Initialise (HWND hWnd, unsigned int uID);
		void OutputInt (LPCSTR lpszText, int iValue);
		void OutputText (LPCSTR lpszText);
		void OutputText (LPCSTR lpszName, LPCSTR lpszValue);
		char *GetLastPassword ();
		void ParseUserInput ();
		void ParseUserInput2 ();
		static long FAR PASCAL SubProc(HWND hWnd, WORD wMessage, WORD wParam, LONG lParam);
		static long FAR PASCAL SubProc2(HWND hWnd, WORD wMessage, WORD wParam, LONG lParam);
		void CheckPassword ();
		void SetSessionMode (bool bSessionmode, bool bEncrypt);
		bool GetSessionMode ();
		void SetEncryptMode (bool bEncrypt);

		void ClearPassword ();
		void SetSecondFocus ();

		bool m_bEncryptmode; // Need to make this public so it can be accessed by the static functions through the class pointer
	private:
		// Private Member Variables & objects		
		static FARPROC lpfnOldWndProc;
		static FARPROC lpfnOldWndProc2;
		static PasswordWindow *ppwnd;

		// The UI Handler required for multiple handling of custom controls.
		UIHandler m_uihandler;
		
		// ID ofthis window - required for window class registration purposes
		unsigned int m_ID;

		// Global Window Handle
		HWND m_hwnd;
		HWND m_parenthwnd;

		bool m_bSessionmode;
		
		bool m_bInitialised;

		// Last entered password
		char m_szPassword[SIZE_NAME];

		// Control handles
		HWND m_hwndeditpass;
		HWND m_hwndeditpass2;
		HWND m_lblpass2info;
		HWND m_hwndbtnpassok;
		HWND m_hwndbtnpasscancel;
		HWND m_hwndsessionlbl;

		// The header bitmap image
		UIBanner m_header;

		// A rect
		UIRect m_inforect;
		UILabel m_info1;
		UILabel m_info2;

		// The MultiContent Control
		MultiContent m_mainconv;				
		
		// Flag indicating if we're using diagnostics
		bool m_bUseDiagnostics;
		Diagnostics *m_pdiag;

		// Registered class name
		// We need a different class name for every instance of
		// this window. This class name
		// Is created by the Initialise routine
		// with a uID value suffixed to the end
		char m_szClassname[SIZE_STRING];

		// event notification from base class
		void OnDestroy (HWND hWnd);
		void OnCreate (HWND hWnd);		
		void OnCommand (HWND hWnd, WPARAM wParam, LPARAM lParam);
		void OnUICommand (HWND hWnd, WPARAM wParam, LPARAM lParam);		
		void OnUIScroll (HWND hWnd, WPARAM wParam, LPARAM lParam);
		void OnPaint (HWND hWnd);
		void OnTimer (WPARAM wParam);
		void OnMouseMove (HWND hWnd, int mouseXPos, int mouseYPos);
		void OnLButtonDown (HWND hWnd);
		void OnLButtonUp (HWND hWnd);
};
