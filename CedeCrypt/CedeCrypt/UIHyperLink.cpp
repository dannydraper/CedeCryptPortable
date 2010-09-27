#include "UIHyperLink.h"

UIHyperLink::UIHyperLink ()
{
	m_bMouseover = false;
	m_bVisible = true;
	m_bUseCommand = false;
	m_Handcursor = LoadCursor (NULL, IDC_HAND);
}

UIHyperLink::~UIHyperLink ()
{

}

unsigned long UIHyperLink::GetSize () {
	return sizeof (UIHyperLink);
}

void UIHyperLink::Paint (HDC hdc)
{
	if (m_bVisible == false) {
		return;
	}

	PaintText (hdc);
}

void UIHyperLink::SetCommandMode (bool bCommand)
{
	m_bUseCommand = bCommand;
}

void UIHyperLink::SetVisible (bool bVisible)
{
	m_bVisible = bVisible;
	InvalidateControl (false);
}

void UIHyperLink::SetHyperlinkProperties (TCHAR *szFontName, int FontSize, char *szCaption, char *szTarget)
{
	m_szTarget = (int *) szTarget;
	SetTextProperties (szFontName, FontSize, 4, 0, 0, szCaption, RGB (0, 0, 255));
}

void UIHyperLink::NotifyMouseMove (int iXPos, int iYPos)
{
	g_mouseXPos = iXPos;
	g_mouseYPos = iYPos;

	if (m_bVisible == false) {
		return;
	}

	if (IsMouseRect (true) == true) {
		if (m_bMouseover == false) {
			m_bMouseover = true;
			SetTextForeColor (RGB (0, 150, 255));			
			m_Prevcursor = SetCursor (m_Handcursor);
			InvalidateControl (true);
			SetClassLongPtr (g_hWnd, GCLP_HCURSOR, (LONG) m_Handcursor);
			if (m_bUseCommand == true) {
				PostMessage (g_hWnd, WM_UIHIGHLIGHT, (WPARAM) g_controlID, 0);
			}
		}
	} else {
		if (m_bMouseover == true) {
			m_bMouseover = false;
			SetTextForeColor (RGB (0, 0, 255));
			InvalidateControl (true);
			SetClassLongPtr (g_hWnd, GCLP_HCURSOR, (LONG) m_Prevcursor);
			if (m_bUseCommand == true) {
				PostMessage (g_hWnd, WM_UINOHIGHLIGHT, (WPARAM) g_controlID, 0);
			}
		}
	}
}

void UIHyperLink::NotifyMouseDown ()
{
	if (m_bVisible == false) {
		return;
	}

	if (m_bMouseover == true) {
		SetTextForeColor (RGB (0, 190, 255));
		InvalidateControl (true);
	}
}

void UIHyperLink::NotifyMouseUp ()
{
	if (m_bVisible == false) {
		return;
	}

	if (m_bMouseover == false) {
		SetTextForeColor (RGB (0, 0, 255));
	} else {
		SetTextForeColor (RGB (0, 150, 255));
		if (m_bUseCommand == false) {
			//MessageBox (g_hWnd, (char *) m_szTarget, "Target", MB_OK);
			ShellExecute (g_hWnd, "open", (char *) m_szTarget, NULL, NULL, SW_SHOWNORMAL);
		} else {
			PostMessage (g_hWnd, WM_UICOMMAND, (WPARAM) g_controlID, 0);
		}
	}

	InvalidateControl (true);
	
}