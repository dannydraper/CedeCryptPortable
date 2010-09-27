#include "UIControl.h"

UIControl::UIControl ()
{
	// Initialise important variables
	g_mouseDown = false;
	g_bUsehighlight = false;
	g_bUsefade = false;
	g_bUsingtext = false;
	m_bClippingUsed = false;
	m_bInitialYPosSet = false;
	m_bTextSizeKnown = false;
	m_bUsingTextbounds = false;
	//ZeroMemory (g_szCaption, 100);
	ZeroMemory (g_szFontName, SIZE_NAME);
}

UIControl::~UIControl ()
{
}

HBITMAP UIControl::LoadBitmapResource (WORD wResource)
{
	// Load a single bitmap resource and return a bitmap handle to it.
	HBITMAP hbmTemp;
	hbmTemp = LoadBitmap (GetModuleHandle (NULL), MAKEINTRESOURCE (wResource));
	if (hbmTemp == NULL) {
		MessageBox (NULL, "Could not load bitmap resource!", "Error", MB_OK);
	}
	return hbmTemp;
}

// SetProperties default implementation. The width and height is explicitly specified
void UIControl::SetProperties (HWND hWnd, int cID, int iXPos, int iYPos, int iWidth, int iHeight)
{
	g_xPos = iXPos;
	g_yPos = iYPos;
	g_Width = iWidth;
	g_Height = iHeight;
	g_hWnd = hWnd;
	g_Rect.left = iXPos;
	g_Rect.right = iXPos+iWidth;
	g_Rect.top = iYPos;
	g_Rect.bottom = iYPos+iHeight;
	g_controlID = cID;

	SetClippingRect (); // Adjust the rect for this control if any clipping has been applied
	m_bTextSizeKnown = false;
}

// SetProperties first overload. The height is assumed to be the height of the base bitmap.
void UIControl::SetProperties (HWND hWnd, int cID, int iXPos, int iYPos, int iWidth)
{
	g_xPos = iXPos;
	g_yPos = iYPos;
	g_Width = iWidth;
	g_Height = g_iBitmapheight;
	g_hWnd = hWnd;
	g_Rect.left = iXPos;
	g_Rect.right = iXPos+iWidth;
	g_Rect.top = iYPos;
	g_Rect.bottom = iYPos+g_iBitmapheight;
	g_controlID = cID;

	SetClippingRect (); // Adjust the rect for this control if any clipping has been applied
	m_bTextSizeKnown = false;
}

// Set position function.
void UIControl::SetPosition (int iXPos, int iYPos)
{
	SIZE tSize;

	if (m_bTextSizeKnown == true) {
		// if the text size was previously known then work out what the width and height of the text is
		// before we readjust the position of the control.
		tSize.cx = m_rctTextRect.right - m_rctTextRect.left;
		tSize.cy = m_rctTextRect.bottom - m_rctTextRect.top;
	}

	g_xPos = iXPos;
	g_yPos = iYPos;
	g_Rect.left = iXPos;
	g_Rect.right = iXPos+g_Width;
	g_Rect.top = iYPos;
	g_Rect.bottom = iYPos+g_Height;
	
	if (m_bTextSizeKnown == true) {
		// If the text size was previously known the readjust the text rect to the new
		// position and size
		m_rctTextRect.left = g_xPos;
		m_rctTextRect.top = g_yPos;
		m_rctTextRect.right = g_xPos + tSize.cx;
		m_rctTextRect.bottom = g_yPos + tSize.cy;
	}

	SetClippingRect (); // Adjust the rect for this control if any clipping has been applied
}

void UIControl::SetYPositionOffset (int iOffset)
{
	// This little function allows you to move the control the vertical axis
	// by as much specified in the iOffset argument.
	if (m_bInitialYPosSet == false) {
		// We need to remember where the control was at last before we apply an offset
		// because this function is expected to be called from something like a scroll bar
		// whereby this function will be repeatedly called every time the offset changes.
		m_bInitialYPosSet = true;
		m_initialYPos = g_yPos;
	}
	
	SetPosition (g_xPos, m_initialYPos-iOffset);
}

void UIControl::ResetYOffsetPosition ()
{
	// Resets the initial Y position for the offset function.
	// This means that when the Offset is changed it is moved relative to the current yposition
	// and not where it was at before.
	m_bInitialYPosSet = false;
}

void UIControl::SetClippingProperties (RECT rctClip)
{
	m_bClippingUsed = true;
	m_rctClipping = rctClip;
	SetClippingRect (); // Adjust the rect for this control if any clipping has been applied
}

void UIControl::SetClippingRect ()
{
	// Adjust the rect if this control has had any clipping applied to it.
	if (m_bClippingUsed == true) {
		g_Rect.left = g_Rect.left + m_rctClipping.left;
		g_Rect.right = g_Rect.right + m_rctClipping.left;
		g_Rect.top = g_Rect.top + m_rctClipping.top;
		g_Rect.bottom = g_Rect.bottom + m_rctClipping.top;

		if (m_bTextSizeKnown == true) {
			m_rctTextRect.top = m_rctTextRect.top + m_rctClipping.top;
			m_rctTextRect.left = m_rctTextRect.left + m_rctClipping.left;
			m_rctTextRect.bottom = m_rctTextRect.bottom + m_rctClipping.top;
			m_rctTextRect.right = m_rctTextRect.right + m_rctClipping.left;
		}

		// Check if this control rect goes outside the boundaries of the clipping rect.
		// if it does, then we need to limit this controls rect to the boundaries of the clipping rect.
		if (m_rctClipping.left > g_Rect.left) {g_Rect.left = m_rctClipping.left;}
		if (m_rctClipping.top > g_Rect.top) {g_Rect.top = m_rctClipping.top;}
		if (m_rctClipping.right < g_Rect.right) {g_Rect.right = m_rctClipping.right;}
		if (m_rctClipping.bottom < g_Rect.bottom) {g_Rect.bottom = m_rctClipping.bottom;}
	}
}

void UIControl::SetValue (int iValue)
{
	g_Value = iValue;
}

int UIControl::GetValue ()
{
	return g_Value;
}

void UIControl::SetHWND (HWND hWnd) 
{
	// Late setting of window handle. We need to do this for menu items
	// as the menu control itself will most likely have a valid handle
	// only after menu items themselves have been created.
	g_hWnd = hWnd;
}

void UIControl::CalcTextRect ()
{
	// Private routine to calculate the size of a text rectangle.
	HDC hdc = GetDC (g_hWnd);
	HFONT hfCurFont;
	hfCurFont = EzCreateFont (hdc, g_szFontName, g_FontSize, 0, g_FontStyle, TRUE);
	SIZE tSize;

	SelectObject (hdc, hfCurFont);
	
	// Calculate the size of the specified string of text.
	if (m_bUsingTextbounds == true) {
		GetTextExtentPoint32 (hdc, (char *) g_szCaption+m_lTextStart, m_lTextLength, &tSize);
	} else {
		GetTextExtentPoint32 (hdc, (char *) g_szCaption, strlen ((char *) g_szCaption), &tSize);
	}

	m_rctTextRect.left = g_xPos;
	m_rctTextRect.top = g_yPos;
	m_rctTextRect.right = g_xPos + tSize.cx;
	m_rctTextRect.bottom = g_yPos + tSize.cy;

	// Set the member flag so that we know that we have a valid text size.
	m_bTextSizeKnown = true;

	DeleteObject (hfCurFont);
	DeleteDC (hdc);
}

void UIControl::GetControlRect (LPRECT lpRect)
{
	RECT ctlRect;
	ctlRect.top = g_yPos;
	ctlRect.left = g_xPos;
	ctlRect.bottom = g_yPos+g_Height;
	ctlRect.right = g_xPos+g_Width;
	
	*lpRect = ctlRect;
}

bool UIControl::IsWithinRect (RECT rctRect, bool bIncludeText)
{
	RECT rctTemp;
	RECT ctlRect;
	RECT rctUnion;
	
	// Get the rect for this control not including the text
	GetControlRect (&ctlRect);

	if (bIncludeText == true) {
		if (m_bTextSizeKnown == true) {
			UnionRect (&rctUnion, &m_rctTextRect, &ctlRect);
			if (IntersectRect (&rctTemp, &rctRect, &rctUnion) == TRUE) {
				return true;
			} else {
				return false;
			}
		} else {
			if (IntersectRect (&rctTemp, &rctRect, &ctlRect) == TRUE) {
				return true;
			} else {
				return false;
			}
		}
	} else {
		if (IntersectRect (&rctTemp, &rctRect, &ctlRect) == TRUE) {
			return true;
		} else {
			return false;
		}
	}
	return false;
}

bool UIControl::IsWithinRect (int iLeft, int iRight, int iTop, int iBottom, bool bIncludeText)
{
	RECT rctRect;
	rctRect.left = iLeft;
	rctRect.right = iRight;
	rctRect.top = iTop;
	rctRect.bottom = iBottom;
	return IsWithinRect (rctRect, bIncludeText);
}

void UIControl::SetTextProperties (TCHAR *szFontName, int FontSize, int FontStyle, int xOffset, int yOffset, char *szCaption, COLORREF color)
{
	ZeroMemory (g_szFontName, SIZE_NAME);
	strcpy_s (g_szFontName, SIZE_NAME, szFontName);
	g_FontSize = FontSize;
	g_FontStyle = FontStyle;
	g_xOffset = xOffset;
	g_yOffset = yOffset;
	//g_szCaption = (int *) szCaption;
	m_memCaption.SetSize (strlen (szCaption)+2);
	m_memCaption.Write (szCaption, 0, strlen (szCaption));
	g_szCaption = (char *) m_memCaption.GetBuffer ();
	//strcpy (g_szCaption, szCaption);
	g_textcolor = color;
	g_bUsingtext = true;
}

void UIControl::SetTextProperties (TCHAR *szFontName, int FontSize, int FontStyle, int xOffset, int yOffset, char *szCaption, unsigned long lStart, unsigned long lLength, COLORREF color)
{
	
	ZeroMemory (g_szFontName, SIZE_NAME);
	strcpy_s (g_szFontName, SIZE_NAME, szFontName);
	g_FontSize = FontSize;
	g_FontStyle = FontStyle;
	g_xOffset = xOffset;
	g_yOffset = yOffset;
	m_memCaption.SetSize (strlen (szCaption)+2);
	m_memCaption.Write (szCaption, 0, strlen (szCaption));
	g_szCaption = (char *) m_memCaption.GetBuffer ();
	//g_szCaption = (int *) szCaption;
	//strcpy (g_szCaption, szCaption);
	g_textcolor = color;
	m_lTextStart = lStart;
	m_lTextLength = lLength;
	m_bUsingTextbounds = true;
	g_bUsingtext = true;
}

void UIControl::SetTextForeColor (COLORREF clColor)
{
	// Change the color of the text;
	g_textcolor = clColor;
}

void UIControl::SetTextCaption (char *szCaption)
{
	//g_szCaption = (int *) szCaption;
	//strcpy (g_szCaption, szCaption);
	m_memCaption.SetSize (strlen (szCaption)+2);
	m_memCaption.Write (szCaption, 0, strlen (szCaption));
	g_szCaption = (char *) m_memCaption.GetBuffer ();
	InvalidateControl (false);
}

void UIControl::PaintText (HDC hdc)
{
	HFONT hfCurFont;
	HFONT hfOldFont;
	hfCurFont = EzCreateFont (hdc, g_szFontName, g_FontSize, 0, g_FontStyle, TRUE);
	SIZE tSize;

	hfOldFont = (HFONT) SelectObject (hdc, hfCurFont);
	
	SetBkMode (hdc, TRANSPARENT);
	SetTextColor (hdc, g_textcolor);

	if (m_bUsingTextbounds == true) {
		TextOut (hdc, g_xPos+g_xOffset, g_yPos+g_yOffset, (char *) g_szCaption+m_lTextStart, m_lTextLength);
	} else {
		TextOut (hdc, g_xPos+g_xOffset, g_yPos+g_yOffset, (char *) g_szCaption, strlen ((char *) g_szCaption));		
	}	

	SelectObject (hdc, hfOldFont);
	DeleteObject (hfCurFont);
	DeleteObject (hfOldFont);
}

void UIControl::PaintText (HDC hdc, int xPos, int yPos, char *szFontName, int FontSize, char *szCaption, COLORREF textcolor)
{
	HFONT hfCurFont;
	HFONT hfOldFont;
	hfCurFont = EzCreateFont (hdc, szFontName, FontSize, 0, 0, TRUE);
	SIZE tSize;

	hfOldFont = (HFONT) SelectObject (hdc, hfCurFont);
	
	SetBkMode (hdc, TRANSPARENT);
	SetTextColor (hdc, textcolor);

	TextOut (hdc, xPos, yPos, (char *) szCaption, strlen ((char *) szCaption));		

	SelectObject (hdc, hfOldFont);
	DeleteObject (hfCurFont);
	DeleteObject (hfOldFont);
}

void UIControl::PaintText (HDC hdc, bool bCalcSize, bool bDrawText)
{
	// PaintText override with additonal options for calculating the size of the text
	// and choosing whether to display the text.
	HFONT hfCurFont;
	HFONT hfOldFont;
	hfCurFont = EzCreateFont (hdc, g_szFontName, g_FontSize, 0, g_FontStyle, TRUE);
	SIZE tSize;

	hfOldFont = (HFONT) SelectObject (hdc, hfCurFont);
	
	SetBkMode (hdc, TRANSPARENT);
	SetTextColor (hdc, g_textcolor);
	
	if (bDrawText == true ) {
		TextOut (hdc, g_xPos+g_xOffset, g_yPos+g_yOffset, (char *) g_szCaption, strlen ((char *) g_szCaption));
	}

	if (bCalcSize == true) {
		// Calculate the size of the specified string of text.
		GetTextExtentPoint32 (hdc, (char *) g_szCaption, strlen ((char *) g_szCaption), &tSize);

		m_rctTextRect.left = g_xPos;
		m_rctTextRect.top = g_yPos;
		m_rctTextRect.right = g_xPos + tSize.cx;
		m_rctTextRect.bottom = g_yPos + tSize.cy;
		m_bTextSizeKnown = true;
	}

	SelectObject (hdc, hfOldFont);
	DeleteObject (hfCurFont);
	DeleteObject (hfOldFont);
}

void UIControl::GetTextRect (LPRECT lpRect)
{
	// Get the rectangle for the text.
	// NOTE for this to work properly the correct PaintText override must be used with the bCalcSize value set to true.
	// OR, if the text size is not known the it is calculated.
	RECT txtRect;

	if (m_bTextSizeKnown == false) {
		CalcTextRect ();
	}

	txtRect.top = m_rctTextRect.top;
	txtRect.left = m_rctTextRect.left;
	txtRect.bottom = m_rctTextRect.bottom;
	txtRect.right = m_rctTextRect.right;
	
	*lpRect = txtRect;
}

void UIControl::SetHighlightProperties (bool bUseHighlight, bool bUseFade)
{
	g_bUsehighlight = bUseHighlight;
	g_bUsefade = bUseFade;
}

bool UIControl::IsMouseRect ()
{
	bool bRes = false;
	if (g_mouseXPos >= g_Rect.left && g_mouseXPos <= g_Rect.right) {
		if (g_mouseYPos >= g_Rect.top && g_mouseYPos <= g_Rect.bottom) {
			bRes = true;
		}
	}
	return bRes;
}

bool UIControl::IsMouseRect (int iMouseXPos, int iMouseYPos)
{
	bool bRes = false;
	if (iMouseXPos >= g_Rect.left && iMouseXPos <= g_Rect.right) {
		if (iMouseYPos >= g_Rect.top && iMouseYPos <= g_Rect.bottom) {
			bRes = true;
		}
	}
	return bRes;
}

bool UIControl::IsMouseRect (RECT rect)
{
	bool bRes = false;
	if (g_mouseXPos >= rect.left && g_mouseXPos <= rect.right) {
		if (g_mouseYPos >= rect.top && g_mouseYPos <= rect.bottom) {
			bRes = true;
		}
	}
	return bRes;
}

bool UIControl::IsMouseRect (bool bIncludeText)
{
	RECT rctTemp;
	RECT ctlRect;
	RECT rctUnion;
	
	// Get the rect for this control not including the text
	GetControlRect (&ctlRect);

	if (bIncludeText == true) {
		if (m_bTextSizeKnown == true) {
			UnionRect (&rctUnion, &m_rctTextRect, &ctlRect);
			return IsMouseRect (rctUnion);
		} else {
			return IsMouseRect (ctlRect);
		}
	} else {
		return IsMouseRect (ctlRect);
	}

	return IsMouseRect (ctlRect);
}

void UIControl::InvalidateControl (bool bIncludeText)
{
	RECT rctTemp;
	RECT ctlRect;
	RECT rctUnion;
	
	// Get the rect for this control not including the text
	GetControlRect (&ctlRect);

	if (bIncludeText == true) {
		if (m_bTextSizeKnown == true) {
			UnionRect (&rctUnion, &m_rctTextRect, &ctlRect);
			InvalidateRect (g_hWnd, &rctUnion, false);
			return;
		} else {
			InvalidateRect (g_hWnd, &ctlRect, false);
			return;
		}
	} else {
		InvalidateRect (g_hWnd, &ctlRect, false);
		return;
	}

	InvalidateRect (g_hWnd, &ctlRect, false);
}

// Simple font creation routine.
HFONT UIControl::EzCreateFont (HDC hdc, TCHAR * szFaceName, int iDeciPtHeight,int iDeciPtWidth, int iAttributes, BOOL fLogRes)
{
     FLOAT cxDpi, cyDpi;
     HFONT hFont;
     LOGFONT lf;
     POINT pt;
     TEXTMETRIC tm;
     
     SaveDC (hdc) ;
     
     SetGraphicsMode (hdc, GM_ADVANCED) ;
     ModifyWorldTransform (hdc, NULL, MWT_IDENTITY) ;
     SetViewportOrgEx (hdc, 0, 0, NULL) ;
     SetWindowOrgEx   (hdc, 0, 0, NULL) ;
     
     if (fLogRes) {
	cxDpi = (FLOAT) GetDeviceCaps (hdc, LOGPIXELSX);
	cyDpi = (FLOAT) GetDeviceCaps (hdc, LOGPIXELSY);
     } else {
	cxDpi = (FLOAT) (25.4 * GetDeviceCaps (hdc, HORZRES) / GetDeviceCaps (hdc, HORZSIZE));
	cyDpi = (FLOAT) (25.4 * GetDeviceCaps (hdc, VERTRES) / GetDeviceCaps (hdc, VERTSIZE));
     }
     
     pt.x = (int) (iDeciPtWidth  * cxDpi / 72);
     pt.y = (int) (iDeciPtHeight * cyDpi / 72);

     DPtoLP (hdc, &pt, 1);
	 lf.lfHeight         = - (int) (fabs ((double) (pt.y) / 10.0 + 0.5)) ;
     lf.lfWidth          = 0 ;
     lf.lfEscapement     = 0 ;
     lf.lfOrientation    = 0 ;
     lf.lfWeight         = iAttributes & EZ_ATTR_BOLD      ? 700 : 0 ;
     lf.lfItalic         = iAttributes & EZ_ATTR_ITALIC    ?   1 : 0 ;
     lf.lfUnderline      = iAttributes & EZ_ATTR_UNDERLINE ?   1 : 0 ;
     lf.lfStrikeOut      = iAttributes & EZ_ATTR_STRIKEOUT ?   1 : 0 ;
     lf.lfCharSet        = DEFAULT_CHARSET ;
     lf.lfOutPrecision   = 0 ;
     lf.lfClipPrecision  = 0 ;
     lf.lfQuality        = 0 ;
     lf.lfPitchAndFamily = 0 ;
     
     lstrcpy (lf.lfFaceName, szFaceName);
     hFont = CreateFontIndirect (&lf);
     
     if (iDeciPtWidth != 0) {
	hFont = (HFONT) SelectObject (hdc, hFont);
        GetTextMetrics (hdc, &tm);
        DeleteObject (SelectObject (hdc, hFont));
	lf.lfWidth = (int) (tm.tmAveCharWidth * fabs ((double) pt.x) / fabs ((double) pt.y) + 0.5);
	hFont = CreateFontIndirect (&lf);
     }
     
     RestoreDC (hdc, -1) ;
     return hFont ;
}