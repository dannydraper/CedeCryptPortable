#pragma once
#include "UIControl.h"

class UILabel : public UIControl
{
	public:
		UILabel ();
		~UILabel ();
		void Paint (HDC hdc);
		void SetVisible (bool bVisible);
		unsigned long GetSize ();

		void NotifyMouseMove (int iXPos, int iYPos);
		void NotifyMouseDown ();
		void NotifyMouseUp ();
	private:
		///////////////// METHOD DEFINITIONS ///////////////////
		bool m_bVisible;
		bool m_bMouseover;
		bool m_bMousedown;
		bool m_bHighlighted;
		bool m_bToggle;
		COLORREF m_origTextcolour;
};
