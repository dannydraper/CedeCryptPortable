#pragma once
#include <windows.h>
#include <commctrl.h>
#include "resource.h"
#include "Diagnostics.h"

class MenuHandler {
	
	public:
		MenuHandler ();
		~MenuHandler ();
		void CreateMainMenu (HWND hwndParent, bool bRecovery);		
		void SetDiagnostics (Diagnostics *pdiag);		
	private:
		// Diagnostics window pointer
		Diagnostics *m_pdiag;
};