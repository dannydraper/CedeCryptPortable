#include "UILabel.h"

UILabel::UILabel ()
{
	m_bVisible = true;
	m_bMouseover = false;
	m_bMousedown = false;
	m_bHighlighted = false;
	m_bToggle = false;
}

UILabel::~UILabel ()
{

}

unsigned long UILabel::GetSize () {
	return sizeof (UILabel);
}

void UILabel::Paint (HDC hdc)
{
	if (m_bVisible == false) {
		return;
	}

	
	if (m_bHighlighted == true) {
		RECT rctText;
		RECT rctBg;
		rctBg.left = g_xPos;
		rctBg.top = g_yPos;
		//rctBg.right = g_xPos+20;
		//rctBg.bottom = g_yPos+10;
		GetTextRect (&rctText);
		rctBg.right = g_xPos+(rctText.right-rctText.left);
		rctBg.bottom = g_yPos+(rctText.bottom-rctText.top);


		FillRect (hdc, &rctBg, CreateSolidBrush (RGB (0, 100, 200)));
	}
	

	PaintText (hdc);
	//PaintText (hdc, true, false);
}

void UILabel::SetVisible (bool bVisible)
{
	m_bVisible = bVisible;
	InvalidateControl (false);
}


void UILabel::NotifyMouseMove (int iXPos, int iYPos)
{
	g_mouseXPos = iXPos;
	g_mouseYPos = iYPos;

	if (m_bVisible == false) {
		return;
	}
	
	if (m_bMousedown == true) {	
		if (IsMouseRect (true) == true) {
			//Beep (1000, 10);
			if (m_bToggle = true) {
				
					m_bHighlighted = true;
					m_origTextcolour = g_textcolor;
					SetTextForeColor (RGB (255, 255, 255));			
					InvalidateControl (true);
				
			} else {

					m_bHighlighted = false;					
					SetTextForeColor (m_origTextcolour);			
					InvalidateControl (true);
				
			}
			

			//if (m_bHighlighted == true) {
				
				//if (m_bMouseover == false) {
					//m_bMouseover = true;
					//m_bHighlighted = false;
					//m_origTextcolour = g_textcolor;
					//SetTextForeColor (m_origTextcolour);			
					//InvalidateControl (true);
				//}
			//}
			
			if (m_bMouseover == false) {
				m_bMouseover = true;
				//m_origTextcolour = g_textcolor;
				//SetTextForeColor (RGB (255, 255, 255));			
				//InvalidateControl (true);
				
			}
		} else {
			if (m_bMouseover == true) {
				m_bMouseover = false;
				//SetTextForeColor (m_origTextcolour);
				//InvalidateControl (true);
			}
		}
	}
}

void UILabel::NotifyMouseDown ()
{
	if (m_bToggle == true) {
		m_bToggle = false;
	} else {
		m_bToggle = true;
	}
	m_bMousedown = true;
}

void UILabel::NotifyMouseUp ()
{
	m_bMousedown = false;
}