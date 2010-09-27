#pragma once
#include <windows.h>
#include <io.h>
#include <stdio.h>
#include <direct.h>
#include <shlobj.h>
#include <commctrl.h>
#include "UIWindow.h"
#include "Diagnostics.h"
#include "UIHandler.h"
#include "UIBanner.h"
#include "MultiContent.h"
#include "UIRect.h"
#include "UIPicButton.h"

class ProtectedFoldersWindow : public UIWindow
{
	public:
		ProtectedFoldersWindow ();
		~ProtectedFoldersWindow ();
		void SetDiagnostics (Diagnostics *pdiag);
		
		void Initialise (HWND hWnd, unsigned int uID);
		void OutputInt (LPCSTR lpszText, int iValue);
		void OutputText (LPCSTR lpszText);
		void OutputText (LPCSTR lpszName, LPCSTR lpszValue);
		void SaveList ();
		void LoadList ();
		bool PrepareListPath ();
		bool FileExists (char *FileName);
		void GetListFilePath (char *szDestpath);

	private:
		// Private Member Variables & objects
		bool BrowseForFolder (LPCTSTR szTitle, char *szOutPath);

		// The UI Handler required for multiple handling of custom controls.
		UIHandler m_uihandler;
		
		// ID ofthis window - required for window class registration purposes
		unsigned int m_ID;

		// Global Window Handle
		HWND m_hwnd;
		HWND m_parenthwnd;

		HWND m_lstfolders;
		HWND m_btnaddfolder;
		HWND m_btnremfolder;
		HWND m_btnok;
		HWND m_lblheader;

		// The header bitmap image
		UIBanner m_header;

		// The MultiContent Control
		MultiContent m_mainconv;				
		
		// The path to the file containing the
		// protected folder list
		char m_szListfile[SIZE_STRING];

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
