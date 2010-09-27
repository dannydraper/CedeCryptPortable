#pragma once

#ifndef _WIN32_WINDOWS        // Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0501 // Change this to the appropriate value to target Windows Me or later.
#endif


#include <windows.h>
#include <stdio.h>
#include "resource.h"

class UIWindow
{
	private:
		static LRESULT WINAPI BaseProc (HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		int g_Value;
		HWND g_ParentHWND;
		
		LPCSTR g_lpszCaption;
		COLORREF g_bgcolour;
		bool m_bStyleset;
		unsigned int m_iSelectedstyle;
		
		bool g_bUsingSysColour;
		int g_iChosenSysColour;

		void GetScreenResolution ();
	public:
		UIWindow ();
		~UIWindow ();
		
		// Delayed Painting Variables
		bool g_bWindowActive;
		int g_iDelayedPaintCount;

		void CreateAppWindow (LPCSTR lpszClassName, int xPos, int yPos, int Width, int Height, bool bChild);
		void SetWindowStyle (unsigned int iStyle);
		void SetParentHWND (HWND hWnd);
		void SetCaption (LPCSTR lpszCaption);
		void SetCaption (LPCSTR lpszCaption, bool refresh);
		void SetValue (int iValue);
		void SetBgColor (COLORREF bgcolour);
		void SetProperties (int xPos, int yPos, int Width, int Height);
		void SetAlwaysOnTop (bool bOntop);
		bool IsWindowFocused ();
		void FlashWindow ();
		void MoveUIWindow (int xPos, int yPos);
		void Show ();
		void ShowEx (int nCmdShow);
		void Restore ();
		void Minimise ();
		void Hide ();
		void SetBgSysColor (int iSysColor);
		void ResizeWindow (int iWidth, int iHeight);
		HWND GetBaseHWND ();

	protected:
		int g_iScreenResX;
		int g_iScreenResY;
		int g_iWidth;
		int g_iHeight;
		int g_xPos;
		int g_yPos;
		HWND g_hWnd;
		RECT m_rctWndRect;
		// Window events. These are declared virtual meaning that they are expected
		// to be overridden by derived classes. If any event method is not overridden by 
		// a derived class then a default implementation runs instead.
		void ShowInt (int iInttoShow);
		
		// Wrapper functions for creating standard windows controls
		HWND CreateGroupBox (char *szCaption, int xPos, int yPos, int Width, int Height, unsigned int iControlID);
		HWND CreateRadioButton (char *szCaption, int xPos, int yPos, int Width, int Height, unsigned int iControlID);
		HWND CreateCheckBox (char *szCaption, int xPos, int yPos, int Width, int Height, unsigned int iControlID);
		HWND CreateButton (char *szCaption, int xPos, int yPos, int Width, int Height, unsigned int iControlID);
		HWND CreateLabel (char *szCaption, int xPos, int yPos, int Width, int Height, unsigned int iControlID);
		HWND CreateTextBox (int xPos, int yPos, int Width, int Height, unsigned int iControlID);

		// Wrapper functions for manipulating standard windows controls
		void SetCheck (HWND hwndcontrol, bool bchecked);

		void SetWindowPosition (unsigned int iPosition);
		virtual void OnCreate (HWND hWnd) {};
		virtual void OnClose (HWND hWnd) {};
		virtual void OnUICommand (HWND hWnd, WPARAM wParam) {};
		virtual void OnUICommand (HWND hWnd, WPARAM wParam, LPARAM lParam) {};		
		virtual void OnCommand (HWND hWnd, WPARAM wParam, LPARAM lParam) {};
		virtual void OnNotify (HWND hWnd, WPARAM wParam, LPARAM lParam) {};
		virtual void OnPointSelect (HWND hWnd, WPARAM wParam, LPARAM lParam) {};
		virtual void OnUIHighlight (HWND hWnd, WPARAM wParam) {};
		virtual void OnUINoHighlight (HWND hWnd, WPARAM wParam) {};
		virtual void OnUIScroll (HWND hWnd, WPARAM wParam, LPARAM lParam) {};
		virtual void OnPaint (HWND hWnd) {};
		virtual void OnMouseMove (HWND hWnd, int mouseXPos, int mouseYPos) {};
		virtual void OnLButtonDown (HWND hWnd) {};
		virtual void OnLButtonDblClick (HWND hWnd, WPARAM wParam, LPARAM lParam) {};
		virtual void OnLButtonUp (HWND hWnd) {};
		virtual void OnTimer (WPARAM wParam) {};
		virtual void OnHotKey (HWND hWnd, WPARAM wParam, LPARAM lParam) {};
		virtual void OnKeyUp (HWND hWnd, WPARAM wParam, LPARAM lParam) {};
		virtual void OnMouseWheel (HWND hWnd, WPARAM wParam, LPARAM lParam) {};
		virtual void OnNetworkEvent (HWND hWnd, WPARAM wParam, LPARAM lParam) {};
		virtual void OnMessagingEvent (HWND hWnd, WPARAM wParam, LPARAM lParam) {};
		virtual void OnConversationEvent (HWND hWnd, WPARAM wParam, LPARAM lParam) {};
		virtual void OnNewCommsMessage (HWND hWnd, WPARAM wParam, LPARAM lParam) {};
		virtual void OnCryptEvent (HWND hWnd, WPARAM wParam, LPARAM lParam) {};
		virtual void OnDropFiles (HWND hWnd, WPARAM wParam, LPARAM lParam) {};
		virtual LRESULT OnQueryEndSession (HWND hWnd, LPARAM lParam) {return TRUE;};
		virtual LRESULT OnEndSession (HWND hWnd, WPARAM wParam, LPARAM lParam) {return 0;};
		virtual LRESULT	OnPowerBroadcast (HWND hWnd, WPARAM wParam, LPARAM lParam) {return TRUE;};

		virtual void OnDestroy (HWND hWnd)
		{
			// Default implementation for a Destroy event.
			PostQuitMessage (0);
		};
};