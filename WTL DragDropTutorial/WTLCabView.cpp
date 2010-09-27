// WTLCabView.cpp : main source file for WTLCabView.exe
//

#include "stdafx.h"
#include "resource.h"
#include "MainFrm.h"

CAppModule _Module;
bool g_bXPOrLater;
std::vector<CString> g_vecsTempFiles;

int Run(LPTSTR /*lpstrCmdLine*/ = NULL, int nCmdShow = SW_SHOWDEFAULT)
{
int nRet;
CMessageLoop theLoop;

    _Module.AddMessageLoop ( &theLoop );

CMainFrame wndMain;

    if ( wndMain.CreateEx() == NULL )
        {
        ATLTRACE(_T("Main window creation failed!\n"));
        return 0;
        }

    wndMain.ShowWindow(nCmdShow);

    nRet = theLoop.Run();

    _Module.RemoveMessageLoop();
    return nRet;
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
    OleInitialize(NULL);
    AtlInitCommonControls(ICC_COOL_CLASSES|ICC_BAR_CLASSES|ICC_WIN95_CLASSES);
    _Module.Init(NULL, hInstance);

    g_bXPOrLater = _winmajor > 5 || (_winmajor == 5 && _winminor > 0);

int nRet = Run ( lpstrCmdLine, nCmdShow );

    _Module.Term();
    OleUninitialize();
    return nRet;
}
