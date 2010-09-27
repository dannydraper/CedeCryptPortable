#define _WIN32_WINNT 0x0501

#include <windows.h>
#include <commctrl.h>
#include "Licensing.h"
#include "InternetHandler.h"
#include "resource.h"

#define SIZE_STRING		1024
#define SIZE_NAME			64
#define APPTIMER			4000

#define ACTIVATE_SUCCESS	300
#define ACTIVATE_FAILED		301
#define ACTIVATE_EXISTS		302
#define ACTIVATE_NOCONNECTION	303
#define DEACTIVATE_SUCCESS	304
#define DEACTIVATE_FAILED	305
#define DEACTIVATE_NOTACTIVATED	306

// Globals
HWND g_hWnd = NULL;
const char g_szClassName[] = "myWindowClass";
HBITMAP g_hbmLicenseimage = NULL;
bool g_bLicenseimageloaded = false;
char g_szCedecryptpath[SIZE_STRING];
Licensing g_lic;
InternetHandler g_inet;

char g_szEnteredlicense[SIZE_STRING];

bool g_bDeactivate = false;
char g_szLicensekey[SIZE_STRING];

void ShowInt (unsigned long l)
{
	char szMessage[SIZE_STRING];
	ZeroMemory (szMessage, SIZE_STRING);
	sprintf_s (szMessage, SIZE_STRING, "Unsigned long is: %i", l);
	MessageBox (NULL, szMessage, "ShowInt", MB_OK | MB_ICONINFORMATION);
}

bool FileExists (char *szFilepath)
{
	struct _finddata_t c_file;
	long hFile;
	
	if( (hFile = _findfirst(szFilepath, &c_file )) == -1L ) {
	}
	else
	{
		_findclose( hFile );
		return true;				
	}

	return false;
}

void GetPathOnly (char *szInpath, char *szOutpath, char *szOutfile, char *szSep)
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

bool ValidateCedeCryptPath ()
{
	char szModule[SIZE_STRING];
	ZeroMemory (szModule, SIZE_STRING);

	GetModuleFileName (NULL, szModule, SIZE_STRING);

	char szPath[SIZE_STRING];
	char szFile[SIZE_STRING];

	char szFullpath1[SIZE_STRING];
	ZeroMemory (szFullpath1, SIZE_STRING);

	char szFullpath2[SIZE_STRING];
	ZeroMemory (szFullpath2, SIZE_STRING);

	ZeroMemory (szPath, SIZE_STRING);
	ZeroMemory (szFile, SIZE_STRING);

	GetPathOnly (szModule, szPath, szFile, "\\");

	strcpy_s (szFullpath1, SIZE_STRING, szPath);
	strcat_s (szFullpath1, SIZE_STRING, "CedeCrypt.exe");

	strcpy_s (szFullpath2, SIZE_STRING, szPath);
	strcat_s (szFullpath2, SIZE_STRING, "CedeCryptPortable.exe");

	bool filevalidated = false;

	if (FileExists (szFullpath1) == true) {
		g_lic.SetDLLPath (szFullpath1);
		strcpy_s (g_szCedecryptpath, SIZE_STRING, szFullpath1);
		filevalidated = true;
	}

	if (FileExists (szFullpath2) == true) {
		g_lic.SetDLLPath (szFullpath2);
		strcpy_s (g_szCedecryptpath, SIZE_STRING, szFullpath2);
		filevalidated = true;
	}

	if (filevalidated == true) {
		return true;
	} else {
		return false;
	}
}

bool Findmemstring (char *szString, MemoryBuffer *memBuffer)
{
	unsigned long b = 0;

	char szCurrent[SIZE_STRING];
	
	if (memBuffer->GetSize () >= strlen (szString)) {
		
		for (b=0;b<memBuffer->GetSize ()-strlen (szString);b++) {
			
			ZeroMemory (szCurrent, SIZE_STRING);
			memcpy (szCurrent, (char *) memBuffer->GetBuffer ()+b, strlen (szString));

			if (strcmp (szString, szCurrent) == 0) {
				return true;
			}
		}

		return false;

	} else {
		return false;
	}
}

unsigned int ActivateProductKey (char *szProductKey, bool bActivate)
{
	MemoryBuffer memResponse;
	memResponse.SetSize (50000);

	char szUrl[SIZE_STRING];
	ZeroMemory (szUrl, SIZE_STRING);
	strcpy_s (szUrl, SIZE_STRING, "http://www.cedesoft.com/ProdAct147770747.aspx?pk=");
	strcat_s (szUrl, SIZE_STRING, szProductKey);
	
	if (bActivate == true) {
		strcat_s (szUrl, SIZE_STRING, "&cm=AAAA");
	} else {
		strcat_s (szUrl, SIZE_STRING, "&cm=DDDD");
	}
	

	if (g_inet.InternetFiletoMemoryBuffer (szUrl, &memResponse, memResponse.GetSize ()) == true) {
		
		//memResponse.SaveToFile ("C:\\temp\\dtemp\\response.dat");

		if (bActivate == true) {

			if (Findmemstring ("7777777.PSALMS118:8.777777714700014700000007777777", &memResponse) == true) {
				memResponse.Clear ();
				return ACTIVATE_SUCCESS;
			}

			if (Findmemstring ("7777777.PSALMS118:8.777777714800014800000007777777", &memResponse) == true) {
				memResponse.Clear ();
				return ACTIVATE_FAILED;
			}

			if (Findmemstring ("7777777.PSALMS118:8.777777714900014900000007777777", &memResponse) == true) {
				memResponse.Clear ();
				return ACTIVATE_EXISTS;
			}
		
		} else {
			
			if (Findmemstring ("7777777.PSALMS118:8.777777717700017700000007777777", &memResponse) == true) {
				memResponse.Clear ();
				return DEACTIVATE_SUCCESS;
			}

			if (Findmemstring ("7777777.PSALMS118:8.777777717800017800000007777777", &memResponse) == true) {
				memResponse.Clear ();
				return DEACTIVATE_FAILED;
			}

			if (Findmemstring ("7777777.PSALMS118:8.777777717900017900000007777777", &memResponse) == true) {
				memResponse.Clear ();
				return DEACTIVATE_NOTACTIVATED;
			}

		}


		memResponse.Clear ();
		return ACTIVATE_FAILED;
	} else {
		//MessageBox (NULL, "Internet Communication failed.", "Failed.", MB_OK);
		memResponse.Clear ();
		return ACTIVATE_FAILED;
	}

	memResponse.Clear ();
	return ACTIVATE_FAILED;
}

INT_PTR CALLBACK LicenseDlgProc (HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch (Message)
	{
		case WM_INITDIALOG:
		{		
			g_hbmLicenseimage = LoadBitmap (GetModuleHandle (NULL), MAKEINTRESOURCE (IDB_LICENSEIMAGE));
			
			if (g_hbmLicenseimage == NULL) {
				g_bLicenseimageloaded = false;
			} else {
				g_bLicenseimageloaded = true;
			}
		}
		break;
		case WM_PAINT:
		{
			BITMAP bm;
			PAINTSTRUCT ps;
			
			if (g_bLicenseimageloaded == true) {
				HDC hdc = BeginPaint (hWnd, &ps);

				HDC hdcMem = CreateCompatibleDC (hdc);
				HBITMAP hbmOld = (HBITMAP) SelectObject (hdcMem, g_hbmLicenseimage);

				GetObject (g_hbmLicenseimage, sizeof (bm), &bm);
				BitBlt (hdc, 10, 10, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);

				SelectObject (hdcMem, hbmOld);
				DeleteDC (hdcMem);
			}
						
			EndPaint (hWnd, &ps);
		}
		break;
		case WM_TIMER:
		{
			switch (wParam)
			{

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
					ZeroMemory (g_szEnteredlicense, SIZE_STRING);
					GetDlgItemText (hWnd, IDC_EDITLICENSEKEY, g_szEnteredlicense, SIZE_STRING);
					EndDialog (hWnd, IDOK);
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



bool ValidateLicenseKey ()
{
	if (g_lic.ValidateUserKey (g_szEnteredlicense) == true) {
		// License is ok.

		// Now check the product is for CedeCrypt Portable
		if (strcmp (g_lic.GetUniqueProductID (), "11977474777777") == 0) {
			
			// License is ok for CedeCrypt Portabler
			if (g_lic.GetTrialMode () == true) {
				// This is a trial license key.
				MessageBox (NULL, "The license key you have entered is a trial mode license. Please contact CedeSoft for a full license key.", "License Invalid.", MB_OK | MB_ICONEXCLAMATION);
			} else {
				// This is a full mode license key
				
				// Here, we now must check if this license is activated, if not activate it and we're done
				// If it's already activated tell the user about it, and quit.
				int iaret = ActivateProductKey (g_szEnteredlicense, true);

				if (iaret == ACTIVATE_SUCCESS) {
					// Now save the license in the CP exe
					// also save this volume serial in the CP exe
					g_lic.ClearAllKeys ();
					g_lic.SaveUserKey (g_szEnteredlicense);
					g_lic.SaveMachineKey ();

					MessageBox (NULL, "Your license key was accepted and activation was successful. Thank you for purchasing CedeCrypt portable edition.", "Product Activation", MB_OK | MB_ICONINFORMATION);
				}

				if (iaret == ACTIVATE_FAILED) {
					MessageBox (NULL, "Activation failed. Please check your internet connection and try again.", "Product Activation", MB_OK | MB_ICONEXCLAMATION);
				}

				if (iaret == ACTIVATE_EXISTS) {
					MessageBox (NULL, "This license key has already been activated. You can purchase additional license keys from http://www.cedesoft.com. However if you wish to move your CedeCrypt portable license to another storage device you can deactivate your existing license, and reactivate it on the new storage device.", "Product Activation", MB_OK | MB_ICONINFORMATION);
				}

				if (iaret == DEACTIVATE_SUCCESS) {
					MessageBox (NULL, "Deactivation was successful. You can re-activate this product at any time.", "Product Activation", MB_OK | MB_ICONINFORMATION);
				}

				if (iaret == DEACTIVATE_FAILED) {
					MessageBox (NULL, "Deactivation failed. Please check your internet connection and try again.", "Product Activation", MB_OK | MB_ICONEXCLAMATION);
				}

				if (iaret == DEACTIVATE_NOTACTIVATED) {
					MessageBox (NULL, "This product has not been activated, and can be activated at any time.", "Product Activation", MB_OK | MB_ICONINFORMATION);
				}

				//unsigned long loffset = g_lic.GetWriteableOffset ();
				
				//ShowInt (loffset);
				//MessageBox (NULL, "Full license accepted. Thank you for purchasing CedeCrypt Portable edition.", "License Accepted", MB_OK | MB_ICONINFORMATION);
				return true;
			}

		} else {
			MessageBox (NULL, "The license key you have entered is not for this CedeSoft product. Please enter a valid license key.", "License Invalid.", MB_OK | MB_ICONEXCLAMATION);
		}

	} else {
		MessageBox (NULL, "You have entered an invalid license key.", "Invalid License", MB_OK | MB_ICONEXCLAMATION);
	}

	return false;
}

LRESULT CALLBACK WndProc (HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
		
		case WM_CREATE:
		{
			g_hWnd = hwnd;
			int iRet = 0;		

			//SetTimer (g_hWnd, APPTIMER, 3000, NULL);
			if (ValidateCedeCryptPath () == true) {

				if (g_inet.DoesWebConnectionExist () == true) {

					if (g_bDeactivate == true) {
						//char g_szLicensekey[SIZE_STRING];

						int iaret = ActivateProductKey (g_szLicensekey, false);

						if (iaret == DEACTIVATE_SUCCESS) {
							g_lic.ClearAllKeys ();
							MessageBox (NULL, "Deactivation was successful. You can re-activate this product at any time.", "Product Activation", MB_OK | MB_ICONINFORMATION);
						}

						if (iaret == DEACTIVATE_FAILED) {
							MessageBox (NULL, "Deactivation failed. Please check your internet connection and try again.", "Product Activation", MB_OK | MB_ICONEXCLAMATION);
						}

						if (iaret == DEACTIVATE_NOTACTIVATED) {
							MessageBox (NULL, "This product has not been activated, and can be activated at any time.", "Product Activation", MB_OK | MB_ICONINFORMATION);
						}

						ShellExecute (NULL, "open", g_szCedecryptpath, NULL, NULL, SW_SHOWNORMAL);

					} else {
						iRet = DialogBox (GetModuleHandle (NULL), MAKEINTRESOURCE (IDD_LICENSEDIALOG), g_hWnd, LicenseDlgProc);
					
						if (iRet == IDOK) {
							//MessageBox (NULL, g_szEnteredlicense, "Licensekey", MB_OK);
							ValidateLicenseKey ();

							// Launch CedeCrypt
							ShellExecute (NULL, "open", g_szCedecryptpath, NULL, NULL, SW_SHOWNORMAL);

						}

						if (iRet == IDCANCEL) {
							// Launch CedeCrypt
							ShellExecute (NULL, "open", g_szCedecryptpath, NULL, NULL, SW_SHOWNORMAL);
						}
					}

					PostQuitMessage (0);

				} else {
					MessageBox (NULL, "Unable to contact CedeSoft for product activation. Please check that you are connected to the Internet and try again.", "No internet connection", MB_OK | MB_ICONEXCLAMATION);
				
					ShellExecute (NULL, "open", g_szCedecryptpath, NULL, NULL, SW_SHOWNORMAL);
					PostQuitMessage (0);
				}
				
			} else {
				MessageBox (NULL, "License activation failed. Please ensure this application is in the same folder as CedeCrypt portable edition.", "License activation failed.", MB_OK | MB_ICONEXCLAMATION);			
				PostQuitMessage (0);
			}
		}
		break;
		case WM_COMMAND:
		{
			switch (wParam)
			{
				
			}
		}
		break;
		case WM_TIMER:
		{
			switch (wParam)
			{
				case APPTIMER:
				{
					
				}
				break;
			}
		}
		break;
		case WM_CLOSE:
			DestroyWindow (hwnd);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;		
		default:
			return DefWindowProc (hwnd, msg, wParam, lParam);
	}
	return 0;
}


int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) 
{

	InitCommonControls ();

	WNDCLASSEX wc;
	HWND hwnd;
	MSG Msg;	
	
	// Step1: Registering the window class
	wc.cbSize = sizeof (WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0L;
	wc.cbWndExtra = 0L;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon (hInstance, IDI_APPLICATION);
	wc.hCursor = LoadCursor (NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH) CreateSolidBrush (RGB (236, 233, 216));
	wc.lpszMenuName = NULL;
	wc.lpszClassName = g_szClassName;
	wc.hIconSm = LoadIcon (hInstance, IDI_APPLICATION);

	if (!RegisterClassEx (&wc)) {
		MessageBox (NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	if (strlen (lpCmdLine) > 5) {
		g_bDeactivate = true;
		strcpy_s (g_szLicensekey, SIZE_STRING, lpCmdLine);
	}

	// Step2: Creating the window
	hwnd = CreateWindowEx (NULL, g_szClassName, "CedeCrypt Activation", 
					WS_MINIMIZEBOX | WS_BORDER, NULL, NULL, 568, 180,
					NULL, NULL, hInstance, NULL);

	if (hwnd == NULL) {
		MessageBox (NULL, "Window creation failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	

	//ShowWindow (hwnd, nCmdShow);
	UpdateWindow (hwnd);

	// Step3: The message loop	
	while (GetMessage (&Msg, NULL, 0, 0) > 0) {
		TranslateMessage (&Msg);
		DispatchMessage (&Msg);
	}

	return Msg.wParam;
	return 0;
}