#pragma once
#include <windows.h>
#include <commctrl.h>
#include "resource.h"
#include "Diagnostics.h"
#include "UIHandler.h"

class UIToolbar {
	
	public:
		UIToolbar ();
		~UIToolbar ();

		void SetDiagnostics (Diagnostics *pdiag);
		void CreateToolbar (UIHandler *phandler, HWND hwndParent);


	private:
	
		HWND m_hwnd;


		UIPicButton m_btnParent;
		UIPicButton m_btnDelete;
		UIPicButton m_btnIconview;
		UIPicButton m_btnListview;
		UIPicButton m_btnDetailview;
		UIPicButton m_btnAddfiles;
		UIPicButton m_btnExtractfiles;

		// Diagnostics window pointer
		Diagnostics *m_pdiag;

};