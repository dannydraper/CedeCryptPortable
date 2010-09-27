#include "MultiContent.h"

MultiContent::MultiContent ()
{
	m_bVisible = true;
	m_bBold = false;
	m_bItalic = false;
	m_bUnderline = false;
	m_bStrikeout = false;
	m_iFontSize = 90;
	m_clTextColor = RGB (0, 0, 0);
	m_iLineBreakHeight = 0;
	m_bUseDiagnostics = true;
	m_bUsescroll = true;
	m_bNotificationsEnabled = true;
	ZeroMemory (m_szCurrentFont, SIZE_STRING);
	strcpy_s (m_szCurrentFont, SIZE_STRING, "Arial");
}

MultiContent::~MultiContent ()
{
}

unsigned long MultiContent::GetSize () {
	return sizeof (MultiContent);
}

void MultiContent::SetNotificationsEnabled (bool bStatus)
{
	m_bNotificationsEnabled = bStatus;
}

void MultiContent::SetDiagnostics (Diagnostics *pdiag)
{
	m_pdiag = pdiag;
	m_vscroll.SetDiagnostics (pdiag);
}

void MultiContent::OutputInt (LPCSTR lpszText, int iValue)
{
	if (m_bUseDiagnostics == true) {
		m_pdiag->OutputInt (lpszText, iValue);
	}
}

void MultiContent::OutputText (LPCSTR lpszText)
{
	if (m_bUseDiagnostics == true) {
		m_pdiag->OutputText (lpszText);
	}
}

void MultiContent::OutputText (LPCSTR lpszName, LPCSTR lpszValue)
{
	if (m_bUseDiagnostics == true) {
		m_pdiag->OutputText (lpszName, lpszValue);
	}
}

void MultiContent::SetMCProperties (HWND hWnd, int cID, int xPos, int yPos, int Width, int Height)
{
	m_hwnd = hWnd;

	SetProperties (hWnd, cID, xPos, yPos, Width, Height);

	m_uih.SetWindowProperties (xPos, yPos, Width, Height, RGB (110, 110, 110));
	
	RECT rctClip;
	rctClip.left = xPos+1;
	if (m_bUsescroll == true) {
		rctClip.right = xPos+Width-16;
	} else {
		rctClip.right = xPos+Width-1;
	}
	rctClip.top = yPos+1;
	rctClip.bottom = yPos+Height-1;
	
	m_uih.SetClippingArea (rctClip, RGB (228, 233, 240));

	if (m_bUsescroll == true) {
		m_vscroll.InitialiseDefault (hWnd, CID_MULTISCROLL+cID, xPos+Width-15, yPos, 15, Height);
		m_vscroll.SetAlwaysatend ();
		m_uih.AddControl (&m_vscroll);
		SetEndScrollPos ();
	}

	m_iWindowWidth = Width-25;
	m_iWindowHeight = Height-20;
	
	// initial position of the cursor reference
	m_iCursorXPos = 0;
	m_iCursorYPos = 5;

	//AddNotificationString ("Danhead says:");
	//AddString ("Hello, this is a long piece of text which hopefully should wrap around the window area and this.");
	
	//AddNotificationString ("Claire says:");

	//AddPicture (29, 30, false, true, IDB_EICSMILE1);
	
	//AddString ("and this is something that is very good, and this work is in the Lord's hands.");
	//AddString ("Hello, this is a long piece of text which hopefully should wrap around the window area and this.");
	//AddPicture (29, 30, false, true, IDB_EICSMILE1);

	//AddNotificationString ("Sarah says:");

	//AddString ("and this is something that is very good, and this");
	//AddPicture (29, 30, false, true, IDB_EICSMILE1);
	//AddString(" work is in the Lord's hands.");
	//AddHyperlink ("Test Hyperlink", "http://www.bbc.co.uk");
	//AddNotificationString ("Danhead says:");
	//AddPicture (29, 30, false, true, IDB_EICSMILE1);
	//AddNotificationString ("Claire says:");
	//AddString ("Hello, this is a long piece of text which hopefully should wrap around the window area and this.");
}

void MultiContent::SetVisible (bool bVisible)
{
	m_bVisible = bVisible;
	InvalidateControl (false);
}

void MultiContent::SetScrollbar (bool bUsescroll)
{
	m_bUsescroll = bUsescroll;
}

void MultiContent::SetEndScrollPos ()
{
	int iSlackoffset = 40;
	m_vscroll.SetMaxValue ((m_iCursorYPos-g_Height)+iSlackoffset);	

	if (m_bUsescroll == true) {
		if ((m_iCursorYPos-g_Height)+iSlackoffset > 0) {
			NotifyUIScroll (g_hWnd, CID_MULTISCROLL+g_controlID, (m_iCursorYPos-g_Height)+iSlackoffset);	
		}
	}
	//m_vscroll.Movetoend ();
}

SIZE MultiContent::GetTextSize (HWND hWnd, char *szCaption, unsigned long lStart, unsigned long lLength)
{
	SIZE tSize;
	RECT rctTextRect;
	RECT rctClippingRect;

	m_uih.GetClippingRect (&rctClippingRect);

	UILabel lblText;
	lblText.SetProperties (hWnd, 7997, 0, 0,0);
	lblText.SetTextProperties (TEXT ("Arial"), m_iFontSize, GetFontStyle (), 0, 0, szCaption, lStart, lLength, m_clTextColor);
	lblText.SetClippingProperties (rctClippingRect);
	
	lblText.GetTextRect (&rctTextRect);
	
	tSize.cx = rctTextRect.right - rctTextRect.left;
	tSize.cy = rctTextRect.bottom - rctTextRect.top;
	
	return tSize;
}

SIZE MultiContent::GetHyperlinkSize (HWND hWnd, char *szCaption)
{
	SIZE tSize;
	RECT rctTextRect;
	RECT rctClippingRect;

	m_uih.GetClippingRect (&rctClippingRect);

	UIHyperLink hplLink;
	hplLink.SetProperties (hWnd, 7997, 0, 0,0);
	hplLink.SetHyperlinkProperties (TEXT ("Arial"), m_iFontSize, szCaption, "http://www.nothing.com");
	hplLink.SetClippingProperties (rctClippingRect);
	
	hplLink.GetTextRect (&rctTextRect);
	
	tSize.cx = rctTextRect.right - rctTextRect.left;
	tSize.cy = rctTextRect.bottom - rctTextRect.top;
	
	return tSize;
}

int MultiContent::GetFontStyle ()
{
	int iFontstyle = 0;

	if (m_bBold == true && m_bItalic == false && m_bUnderline == false && m_bStrikeout == false) {
		iFontstyle = 1;
	}

	if (m_bBold == false && m_bItalic == true && m_bUnderline == false && m_bStrikeout == false) {
		iFontstyle = 2;
	}

	if (m_bBold == false && m_bItalic == false && m_bUnderline == true && m_bStrikeout == false) {
		iFontstyle = 4;
	}

	if (m_bBold == false && m_bItalic == false && m_bUnderline == false && m_bStrikeout == true) {
		iFontstyle = 8;
	}

	if (m_bBold == true && m_bItalic == true && m_bUnderline == false && m_bStrikeout == false) {
		iFontstyle = 1 | 2;
	}

	if (m_bBold == true && m_bItalic == false && m_bUnderline == true && m_bStrikeout == false) {
		iFontstyle = 1 | 4;
	}
	
	if (m_bBold == false && m_bItalic == true && m_bUnderline == true && m_bStrikeout == false) {
		iFontstyle = 2 | 4;
	}

	if (m_bBold == false && m_bItalic == false && m_bUnderline == true && m_bStrikeout == true) {
		iFontstyle = 8 | 4;
	}

	if (m_bBold == true && m_bItalic == false && m_bUnderline == false && m_bStrikeout == true) {
		iFontstyle = 1 | 8;
	}

	if (m_bBold == true && m_bItalic == true && m_bUnderline == true && m_bStrikeout == false) {
		iFontstyle = 1 | 2 | 4 ;
	}

	if (m_bBold == true && m_bItalic == true && m_bUnderline == true && m_bStrikeout == true) {
		iFontstyle = 1 | 2 | 4 | 8;
	}

	return iFontstyle;
}

void MultiContent::AddLabel (HWND hWnd, int iXPos, int iYPos, char *szCaption, LPRECT lpRect, unsigned long lStart, unsigned long lLength)
{
	RECT rctTextRect;
	RECT rctClippingRect;
	int iFontstyle = 0;

	iFontstyle = GetFontStyle ();
	
	m_uih.GetClippingRect (&rctClippingRect);

	UILabel lblText;
	lblText.SetProperties (hWnd, 7997, iXPos, iYPos, 0);
	lblText.SetTextProperties (m_szCurrentFont, m_iFontSize, iFontstyle, 0, 0, szCaption, lStart, lLength, m_clTextColor);
	lblText.SetClippingProperties (rctClippingRect);
	
	lblText.GetTextRect (&rctTextRect);
	lblText.SetYPositionOffset (0);

	m_uih.StoreControl (&lblText, true);

	SetEndScrollPos ();

	*lpRect = rctTextRect;
}

void MultiContent::AddSmallBreak ()
{
	m_iCursorYPos+=5;
	m_iCursorXPos=0;
}

void MultiContent::AddWord (HWND hWnd, char *szCaption, unsigned long lStart, unsigned long lLength)
{
	// Add a word to the document, which accounts for word wrapping.
	RECT txtRect;
	SIZE tSize;

	tSize = GetTextSize (hWnd, szCaption, lStart, lLength);	

	m_iLastTextHeight = tSize.cy; // Set the height of the last label so pictures know how far down to go.

	if (tSize.cx > (m_iWindowWidth - m_iCursorXPos)) {
		m_iCursorXPos = 0 + m_iIndent;
		if (m_iLineBreakHeight > 0) {
			m_iCursorYPos+=m_iLineBreakHeight;
			m_iLineBreakHeight = 0;
		} else {
			m_iCursorYPos+=tSize.cy;
		}
		
	
		if (strncmp (szCaption+lStart, " ", 1) == 0) { // if we have done a word wrap and the first char is a space then remove it.
			AddLabel (hWnd, m_iCursorXPos, m_iCursorYPos, szCaption, &txtRect, lStart+1, lLength-1); // skip past the first space
		} else {
			AddLabel (hWnd, m_iCursorXPos, m_iCursorYPos, szCaption, &txtRect, lStart, lLength); // don't do any skipping.
		}
		m_iCursorXPos+=txtRect.right-txtRect.left;
	} else {

		AddLabel (hWnd, m_iCursorXPos, m_iCursorYPos, szCaption, &txtRect, lStart, lLength);
		m_iCursorXPos+=tSize.cx;
	}
}

// DJD String manipulation function. Find a given string in another string, and return it's location in the string
int MultiContent::FindString (char *szStrtolook, char *szStrtofind) {
	char szTemp[SIZE_STRING];
	int i = 0;
	if (strlen (szStrtolook) >= strlen (szStrtofind)) {
		if (strlen (szStrtolook) > 0 && strlen (szStrtofind) > 0) {
			for (i=0;i<(strlen (szStrtolook) - strlen (szStrtofind))+1;i++) {
				ZeroMemory (szTemp, SIZE_STRING);
				strncpy_s (szTemp, SIZE_STRING, szStrtolook+i, strlen (szStrtofind));
				if (strcmp (szTemp, szStrtofind) == 0) {
					// The string has been found
					return i;
				}
			}
		}
	}
	return -1;
}

void MultiContent::AddWords (char *szString)
{
	HWND hWnd = g_hWnd;
	unsigned long lfirstPointer = 0;
			
	for (int c=0;c<strlen(szString);c++) {
		if (strncmp (szString+c, " ", 1) == 0) {
			AddWord (hWnd, szString, lfirstPointer, c-lfirstPointer);
			lfirstPointer = c;
		}

		if (c==strlen (szString)-1) {
			// We are at the end of the input string
			AddWord (hWnd, szString, lfirstPointer, c-(lfirstPointer-1));
		}
	}
	InvalidateControl (true);
}

void MultiContent::AddString (char *szString)
{
	AddWords (szString);	
}

void MultiContent::AddNotificationString (char *szString)
{
	if (m_iLineBreakHeight > 0) {
		m_iCursorYPos+=m_iLineBreakHeight;
		m_iLineBreakHeight=0;
	} else {
		m_iCursorYPos+=m_iLastTextHeight;
	}	
	SetIndent (2);
	SetTextForeColor (RGB (100, 100, 100));
	SetBold (false);
	SetItalic (false);
	SetUnderline (false);
	SetStrikeout (false);
	AddString (szString);
	SetTextForeColor (RGB (0, 0, 0));
	SetIndent (15);
	
	if (m_iLineBreakHeight > 0) {
		m_iCursorYPos+=m_iLineBreakHeight;
		m_iLineBreakHeight=0;
	} else {
		m_iCursorYPos+=m_iLastTextHeight;
	}
	
}

void MultiContent::AddPictureControl (HWND hWnd, int iXPos, int iYPos, int iWidth, int iHeight, unsigned int iResource)
{
	RECT rctClippingRect;

	m_uih.GetClippingRect (&rctClippingRect);

	UIPicButton picImage;
	picImage.SetBitmapResources (true, iResource, iResource, iResource);
	picImage.SetBitmapProperties (iWidth, iHeight);
	picImage.SetProperties (hWnd, 7996, iXPos, iYPos, iWidth);
	picImage.SetHighlightProperties (true, false);
	picImage.SetClippingProperties (rctClippingRect);
	m_uih.StoreControl (&picImage, true);
	SetEndScrollPos ();
}

void MultiContent::AddSmileyControl (HWND hWnd, int iXPos, int iYPos, int iWidth, int iHeight, unsigned int iResource, int cID)
{
	RECT rctClippingRect;

	m_uih.GetClippingRect (&rctClippingRect);

	UIPicButton picImage;
	picImage.SetBitmapResources (true, iResource, iResource, iResource);
	picImage.SetBitmapProperties (iWidth, iHeight);
	picImage.SetProperties (hWnd, cID, iXPos, iYPos, iWidth);
	picImage.SetRectHighlight (true);
	picImage.SetHighlightProperties (true, false);
	picImage.SetClippingProperties (rctClippingRect);
	m_uih.StoreControl (&picImage, true);
	SetEndScrollPos ();
}

void MultiContent::AddPicture (int iWidth, int iHeight, bool bCenter, bool Inline, unsigned int iResource, bool issmiley, int cID)
{
	HWND hWnd = g_hWnd;
	SIZE tSize;

	tSize.cx = iWidth;
	tSize.cy = iHeight;

	if (Inline == true) {
		if (iWidth > (m_iWindowWidth - m_iCursorXPos)) {
			m_iCursorXPos = 0 + m_iIndent;
			if (m_iLineBreakHeight > 0) {
				m_iCursorYPos+=m_iLineBreakHeight;
				m_iLineBreakHeight = 0;
			} else {
				m_iCursorYPos+=m_iLastTextHeight;
			}
			
			if (issmiley == false) {
				AddPictureControl (hWnd, m_iCursorXPos, m_iCursorYPos, iWidth, iHeight, iResource);
			} else {
				AddSmileyControl (hWnd, m_iCursorXPos, m_iCursorYPos, iWidth, iHeight, iResource, cID);
			}
			m_iCursorXPos+=iWidth;
			m_iLineBreakHeight = iHeight;
		} else {
			if (issmiley == false) {
				AddPictureControl (hWnd, m_iCursorXPos, m_iCursorYPos, iWidth, iHeight, iResource);
			} else {
				AddSmileyControl (hWnd, m_iCursorXPos, m_iCursorYPos, iWidth, iHeight, iResource, cID);
			}
			
			m_iCursorXPos+=iWidth;
			m_iLineBreakHeight = iHeight;
		}
		
		
	} else {
		if (bCenter == true) {
			m_iCursorXPos = (m_iWindowWidth / 2) - (iWidth / 2);
		} else {
			m_iCursorXPos = 0;
		}
		
		m_iCursorYPos+=m_iLastTextHeight;
		if (issmiley == false) {
			AddPictureControl (hWnd, m_iCursorXPos, m_iCursorYPos, iWidth, iHeight, iResource);
		} else {
			AddSmileyControl (hWnd, m_iCursorXPos, m_iCursorYPos, iWidth, iHeight, iResource, cID);
		}
		
		m_iCursorYPos+=tSize.cy;
	}
	SetEndScrollPos ();
}

void MultiContent::AddHyperLinkControl (HWND hWnd, int iXPos, int iYPos, char *szCaption, char *szTarget, LPRECT lpRect)
{
	RECT rctTextRect;
	RECT rctClippingRect;

	m_uih.GetClippingRect (&rctClippingRect);

	UIHyperLink hplLink;
	hplLink.SetProperties (hWnd, 7994, iXPos, iYPos,0);
	hplLink.SetHyperlinkProperties (TEXT ("Arial"), m_iFontSize, szCaption, szTarget);
	hplLink.GetTextRect (&rctTextRect);	
	hplLink.SetClippingProperties (rctClippingRect);
	
	m_uih.StoreControl (&hplLink, true);

	SetEndScrollPos ();

	*lpRect = rctTextRect;
}

void MultiContent::AddHyperlink (char *szCaption, char *szTarget)
{
	// Add a hyperlink to the document, no word wrapping included.
	HWND hWnd = g_hWnd;
	RECT txtRect;
	SIZE tSize;

	tSize = GetHyperlinkSize (hWnd, szCaption);

	m_iLastTextHeight = tSize.cy; // Set the height of the last hyperlink so pictures know how far down to go.

	if (tSize.cx > (m_iWindowWidth - m_iCursorXPos)) {
		m_iCursorXPos = 0 + m_iIndent;
		m_iCursorYPos+=tSize.cy;
		AddHyperLinkControl (hWnd, m_iCursorXPos, m_iCursorYPos, szCaption, szTarget, &txtRect);
		m_iCursorXPos+=txtRect.right-txtRect.left;
	} else {
		AddHyperLinkControl (hWnd, m_iCursorXPos, m_iCursorYPos, szCaption, szTarget, &txtRect);
		m_iCursorXPos+=tSize.cx;		
	}
}

void MultiContent::Clear ()
{
	m_uih.ClearStoredControls ();
	
	// initial position of the cursor reference
	m_iCursorXPos = 0;
	m_iCursorYPos = 5;
	m_iFontSize = 90;
	m_iIndent = 0;
}

void MultiContent::SetItalic (bool bItalic)
{
	m_bItalic = bItalic;
}

void MultiContent::SetBold (bool bBold)
{
	
	m_bBold = bBold;
}

void MultiContent::SetUnderline (bool bUnderline)
{
	m_bUnderline = bUnderline;		
}

void MultiContent::SetStrikeout (bool bStrikeout)
{
	m_bStrikeout = bStrikeout;
}

void MultiContent::SetFontFace (char *szFontname)
{
	strcpy_s (m_szCurrentFont, SIZE_STRING, szFontname);
}

void MultiContent::SetFontSize (int iSize)
{
	m_iFontSize = iSize;
}

void MultiContent::SetTextForeColor (COLORREF clColor)
{
	m_clTextColor = clColor;
}

void MultiContent::SetIndent (int iVal)
{
	m_iIndent = iVal;
	m_iCursorXPos = m_iIndent;
}

void MultiContent::Paint (HDC hdc)
{
	if (m_bVisible == false) {
		return;
	}

	//HBRUSH bgColour = CreateSolidBrush (RGB (228, 233, 240));
	//RECT fRect;
	//fRect.top = g_yPos;
	//fRect.left = g_xPos;
	//fRect.right = g_xPos+g_Width;
	//fRect.bottom = g_yPos+g_Height;

	//FillRect (hdc, &fRect, bgColour);

	//DeleteObject (bgColour);

	//m_vscroll.Paint (hdc);

	//if (m_bNotificationsEnabled == true) {
		m_uih.PainttoHDC (hdc);	
	//}
}

void MultiContent::NotifyUIScroll (HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	int controlID = wParam;
	int scrollPos = lParam;

	if (controlID == (g_controlID+CID_MULTISCROLL)) {
		//Beep (500+scrollPos, 4);
			
		RECT rctClip;
		DynList *pdlStoredControls;
		UIControl *pUIControl;

		pdlStoredControls = (DynList *) m_uih.GetStoredControls ();		

		for (int s=0;s<pdlStoredControls->GetNumItems ();s++)
		{
			pUIControl = (UIControl *) pdlStoredControls->GetItem (s);				
			pUIControl->SetYPositionOffset (scrollPos);
		}

		m_uih.GetClippingRect (&rctClip);
		InvalidateRect (hWnd, &rctClip, false);
	}

	/*
	switch (controlID) {
		case (CID_MULTISCROLL+g_controlID):
		{
			
		}
		break;
	}
	*/
}

void MultiContent::NotifyTimer (unsigned int wParam)
{	
	//if (wParam == TMR_FADE+g_controlID) {
		
	//}
	if (m_bNotificationsEnabled == true) {
		m_uih.NotifyTimer (wParam);
	}
}

void MultiContent::NotifyMouseMove (int iXPos, int iYPos)
{
	g_mouseXPos = iXPos;
	g_mouseYPos = iYPos;
	
	if (m_bVisible == false) {
		return;
	}
	
	if (m_bNotificationsEnabled == true) {

		//m_vscroll.NotifyMouseMove (iXPos, iYPos);
		m_uih.NotifyMouseMove (iXPos, iYPos);
	}
}

void MultiContent::NotifyMouseDown ()
{
	if (m_bVisible == false) {
		return;
	}

	if (m_bNotificationsEnabled == true) {
		//m_vscroll.NotifyMouseDown ();
		m_uih.NotifyMouseDown ();
	}
}

void MultiContent::NotifyMouseUp ()
{
	if (m_bVisible == false) {
		return;
	}
	
	if (m_bNotificationsEnabled == true) {
		//m_vscroll.NotifyMouseUp ();
		m_uih.NotifyMouseUp ();
	}
}
