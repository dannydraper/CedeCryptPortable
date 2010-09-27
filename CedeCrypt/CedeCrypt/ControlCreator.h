#pragma once
#include "UIHandler.h"
#include "UIPicButton.h"

class ControlCreator
{
	public:
		ControlCreator ();
		~ControlCreator ();		
		void SetUIHandler (UIHandler *uihandler);
	private:
		///////////////// METHOD DEFINITIONS ///////////////////
		UIHandler *m_puihandler;
};