// This work is dedicated to the Lord our God. King of Heaven, Lord of Heaven's Armies.

#include "ToolsOptionsWindow.h"

ToolsOptionsWindow::ToolsOptionsWindow ()
{
	m_bUseDiagnostics = true;
	ZeroMemory (m_szClassname, SIZE_STRING);
	m_ID = 0;
	m_binitialised = false;

	m_bpfolderspromptpowerup = false;
	m_bpfolderspromptpowerdown = false;
	m_buseautoupdate = false;
}

ToolsOptionsWindow::~ToolsOptionsWindow ()
{

}

void ToolsOptionsWindow::SetDiagnostics (Diagnostics *pdiag)
{
	m_pdiag = pdiag;

}

void ToolsOptionsWindow::Initialise (HWND hWnd, unsigned int uID)
{	
	// Make the ID global
	m_ID = uID;
	m_parenthwnd = hWnd;
	//m_hwnd = hWnd;

	// Temporary string value for uID
	char szID[SIZE_STRING];
	ZeroMemory (szID, SIZE_STRING);

	SetParentHWND (hWnd);
	SetBgColor (RGB (236, 233, 216));
	SetCaption (TEXT ("CedeCrypt Options"));
	SetWindowStyle (FS_STYLESTANDARD);

	// Create the class name
	strcat_s (m_szClassname, SIZE_STRING, "CRYPTOptionsWindowClass");
	sprintf_s (szID, SIZE_STRING, "%d", uID);
	strcat_s (m_szClassname, SIZE_STRING, szID);

	//if (m_binitialised == false ) {
		CreateAppWindow (m_szClassname, 70, 0, 550, 500, true);
		m_uihandler.SetWindowProperties (0, 0, 300, 0, RGB (230, 230, 240));
		m_binitialised = true;
	//}

	SetWindowPosition (FS_CENTER);
	Show ();
}

void ToolsOptionsWindow::OnDestroy (HWND hWnd)
{			
	//Hide ();
}

void ToolsOptionsWindow::OnClose (HWND hWnd)
{
	Hide ();
}

void ToolsOptionsWindow::OnCreate (HWND hWnd)
{				
	// Make this window handle global
	m_hwnd = hWnd;
	g_hWnd = hWnd; // Need to do this otherwise the control creation wrapper functions don't work.

	HFONT hfDefault = (HFONT) GetStockObject (DEFAULT_GUI_FONT);

	// The status list box
	m_lstcategory = CreateWindow ("listbox", NULL, WS_CHILD | WS_BORDER | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | LBS_NOTIFY, 10, 10, 100, 480, hWnd, (HMENU) ID_LSTOPTIONCATEGORY, GetModuleHandle (NULL), NULL);
	SendMessage (m_lstcategory, WM_SETFONT, (WPARAM) hfDefault, MAKELPARAM (FALSE, 0));

	SendMessage (m_lstcategory, LB_SETITEMHEIGHT, 0, 32);

	CreateGeneralPanel ();
	CreateAlgorithmsPanel ();

	SetAlgorithmsPanelVisible (false);

	//ID_OPTWINDOWCLOSE
	m_btnclose = CreateButton ("Close", 455, 440, 70, 23, ID_OPTWINDOWCLOSE);

	// Add the categories to the list box
	char szGeneral[SIZE_NAME];
	ZeroMemory (szGeneral, SIZE_NAME);
	strcpy_s (szGeneral, SIZE_NAME, "General");

	SendMessage (m_lstcategory, LB_ADDSTRING, 0, (LPARAM) szGeneral);

	char szAlgorithms[SIZE_NAME];
	ZeroMemory (szAlgorithms, SIZE_NAME);
	strcpy_s (szAlgorithms, SIZE_NAME, "Algorithms");

	SendMessage (m_lstcategory, LB_ADDSTRING, 0, (LPARAM) szAlgorithms);

	// Read the registry settings, and set the control values
	ReadRegistrySettings ();
}

void ToolsOptionsWindow::SetAlgorithm (unsigned int iAlg)
{

	/*
	#define ALG_CYBERCEDE					100
	#define ALG_AES256						101
	#define ALG_AES512						102
	#define ALG_DES							103
	#define ALG_3DES		
	*/

	if (iAlg == ALG_CYBERCEDE) {
		SetDlgItemText (m_hwnd, ID_LBLCURRENTALG, "Current Selected Algorithm: CyberCede");
		m_registry.WriteStringSetting (SELECTEDALGORITHM, "Cybercede");

		SetCheck (m_algoptcybercede, true);
		SetCheck (m_algoptaes256, false);
		SetCheck (m_algopt3des, false);
		SetCheck (m_algoptdes, false);

	}

	if (iAlg == ALG_AES256) {
		SetDlgItemText (m_hwnd, ID_LBLCURRENTALG, "Current Selected Algorithm: AES 256");
		m_registry.WriteStringSetting (SELECTEDALGORITHM, "AES256");		

		SetCheck (m_algoptcybercede, false);
		SetCheck (m_algoptaes256, true);
		SetCheck (m_algopt3des, false);
		SetCheck (m_algoptdes, false);
	}

	if (iAlg == ALG_3DES) {
		SetDlgItemText (m_hwnd, ID_LBLCURRENTALG, "Current Selected Algorithm: 3DES");
		m_registry.WriteStringSetting (SELECTEDALGORITHM, "3DES");		

		SetCheck (m_algoptcybercede, false);
		SetCheck (m_algoptaes256, false);
		SetCheck (m_algopt3des, true);
		SetCheck (m_algoptdes, false);
	}

	if (iAlg == ALG_DES) {
		SetDlgItemText (m_hwnd, ID_LBLCURRENTALG, "Current Selected Algorithm: DES");
		m_registry.WriteStringSetting (SELECTEDALGORITHM, "DES");		

		SetCheck (m_algoptcybercede, false);
		SetCheck (m_algoptaes256, false);
		SetCheck (m_algopt3des, false);
		SetCheck (m_algoptdes, true);
	}
}

void ToolsOptionsWindow::CreateGeneralPanel ()
{
	m_pfoldersframe = CreateGroupBox ("Protected Folders", 130, 15, 390, 103, ID_LBLSTATIC);
	
	m_pfoldersinfo1 = CreateLabel ("When my computer starts up / wakes up:", 140, 35, 300, 20, ID_LBLSTATIC);
	m_pfolderspromptpowerup = CreateCheckBox ("Ask me before Decrypting protected folders", 155, 53, 300, 20, ID_CHKPFOLDERPROMPTPUP);
	m_pfoldersinfo2 = CreateLabel ("When my computer shuts down / sleeps:", 140, 75, 300, 20, ID_LBLSTATIC);
	m_pfolderspromptpowerdown = CreateCheckBox ("Ask me before Encrypting protected folders", 155, 92, 300, 20, ID_CHKPFOLDERPROMPTPDN);

	m_ptextencframe = CreateGroupBox ("Universal Text Encryption", 130, 130, 390, 120, ID_LBLSTATIC);
	
	m_lblenc = CreateLabel ("Encrypt Hotkey", 170, 164, 100, 20, ID_LBLSTATIC);
	m_chkencctrl = CreateCheckBox ("Ctrl", 280, 163, 40, 20, ID_UTEBTNHOTENCCTRL);
	m_chkencalt = CreateCheckBox ("Alt", 340, 163, 40, 20, ID_UTEBTNHOTENCALT);
	m_txtenckey = CreateTextBox (400, 163, 40, 20, ID_UTETXTHOTENC);

	m_lbldec = CreateLabel ("Decrypt Hotkey", 170, 194, 100, 20, ID_LBLSTATIC);
	m_chkdecctrl = CreateCheckBox ("Ctrl", 280, 193, 40, 20, ID_UTEBTNHOTDECCTRL);
	m_chkdecalt = CreateCheckBox ("Alt", 340, 193, 40, 20, ID_UTEBTNHOTDECALT);
	m_txtdeckey = CreateTextBox (400, 193, 40, 20, ID_UTETXTHOTDEC);

	m_lblinfo = CreateLabel ("NOTE: Hot Key settings will take effect after next reboot.", 170, 227, 300, 20, ID_LBLSTATIC);

	m_autoupdateframe = CreateGroupBox ("Automatic Updates", 130, 265, 390, 103, ID_LBLSTATIC);
	m_chkautoupdate = CreateCheckBox ("Automatically check online for a newer version of CedeCrypt", 155, 310, 300, 20, ID_CHKAUTOUPDATECHECK);

	ShowWindow (m_chkencalt, SW_HIDE);
	ShowWindow (m_chkdecalt, SW_HIDE);
}

void ToolsOptionsWindow::CreateAlgorithmsPanel ()
{
	m_algframe = CreateGroupBox ("Encryption Algorithms", 130, 15, 390, 403, ID_LBLSTATIC);

	m_algimage.SetBitmapResources (IDB_ALGORITHMSIMAGE);
	m_algimage.SetBitmapProperties (0, 0, 345, 254);
	m_algimage.SetProperties (m_hwnd, CID_HEADER, 170, 75, 345, 254);
	m_uihandler.AddDirectControl (&m_algimage);

	/*

	#define ID_OPTALGCYBERCEDE				943
	#define ID_OPTALGAES256					944
	#define ID_OPTALG3DES					945
	#define ID_OPTALGDES					946
	ID_LBLCURRENTALG
	*/

	m_algoptcybercede = CreateRadioButton ("", 145, 95, 15, 15, ID_OPTALGCYBERCEDE);
	m_algoptaes256 = CreateRadioButton ("", 145, 160, 15, 15, ID_OPTALGAES256);
	m_algopt3des = CreateRadioButton ("", 145, 225, 15, 15, ID_OPTALG3DES);
	m_algoptdes = CreateRadioButton ("", 145, 290, 15, 15, ID_OPTALGDES);

	m_lblcurrentalg = CreateLabel ("Current Selected Algorithm: ", 173, 45, 290, 19, ID_LBLCURRENTALG);
}

void ToolsOptionsWindow::SetAlgorithmsPanelVisible (bool bVisible)
{
	if (bVisible == true) {
		ShowWindow (m_algframe, SW_SHOW);
		ShowWindow (m_algoptcybercede, SW_SHOW);
		ShowWindow (m_algoptaes256, SW_SHOW);
		ShowWindow (m_algopt3des, SW_SHOW);
		ShowWindow (m_algoptdes, SW_SHOW);
		ShowWindow (m_lblcurrentalg, SW_SHOW);

		m_algimage.SetVisible (true);
	} else {
		ShowWindow (m_algframe, SW_HIDE);
		ShowWindow (m_algoptcybercede, SW_HIDE);
		ShowWindow (m_algoptaes256, SW_HIDE);
		ShowWindow (m_algopt3des, SW_HIDE);
		ShowWindow (m_algoptdes, SW_HIDE);
		ShowWindow (m_lblcurrentalg, SW_HIDE);

		m_algimage.SetVisible (false);
	}
}

void ToolsOptionsWindow::SetGeneralPanelVisible (bool bVisible)
{
	if (bVisible == true) {
		ShowWindow (m_pfoldersframe, SW_SHOW);
		ShowWindow (m_pfoldersinfo1, SW_SHOW);
		ShowWindow (m_pfolderspromptpowerup, SW_SHOW);
		ShowWindow (m_pfoldersinfo2, SW_SHOW);
		ShowWindow (m_pfolderspromptpowerdown, SW_SHOW);
		ShowWindow (m_ptextencframe, SW_SHOW);
		ShowWindow (m_lblenc, SW_SHOW);
		ShowWindow (m_chkencctrl, SW_SHOW);
		ShowWindow (m_chkencalt, SW_HIDE); // always hidden
		ShowWindow (m_txtenckey, SW_SHOW);
		ShowWindow (m_lbldec, SW_SHOW);
		ShowWindow (m_chkdecctrl, SW_SHOW);
		ShowWindow (m_chkdecalt, SW_HIDE); // always hidden
		ShowWindow (m_txtdeckey, SW_SHOW);
		ShowWindow (m_lblinfo, SW_SHOW);
		ShowWindow (m_autoupdateframe, SW_SHOW);
		ShowWindow (m_chkautoupdate, SW_SHOW);
	} else {
		ShowWindow (m_pfoldersframe, SW_HIDE);
		ShowWindow (m_pfoldersinfo1, SW_HIDE);
		ShowWindow (m_pfolderspromptpowerup, SW_HIDE);
		ShowWindow (m_pfoldersinfo2, SW_HIDE);
		ShowWindow (m_pfolderspromptpowerdown, SW_HIDE);
		ShowWindow (m_ptextencframe, SW_HIDE);
		ShowWindow (m_lblenc, SW_HIDE);
		ShowWindow (m_chkencctrl, SW_HIDE);
		ShowWindow (m_chkencalt, SW_HIDE);
		ShowWindow (m_txtenckey, SW_HIDE);
		ShowWindow (m_lbldec, SW_HIDE);
		ShowWindow (m_chkdecctrl, SW_HIDE);
		ShowWindow (m_chkdecalt, SW_HIDE);
		ShowWindow (m_txtdeckey, SW_HIDE);
		ShowWindow (m_lblinfo, SW_HIDE);
		ShowWindow (m_autoupdateframe, SW_HIDE);
		ShowWindow (m_chkautoupdate, SW_HIDE);
	}
	
}

void ToolsOptionsWindow::ActivatePanel (char *szPanel)
{
	if (strcmp (szPanel, "General") == 0) {
		SetGeneralPanelVisible (true);
		SetAlgorithmsPanelVisible (false);
	}

	if (strcmp (szPanel, "Algorithms") == 0) {
		SetGeneralPanelVisible (false);
		SetAlgorithmsPanelVisible (true);
	}
}

void ToolsOptionsWindow::ReadRegistrySettings ()
{
	if (m_registry.DoesSettingExist (PROTECTEDFOLDERSPROMPTPUP) == true) {
		if (strcmp (m_registry.ReadStringSetting (PROTECTEDFOLDERSPROMPTPUP), "yes") == 0) {
			SetCheck (m_pfolderspromptpowerup, true);
			m_bpfolderspromptpowerup = true;
		}
	}

	if (m_registry.DoesSettingExist (PROTECTEDFOLDERSPROMPTPDN) == true) {
		if (strcmp (m_registry.ReadStringSetting (PROTECTEDFOLDERSPROMPTPDN), "yes") == 0) {
			SetCheck (m_pfolderspromptpowerdown, true);
			m_bpfolderspromptpowerdown = true;
		}
	}

	// UNIVERSAL TEXT ENCRYPTION SETTINGS - ENCRYPTION
	// Use CTRL tickbox
	if (m_registry.DoesSettingExist (UTEENCRYPTUSECTRL) == true) {
		if (strcmp (m_registry.ReadStringSetting (UTEENCRYPTUSECTRL), "yes") == 0) {
			SetCheck (m_chkencctrl, true);
			m_buseencctrl = true;
		} else {
			SetCheck (m_chkencctrl, false);
			m_buseencctrl = false;
		}
	} else {
		SetCheck (m_chkencctrl, true); // Default Value
		m_buseencctrl = true;
	}

	// Use ALT tickbox
	if (m_registry.DoesSettingExist (UTEENCRYPTUSEALT) == true) {
		if (strcmp (m_registry.ReadStringSetting (UTEENCRYPTUSEALT), "yes") == 0) {
			SetCheck (m_chkencalt, true);
			m_buseencalt = true;
		} else {
			SetCheck (m_chkencalt, false);
			m_buseencalt = false;
		}
	} else {
		SetCheck (m_chkencalt, false); // Default Value
	}

	// Encrypt HotKey
	if (m_registry.DoesSettingExist (UTEENCRYPTHOTKEY) == true) {
		SetDlgItemText (m_hwnd, ID_UTETXTHOTENC, m_registry.ReadStringSetting (UTEENCRYPTHOTKEY));
	} else {
		SetDlgItemText (m_hwnd, ID_UTETXTHOTENC, "E");
	}

	// UNIVERSAL TEXT ENCRYPTION SETTINGS - DECRYPTION
	// Use CTRL tickbox
	if (m_registry.DoesSettingExist (UTEDECRYPTUSECTRL) == true) {
		if (strcmp (m_registry.ReadStringSetting (UTEDECRYPTUSECTRL), "yes") == 0) {
			SetCheck (m_chkdecctrl, true);
			m_busedecctrl = true;
		} else {
			SetCheck (m_chkdecctrl, false);
			m_busedecctrl = false;
		}
	} else {
		SetCheck (m_chkdecctrl, true); // Default Value
		m_busedecctrl = true;
	}

	// Use ALT tickbox
	if (m_registry.DoesSettingExist (UTEDECRYPTUSEALT) == true) {
		if (strcmp (m_registry.ReadStringSetting (UTEDECRYPTUSEALT), "yes") == 0) {
			SetCheck (m_chkdecalt, true);
			m_busedecalt = true;
		} else {
			SetCheck (m_chkdecalt, false);
			m_busedecalt = false;
		}
	} else {
		SetCheck (m_chkdecalt, false); // Default Value
	}

	// Decrypt HotKey
	if (m_registry.DoesSettingExist (UTEDECRYPTHOTKEY) == true) {
		SetDlgItemText (m_hwnd, ID_UTETXTHOTDEC, m_registry.ReadStringSetting (UTEDECRYPTHOTKEY));
	} else {
		SetDlgItemText (m_hwnd, ID_UTETXTHOTDEC, "D");
	}

	// Automatic Update setting
	if (m_registry.DoesSettingExist (AUTOUPDATECHECK) == true) {
		if (strcmp (m_registry.ReadStringSetting (AUTOUPDATECHECK), "yes") == 0) {
			SetCheck (m_chkautoupdate, true);
			m_buseautoupdate = true;
		} else {
			SetCheck (m_chkautoupdate, false);
			m_buseautoupdate = false;			
		}
	} else {
		SetCheck (m_chkautoupdate, true); // Default value
		m_buseautoupdate = true; // Default value
	}

	// CURRENT SELECTED ENCRYPTION ALGORITHM
	if (m_registry.DoesSettingExist (SELECTEDALGORITHM) == true) {

		if (strcmp (m_registry.ReadStringSetting (SELECTEDALGORITHM), "Cybercede") == 0) {
			SetAlgorithm (ALG_CYBERCEDE);
		}

		if (strcmp (m_registry.ReadStringSetting (SELECTEDALGORITHM), "AES256") == 0) {
			SetAlgorithm (ALG_AES256);
		}

		if (strcmp (m_registry.ReadStringSetting (SELECTEDALGORITHM), "3DES") == 0) {
			SetAlgorithm (ALG_3DES);
		}

		if (strcmp (m_registry.ReadStringSetting (SELECTEDALGORITHM), "DES") == 0) {
			SetAlgorithm (ALG_DES);
		}

	} else {
		SetAlgorithm (ALG_CYBERCEDE);
	}


}

unsigned int ToolsOptionsWindow::GetSelectedAlgorithm ()
{
	if (m_registry.DoesSettingExist (SELECTEDALGORITHM) == true) {

		if (strcmp (m_registry.ReadStringSetting (SELECTEDALGORITHM), "Cybercede") == 0) {
			return ALG_CYBERCEDE;
		}

		if (strcmp (m_registry.ReadStringSetting (SELECTEDALGORITHM), "AES256") == 0) {
			return ALG_AES256;
		}

		if (strcmp (m_registry.ReadStringSetting (SELECTEDALGORITHM), "3DES") == 0) {
			return ALG_3DES;
		}

		if (strcmp (m_registry.ReadStringSetting (SELECTEDALGORITHM), "DES") == 0) {
			return ALG_DES;
		}

		// If the registry values didn't match any of our criteria then default to CyberCede;
		return ALG_CYBERCEDE;
	} else {
		return ALG_CYBERCEDE;
	}
}

void ToolsOptionsWindow::OutputInt (LPCSTR lpszText, int iValue)
{
	if (m_bUseDiagnostics == true) {
		m_pdiag->OutputInt (lpszText, iValue);
	}
}

void ToolsOptionsWindow::OutputText (LPCSTR lpszText)
{
	if (m_bUseDiagnostics == true) {
		m_pdiag->OutputText (lpszText);
	}
}

void ToolsOptionsWindow::OutputText (LPCSTR lpszName, LPCSTR lpszValue)
{
	if (m_bUseDiagnostics == true) {
		m_pdiag->OutputText (lpszName, lpszValue);
	}
}

bool ToolsOptionsWindow::SaveHotKeySettings ()
{
	char szEncHotkey[SIZE_NAME];
	ZeroMemory (szEncHotkey, SIZE_NAME);
	GetDlgItemText (m_hwnd, ID_UTETXTHOTENC, szEncHotkey, SIZE_NAME);

	char szDecHotkey[SIZE_NAME];
	ZeroMemory (szDecHotkey, SIZE_NAME);
	GetDlgItemText (m_hwnd, ID_UTETXTHOTDEC, szDecHotkey, SIZE_NAME);

	if (strlen (szEncHotkey) != 1) {
		MessageBox (NULL, "Please enter a valid Encryption Hotkey.", "Hotkey Invalid", MB_OK | MB_ICONEXCLAMATION);
		return false;
	} else {
		_strupr (szEncHotkey);
		m_registry.WriteStringSetting (UTEENCRYPTHOTKEY, szEncHotkey);
	}

	if (strlen (szDecHotkey) != 1) {
		MessageBox (NULL, "Please enter a valid Decryption Hotkey.", "Hotkey Invalid", MB_OK | MB_ICONEXCLAMATION);
		return false;
	} else {
		_strupr (szDecHotkey);
		m_registry.WriteStringSetting (UTEDECRYPTHOTKEY, szDecHotkey);
	}

	return true;
}

void ToolsOptionsWindow::OnCommand (HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD (wParam)) {
		case ID_LSTOPTIONCATEGORY:
		{
			switch (HIWORD (wParam))
			{
				case LBN_SELCHANGE:
				{					
					int iIndex = SendMessage (m_lstcategory, LB_GETCURSEL, 0, 0);
					char selItem[SIZE_STRING];
					ZeroMemory (selItem, SIZE_STRING);

					if (iIndex != LB_ERR) {
						SendMessage (m_lstcategory,LB_GETTEXT, iIndex, (LPARAM) selItem);

						ActivatePanel (selItem);
					}
				}
				break;
			}
			break;
		}
		break;
		case ID_OPTWINDOWCLOSE:
		{
			if (SaveHotKeySettings () == true) {
				Hide ();
			}
		}
		break;

		case ID_CHKPFOLDERPROMPTPUP:
		{
			if (m_bpfolderspromptpowerup == false) {
				SetCheck (m_pfolderspromptpowerup, true);
				m_bpfolderspromptpowerup = true;
				m_registry.WriteStringSetting (PROTECTEDFOLDERSPROMPTPUP, "yes");
			} else {
				m_registry.WriteStringSetting (PROTECTEDFOLDERSPROMPTPUP, "no");
				SetCheck (m_pfolderspromptpowerup, false);
				m_bpfolderspromptpowerup = false;
			}
		}
		break;

		case ID_CHKPFOLDERPROMPTPDN:
		{
			if (m_bpfolderspromptpowerdown == false) {
				SetCheck (m_pfolderspromptpowerdown, true);
				m_bpfolderspromptpowerdown = true;
				m_registry.WriteStringSetting (PROTECTEDFOLDERSPROMPTPDN, "yes");
			} else {
				m_registry.WriteStringSetting (PROTECTEDFOLDERSPROMPTPDN, "no");
				SetCheck (m_pfolderspromptpowerdown, false);
				m_bpfolderspromptpowerdown = false;
			}
		}
		break;

		// Encryption CTRL key
		case ID_UTEBTNHOTENCCTRL:
		{
			if (m_buseencctrl == false) {
				SetCheck (m_chkencctrl, true);
				m_buseencctrl = true;
				m_registry.WriteStringSetting (UTEENCRYPTUSECTRL, "yes");

			} else {
				SetCheck (m_chkencctrl, false);
				m_buseencctrl = false;
				m_registry.WriteStringSetting (UTEENCRYPTUSECTRL, "no");
			}
		}
		break;

		// Encryption ALT key
		case ID_UTEBTNHOTENCALT:
		{
			if (m_buseencalt == false) {
				SetCheck (m_chkencalt, true);
				m_buseencalt = true;
				m_registry.WriteStringSetting (UTEENCRYPTUSEALT, "yes");

			} else {				
				SetCheck (m_chkencalt, false);
				m_buseencalt = false;
				m_registry.WriteStringSetting (UTEENCRYPTUSEALT, "no");
			}
		}
		break;

		// Decryption CTRL key
		case ID_UTEBTNHOTDECCTRL:
		{
			if (m_busedecctrl == false) {
				SetCheck (m_chkdecctrl, true);
				m_busedecctrl = true;
				m_registry.WriteStringSetting (UTEDECRYPTUSECTRL, "yes");

			} else {
				SetCheck (m_chkdecctrl, false);
				m_busedecctrl = false;
				m_registry.WriteStringSetting (UTEDECRYPTUSECTRL, "no");
			}
		}
		break;

		// Decryption ALT key
		case ID_UTEBTNHOTDECALT:
		{
			if (m_busedecalt == false) {
				SetCheck (m_chkdecalt, true);
				m_busedecalt = true;
				m_registry.WriteStringSetting (UTEDECRYPTUSEALT, "yes");

			} else {				
				SetCheck (m_chkdecalt, false);
				m_busedecalt = false;
				m_registry.WriteStringSetting (UTEDECRYPTUSEALT, "no");
			}
		}
		break;

		case ID_CHKAUTOUPDATECHECK:
		{
			//		SetCheck (m_chkautoupdate, true); // Default value
			//		m_buseautoupdate = true; // Default value
			if (m_buseautoupdate == true) {
				SetCheck (m_chkautoupdate, false);
				m_buseautoupdate = false;
				m_registry.WriteStringSetting (AUTOUPDATECHECK, "no");
			} else {
				SetCheck (m_chkautoupdate, true);
				m_buseautoupdate = true;
				m_registry.WriteStringSetting (AUTOUPDATECHECK, "yes");
			}
		}
		break;

		case ID_OPTALGCYBERCEDE:
		{

			SetAlgorithm (ALG_CYBERCEDE);
		}
		break;
		case ID_OPTALGAES256:
		{
			SetAlgorithm (ALG_AES256);
		}
		break;
		case ID_OPTALG3DES:
		{			
			SetAlgorithm (ALG_3DES);
		}
		break;
		case ID_OPTALGDES:
		{		
			SetAlgorithm (ALG_DES);
		}
		break;
	}
}

void ToolsOptionsWindow::OnUIScroll (HWND hWnd, WPARAM wParam, LPARAM lParam)
{

}

void ToolsOptionsWindow::OnUICommand (HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
		
	}
}

void ToolsOptionsWindow::OnTimer (WPARAM wParam)
{	
	m_uihandler.NotifyTimer (wParam);	
}

void ToolsOptionsWindow::OnPaint (HWND hWnd)
{
	m_uihandler.PaintControls (hWnd);	
}

void ToolsOptionsWindow::OnMouseMove (HWND hWnd, int mouseXPos, int mouseYPos)
{	
	m_uihandler.NotifyMouseMove (mouseXPos, mouseYPos);
}

void ToolsOptionsWindow::OnLButtonDown (HWND hWnd)
{
	m_uihandler.NotifyMouseDown ();
}

void ToolsOptionsWindow::OnLButtonUp (HWND hWnd)
{
	m_uihandler.NotifyMouseUp ();
}