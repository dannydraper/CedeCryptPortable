#include "MenuHandler.h"

MenuHandler::MenuHandler ()
{

}

MenuHandler::~MenuHandler ()
{
	
}

void MenuHandler::SetDiagnostics (Diagnostics *pdiag)
{
	m_pdiag = pdiag;
}

void MenuHandler::CreateMainMenu (HWND hwndParent, bool bRecovery)
{
	HMENU hMenu, hSubMenu, hSubMenu2, hSubMenu3, hSubMenu4;
	
	hMenu = CreateMenu ();
		
	hSubMenu = CreatePopupMenu ();
	if (bRecovery == true) {
		AppendMenu (hSubMenu, MF_STRING, IDM_FILE_RECOVER, "R&ecover");
	}

	
	AppendMenu (hSubMenu, MF_STRING, IDM_ENTERLICENSE, "Enter license key...");
	AppendMenu (hSubMenu, MF_STRING, IDM_DEACTIVATELICENSE, "Deactivate license");
	//AppendMenu (hSubMenu, MF_STRING, IDM_FILE_TEST, "T&est");
	//AppendMenu (hSubMenu, MF_STRING, IDM_CHECKUPDATES, "Check for updates...");
	//AppendMenu (hSubMenu, MF_STRING, IDM_FILE_PDIR, "Protected Folders");
	AppendMenu (hSubMenu, MF_STRING, IDM_FILE_EXIT, "E&xit");
	AppendMenu (hMenu, MF_STRING | MF_POPUP, (UINT) hSubMenu, "&File");

	//hSubMenu2 = CreatePopupMenu ();
	//AppendMenu (hSubMenu2, MF_STRING, IDM_TOOLS_OPTIONS, "O&ptions");
	//AppendMenu (hMenu, MF_STRING | MF_POPUP, (UINT) hSubMenu2, "&Tools");
	
	hSubMenu4 = CreatePopupMenu ();
	AppendMenu (hSubMenu4, MF_STRING, IDM_HELP_ABOUT, "&About");
	AppendMenu (hMenu, MF_STRING | MF_POPUP, (UINT) hSubMenu4, "&Help");

	SetMenu (hwndParent, hMenu);
}

