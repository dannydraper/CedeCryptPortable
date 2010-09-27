#include <windows.h>
#include "resource.h"
#include "UIWindow.h"
#include "MainWindow.h"
#include "Diagnostics.h"


////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// GLOBAL VARIABLES
////////////////////////////////////////////////////////////////////////////////////////

MainWindow g_windowmain;
bool MainWindow::m_sbcancelwaitdlg = false;	
Diagnostics g_windowdiag;

FARPROC PasswordWindow::lpfnOldWndProc = NULL;
FARPROC PasswordWindow::lpfnOldWndProc2 = NULL;
PasswordWindow *PasswordWindow::ppwnd = NULL;
EncryptWindowEx *EncryptWindowEx::m_pwndenc = NULL;
DirScanner *DirScanner::m_pdirscanner = NULL;

////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// FUNCTION PROTOTYPES
////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// IMPLEMENTATION
////////////////////////////////////////////////////////////////////////////////////////


int WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) 
{
	HWND hWnd = NULL;
	MSG msg;

	//if (g_windowmain.ValidateOS () == false) {
	//	return 0;
	//}

	// Ensure that the common control DLL is loaded
	InitCommonControls ();

	OleInitialize(0);

	g_windowdiag.Initialise (hWnd, lpCmdLine);
	
	g_windowmain.SetDiagnostics (&g_windowdiag);
	g_windowmain.Initialise (hWnd, lpCmdLine);
	
	// Enter the message loop
	while (GetMessage (&msg, NULL, 0, 0))
	{
		TranslateMessage (&msg);
		DispatchMessage (&msg);
	}


	// Shutdown COM
	OleUninitialize();


	return 0;
}
