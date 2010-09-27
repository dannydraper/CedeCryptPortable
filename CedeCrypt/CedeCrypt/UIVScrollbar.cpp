#include "UIVScrollbar.h"

UIVScrollbar::UIVScrollbar ()
{
	m_iUpperBound = 0;
	m_iLowerBound = 0;
	m_ioriginyPos = 0;
	m_ioriginDiff = 0;
	m_iBaryPos = 0;
	m_iBarHeight = 100;
	m_iScrollstep = 1;
	m_bBarPosInitialised = false;
	m_iBarorigin = 0;
	m_iInitialBarYPos = 0;
	m_iScrollpos = 0;
	m_iScrollmax = 0;

	m_rctBar.top = 0;
	m_rctBar.bottom = 0;
	m_rctBar.left = 0;
	m_rctBar.right = 0;

	m_rctUpArrow.top = 0;
	m_rctUpArrow.bottom = 0;
	m_rctUpArrow.left = 0;
	m_rctUpArrow.right = 0;

	m_rctDownArrow.top = 0;
	m_rctDownArrow.bottom = 0;
	m_rctDownArrow.left = 0;
	m_rctDownArrow.right = 0;

	m_bMouseover = false;
	m_bHighlighted = false;

	m_bMouseoverUpArrow = false;
	m_bMousedownUpArrow = false;
	m_bHighlightedUpArrow = false;
		
	m_bMouseoverDownArrow = false;
	m_bMousedownDownArrow = false;
	m_bHighlightedDownArrow = false;

	m_bMouseoverUpperTrack = false;
	m_bMouseoverLowerTrack = false;

	m_bHighlightlocked = false;
	m_bScrollTimerActive = false;
	m_iScrollInterval = 200;
	m_bUseDiagnostics = false;
	m_bStartatend = false;
}

UIVScrollbar::~UIVScrollbar ()
{

}

void UIVScrollbar::SetMaxValue (int iMax)
{
	m_bBarPosInitialised = false;
	if (iMax < 0) {
		m_iScrollmax = 0;
	} else {
		m_iScrollmax = iMax;
	}
	OutputText ("SetMaxValue called...");
}

void UIVScrollbar::SetDiagnostics (Diagnostics *pdiag)
{
	m_pdiag = pdiag;	
}

void UIVScrollbar::InitialiseDefault (HWND hWnd, int cID, int xPos, int yPos, int Width, int Height)
{
	// Initialise the scroll bar with a default bitmap
	SetBitmapResources (IDB_VSCROLLBASE);
	SetBitmapUpArrow (0, 0, 16, 16);
	SetBitmapUpArrowHigh (46, 0, 16, 16);
	SetBitmapUpArrowDown (62, 0, 16, 16);
	SetBitmapDownArrowHigh (46, 31, 16, 16);
	SetBitmapDownArrowDown (62, 31, 16, 16);
	SetBitmapDownArrow (0, 31, 16, 16);
	SetBitmapScrollTrack (0, 16, 16, 14);
	SetBitmapScrollbarHighTop (16, 0, 15, 10);
	SetBitmapScrollbarHighMiddle (16, 11, 15, 10);
	SetBitmapScrollbarHighBottom (16, 23, 15, 11);
	SetBitmapScrollbarHighHandle (16, 34, 15, 12);
	SetBitmapScrollbarNormTop (31, 0, 15, 10);
	SetBitmapScrollbarNormMiddle (31, 11, 15, 10);
	SetBitmapScrollbarNormBottom (31, 23, 15, 11);
	SetBitmapScrollbarNormHandle (31, 34, 15, 12);
	SetBitmapScrollbarDownTop (78, 0, 15, 10);
	SetBitmapScrollbarDownMiddle (78, 11, 15, 10);
	SetBitmapScrollbarDownBottom (78, 23, 15, 11);
	SetBitmapScrollbarDownHandle (78, 34, 15, 12);

	SetProperties (hWnd, cID, xPos, yPos, Width, Height);
}

void UIVScrollbar::SetBitmapResources (unsigned int wResbase)
{
	// Load the bitmap resource. Only one resource is needed for the scroll bar. This one 
	// resource contains all the images necessary to paint a complete verticle scrollbar, in any size and position.
	// We need to specify how the scrollbar bitmap is constructed in later functions.
	g_hbmBase = LoadBitmapResource (wResbase);
}

void UIVScrollbar::SetBitmapUpArrow (int xPos, int yPos, int Width, int Height)
{
	m_iUpArrowxPos = xPos;
	m_iUpArrowyPos = yPos;
	m_iUpArrowWidth = Width;
	m_iUpArrowHeight = Height;
}

void UIVScrollbar::SetBitmapUpArrowHigh (int xPos, int yPos, int Width, int Height)
{
	m_iUpArrowHighxPos = xPos;
	m_iUpArrowHighyPos = yPos;
	m_iUpArrowHighWidth = Width;
	m_iUpArrowHighHeight = Height;
}

void UIVScrollbar::SetBitmapUpArrowDown (int xPos, int yPos, int Width, int Height)
{
	m_iUpArrowDownxPos = xPos;
	m_iUpArrowDownyPos = yPos;
	m_iUpArrowDownWidth = Width;
	m_iUpArrowDownHeight = Height;
}

void UIVScrollbar::SetBitmapDownArrow (int xPos, int yPos, int Width, int Height)
{
	m_iDownArrowxPos = xPos;
	m_iDownArrowyPos = yPos;
	m_iDownArrowWidth = Width;
	m_iDownArrowHeight = Height;
}

void UIVScrollbar::SetBitmapDownArrowHigh (int xPos, int yPos, int Width, int Height)
{
	m_iDownArrowHighxPos = xPos;
	m_iDownArrowHighyPos = yPos;
	m_iDownArrowHighWidth = Width;
	m_iDownArrowHighHeight = Height;
}

void UIVScrollbar::SetBitmapDownArrowDown (int xPos, int yPos, int Width, int Height)
{
	m_iDownArrowDownxPos = xPos;
	m_iDownArrowDownyPos = yPos;
	m_iDownArrowDownWidth = Width;
	m_iDownArrowDownHeight = Height;
}

void UIVScrollbar::SetBitmapScrollTrack (int xPos, int yPos, int Width, int Height)
{
	m_iScrollTrackxPos = xPos;
	m_iScrollTrackyPos = yPos;
	m_iScrollTrackWidth = Width;
	m_iScrollTrackHeight = Height;
}

void UIVScrollbar::SetBitmapScrollbarHighTop (int xPos, int yPos, int Width, int Height)
{
	m_iScrollBarHighTopxPos = xPos;
	m_iScrollBarHighTopyPos = yPos;
	m_iScrollBarHighTopWidth = Width;
	m_iScrollBarHighTopHeight = Height;
}

void UIVScrollbar::SetBitmapScrollbarHighBottom (int xPos, int yPos, int Width, int Height)
{
	m_iScrollBarHighBottomxPos = xPos;
	m_iScrollBarHighBottomyPos = yPos;
	m_iScrollBarHighBottomWidth = Width;
	m_iScrollBarHighBottomHeight = Height;
}

void UIVScrollbar::SetBitmapScrollbarHighMiddle (int xPos, int yPos, int Width, int Height)
{
	m_iScrollBarHighMiddlexPos = xPos;
	m_iScrollBarHighMiddleyPos = yPos;
	m_iScrollBarHighMiddleWidth = Width;
	m_iScrollBarHighMiddleHeight = Height;
}

void UIVScrollbar::SetBitmapScrollbarHighHandle (int xPos, int yPos, int Width, int Height)
{
	m_iScrollBarHighHandlexPos = xPos;
	m_iScrollBarHighHandleyPos = yPos;
	m_iScrollBarHighHandleWidth = Width;
	m_iScrollBarHighHandleHeight = Height;
}

void UIVScrollbar::SetBitmapScrollbarNormTop (int xPos, int yPos, int Width, int Height)
{
	m_iScrollBarNormTopxPos = xPos;
	m_iScrollBarNormTopyPos = yPos;
	m_iScrollBarNormTopWidth = Width;
	m_iScrollBarNormTopHeight = Height;
}

void UIVScrollbar::SetBitmapScrollbarNormBottom (int xPos, int yPos, int Width, int Height)
{
	m_iScrollBarNormBottomxPos = xPos;
	m_iScrollBarNormBottomyPos = yPos;
	m_iScrollBarNormBottomWidth = Width;
	m_iScrollBarNormBottomHeight = Height;
}

void UIVScrollbar::SetBitmapScrollbarNormMiddle (int xPos, int yPos, int Width, int Height)
{
	m_iScrollBarNormMiddlexPos = xPos;
	m_iScrollBarNormMiddleyPos = yPos;
	m_iScrollBarNormMiddleWidth = Width;
	m_iScrollBarNormMiddleHeight = Height;
}

void UIVScrollbar::SetBitmapScrollbarNormHandle (int xPos, int yPos, int Width, int Height)
{
	m_iScrollBarNormHandlexPos = xPos;
	m_iScrollBarNormHandleyPos = yPos;
	m_iScrollBarNormHandleWidth = Width;
	m_iScrollBarNormHandleHeight = Height;
}

void UIVScrollbar::SetBitmapScrollbarDownTop (int xPos, int yPos, int Width, int Height)
{
	m_iScrollBarDownTopxPos = xPos;
	m_iScrollBarDownTopyPos = yPos;
	m_iScrollBarDownTopWidth = Width;
	m_iScrollBarDownTopHeight = Height;
}

void UIVScrollbar::SetBitmapScrollbarDownBottom (int xPos, int yPos, int Width, int Height)
{
	m_iScrollBarDownBottomxPos = xPos;
	m_iScrollBarDownBottomyPos = yPos;
	m_iScrollBarDownBottomWidth = Width;
	m_iScrollBarDownBottomHeight = Height;
}

void UIVScrollbar::SetBitmapScrollbarDownMiddle (int xPos, int yPos, int Width, int Height)
{
	m_iScrollBarDownMiddlexPos = xPos;
	m_iScrollBarDownMiddleyPos = yPos;
	m_iScrollBarDownMiddleWidth = Width;
	m_iScrollBarDownMiddleHeight = Height;
}

void UIVScrollbar::SetBitmapScrollbarDownHandle (int xPos, int yPos, int Width, int Height)
{
	m_iScrollBarDownHandlexPos = xPos;
	m_iScrollBarDownHandleyPos = yPos;
	m_iScrollBarDownHandleWidth = Width;
	m_iScrollBarDownHandleHeight = Height;
}

void UIVScrollbar::Paint (HDC hdc)
{
	int iAvailHeight = g_Height;
	int iTempyPos = 0;
	int iTempDownyPos = 0;
	int iTempAvailHeight = 0;

	int iScrollBarTopxPos;
	int iScrollBarTopyPos;
	int iScrollBarTopWidth;
	int iScrollBarTopHeight;

	int iScrollBarBottomxPos;
	int iScrollBarBottomyPos;
	int iScrollBarBottomWidth;
	int iScrollBarBottomHeight;

	int iScrollBarMiddlexPos;
	int iScrollBarMiddleyPos;
	int iScrollBarMiddleWidth;
	int iScrollBarMiddleHeight;

	int iScrollBarHandlexPos;
	int iScrollBarHandleyPos;
	int iScrollBarHandleWidth;
	int iScrollBarHandleHeight;

	//OutputText ("Scrollbar repainting...");

	// Paint the scrollbar to the screen
	BITMAP bm;
	HDC hdcMem = CreateCompatibleDC (hdc);
	HBITMAP hbmOld = (HBITMAP) SelectObject (hdcMem, g_hbmBase);

	GetObject (g_hbmBase, sizeof (bm), &bm);
	
	// Paint the scroll track - Important thing to note about this function is that we're blitting the scroll track to the exact height
	// in pixels specified by the caller. Now because the scroll track consists of fixed height bitmaps, we need to make sure that when
	// we approach the avail height limit we don't blit over the height specified by the caller.

	// First the Up Arrow
	if (m_bMouseoverUpArrow == true) {
		if (m_bMousedownUpArrow == true) {
			BitBlt (hdc, g_xPos, g_yPos, m_iUpArrowDownWidth, m_iUpArrowDownHeight, hdcMem, m_iUpArrowDownxPos, m_iUpArrowDownyPos, SRCCOPY);
		} else {
			BitBlt (hdc, g_xPos, g_yPos, m_iUpArrowHighWidth, m_iUpArrowHighHeight, hdcMem, m_iUpArrowHighxPos, m_iUpArrowHighyPos, SRCCOPY);
		}
	} else {
		BitBlt (hdc, g_xPos, g_yPos, m_iUpArrowWidth, m_iUpArrowHeight, hdcMem, m_iUpArrowxPos, m_iUpArrowyPos, SRCCOPY);
	}
	
	// Set the rect for the Up Arrow (needed for highlighting and mouse down images)
	m_rctUpArrow.left = g_xPos;
	m_rctUpArrow.top = g_yPos;
	m_rctUpArrow.bottom = g_yPos+m_iUpArrowHeight;
	m_rctUpArrow.right = g_xPos+m_iUpArrowWidth;
	
	iAvailHeight -= m_iUpArrowHeight; // Local variable to track how much height we've got left to keep blitting the track
	iTempyPos = g_yPos+m_iUpArrowHeight; // Local temp variable to keep track of where we last blitted to
	m_iUpperBound = iTempyPos; // Set the upper bound for the scroll bar. This is the highest point it can move to.
	iTempAvailHeight = iAvailHeight;

	// Now blit the scroll track. Repeatedly blits the middle part until we reach the avail height limit.
	for (int r=0;r<iAvailHeight;r+=m_iScrollTrackHeight)
	{		
		iTempAvailHeight = iAvailHeight-r; // Keep track of how much we can still blit to.
		iTempDownyPos = iTempyPos+r; // Keep track of where we last blitted to.

		if (iTempAvailHeight >= m_iScrollTrackHeight) { // If the scroll track bitmap can't fit in the remaining height we have left, then don't try.
			BitBlt (hdc, g_xPos, iTempyPos+r, m_iScrollTrackWidth, m_iScrollTrackHeight, hdcMem, m_iScrollTrackxPos, m_iScrollTrackyPos, SRCCOPY);
		}
	}

	// Now Blit the Down Arrow. - What we're doing here is making sure that when we blit the down arrow, it makes the entire scroll track
	// the exact height specified by the caller. So for that minus the height of the down arrow from the position we last blitted to,
	// then add the remaining height we have left. Basically this means that we're blitting the down arrow over the top of the last scroll 
	// track bitmap, just so we get the exact height we need.

	iAvailHeight = iTempAvailHeight;
	iTempyPos = iTempDownyPos;

	iTempyPos -= m_iDownArrowHeight;
	iTempyPos += iAvailHeight;
	m_iLowerBound = iTempyPos; // Set the lower bound for the scroll bar. This is the lowest point it can move to.

	if (m_bMouseoverDownArrow == true) {
		if (m_bMousedownDownArrow == true) {
			BitBlt (hdc, g_xPos, iTempyPos, m_iDownArrowDownWidth, m_iDownArrowDownHeight, hdcMem, m_iDownArrowDownxPos, m_iDownArrowDownyPos, SRCCOPY);
		} else {
			BitBlt (hdc, g_xPos, iTempyPos, m_iDownArrowHighWidth, m_iDownArrowHighHeight, hdcMem, m_iDownArrowHighxPos, m_iDownArrowHighyPos, SRCCOPY);
		}
	} else {
		BitBlt (hdc, g_xPos, iTempyPos, m_iDownArrowWidth, m_iDownArrowHeight, hdcMem, m_iDownArrowxPos, m_iDownArrowyPos, SRCCOPY);
	}
	
	
	// Now set the member RECT of the Down Arrow (needed for highlighting and down images)
	m_rctDownArrow.left = g_xPos;
	m_rctDownArrow.top = iTempyPos;
	m_rctDownArrow.bottom = iTempyPos + m_iDownArrowHeight;
	m_rctDownArrow.right = g_xPos + m_iDownArrowWidth;

	if (m_bMouseover == true) {
		if (g_mouseDown == true) {
			iScrollBarTopxPos = m_iScrollBarDownTopxPos;
			iScrollBarTopyPos = m_iScrollBarDownTopyPos;
			iScrollBarTopWidth = m_iScrollBarDownTopWidth;
			iScrollBarTopHeight = m_iScrollBarDownTopHeight;

			iScrollBarBottomxPos = m_iScrollBarDownBottomxPos;
			iScrollBarBottomyPos = m_iScrollBarDownBottomyPos;
			iScrollBarBottomWidth = m_iScrollBarDownBottomWidth;
			iScrollBarBottomHeight = m_iScrollBarDownBottomHeight;

			iScrollBarMiddlexPos = m_iScrollBarDownMiddlexPos;
			iScrollBarMiddleyPos = m_iScrollBarDownMiddleyPos;
			iScrollBarMiddleWidth = m_iScrollBarDownMiddleWidth;
			iScrollBarMiddleHeight = m_iScrollBarDownMiddleHeight;

			iScrollBarHandlexPos = m_iScrollBarDownHandlexPos;
			iScrollBarHandleyPos = m_iScrollBarDownHandleyPos;
			iScrollBarHandleWidth = m_iScrollBarDownHandleWidth;
			iScrollBarHandleHeight = m_iScrollBarDownHandleHeight;
		} else {
			iScrollBarTopxPos = m_iScrollBarHighTopxPos;
			iScrollBarTopyPos = m_iScrollBarHighTopyPos;
			iScrollBarTopWidth = m_iScrollBarHighTopWidth;
			iScrollBarTopHeight = m_iScrollBarHighTopHeight;

			iScrollBarBottomxPos = m_iScrollBarHighBottomxPos;
			iScrollBarBottomyPos = m_iScrollBarHighBottomyPos;
			iScrollBarBottomWidth = m_iScrollBarHighBottomWidth;
			iScrollBarBottomHeight = m_iScrollBarHighBottomHeight;

			iScrollBarMiddlexPos = m_iScrollBarHighMiddlexPos;
			iScrollBarMiddleyPos = m_iScrollBarHighMiddleyPos;
			iScrollBarMiddleWidth = m_iScrollBarHighMiddleWidth;
			iScrollBarMiddleHeight = m_iScrollBarHighMiddleHeight;

			iScrollBarHandlexPos = m_iScrollBarHighHandlexPos;
			iScrollBarHandleyPos = m_iScrollBarHighHandleyPos;
			iScrollBarHandleWidth = m_iScrollBarHighHandleWidth;
			iScrollBarHandleHeight = m_iScrollBarHighHandleHeight;			
		}
	} else {
		iScrollBarTopxPos = m_iScrollBarNormTopxPos;
		iScrollBarTopyPos = m_iScrollBarNormTopyPos;
		iScrollBarTopWidth = m_iScrollBarNormTopWidth;
		iScrollBarTopHeight = m_iScrollBarNormTopHeight;

		iScrollBarBottomxPos = m_iScrollBarNormBottomxPos;
		iScrollBarBottomyPos = m_iScrollBarNormBottomyPos;
		iScrollBarBottomWidth = m_iScrollBarNormBottomWidth;
		iScrollBarBottomHeight = m_iScrollBarNormBottomHeight;

		iScrollBarMiddlexPos = m_iScrollBarNormMiddlexPos;
		iScrollBarMiddleyPos = m_iScrollBarNormMiddleyPos;
		iScrollBarMiddleWidth = m_iScrollBarNormMiddleWidth;
		iScrollBarMiddleHeight = m_iScrollBarNormMiddleHeight;

		iScrollBarHandlexPos = m_iScrollBarNormHandlexPos;
		iScrollBarHandleyPos = m_iScrollBarNormHandleyPos;
		iScrollBarHandleWidth = m_iScrollBarNormHandleWidth;
		iScrollBarHandleHeight = m_iScrollBarNormHandleHeight;	
	}

	// Now Blit the Scroll Bar itself!	
	int iBarAvailHeight = 0;
	int iBarTempAvailHeight = 0;
	int iBarTempDownyPos = 0;
	int iBarTempyPos = 0;

	if (m_bBarPosInitialised == false) {
		m_bBarPosInitialised = true;
				
		InitialiseScrollValues ();

		if (m_bStartatend == true) {
			m_iBaryPos = m_iLowerBound-m_iBarHeight;
			m_iInitialBarYPos = m_iUpperBound;
		} else {
			m_iBaryPos = m_iUpperBound;
			m_iInitialBarYPos = m_iBaryPos;
		}
		
	}

	iBarTempyPos = m_iBaryPos;

	// Set the member rect of the bar.
	m_rctBar.top = iBarTempyPos;
	m_rctBar.left = g_xPos;
	m_rctBar.right = g_xPos+iScrollBarTopWidth;

	BitBlt (hdc, g_xPos, iBarTempyPos, iScrollBarTopWidth, iScrollBarTopHeight, hdcMem, iScrollBarTopxPos, iScrollBarTopyPos, SRCCOPY);
	iBarTempyPos += iScrollBarTopHeight;
	iBarAvailHeight = m_iBarHeight;

	iBarAvailHeight -= iScrollBarTopHeight;
	iBarTempAvailHeight = iBarAvailHeight;

	// Now blit the rest of the scroll bar. Repeatedly blits the middle part until we reach the avail height limit.
	for (int b=0;b<iBarAvailHeight;b+=iScrollBarMiddleHeight)
	{		
		iBarTempAvailHeight = iBarAvailHeight-b; // Keep track of how much we can still blit to.
		iBarTempDownyPos = iBarTempyPos+b; // Keep track of where we last blitted to.

		if (iBarTempAvailHeight >= iScrollBarMiddleHeight) { // If the scroll track bitmap can't fit in the remaining height we have left, then don't try.
			BitBlt (hdc, g_xPos, iBarTempyPos+b, iScrollBarMiddleWidth, iScrollBarMiddleHeight, hdcMem, iScrollBarMiddlexPos, iScrollBarMiddleyPos, SRCCOPY);
		}
	}

	// Now Blit the Bottom part of the bar. - What we're doing here is making sure that when we blit the bottom, it makes the entire scroll bar
	// the exact height specified by the caller. So for that we minus the height of the bottom part from the position we last blitted to,
	// then add the remaining height we have left. Basically this means that we're blitting the bottom part over the top of the last middle 
	// bar bitmap, just so we get the exact height we need.

	iBarAvailHeight = iBarTempAvailHeight;
	iBarTempyPos = iBarTempDownyPos;

	iBarTempyPos -= iScrollBarBottomHeight;
	iBarTempyPos += iBarAvailHeight;	
	// Blit the bottom part of the scroll bar.
	BitBlt (hdc, g_xPos, iBarTempyPos, iScrollBarBottomWidth, iScrollBarBottomHeight, hdcMem, iScrollBarBottomxPos, iScrollBarBottomyPos, SRCCOPY);
	// Blit the scroll bar handle
	BitBlt (hdc, g_xPos, m_iBaryPos + ((m_iBarHeight / 2) - (iScrollBarHandleHeight / 2)), iScrollBarHandleWidth, iScrollBarHandleHeight, hdcMem, iScrollBarHandlexPos, iScrollBarHandleyPos, SRCCOPY);

	// Set the bottom member rect of the bar
	m_rctBar.bottom = iBarTempyPos+iScrollBarBottomHeight;

	// Set the rect structures for the upper and lower scroll tracks
	m_rctUpperTrack.left = g_xPos;
	m_rctUpperTrack.right = g_xPos+iScrollBarTopWidth;
	m_rctUpperTrack.top = m_iUpperBound;
	m_rctUpperTrack.bottom = m_rctBar.top;

	m_rctLowerTrack.left = g_xPos;
	m_rctLowerTrack.right = g_xPos+iScrollBarTopWidth;
	m_rctLowerTrack.bottom = m_iLowerBound;
	m_rctLowerTrack.top = m_rctBar.bottom;

	SelectObject (hdcMem, hbmOld);
	DeleteDC (hdcMem);
	DeleteObject (hbmOld);
	
}

void UIVScrollbar::InitialiseScrollValues ()
{
	// Scroll max is the number of pixels requested by the caller
	// if the maximum requested is beyond the amount the bar can physically move
	// then we need to increase the scroll step and recalculate the height of the bar.
	
	int iTrackspace = m_iLowerBound - m_iUpperBound;
	int iWantedSpace = m_iScrollmax;
	int iMinBarHeight = 20;

	int iAvailspace = iTrackspace - iMinBarHeight;

	//m_diag.OutputDebugInt ("m_iAvailspace: ", iAvailspace);
	
	if (iWantedSpace > iAvailspace) {
		m_iScrollstep = (iWantedSpace / iAvailspace) + 1;
		m_iBarHeight = iAvailspace - (iWantedSpace / m_iScrollstep);
	} else {
		m_iBarHeight = iTrackspace - iWantedSpace;
	}

	if (m_iBarHeight < iMinBarHeight) {
		m_iBarHeight = iMinBarHeight;
	}


	OutputText ("Scroll values initialised...");

	//m_diag.OutputDebugInt ("m_iScrollstep: ", m_iScrollstep);
	//m_diag.OutputDebugInt ("m_iBarHeight: ", m_iBarHeight);
}

void UIVScrollbar::SetAlwaysatend ()
{
	//m_iBaryPos = m_iLowerBound;
	//m_iBaryPos = 200;

	//m_iScrollpos = (m_iBaryPos - m_iInitialBarYPos) * m_iScrollstep;
	//InvalidateRect (g_hWnd, &g_Rect, false);
	//OutputText ("Scrollbar: Movetoend called.");
	//InvalidateControl (false);
	//NotifyMouseWheel (-120);
	m_bStartatend = true;
}

void UIVScrollbar::OutputInt (LPCSTR lpszText, int iValue)
{
	if (m_bUseDiagnostics == true) {
		m_pdiag->OutputInt (lpszText, iValue);
	}
}

void UIVScrollbar::OutputText (LPCSTR lpszText)
{
	if (m_bUseDiagnostics == true) {
		m_pdiag->OutputText (lpszText);
	}
}

void UIVScrollbar::OutputText (LPCSTR lpszName, LPCSTR lpszValue)
{
	if (m_bUseDiagnostics == true) {
		m_pdiag->OutputText (lpszName, lpszValue);
	}
}

void UIVScrollbar::NotifyMouseWheel (short zDelta)
{
	if (zDelta > 0) { // Wheel was moved up
		m_iBaryPos -= m_iScrollstep*2;
		//m_iBaryPos -= 10;
	} else { // Wheel was moved down
		m_iBaryPos += m_iScrollstep*2;	
		//m_iBaryPos += 10;
	}

	InvalidateRect (g_hWnd, &g_Rect, false);
	m_iScrollpos = (m_iBaryPos - m_iInitialBarYPos) * m_iScrollstep;
	
	// Inform the parent window handle of the Scroll.
	PostMessage (g_hWnd, WM_UISCROLL, (WPARAM) g_controlID, (LPARAM) m_iScrollpos);

	if (m_iBaryPos < m_iUpperBound) {
		m_iBaryPos = m_iUpperBound;
	}

	if (m_iBaryPos+m_iBarHeight > m_iLowerBound) {
		m_iBaryPos = m_iLowerBound-m_iBarHeight;
	}
}

void UIVScrollbar::NotifyMouseMove (int iXPos, int iYPos)
{
	g_mouseXPos = iXPos;
	g_mouseYPos = iYPos;

	if (g_mouseDown == true) {
		
		// Get the origin difference for the Y position of the mouse. Basically, when the mouse has been
		// pressed down on the bar, we simply want to know how far up or down to move the bar. Because the mouse
		// position could be anywhere on the bar, we need a Zero based reference for where the mouse is on the bar.
		// So, we simply grab the current mouse Y position when the left mouse button has been pressed, then we 
		// subtract this from the current mouse Y position. That way, every time the left mouse button is pressed,
		// the origin always starts at 0, and then it goes Up or Down (to minus figures) depending if the user
		// moves the mouse up or down.
		m_ioriginDiff = g_mouseYPos - m_ioriginyPos;

		// Same thing with the Bar itself. When the left mouse button is pressed, we grab the bars last known position.
		// and use this to move the bar by the mouse origin difference.
		m_iBaryPos = m_iBarorigin + m_ioriginDiff;
		
		// Make sure the Bar doesn't go outside of it's Scroll Track.
		// ensure it stays within the bounds.
		if (m_iBaryPos < m_iUpperBound) {
			m_iBaryPos = m_iUpperBound;
		}

		if (m_iBaryPos+m_iBarHeight > m_iLowerBound) {
			m_iBaryPos = m_iLowerBound-m_iBarHeight;
		}

		InvalidateRect (g_hWnd, &g_Rect, false);

		m_iScrollpos = (m_iBaryPos - m_iInitialBarYPos) * m_iScrollstep;

		// Inform the parent window handle of the Scroll.
		PostMessage (g_hWnd, WM_UISCROLL, (WPARAM) g_controlID, (LPARAM) m_iScrollpos);
	}

	// Check if the mouse is over the Bar.
	if (IsMouseRect (m_rctBar) == true) {
		m_bMouseover = true;

		if (m_bScrollTimerActive == true) {
			m_bScrollTimerActive = false;
			m_iScrollInterval = 200;
			KillTimer (g_hWnd, IDT_SCROLLUP+g_controlID);
			KillTimer (g_hWnd, IDT_SCROLLDOWN+g_controlID);
		}

		if (m_bHighlighted == false) {
			m_bHighlighted = true;
			InvalidateRect (g_hWnd, &g_Rect, false);
		}
	} else {
		if (m_bHighlightlocked == false) {
			m_bMouseover = false;
			if (m_bHighlighted == true) {
				m_bHighlighted = false;
				InvalidateRect (g_hWnd, &g_Rect, false);
			}
		}
	}

	// Check if the mouse is over the Up Arrow
	if (IsMouseRect (m_rctUpArrow) == true) {
		m_bMouseoverUpArrow = true;
		if (m_bHighlightedUpArrow == false) {
			m_bHighlightedUpArrow = true;
			InvalidateRect (g_hWnd, &m_rctUpArrow, false);
		}
	} else {
		m_bMouseoverUpArrow = false;
		if (m_bHighlightedUpArrow == true) {
			m_bHighlightedUpArrow = false;
			InvalidateRect (g_hWnd, &m_rctUpArrow, false);
		}
	}

	// Check if the mouse is over the upper or lower scroll tracks
	if (IsMouseRect (m_rctUpperTrack) == true) {
		m_bMouseoverUpperTrack = true;
	} else {
		m_bMouseoverUpperTrack = false;
	}

	if (IsMouseRect (m_rctLowerTrack) == true) {
		m_bMouseoverLowerTrack = true;
	} else {
		m_bMouseoverLowerTrack = false;
	}

	// Check if the mouse is over the Down Arrow
	if (IsMouseRect (m_rctDownArrow) == true) {
		m_bMouseoverDownArrow = true;
		if (m_bHighlightedDownArrow == false) {
			m_bHighlightedDownArrow = true;
			InvalidateRect (g_hWnd, &m_rctDownArrow, false);
		}
	} else {
		m_bMouseoverDownArrow = false;
		if (m_bHighlightedDownArrow == true) {
			m_bHighlightedDownArrow = false;
			InvalidateRect (g_hWnd, &m_rctDownArrow, false);
		}
	}
}

void UIVScrollbar::NotifyMouseDown ()
{
	if (IsMouseRect (m_rctBar) == true) {
		g_mouseDown = true;
		m_bHighlightlocked = true;

		// When the mouse is down over the bar, immediately get the current position of the
		// mouse and the bar. The bar is then moved along with the mouse with respect to it's
		// origin - or position it was at last.
		m_ioriginyPos = g_mouseYPos;
		m_iBarorigin = m_iBaryPos;

		InvalidateRect (g_hWnd, &g_Rect, false);
	}

	if (m_bMouseoverUpArrow == true || m_bMouseoverUpperTrack == true) {
		m_bMousedownUpArrow = true;
		InvalidateRect (g_hWnd, &m_rctUpArrow, false);
		NotifyMouseWheel (120);

		if (m_bScrollTimerActive == false) {
			SetTimer (g_hWnd, IDT_SCROLLUP+g_controlID, m_iScrollInterval, NULL);
			m_bScrollTimerActive = true;
		}
	}

	if (m_bMouseoverDownArrow == true || m_bMouseoverLowerTrack == true) {
		m_bMousedownDownArrow = true;
		InvalidateRect (g_hWnd, &m_rctDownArrow, false);
		NotifyMouseWheel (-120);

		if (m_bScrollTimerActive == false) {
			SetTimer (g_hWnd, IDT_SCROLLDOWN+g_controlID, m_iScrollInterval, NULL);
			m_bScrollTimerActive = true;
		}
	}
}

void UIVScrollbar::NotifyMouseUp ()
{
	g_mouseDown = false;
	m_bMousedownUpArrow = false;
	m_bMousedownDownArrow = false;
	m_bHighlightlocked = false;

	if (m_bScrollTimerActive == true) {
		m_bScrollTimerActive = false;
		m_iScrollInterval = 200;
		KillTimer (g_hWnd, IDT_SCROLLUP+g_controlID);
		KillTimer (g_hWnd, IDT_SCROLLDOWN+g_controlID);
	}

	if (IsMouseRect (m_rctBar) == false) {
		m_bMouseover = false;
	}

	//if (IsMouseRect () == true) {
		InvalidateRect (g_hWnd, &g_Rect, false);
		//PostMessage (g_hWnd, WM_UICOMMAND, (WPARAM) g_controlID, 0);
	//}
}

void UIVScrollbar::NotifyTimer (unsigned int wParam)
{
	if (wParam == IDT_SCROLLUP+g_controlID) {
		NotifyMouseWheel (120);

		KillTimer (g_hWnd, IDT_SCROLLUP+g_controlID);
		m_iScrollInterval -= 50;
	
		if (m_iScrollInterval < 0) {
			m_iScrollInterval = 30;
		}
		SetTimer (g_hWnd, IDT_SCROLLUP+g_controlID, m_iScrollInterval, NULL);
	}

	if (wParam == IDT_SCROLLDOWN+g_controlID) {

		NotifyMouseWheel (-120);

		KillTimer (g_hWnd, IDT_SCROLLDOWN+g_controlID);
		m_iScrollInterval -= 50;
	
		if (m_iScrollInterval < 0) {
			m_iScrollInterval = 30;
		}
		SetTimer (g_hWnd, IDT_SCROLLDOWN+g_controlID, m_iScrollInterval, NULL);
	}

	NotifyMouseMove (g_mouseXPos, g_mouseYPos);
}