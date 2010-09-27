#include "UIRect.h"

UIRect::UIRect ()
{
	m_bVisible = true;
	m_bMouseover = false;
}

UIRect::~UIRect ()
{

}

unsigned long UIRect::GetSize () {
	return sizeof (UIRect);
}

void UIRect::Paint (HDC hdc)
{
	if (m_bVisible == false) {
		return;
	}
	
	HBRUSH bgColour = CreateSolidBrush (RGB (255, 255, 255));

	RECT bgRect;
	bgRect.left = g_xPos;
	bgRect.right = g_xPos+g_Width;
	bgRect.top = g_yPos;
	bgRect.bottom = g_yPos+g_Height;

	FillRect (hdc, &bgRect, bgColour);

	DeleteObject (bgColour);
}

void UIRect::SetVisible (bool bVisible)
{
	m_bVisible = bVisible;
	InvalidateControl (false);
}
