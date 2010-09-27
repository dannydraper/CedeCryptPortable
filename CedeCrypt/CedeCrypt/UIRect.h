#pragma once
#include "UIControl.h"

class UIRect : public UIControl
{
	public:
		UIRect ();
		~UIRect ();
		void Paint (HDC hdc);
		void SetVisible (bool bVisible);
		unsigned long GetSize ();

		//void NotifyMouseMove (int iXPos, int iYPos);
	private:
		///////////////// METHOD DEFINITIONS ///////////////////
		bool m_bVisible;
		bool m_bMouseover;

};
