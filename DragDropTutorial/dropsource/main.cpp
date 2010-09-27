//
//	MAIN.CPP
//
//	DragSource 
//
//	Freeware written by J Brown 2004
//
//	www.catch22.net
//

#define STRICT

#include <windows.h>
#include "resource.h"

#define APPNAME "IDropSource"

#pragma comment(linker, "/OPT:NOWIN98")

HWND		hwndMain;
HWND		hwndEdit;
HINSTANCE	hInstance;

WNDPROC		OldEditWndProc;

HRESULT CreateDropSource(IDropSource **ppDropSource);
HRESULT CreateDataObject(FORMATETC *fmtetc, STGMEDIUM *stgmeds, UINT count, IDataObject **ppDataObject);

//
//	Is the mouse cursor within the edit control's selected text?
//
//	Return TRUE/FALSE 
//
BOOL MouseInSelection(HWND hwndEdit, LPARAM MouseMsgParam)
{
	DWORD nSelStart;
	DWORD nSelEnd;

	// get the selection inside the edit control
	SendMessage(hwndEdit, EM_GETSEL, (WPARAM)&nSelStart, (LPARAM)&nSelEnd);

	if(nSelStart != nSelEnd)
	{
		DWORD nCurPos;

		// Get the cursor position the mouse has clicked on
		nCurPos = SendMessage(hwndEdit, EM_CHARFROMPOS, 0, MouseMsgParam);
		nCurPos = LOWORD(nCurPos);

		// Did the mouse click inside the active selection?
		return (nCurPos >= nSelStart && nCurPos < nSelEnd) ? TRUE : FALSE;
	}

	return FALSE;
}

//
//	Remove any selection from the edit control
//
void ClearSelection(HWND hwndEdit)
{
	SendMessage(hwndEdit, EM_SETSEL, -1, -1);
}

//
//	Copy selected text to a HGLOBAL and return it
//
HGLOBAL CopySelection(HWND hwndEdit)
{
	DWORD   nSelStart, nSelEnd;
	DWORD   nSelLength, nEditLength;
	HGLOBAL hMem;
	char   *ptr;
	char   *tmp;

	SendMessage(hwndEdit, EM_GETSEL, (WPARAM)&nSelStart, (LPARAM)&nSelEnd);

	nSelLength = nSelEnd - nSelStart;

	// get the entire contents of the control
	nEditLength = SendMessage(hwndEdit, EM_GETLIMITTEXT, 0, 0);
	tmp  = (char *)malloc(nEditLength);

	SendMessage(hwndEdit, WM_GETTEXT, nEditLength, (LPARAM)tmp);

	hMem = GlobalAlloc(GHND, nSelLength+1);
	ptr  = (char *)GlobalLock(hMem);

	// copy the selected text and nul-terminate
	memcpy(ptr, tmp+nSelStart, nSelLength);
	ptr[nSelLength] = '\0';

	GlobalUnlock(hMem);

	free(tmp);

	return hMem;
}

//
//	Subclass window-procedure for EDIT control
//
LRESULT CALLBACK EditWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static BOOL  fMouseDown   = FALSE;
	static BOOL  fDidDragDrop = FALSE;
		   
	switch(msg)
	{
	case WM_KEYDOWN:
		
		// when ESCAPE is pressed clear the current selection
		if(wParam == VK_ESCAPE)
			ClearSelection(hwnd);

		break;

	case WM_LBUTTONDOWN:
	case WM_LBUTTONDBLCLK:

		// if the mouse is pressed when it is over a selection,
		// then start a drag-drop as soon as the mouse moves next
		if(MouseInSelection(hwndEdit, lParam))
		{
			fMouseDown = TRUE;
			fDidDragDrop = FALSE;
			SetCapture(hwnd);
			return 0;
		}

		break;

	case WM_SETCURSOR:

		// set the mouse cursor to an ARROW when it intersects the
		// current selection, or the default IBEAM otherwise
		if((HWND)wParam == hwnd)
		{
			POINT pt;
			GetCursorPos(&pt);
			ScreenToClient(hwndEdit, &pt);

			if(MouseInSelection(hwndEdit, MAKELPARAM(pt.x, pt.y)))
			{
				SetCursor(LoadCursor(0, MAKEINTRESOURCE(IDC_ARROW)));
			}
			else
			{
				SetCursor(LoadCursor(0, MAKEINTRESOURCE(IDC_IBEAM)));
			}
		}

		return TRUE;

	case WM_MOUSEMOVE:

		// if the mouse is held down then start a drag-drop
		if(fMouseDown)
		{
			IDataObject *pDataObject;
			IDropSource *pDropSource;
			DWORD		 dwEffect;
			DWORD		 dwResult;

			FORMATETC fmtetc = { CF_TEXT, 0, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
			STGMEDIUM stgmed = { TYMED_HGLOBAL, { 0 }, 0 };

			// transfer the current selection into the IDataObject
			stgmed.hGlobal = CopySelection(hwndEdit);

			// Create IDataObject and IDropSource COM objects
			CreateDropSource(&pDropSource);
			CreateDataObject(&fmtetc, &stgmed, 1, &pDataObject);

			//
			//	** ** ** The drag-drop operation starts here! ** ** **
			//
			dwResult = DoDragDrop(pDataObject, pDropSource, DROPEFFECT_COPY|DROPEFFECT_MOVE, &dwEffect);

			// success!
			if(dwResult == DRAGDROP_S_DROP)
			{
				if(dwEffect & DROPEFFECT_MOVE)
				{
					// remove selection from edit control
				}
			}
			// cancelled
			else if(dwResult == DRAGDROP_S_CANCEL)
			{
			}

			pDataObject->Release();
			pDropSource->Release();

			ReleaseCapture();
			fMouseDown = FALSE;
			fDidDragDrop = TRUE;
		}

		break;

	case WM_LBUTTONUP:
		
		// stop drag-drop from happening when the mouse is released.
		if(fMouseDown)
		{
			ReleaseCapture();
			fMouseDown = FALSE;

			if(fDidDragDrop == FALSE)
				ClearSelection(hwnd);
		}

		break;
	}

	return CallWindowProc(OldEditWndProc, hwnd, msg, wParam, lParam);
}

//
//	Main Window message handler
//
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_CREATE:

		// create a child-window EDIT control
		hwndEdit = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "",
			WS_CHILD|WS_VISIBLE|ES_MULTILINE|ES_WANTRETURN|WS_VSCROLL,
			0,0,0,0, hwnd, 0, hInstance, 0);

		// fixed-width font
		SendMessage(hwndEdit, WM_SETFONT, (WPARAM)GetStockObject(ANSI_FIXED_FONT), 0);

		// subclass the edit control so we can add drag+drop support to it
		OldEditWndProc = (WNDPROC)SetWindowLong(hwndEdit, GWL_WNDPROC, (LONG)EditWndProc);

		SetFocus(hwndEdit);

		return TRUE;

	case WM_COMMAND:
	
		// react to menu messages
		switch(LOWORD(wParam))
		{
		case IDM_FILE_EXIT:
			CloseWindow(hwnd);
			return 0;

		case IDM_FILE_ABOUT:
			MessageBox(hwnd, "IDropSource Test Application\r\n\r\n"
							 "Copyright(c) 2004 by Catch22 Productions\t\r\n"
							 "Written by J Brown.\r\n\r\n"
							 "Homepage at www.catch22.net", APPNAME, MB_ICONINFORMATION);
			return 0;
		}

		break;

	case WM_CLOSE:

		// shut program down
		DestroyWindow(hwnd);
		PostQuitMessage(0);
		return 0;

	case WM_SIZE:
		
		// resize editbox to fit in main window
		MoveWindow(hwndEdit, 0, 0, LOWORD(lParam), HIWORD(lParam), TRUE);
		return 0;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

void InitMainWnd()
{
	WNDCLASSEX wc = { sizeof(wc) };

	wc.lpfnWndProc   = WndProc;
	wc.lpszClassName = APPNAME;
	wc.lpszMenuName  = MAKEINTRESOURCE(IDR_MENU1);
	wc.hInstance     = hInstance;

	RegisterClassEx(&wc);
}

void CreateMainWnd()
{
	hwndMain = CreateWindowEx(0, APPNAME, APPNAME, 
		WS_VISIBLE|WS_OVERLAPPEDWINDOW, 
		CW_USEDEFAULT, CW_USEDEFAULT, 512, 200, 0,0,
		hInstance, 0);
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpCmdLine, int nShowCmd)
{
	MSG msg;

	hInstance = hInst;

	// This program requires COM
	OleInitialize(0);
	
	InitMainWnd();
	CreateMainWnd();

	// message-pump
	while(GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// Shutdown COM
	OleUninitialize();

	return 0;
}