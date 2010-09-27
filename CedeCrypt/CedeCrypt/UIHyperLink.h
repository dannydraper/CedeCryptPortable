#pragma once
#include "UIControl.h"

class UIHyperLink : public UIControl
{
	public:
		UIHyperLink ();
		~UIHyperLink ();
		void Paint (HDC hdc);

		unsigned long GetSize ();
		void SetVisible (bool bVisible);
		void SetCommandMode (bool bCommand);
		void SetHyperlinkProperties (TCHAR *szFontName, int FontSize, char *szCaption, char *szTarget);

		void NotifyMouseMove (int iXPos, int iYPos);
		void NotifyMouseDown ();
		void NotifyMouseUp ();

	private:
		///////////////// METHOD DEFINITIONS ///////////////////
		bool m_bMouseover;
		int *m_szTarget;
		bool m_bVisible;
		bool m_bUseCommand;

		HCURSOR m_Handcursor;
		HCURSOR m_Prevcursor;

};