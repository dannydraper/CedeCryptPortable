#pragma once
#include "UIControl.h"

class UIBanner : public UIControl
{
	public:
		UIBanner ();
		~UIBanner ();
		void Paint (HDC hdc);
		void SetBitmapResources (unsigned int wResbase);
		void SetBitmapProperties (int rptxStart, int rptxWidth, int Width, int Height);
		void NotifyMouseUp ();
		void SetVisible (bool bVisible);
		void NotifyMouseMove (int iXPos, int iYPos);
	private:
		///////////////// METHOD DEFINITIONS ///////////////////

		// Banner bitmap properties
		// The repeating properties. Defines the xStart position and Width of the section of the bitmap
		// which can be repeated if the desired size is above the base size.
		int m_rptxStart;
		int m_rptxWidth;
		bool m_bVisible;
};
