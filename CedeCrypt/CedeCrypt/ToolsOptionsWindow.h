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
#include "RegistryHandler.h"
#include "Constants.h"

class ToolsOptionsWindow : public UIWindow
{
	public:
		ToolsOptionsWindow ();
		~ToolsOptionsWindow ();
		void SetDiagnostics (Diagnostics *pdiag);
		
		void Initialise (HWND hWnd, unsigned int uID);
		void OutputInt (LPCSTR lpszText, int iValue);
		void OutputText (LPCSTR lpszText);
		void OutputText (LPCSTR lpszName, LPCSTR lpszValue);

		void CreateGeneralPanel ();
		void CreateAlgorithmsPanel ();
		
		void SetGeneralPanelVisible (bool bVisible);
		void SetAlgorithmsPanelVisible (bool bVisible);
		unsigned int GetSelectedAlgorithm ();

		void ActivatePanel (char *szPanel);

		void ReadRegistrySettings ();
		bool SaveHotKeySettings ();

		void SetAlgorithm (unsigned int iAlg);

	private:
		// Private Member Variables & objects

		// The UI Handler required for multiple handling of custom controls.
		UIHandler m_uihandler;
		
		// ID ofthis window - required for window class registration purposes
		unsigned int m_ID;

		// Global Window Handle
		HWND m_hwnd;
		HWND m_parenthwnd;

		HWND m_lstcategory;

		HWND m_pfoldersframe;
		HWND m_pfoldersinfo1;
		HWND m_pfoldersinfo2;
		HWND m_pfolderspromptpowerup;
		HWND m_pfolderspromptpowerdown;

		HWND m_ptextencframe;
		HWND m_btnclose;
		
		HWND m_txtenckey;
		HWND m_txtdeckey;
		
		HWND m_lblenc;
		HWND m_lbldec;
		HWND m_lblinfo;

		HWND m_chkencctrl;
		HWND m_chkencalt;
		HWND m_chkdecctrl;
		HWND m_chkdecalt;

		HWND m_autoupdateframe;
		HWND m_chkautoupdate;

		// Algorithm panel controls
		HWND m_algframe;
		UIBanner m_algimage;

		HWND m_algoptcybercede;
		HWND m_algoptaes256;
		HWND m_algopt3des;
		HWND m_algoptdes;

		HWND m_lblcurrentalg;


		// Options
		bool m_bpfolderspromptpowerup;
		bool m_bpfolderspromptpowerdown;
		bool m_buseencctrl;
		bool m_buseencalt;
		bool m_busedecctrl;
		bool m_busedecalt;
		bool m_buseautoupdate;

		// Registry Handler
		RegistryHandler m_registry;

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


		bool m_binitialised;

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
