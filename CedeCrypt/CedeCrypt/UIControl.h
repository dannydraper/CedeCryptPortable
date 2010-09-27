// Why the heck are we using #pragma once I hear you ask.... Well it's because this UIControl
// class will be derived more than once by child control classes. If we don't include this
// #pragma once then the compiler complains about multiple declarations of this class. We're telling
// the compiler to only parse this class file once. 
#pragma once
#include <windows.h>
#include <math.h>
#include "resource.h"
#include "MemoryBuffer.h"

#define EZ_ATTR_BOLD 1
#define EZ_ATTR_ITALIC 2
#define EZ_ATTR_UNDERLINE 4
#define EZ_ATTR_STRIKEOUT 8
#define MAX_CONTROLS 512

class UIControl
{
	public:
		UIControl ();
		~UIControl ();

		int g_Value;

		void SetValue (int iValue);	
		int GetValue ();
		void SetProperties (HWND hWnd, int cID, int iXPos, int iYPos, int iWidth, int iHeight);
		void SetProperties (HWND hWnd, int cID, int iXPos, int iYPos, int iWidth);
		void SetPosition (int iXPos, int iYPos);
		void SetHighlightProperties (bool bUseHighlight, bool bUseFade);
		void SetTextProperties (TCHAR *szFontName, int FontSize, int FontStyle, int xOffset, int yOffset, char *szCaption, COLORREF color);
		void SetTextProperties (TCHAR *szFontName, int FontSize, int FontStyle, int xOffset, int yOffset, char *szCaption, unsigned long lStart, unsigned long lLength, COLORREF color);
		void SetTextForeColor (COLORREF clColor);
		void SetTextCaption (char *szCaption);
		void InvalidateControl (bool bIncludeText);
		void SetClippingProperties (RECT rctClip);
		void SetYPositionOffset (int iOffset);
		void ResetYOffsetPosition ();
		void GetControlRect (LPRECT lpRect);
		void GetTextRect (LPRECT lpRect);
		bool IsWithinRect (RECT rctRect, bool bIncludeText);
		bool IsWithinRect (int iLeft, int iRight, int iTop, int iBottom, bool bIncludeText);
		bool IsMouseRect (int iMouseXPos, int iMouseYPos);
		void SetHWND (HWND hWnd);

		// Virtual Methods - We expect these to be overridden by any derived classes
		// as any derived control is expected to provide their own methods for handling these methods.
		virtual void Paint (HDC hdc) {};

		virtual void NotifyMouseMove (int iXPos, int iYPos) {};
		virtual void NotifyMouseDown () {};
		virtual void NotifyMouseUp () {};
		virtual void NotifyTimer (unsigned int wParam) {};
		virtual unsigned long GetSize () {return sizeof (UIControl);}

	protected:
		// Essential bitmap base properties
		HBITMAP g_hbmBase;
		HBITMAP g_hbmHighlight;
		HBITMAP g_hbmDown;
		int g_iBitmapwidth;
		int g_iBitmapheight;

		// Parent window handle
		HWND g_hWnd;

		// Position of the mouse cursor
		int g_mouseXPos;
		int g_mouseYPos;
		bool g_mouseDown;

		// General class properties
		int g_controlID;
		int g_xPos;
		int g_yPos;
		int g_Width;
		int g_Height;
		RECT g_Rect;
		bool g_bUsingtext;

		// Control text properties
		char g_szFontName[SIZE_NAME];
		int g_FontSize;
		int g_FontStyle;
		int g_xOffset;
		int g_yOffset;
		char *g_szCaption;
		MemoryBuffer m_memCaption;
		COLORREF g_textcolor;

		// Highlighting properties
		bool g_bUsehighlight;
		bool g_bUsefade;

		// PROTECTED METHODS
		bool IsMouseRect ();
		bool IsMouseRect (RECT rect);
		bool IsMouseRect (bool bIncludeText);		
		HBITMAP LoadBitmapResource (WORD wResource);	
		void PaintText (HDC hdc);
		void PaintText (HDC hdc, bool bCalcSize, bool bDrawText);
		void PaintText (HDC hdc, int xPos, int yPos, char *szFontName, int FontSize, char *szCaption, COLORREF textcolor);

	private:
		int m_initialYPos;
		bool m_bInitialYPosSet;		

		bool m_bClippingUsed;
		RECT m_rctClipping;
		RECT m_rctTextRect;
		bool m_bTextSizeKnown;
		bool m_bUsingTextbounds;
		unsigned long m_lTextStart;
		unsigned long m_lTextLength;
		HFONT EzCreateFont (HDC hdc, TCHAR * szFaceName, int iDeciPtHeight,int iDeciPtWidth, int iAttributes, BOOL fLogRes);
		void SetClippingRect ();
		void CalcTextRect ();
};