#pragma once
#include "UIControl.h"
#include "UIVScrollbar.h"
#include "UIHandler.h"
#include "UILabel.h"
#include "DynList.h"
#include "UIPicButton.h"
#include "UIHyperLink.h"

class MultiContent : public UIControl
{
	public:
		MultiContent ();
		~MultiContent ();
		void Paint (HDC hdc);

		void SetDiagnostics (Diagnostics *pdiag);
		void OutputInt (LPCSTR lpszText, int iValue);
		void OutputText (LPCSTR lpszText);
		void OutputText (LPCSTR lpszName, LPCSTR lpszValue);

		void NotifyMouseMove (int iXPos, int iYPos);
		void NotifyMouseDown ();
		void NotifyMouseUp ();
		void NotifyTimer (unsigned int wParam);
		void NotifyUIScroll (HWND hWnd, WPARAM wParam, LPARAM lParam);
		void SetMCProperties (HWND hWnd, int cID, int xPos, int yPos, int Width, int Height);
		void SetVisible (bool bVisible);
		unsigned long GetSize ();
		void AddString (char *szString);
		void AddPicture (int iWidth, int iHeight, bool bCenter, bool Inline, unsigned int iResource, bool issmiley, int cID);
		void AddHyperlink (char *szCaption, char *szTarget);
		void Clear ();
		void SetItalic (bool bItalic);
		void SetBold (bool bBold);
		void SetUnderline (bool bUnderline);
		void SetStrikeout (bool bStrikeout);
		void SetFontSize (int iSize);
		void SetFontFace (char *szFontname);
		void SetTextForeColor (COLORREF clColor);
		void SetIndent (int iVal);
		void AddNotificationString (char *szString);
		void SetEndScrollPos ();
		void SetScrollbar (bool bUsescroll);		
		void AddSmallBreak ();		
		void SetNotificationsEnabled (bool bStatus);
	private:
		// Private methods
		int FindString (char *szStrtolook, char *szStrtofind);
		void AddWords (char *szString);
		void AddLabel (HWND hWnd, int iXPos, int iYPos, char *szCaption, LPRECT lpRect, unsigned long lStart, unsigned long lLength);
		void AddWord (HWND hWnd, char *szCaption, unsigned long lStart, unsigned long lLength);
		void AddPictureControl (HWND hWnd, int iXPos, int iYPos, int iWidth, int iHeight, unsigned int iResource);
		void AddSmileyControl (HWND hWnd, int iXPos, int iYPos, int iWidth, int iHeight, unsigned int iResource, int cID);
		SIZE GetTextSize (HWND hWnd, char *szCaption, unsigned long lStart, unsigned long lLength);
		SIZE GetHyperlinkSize (HWND hWnd, char *szCaption);
		void AddHyperLinkControl (HWND hWnd, int iXPos, int iYPos, char *szCaption, char *szTarget, LPRECT lpRect);
		int GetFontStyle ();

		bool m_bNotificationsEnabled;

		// The scroll bar control
		UIVScrollbar m_vscroll;

		// Global window handler
		HWND m_hwnd;

		// This controls UI Handler
		UIHandler m_uih;

		// A background image
		UIBanner m_background;
		bool m_bUsebackground;

		// Flag indicating whether the scroll bar
		// is in use
		bool m_bUsescroll;

		// Diagnostics
		bool m_bUseDiagnostics;
		Diagnostics *m_pdiag;

		// Current font in use
		char m_szCurrentFont[SIZE_STRING];

		// Private tracking variables
		int m_iCursorXPos;
		int m_iCursorYPos;
		int m_iWindowWidth;
		int m_iWindowHeight;
		int m_iLastTextHeight;
		int m_iLineBreakHeight;
		int m_iIndent;

		bool m_bBold;
		bool m_bItalic;
		bool m_bUnderline;
		bool m_bStrikeout;
		int m_iFontSize;
		COLORREF m_clTextColor;

		bool m_bVisible;
		unsigned int g_mouseXPos;
		unsigned int g_mouseYPos;
};