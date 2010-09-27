#pragma once
#include <windows.h>
#include <io.h>
#include <commctrl.h>
#include "UIWindow.h"
#include "UIHandler.h"
#include "UIBanner.h"

class Diagnostics : public UIWindow
{
	public:
		Diagnostics ();
		~Diagnostics ();
		
		void Initialise (HWND hWnd, LPSTR lpCmdLine);
		void ParseCommandLine (char *pszCmdline);
		void OutputInt (LPCSTR lpszText, int iValue);
		void OutputText (LPCSTR lpszText);
		void OutputText (LPCSTR lpszName, LPCSTR lpszValue);

		HWND m_hwnddiaglist;

	private:
		// Private Member Variables & objects

		// The UI Handler required for multiple handling of custom controls.
		UIHandler m_uihandler;
		
		// The header bitmap image
		UIBanner m_header;

		// Window controls


		bool m_bUsingdiagnostics;

		// event notification from base class
		void OnCreate (HWND hWnd);		
		void OnCommand (HWND hWnd, WPARAM wParam, LPARAM lParam);
		void OnUICommand (HWND hWnd, WPARAM wParam, LPARAM lParam);		
		void OnPaint (HWND hWnd);
		void OnTimer (WPARAM wParam);
		void OnMouseMove (HWND hWnd, int mouseXPos, int mouseYPos);
		void OnLButtonDown (HWND hWnd);
		void OnLButtonUp (HWND hWnd);
};
