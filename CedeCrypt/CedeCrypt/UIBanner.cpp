#include "UIBanner.h"

UIBanner::UIBanner ()
{
	m_bVisible = true;
}

UIBanner::~UIBanner ()
{

}

void UIBanner::SetVisible (bool bVisible)
{
	m_bVisible = bVisible;
	InvalidateControl (false);
}

void UIBanner::SetBitmapResources (unsigned int wResbase)
{
	// Load the bitmap resource. Only one resource is required for a Banner. Later on we
	// need to specify how the Banner is painted, and we need to specify which area of the banner
	// is designated as a repeating area. So, if the window gets larger or smaller an area of the banner
	// will be repeated if the base bitmap does not meet the required size.
	g_hbmBase = LoadBitmapResource (wResbase);
}

void UIBanner::SetBitmapProperties (int rptxStart, int rptxWidth, int Width, int Height)
{
	m_rptxStart = rptxStart;
	m_rptxWidth = rptxWidth;

	g_iBitmapheight = Height;
	g_iBitmapwidth = Width;
}

void UIBanner::Paint (HDC hdc)
{
	if (m_bVisible == false) {
		return;
	}
	// Paint the box to the screen
	BITMAP bm;
	HDC hdcMem = CreateCompatibleDC (hdc);
	HBITMAP hbmOld = (HBITMAP) SelectObject (hdcMem, g_hbmBase);

	GetObject (g_hbmBase, sizeof (bm), &bm);
	
	/*
	for (int r=0;r<g_Height/g_iBitmapheight;r++) {
		for (int i=0;i<g_Width/g_iBitmapwidth;i++) {
			BitBlt (hdc, g_xPos+(i*g_iBitmapwidth), g_yPos+(r*g_iBitmapheight), g_iBitmapwidth, g_iBitmapheight, hdcMem, 0, 0, SRCCOPY);
		}
	}
	*/

	BitBlt (hdc, g_xPos, g_yPos, g_iBitmapwidth, g_iBitmapheight, hdcMem, 0, 0, SRCCOPY);

	SelectObject (hdcMem, hbmOld);
	DeleteDC (hdcMem);
	DeleteObject (hbmOld);
}

void UIBanner::NotifyMouseMove (int iXPos, int iYPos)
{
	g_mouseXPos = iXPos;
	g_mouseYPos = iYPos;
}

void UIBanner::NotifyMouseUp()
{
	if (IsMouseRect () == true) {
		PostMessage (g_hWnd, WM_UICOMMAND, (WPARAM) g_controlID, g_mouseXPos);
	}
}
