#include "EncryptWindowEx.h"

EncryptWindowEx::EncryptWindowEx()
{

}

EncryptWindowEx::~EncryptWindowEx()
{

}

void EncryptWindowEx::SetDiagnostics (Diagnostics *pdiag)
{
	m_pdiag = pdiag;
	
}

void EncryptWindowEx::SetHWND (HWND hWnd)
{
	m_pwndenc = (EncryptWindowEx *) this;
	m_hwnd = hWnd;
}

void EncryptWindowEx::SetPassword (char *szPassword)
{
	ZeroMemory (m_szPassword, SIZE_STRING);
	strcpy_s (m_szPassword, SIZE_STRING, szPassword);
}

void EncryptWindowEx::GetPathOnly (char *szInpath, char *szOutpath, char *szOutfile, char *szSep)
{
	char szCurchar[SIZE_NAME];
	int seploc = 0;

	for (int c=strlen(szInpath);c>0;c--) {
		
		ZeroMemory (szCurchar, SIZE_NAME);
		strncpy_s (szCurchar, SIZE_NAME, szInpath+c, 1);

		if (strcmp (szCurchar, szSep) == 0) {
			seploc = c+1;
			break;
		}
	}

	strncpy_s (szOutpath, SIZE_STRING, szInpath, seploc);
	strncpy_s (szOutfile, SIZE_STRING, szInpath+seploc, strlen (szInpath)-seploc);
}

void EncryptWindowEx::StripStartPath (char *szStartpath, char *szFullpath, char *szOutpath)
{

	char szTemp[SIZE_STRING];
	ZeroMemory (szTemp, SIZE_STRING);

	strncpy_s (szTemp, SIZE_STRING, szFullpath, strlen (szStartpath));



	if (strcmp (szStartpath, szTemp) == 0) {
		ZeroMemory (szOutpath, SIZE_STRING);
		strcpy_s (szOutpath, SIZE_STRING, szFullpath+strlen (szStartpath));
	}

}

void EncryptWindowEx::ClearFileInfo ()
{
	m_dlFileinfolist.Clear ();
}

DynList *EncryptWindowEx::GetFileInfoList ()
{
	return (DynList *) &m_dlFileinfolist;
}

void EncryptWindowEx::AddFileInfo (char *szFilepath, char *szRootdir, unsigned long lFilesize, bool IsDir)
{
	SingleFileInfo fiCurrent;

	ZeroMemory (fiCurrent.szFilepath, SIZE_STRING);
	strcpy_s (fiCurrent.szFilepath, SIZE_STRING, szFilepath);

	ZeroMemory (fiCurrent.szRootdir, SIZE_STRING);
	strcpy_s (fiCurrent.szRootdir, SIZE_STRING, szRootdir);

	fiCurrent.lFilesize = lFilesize;

	fiCurrent.bIsDir = IsDir;

	m_dlFileinfolist.AddItem (&fiCurrent, sizeof (SingleFileInfo), false);

	//m_dlFileinfolist.AddItem ((SingleFileInfo *) pinfo, sizeof (SingleFileInfo), false);
}

void EncryptWindowEx::AddFileInfo (char *szFilepath, unsigned long lFilesize, bool IsDir)
{
	SingleFileInfo fiCurrent;

	ZeroMemory (fiCurrent.szFilepath, SIZE_STRING);
	strcpy_s (fiCurrent.szFilepath, SIZE_STRING, szFilepath);

	fiCurrent.lFilesize = lFilesize;

	fiCurrent.bIsDir = IsDir;

	m_dlFileinfolist.AddItem (&fiCurrent, sizeof (SingleFileInfo), false);
	//m_dlFileinfolist.AddItem ((SingleFileInfo *) pinfo, sizeof (SingleFileInfo), false);
}

unsigned long long EncryptWindowEx::GetTotalBytes ()
{
	int a = 0;
	unsigned long long ltotal = 0;
	SingleFileInfo *pinfo;

	for (a=0;a<m_dlFileinfolist.GetNumItems ();a++) {
		pinfo = (SingleFileInfo *) m_dlFileinfolist.GetItem (a);

		ltotal+=pinfo->lFilesize;
	}

	return ltotal;
}

void EncryptWindowEx::DoBlankSetup ()
{
	SingleFileInfo *pinfo;

	int a = 0;
	char szDestpath[SIZE_STRING];

	char szFileonly[SIZE_STRING];
	char szPathonly[SIZE_STRING];
	char szTemp[SIZE_STRING];

	char szTempdir[SIZE_STRING];
	ZeroMemory (szTempdir, SIZE_STRING);
	//strcpy_s (szTempdir, SIZE_STRING, "C:\\Temp\\TempEnc");
	GetEnvironmentVariable ("Temp", szTempdir, SIZE_STRING);
	strcat_s (szTempdir, SIZE_STRING, "\\CedeCrypt.tmp");

	for (a=0;a<m_dlFileinfolist.GetNumItems ();a++) {

		pinfo = (SingleFileInfo *) m_dlFileinfolist.GetItem (a);

		if (pinfo->bIsDir == true) {
			
			ZeroMemory (szDestpath, SIZE_STRING);
			strcpy_s (szDestpath, SIZE_STRING, szTempdir);
			strcat_s (szDestpath, SIZE_STRING, "\\");

			ZeroMemory (szFileonly, SIZE_STRING);
			ZeroMemory (szPathonly, SIZE_STRING);
			
			GetPathOnly (pinfo->szRootdir, szPathonly, szFileonly, "\\");

			ZeroMemory (szTemp, SIZE_STRING);
			StripStartPath (pinfo->szRootdir, pinfo->szFilepath, szTemp);

			strcat_s (szDestpath, SIZE_STRING, szFileonly);
			strcat_s (szDestpath, SIZE_STRING, szTemp);
			
			//m_pdiag->OutputText ("Encrypt Source: ", pinfo->szFilepath);
			m_pdiag->OutputText ("BlankFile Dest: ", szDestpath);

			m_enc.SetupBlankFile (szDestpath);
		} else {

			ZeroMemory (szDestpath, SIZE_STRING);
			strcpy_s (szDestpath, SIZE_STRING, szTempdir);
			strcat_s (szDestpath, SIZE_STRING, "\\");

			ZeroMemory (szFileonly, SIZE_STRING);
			ZeroMemory (szPathonly, SIZE_STRING);

			GetPathOnly (pinfo->szFilepath, szPathonly, szFileonly, "\\");
			
			strcat_s (szDestpath, SIZE_STRING, szFileonly);

			m_pdiag->OutputText ("BlankFile Dest: ", szDestpath);

			m_enc.SetupBlankFile (szDestpath);
		}
	}
}

DWORD WINAPI EncryptWindowEx::DecryptionProc (PVOID pParam)
{
	SingleFileInfo *pinfo;

	int a = 0;
	int errorcount = 0;
	char szDestpath[SIZE_STRING];

	char szFileonly[SIZE_STRING];
	char szPathonly[SIZE_STRING];
	char szTemp[SIZE_STRING];

	char szMsgfileonly[SIZE_STRING];
	char szMsgpathonly[SIZE_STRING];

	char szFilemessage[SIZE_STRING];

	char szTempdir[SIZE_STRING];
	ZeroMemory (szTempdir, SIZE_STRING);
	//strcpy_s (szTempdir, SIZE_STRING, "C:\\Temp\\TempEnc");
	GetEnvironmentVariable ("Temp", szTempdir, SIZE_STRING);
	strcat_s (szTempdir, SIZE_STRING, "\\CedeCrypt.tmp");

	unsigned long ltotalbytes = m_pwndenc->GetTotalBytes ();

	m_pwndenc->m_enc.SetProgressHWND (m_pwndenc->m_hwndprogress);
	m_pwndenc->m_enc.SetOutputHWND (m_pwndenc->m_pdiag->m_hwnddiaglist);
	m_pwndenc->m_enc.SetSpeedLabelHWND (m_pwndenc->m_dlghwnd);

	m_pwndenc->m_enc.ltotalbytesprocessed = 0;
	m_pwndenc->m_enc.m_lasttickcount = 0;
	m_pwndenc->m_enc.m_iaveragetrigger = 0;
	m_pwndenc->m_enc.m_addedspeed = 0;
	m_pwndenc->m_enc.ltotalbytes = ltotalbytes;
	m_pwndenc->m_enc.InitEncryptFile ();

	SetDlgItemText (m_pwndenc->m_dlghwnd, ID_LBLOPERATION, "Decrypting data using AES-256 bit cipher...");

	SendMessage(m_pwndenc->m_hwndprogress, PBM_SETRANGE, 0L, MAKELONG (0, 39500));
	
	m_pwndenc->m_pdiag->OutputInt ("ltotal bytes: ", ltotalbytes);

	for (a=0;a<m_pwndenc->m_dlFileinfolist.GetNumItems ();a++) {

		pinfo = (SingleFileInfo *) m_pwndenc->m_dlFileinfolist.GetItem (a);

		if (pinfo->bIsDir == true) {
			
			ZeroMemory (szDestpath, SIZE_STRING);
			strcpy_s (szDestpath, SIZE_STRING, szTempdir);
			strcat_s (szDestpath, SIZE_STRING, "\\");

			ZeroMemory (szFileonly, SIZE_STRING);
			ZeroMemory (szPathonly, SIZE_STRING);
			
			m_pwndenc->GetPathOnly (pinfo->szRootdir, szPathonly, szFileonly, "\\");

			ZeroMemory (szTemp, SIZE_STRING);
			m_pwndenc->StripStartPath (pinfo->szRootdir, pinfo->szFilepath, szTemp);

			strcat_s (szDestpath, SIZE_STRING, szFileonly);
			strcat_s (szDestpath, SIZE_STRING, szTemp);
			
			//m_pdiag->OutputText ("Encrypt Source: ", pinfo->szFilepath);
			//m_pdiag->OutputText ("BlankFile Dest: ", szDestpath);
			
			ZeroMemory (szMsgpathonly, SIZE_STRING);
			ZeroMemory (szMsgfileonly, SIZE_STRING);
			ZeroMemory (szFilemessage, SIZE_STRING);
			
			m_pwndenc->GetPathOnly (pinfo->szFilepath, szMsgpathonly, szMsgfileonly, "\\");

			strcpy_s (szFilemessage, SIZE_STRING, "Decrypting file: ");
			strcat_s (szFilemessage, SIZE_STRING, szMsgfileonly);
			SetDlgItemText (m_pwndenc->m_dlghwnd, ID_LBLFILE, szFilemessage);

			if (m_pwndenc->m_enc.EncryptFileEx3 (pinfo->szFilepath, szDestpath, m_pwndenc->m_szPassword, false) == false) {
				errorcount++;
			}
		} else {

			ZeroMemory (szDestpath, SIZE_STRING);
			strcpy_s (szDestpath, SIZE_STRING, szTempdir);
			strcat_s (szDestpath, SIZE_STRING, "\\");

			ZeroMemory (szFileonly, SIZE_STRING);
			ZeroMemory (szPathonly, SIZE_STRING);

			m_pwndenc->GetPathOnly (pinfo->szFilepath, szPathonly, szFileonly, "\\");
			
			strcat_s (szDestpath, SIZE_STRING, szFileonly);

			//m_pdiag->OutputText ("BlankFile Dest: ", szDestpath);

			ZeroMemory (szMsgpathonly, SIZE_STRING);
			ZeroMemory (szMsgfileonly, SIZE_STRING);
			ZeroMemory (szFilemessage, SIZE_STRING);
			
			m_pwndenc->GetPathOnly (pinfo->szFilepath, szMsgpathonly, szMsgfileonly, "\\");

			strcpy_s (szFilemessage, SIZE_STRING, "Decrypting file: ");
			strcat_s (szFilemessage, SIZE_STRING, szMsgfileonly);
			SetDlgItemText (m_pwndenc->m_dlghwnd, ID_LBLFILE, szFilemessage);

			if (m_pwndenc->m_enc.EncryptFileEx3 (pinfo->szFilepath, szDestpath, m_pwndenc->m_szPassword, false) == false) {
				errorcount++;
			}
		}

		//SendMessage (m_pwndenc->m_hwndprogress, PBM_SETPOS, a, 0L);
	}


	m_pwndenc->m_idecrypterrorcount = errorcount;

	PostMessage (m_pwndenc->m_dlghwnd, DECRYPT_COMPLETE, 0, 0);

	return 0;
}

void EncryptWindowEx::DoDecryptionThread ()
{
	// MULTI-THREADED FUNCTION
	// Spawns the first encryption thread.
	HANDLE hThread;
	DWORD dwThreadID;

	hThread = CreateThread (NULL, 0, DecryptionProc, NULL, 0, &dwThreadID);
}


DWORD WINAPI EncryptWindowEx::EncryptionProc (PVOID pParam)
{

	SingleFileInfo *pinfo;

	int a = 0;
	char szDestpath[SIZE_STRING];

	char szFileonly[SIZE_STRING];
	char szPathonly[SIZE_STRING];
	char szTemp[SIZE_STRING];

	char szMsgfileonly[SIZE_STRING];
	char szMsgpathonly[SIZE_STRING];

	char szFilemessage[SIZE_STRING];

	unsigned long long ltotalbytes = m_pwndenc->GetTotalBytes ();

	m_pwndenc->m_enc.SetProgressHWND (m_pwndenc->m_hwndprogress);
	m_pwndenc->m_enc.SetOutputHWND (m_pwndenc->m_pdiag->m_hwnddiaglist);
	m_pwndenc->m_enc.SetSpeedLabelHWND (m_pwndenc->m_dlghwnd);

	m_pwndenc->m_enc.ltotalbytesprocessed = 0;
	m_pwndenc->m_enc.m_lasttickcount = 0;
	m_pwndenc->m_enc.m_iaveragetrigger = 0;
	m_pwndenc->m_enc.m_addedspeed = 0;
	m_pwndenc->m_enc.ltotalbytes = ltotalbytes;
	m_pwndenc->m_enc.InitEncryptFile ();

	SetDlgItemText (m_pwndenc->m_dlghwnd, ID_LBLOPERATION, "Encrypting data using AES-256 bit cipher...");

	SendMessage(m_pwndenc->m_hwndprogress, PBM_SETRANGE, 0L, MAKELONG (0, 39500));

	for (a=0;a<m_pwndenc->m_dlFileinfolist.GetNumItems ();a++) {
		pinfo = (SingleFileInfo *) m_pwndenc->m_dlFileinfolist.GetItem (a);

		if (pinfo->bIsDir == true) {

			ZeroMemory (szDestpath, SIZE_STRING);
			strcpy_s (szDestpath, SIZE_STRING, m_pwndenc->m_szVaultpath);
			strcat_s (szDestpath, SIZE_STRING, "\\");

			ZeroMemory (szFileonly, SIZE_STRING);
			ZeroMemory (szPathonly, SIZE_STRING);
			
			m_pwndenc->GetPathOnly (pinfo->szRootdir, szPathonly, szFileonly, "\\");

			ZeroMemory (szTemp, SIZE_STRING);

			m_pwndenc->StripStartPath (pinfo->szRootdir, pinfo->szFilepath, szTemp);

			strcat_s (szDestpath, SIZE_STRING, szFileonly);
			//strcat_s (szDestpath, SIZE_STRING, "\\");
			strcat_s (szDestpath, SIZE_STRING, szTemp);

			ZeroMemory (szMsgpathonly, SIZE_STRING);
			ZeroMemory (szMsgfileonly, SIZE_STRING);
			ZeroMemory (szFilemessage, SIZE_STRING);
			
			m_pwndenc->GetPathOnly (pinfo->szFilepath, szMsgpathonly, szMsgfileonly, "\\");

			strcpy_s (szFilemessage, SIZE_STRING, "Encrypting file: ");
			strcat_s (szFilemessage, SIZE_STRING, szMsgfileonly);
			SetDlgItemText (m_pwndenc->m_dlghwnd, ID_LBLFILE, szFilemessage);

			m_pwndenc->m_enc.EncryptFileEx3 (pinfo->szFilepath, szDestpath, m_pwndenc->m_szPassword, true);
			
			m_pwndenc->m_pdiag->OutputText ("Encrypt Source: ", pinfo->szFilepath);
			m_pwndenc->m_pdiag->OutputText ("Encrypt Dest: ", szDestpath);


		} else {
			ZeroMemory (szDestpath, SIZE_STRING);
			strcpy_s (szDestpath, SIZE_STRING, m_pwndenc->m_szVaultpath);
			strcat_s (szDestpath, SIZE_STRING, "\\");

			ZeroMemory (szFileonly, SIZE_STRING);
			ZeroMemory (szPathonly, SIZE_STRING);

			m_pwndenc->GetPathOnly (pinfo->szFilepath, szPathonly, szFileonly, "\\");

			strcat_s (szDestpath, SIZE_STRING, szFileonly);

			m_pwndenc->m_pdiag->OutputText ("Encrypt Source: ", pinfo->szFilepath);
			m_pwndenc->m_pdiag->OutputText ("Encrypt Dest: ", szDestpath);

			ZeroMemory (szMsgpathonly, SIZE_STRING);
			ZeroMemory (szMsgfileonly, SIZE_STRING);
			ZeroMemory (szFilemessage, SIZE_STRING);
			
			m_pwndenc->GetPathOnly (pinfo->szFilepath, szMsgpathonly, szMsgfileonly, "\\");

			strcpy_s (szFilemessage, SIZE_STRING, "Encrypting file: ");
			strcat_s (szFilemessage, SIZE_STRING, szMsgfileonly);
			SetDlgItemText (m_pwndenc->m_dlghwnd, ID_LBLFILE, szFilemessage);

			m_pwndenc->m_enc.EncryptFileEx3 (pinfo->szFilepath, szDestpath, m_pwndenc->m_szPassword, true);
		}

		//SendMessage (m_pwndenc->m_hwndprogress, PBM_SETPOS, a, 0L);
	}

	PostMessage (m_pwndenc->m_dlghwnd, ENCRYPT_COMPLETE, 0, 0);

	return 0;
}


void EncryptWindowEx::DoEncryptionThread ()
{
	// MULTI-THREADED FUNCTION
	// Spawns the first encryption thread.
	HANDLE hThread;
	DWORD dwThreadID;

	hThread = CreateThread (NULL, 0, EncryptionProc, NULL, 0, &dwThreadID);
}

INT_PTR CALLBACK EncryptWindowEx::ProgressDlgProc (HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch (Message)
	{
		case DECRYPT_COMPLETE:
		{
			//EndDialog (hWnd, IDOK);
			SetTimer (hWnd, IDT_DLGWAITCHECK, 500, NULL);
		}
		break;

		case ENCRYPT_COMPLETE:
		{
			//EndDialog (hWnd, IDOK);
			SetTimer (hWnd, IDT_DLGWAITCHECK, 500, NULL);
		}
		break;

		case WM_INITDIALOG:
		{
			m_pwndenc->m_dlghwnd = hWnd;

			m_pwndenc->m_hbmBanner = LoadBitmap (GetModuleHandle (NULL), MAKEINTRESOURCE (IDB_PROGRESSBANNER));
			
			if (m_pwndenc->m_hbmBanner  == NULL) {
				m_pwndenc->m_bBannerloaded = false;
			} else {
				m_pwndenc->m_bBannerloaded = true;
			}

			m_pwndenc->m_hwndprogress  = CreateWindowEx(0L, PROGRESS_CLASS, "", WS_CHILD | WS_VISIBLE, 22, 85, 550, 22, hWnd, (HMENU) ID_ENCPROGRESS, GetModuleHandle (NULL), NULL);

			SendMessage(m_pwndenc->m_hwndprogress, PBM_SETRANGE, 0L, MAKELONG (0, 100));
			SendMessage (m_pwndenc->m_hwndprogress, PBM_SETSTEP, 1, 0L);


			HFONT hfDefault = (HFONT) GetStockObject (DEFAULT_GUI_FONT);

			m_pwndenc->m_lblspeed = CreateWindow ("static", "Calculating speed...", WS_CHILD | WS_VISIBLE, 22, 20, 550, 21, hWnd, (HMENU) ID_LBLSPEED, GetModuleHandle (NULL), NULL);
			SendMessage (m_pwndenc->m_lblspeed, WM_SETFONT, (WPARAM) hfDefault, MAKELPARAM (FALSE, 0));

			m_pwndenc->m_lblfile = CreateWindow ("static", "Working...", WS_CHILD | WS_VISIBLE, 22, 40, 550, 21, hWnd, (HMENU) ID_LBLFILE, GetModuleHandle (NULL), NULL);
			SendMessage (m_pwndenc->m_lblfile, WM_SETFONT, (WPARAM) hfDefault, MAKELPARAM (FALSE, 0));

			m_pwndenc->m_lbloperation = CreateWindow ("static", "Cipher in progress...", WS_CHILD | WS_VISIBLE, 22, 60, 550, 21, hWnd, (HMENU) ID_LBLOPERATION, GetModuleHandle (NULL), NULL);
			SendMessage (m_pwndenc->m_lbloperation, WM_SETFONT, (WPARAM) hfDefault, MAKELPARAM (FALSE, 0));


			if (m_pwndenc->m_bEncryptmode == true) {
				//SetTimer (hWnd, IDT_DLGWAITCHECK, 5000, NULL);
				m_pwndenc->DoEncryptionThread ();
			} else {
				m_pwndenc->DoDecryptionThread ();
			}
			
			
		}
		break;
		case WM_PAINT:
		{
			if (m_pwndenc->m_bBannerloaded == true) {
				BITMAP bm;
				PAINTSTRUCT ps;
				
				HDC hdc = BeginPaint (hWnd, &ps);

				HDC hdcMem = CreateCompatibleDC (hdc);
				HBITMAP hbmOld = (HBITMAP) SelectObject (hdcMem, m_pwndenc->m_hbmBanner);

				GetObject (m_pwndenc->m_hbmBanner, sizeof (bm), &bm);
				BitBlt (hdc, 0, 123, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);

				SelectObject (hdcMem, hbmOld);
				DeleteDC (hdcMem);
				
				EndPaint (hWnd, &ps);
			}
		}
		break;

		case WM_TIMER:
		{
			switch (wParam)
			{
				case IDT_DLGWAITCHECK:
				{					
					EndDialog (hWnd, IDOK);					
				}
				break;
			}
			break;
		}
		break;
		case WM_COMMAND:
		{
			switch (LOWORD (wParam))
			{
				case IDOK:
				{
				}
				break;
				case IDCANCEL:
				{
					EndDialog (hWnd, IDCANCEL);
				}
				break;
			}
			break;
		}
		break;
		default:
			return FALSE;
	}
	return TRUE;
}

void EncryptWindowEx::DoEncryption (char *szVaultpath)
{
	int iRet = 0;
	m_bEncryptmode = true;

	ZeroMemory (m_szVaultpath, SIZE_STRING);
	strcpy_s (m_szVaultpath, SIZE_STRING, szVaultpath);

	iRet = DialogBox (GetModuleHandle (NULL), MAKEINTRESOURCE (IDD_CRYPTDIALOG), m_hwnd, ProgressDlgProc);
	
}

void EncryptWindowEx::DoDecryption ()
{
	int iRet = 0;
	m_bEncryptmode = false;

	iRet = DialogBox (GetModuleHandle (NULL), MAKEINTRESOURCE (IDD_CRYPTDIALOG), m_hwnd, ProgressDlgProc);
	
}