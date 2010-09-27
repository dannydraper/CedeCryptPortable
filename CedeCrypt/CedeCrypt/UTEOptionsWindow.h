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

#define IDT_STATUSFLASH		9700

class UTEOptionsWindow : public UIWindow
{
	public:
		UTEOptionsWindow ();
		~UTEOptionsWindow ();
		void SetDiagnostics (Diagnostics *pdiag);
		
		void Initialise (HWND hWnd, unsigned int uID);
		void OutputInt (LPCSTR lpszText, int iValue);
		void OutputText (LPCSTR lpszText);
		void OutputText (LPCSTR lpszName, LPCSTR lpszValue);

	private:
		// Private Member Variables & objects
		void SetHotKeyControlsEnabled (bool m_benabled);
		void SetFlashingStatus (bool m_benabled);

		bool m_buteenabled;

		bool m_bctrlencenabled;
		bool m_baltencenabled;

		bool m_bctrldecenabled;		
		bool m_baltdecenabled;

		bool m_bstatusshown;

		bool m_benckeylisten;
		bool m_bdeckeylisten;

		// The UI Handler required for multiple handling of custom controls.
		UIHandler m_uihandler;
		
		// ID ofthis window - required for window class registration purposes
		unsigned int m_ID;

		// Global Window Handle
		HWND m_hwnd;
		HWND m_parenthwnd;
		bool m_bwindowinitialised;

		HWND m_btnok;
		HWND m_btncancel;
		HWND m_chkenable;
		HWND m_lblhotenc;
		HWND m_lblhotdec;
		HWND m_chkhotencctrl;
		HWND m_chkhotdecctrl;
		HWND m_chkhotencalt;
		HWND m_chkhotdecalt;
		HWND m_txthotenc;
		HWND m_txthotdec;
		HWND m_btnhotenclisten;
		HWND m_btnhotdeclisten;
		HWND m_lblhotkeystatus;

		// The header bitmap image
		UIBanner m_header;			
		
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
		void OnKeyUp (HWND hWnd, WPARAM wParam, LPARAM lParam);
		void OnPaint (HWND hWnd);
		void OnTimer (WPARAM wParam);
		void OnMouseMove (HWND hWnd, int mouseXPos, int mouseYPos);
		void OnLButtonDown (HWND hWnd);
		void OnLButtonUp (HWND hWnd);
};
