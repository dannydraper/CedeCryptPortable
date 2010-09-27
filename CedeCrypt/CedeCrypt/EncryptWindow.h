#pragma once
#include <windows.h>
#include <io.h>
#include <commctrl.h>
#include <shlobj.h>
#include "UIWindow.h"
#include "Diagnostics.h"
#include "UIHandler.h"
#include "UIBanner.h"
#include "MultiContent.h"
#include "UIRect.h"
#include "UIPicButton.h"
#include "StandardEncryption.h"
#include "SingleFileInfo.h"


class EncryptWindow : public UIWindow
{
	public:
		EncryptWindow ();
		~EncryptWindow ();


		void SetDiagnostics (Diagnostics *pdiag);

		void DoSomething ();
		void DoEncryption (char *szVaultpath);
		
		void AddFileInfo (char *szFilepath, char *szRootdir, bool IsDir);
		void AddFileInfo (char *szFilepath, bool IsDir);
		
		void DoBlankSetup ();

		void ClearFileInfo ();
		void GetPathOnly (char *szInpath, char *szOutpath, char *szOutfile, char *szSep);
		void StripStartPath (char *szStartpath, char *szFullpath, char *szOutpath);
		void DoDecryption ();

		void Initialise (HWND hWnd, unsigned int uID);
		void OutputInt (LPCSTR lpszText, int iValue);
		void OutputText (LPCSTR lpszText);
		void OutputText (LPCSTR lpszName, LPCSTR lpszValue);

		void SetPassword (char *szPassword);

		int m_idecrypterrorcount;

	private:
		// Private Member Variables & objects

		// The UI Handler required for multiple handling of custom controls.
		UIHandler m_uihandler;
		
		// ID ofthis window - required for window class registration purposes
		unsigned int m_ID;

		// Global Window Handle
		HWND m_hwnd;
		HWND m_parenthwnd;
		HWND m_hwndprogress;
		
		// DynList storing all of the File Infos
		// which have been added to encrypt
		DynList m_dlFileinfolist;

		// Flag indicating if we're using diagnostics
		bool m_bUseDiagnostics;
		Diagnostics *m_pdiag;
		bool m_binitialised;

		StandardEncryption m_enc;

		char m_szPassword[SIZE_STRING];
		

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
