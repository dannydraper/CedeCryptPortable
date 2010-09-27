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
#include "Constants.h"
#include "RegistryHandler.h"

class PFolderRequestWindow : public UIWindow
{
	public:
		PFolderRequestWindow ();
		~PFolderRequestWindow ();
		void SetDiagnostics (Diagnostics *pdiag);
		
		void Initialise (HWND hWnd, unsigned int uID);
		void OutputInt (LPCSTR lpszText, int iValue);
		void OutputText (LPCSTR lpszText);
		void OutputText (LPCSTR lpszName, LPCSTR lpszValue);
		void SetStartupMessage (bool m_bUsestartupmessage);
		void ReadRegistrySettings ();
		void SetStartupMode (bool bstartupmode);
		bool GetStartMode ();
		void SetAnswered (bool banswered);
		bool GetAnswered ();

	private:
		// Private Member Variables & objects

		// The UI Handler required for multiple handling of custom controls.
		UIHandler m_uihandler;
		
		// ID ofthis window - required for window class registration purposes
		unsigned int m_ID;

		// Global Window Handle
		HWND m_hwnd;
		HWND m_parenthwnd;
		HWND m_optiongroup;
		HWND m_optdoaction;
		HWND m_optdonothing;
		

		HWND m_btnrequestok;
		
		// Flag indicating if we're using diagnostics
		bool m_bUseDiagnostics;
		Diagnostics *m_pdiag;
		bool m_bdoaction;
		bool m_bstartupmode;
		bool m_banswered;

		// The registry handler
		RegistryHandler m_registry;

		// Registered class name
		// We need a different class name for every instance of
		// this window. This class name
		// Is created by the Initialise routine
		// with a uID value suffixed to the end
		char m_szClassname[SIZE_STRING];

		// event notification from base class
		void OnDestroy (HWND hWnd);
		void OnCreate (HWND hWnd);		
		void OnClose (HWND hWnd);
		void OnCommand (HWND hWnd, WPARAM wParam, LPARAM lParam);
		void OnUICommand (HWND hWnd, WPARAM wParam, LPARAM lParam);		
		void OnUIScroll (HWND hWnd, WPARAM wParam, LPARAM lParam);
		void OnPaint (HWND hWnd);
		void OnTimer (WPARAM wParam);
		void OnMouseMove (HWND hWnd, int mouseXPos, int mouseYPos);
		void OnLButtonDown (HWND hWnd);
		void OnLButtonUp (HWND hWnd);
};
