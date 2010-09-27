#include "UIHandler.h"

UIHandler::UIHandler ()
{
	m_iWidth = 0;
	m_iHeight = 0;
	m_iMouseX = 0;
	m_iMouseY = 0;
	m_bUseClipping = false;
	m_bUseClipBgColor = false;
	m_bUseStoredControls = false;
}

UIHandler::~UIHandler ()
{
}

void UIHandler::SetClippingArea (RECT rctClipping)
{
	m_rctClippingArea = rctClipping;
	m_bUseClipping = true;
}

void UIHandler::SetClippingArea (RECT rctClipping, COLORREF bgColor)
{
	m_rctClippingArea = rctClipping;
	m_bUseClipping = true;
	m_bgClipColor = bgColor;
	m_bUseClipBgColor = true;
}

void UIHandler::GetClippingRect (LPRECT lpRect)
{
	RECT clpRect;
	clpRect.top = m_rctClippingArea.top;
	clpRect.left = m_rctClippingArea.left;
	clpRect.bottom = m_rctClippingArea.bottom;
	clpRect.right = m_rctClippingArea.right;
	
	*lpRect = clpRect;
}

DynList *UIHandler::GetStoredControls ()
{
	return &m_dlStoredControls;
}

void UIHandler::ClearStoredControls ()
{
	m_dlStoredControls.Clear ();
	m_dlStoredControlclipping.Clear ();
	m_bUseStoredControls = false;
}

void UIHandler::SetWindowProperties (int xPos, int yPos, int Width, int Height, COLORREF bgColor)
{
	// This function must be called so that this lovely UI Handler knows the size
	// of the window and desired background colour.
	m_iXpos = xPos;
	m_iYpos = yPos;
	m_iWidth = Width;
	m_iHeight = Height;
	m_bgColor = bgColor;
}

void UIHandler::ShowInt (int iInttoShow) 
{
	char szMsg[255];
	ZeroMemory (szMsg, 255);
	sprintf_s (szMsg, 255, "Value of int: %d", iInttoShow);
	MessageBox (NULL, szMsg, "ShowInt", MB_OK);
}

void UIHandler::AddControl (UIControl *pUIControl)
{
	bool UseClipping = false; // Default setting for this override.
	m_dlControls.AddItem (pUIControl, sizeof (UIControl *), true);
	m_dlControlclipping.AddItem (&UseClipping, sizeof (bool), false);
}

void UIHandler::AddControl (UIControl *pUIControl, bool bUseClipping)
{
	m_dlControls.AddItem (pUIControl, sizeof (UIControl *), true);
	m_dlControlclipping.AddItem (&bUseClipping, sizeof (bool), false);
}

void UIHandler::AddDirectControl (UIControl *pUIControl)
{	
	m_dlDirectControls.AddItem (pUIControl, sizeof (UIControl *), true);
}

void UIHandler::StoreControl (UIControl *pUIControl, bool bUseClipping)
{
	m_bUseStoredControls = true;
	m_dlStoredControls.AddItem (pUIControl, pUIControl->GetSize (), false);
	m_dlStoredControlclipping.AddItem (&bUseClipping, sizeof (bool), false);
}

void UIHandler::PaintControls (HWND hWnd)
{
	RECT myRect;
	RECT ctlRect;
	RECT updRect;
	RECT intRect;
	PAINTSTRUCT ps;
	bool bClippingUsed = false;

	// Very important for performance reasons - we need to get the rect of the region that actually needs painting
	// this is so that we can tell the correct controls to repaint themselves. 
	GetUpdateRect (hWnd, &updRect, false);

	HBITMAP hbmOld;
	HDC hdc = BeginPaint (hWnd, &ps);

	// Background Brush Variables
	HBRUSH bgBrush = CreateSolidBrush (m_bgColor);
	HBRUSH bgClipBrush = CreateSolidBrush (m_bgClipColor);

	// DC variables for a clipping backbuffer
	HDC hdcClipBuf;
	HBITMAP hbmClipOld;
	HBITMAP hbmClipTemp;

	// This nice little UI Paint function is actually doing some Double Buffering.
	// We're painting all of our controls to a backbuffer first, and then when this is all
	// done, we blit the entire back buffer to the screen, which is hdc in this case.
	// This is done so we don't get any nasty flickering when moving scrollbars or other controls.

	HDC hdcBuf = CreateCompatibleDC (hdc);
	HBITMAP hbmTemp = CreateCompatibleBitmap (hdc, m_iWidth, m_iHeight);
	hbmOld = (HBITMAP) SelectObject (hdcBuf, hbmTemp);

	// If Clipping Properties have been set then we need to create a separate back buffer
	// for the clipped area.
	if (m_bUseClipping == true) {
		hdcClipBuf = CreateCompatibleDC (hdc);
		hbmClipTemp = CreateCompatibleBitmap (hdc, m_rctClippingArea.right-m_rctClippingArea.left, m_rctClippingArea.bottom-m_rctClippingArea.top);
		hbmClipOld = (HBITMAP) SelectObject (hdcClipBuf, hbmClipTemp);
	}

	UIControl *pcontrol;
	UIControl *tmpControl;	
	int s = 0;

	// FloodFill the window first with our desired background colour.
	// We have to do this otherwise the background appears to be black, and this is because
	// of the CreateCompatibleBitmap function above.
	myRect.top = 0;
	myRect.left = 0;
	myRect.bottom = m_iHeight;
	myRect.right = m_iWidth;

	FillRect(hdcBuf, &myRect, bgBrush);

	// Cycle through the internal DynList of direct control pointers
	// and paint each one of them directly to the hdc without first going through the backbuffer.
	for (int d=0;d<m_dlDirectControls.GetNumItems ();d++)
	{		
		m_dlDirectControls.GetItem (&pcontrol, d);			
		pcontrol->GetControlRect (&ctlRect); // Get the bounding rect for the current control
		if (IntersectRect (&intRect, &ctlRect, &updRect) != 0) { // Check if the update region intersects with this control
			pcontrol->Paint (hdc); // If the update region intersects with this control then repaint the control.
		}		
	}

	// Cycle through the internal DynList of control pointers
	// and paint each one of them to the supplied window handle.
	for (int m=0;m<m_dlControls.GetNumItems ();m++)
	{
		m_dlControlclipping.GetItem (&bClippingUsed, m);
		if (bClippingUsed == false) { // We only want to paint controls that have not been set as clipped controls.
			m_dlControls.GetItem (&pcontrol, m);
			
			pcontrol->GetControlRect (&ctlRect); // Get the bounding rect for the current control
			if (IntersectRect (&intRect, &ctlRect, &updRect) != 0) { // Check if the update region intersects with this control
				pcontrol->Paint (hdcBuf); // If the update region intersects with this control then repaint the control.
			}
		}
	}
	
	// Cycle through all stored controls that do not have clipping applied
	// and paint these to the buffer.
	if (m_bUseStoredControls == true) {
		
		for (s=0;s<m_dlStoredControls.GetNumItems ();s++)
		{
			m_dlStoredControlclipping.GetItem (&bClippingUsed, s);
			if (bClippingUsed == false) { // Get only stored controls that do not have clipping applied.					
				tmpControl = (UIControl *) m_dlStoredControls.GetItem (s);

				tmpControl->GetControlRect (&ctlRect); // Get the bounding rect for the current control
				if (IntersectRect (&intRect, &ctlRect, &updRect) != 0) { // Check if the update region intersects with this control
					tmpControl->Paint (hdcBuf); // If the update region intersects with this control then repaint the control.
				}

				tmpControl->Paint (hdcBuf);
			}
		}
	}

	// Now we cycle through all controls that are set as clipped controls.
	// Clipped controls are draw onto a separate dc buffer. The entire clipped buffer is then blitted to the
	// primary backbuffer, just before the back buffer is blitted to the screen.
	if (m_bUseClipping == true) {

		// If a background colour for the clipping area has been set, then fill the clip buffer in this colour
		// first.
		if (m_bUseClipBgColor == true) {
			FillRect(hdcClipBuf, &myRect, bgClipBrush);
		}

		for (int c=0;c<m_dlControls.GetNumItems ();c++)
		{
			m_dlControlclipping.GetItem (&bClippingUsed, c);
			if (bClippingUsed == true) { // This time we're interested in controls that are set as clipped controls.
				m_dlControls.GetItem (&pcontrol, c);
				pcontrol->Paint (hdcClipBuf);
			}
		}

		if (m_bUseStoredControls == true) {

			for (s=0;s<m_dlStoredControls.GetNumItems ();s++)
			{
				m_dlStoredControlclipping.GetItem (&bClippingUsed, s);
				if (bClippingUsed == true) { // Get only stored controls that have clipping applied.					
					tmpControl = (UIControl *) m_dlStoredControls.GetItem (s);
					//if (tmpControl->IsWithinRect (m_rctClippingArea, true) == true) {
					if (tmpControl->IsWithinRect (0, m_rctClippingArea.right - m_rctClippingArea.left, 0, m_rctClippingArea.bottom, true) == true) {
						tmpControl->Paint (hdcClipBuf);
					}
				}
			}
		}

		// Blit the clipped buffer to the primary backbuffer
		BitBlt (hdcBuf, m_rctClippingArea.left, m_rctClippingArea.top, m_rctClippingArea.right-m_rctClippingArea.left, m_rctClippingArea.bottom-m_rctClippingArea.top, hdcClipBuf, 0, 0, SRCCOPY);
	}

	// Blit the entire backbuffer to the screen.
	BitBlt (hdc, m_iXpos, m_iYpos, m_iWidth, m_iHeight, hdcBuf, m_iXpos, m_iYpos, SRCCOPY);

	// Clean up any clipping objects
	if (m_bUseClipping == true) {
		SelectObject (hdcClipBuf, hbmClipOld);
	}

	// Clean up
	DeleteDC (hdcClipBuf);
	DeleteObject (hbmClipTemp);
	DeleteObject (hbmClipOld);

	SelectObject (hdcBuf, hbmOld);
	DeleteDC (hdcBuf);
	DeleteDC (hdc);
	DeleteObject (hbmTemp);
	DeleteObject (hbmOld);
	DeleteObject (bgBrush);
	DeleteObject (bgClipBrush);
	
	EndPaint (hWnd, &ps);
}

void UIHandler::PainttoHDC (HDC hdc)
{
	RECT myRect;
	RECT clpRect;
	RECT ctlRect;
	RECT intRect;
	PAINTSTRUCT ps;
	bool bClippingUsed = false;

	HBITMAP hbmOld;

	// Background Brush Variables
	HBRUSH bgBrush = CreateSolidBrush (m_bgColor);
	HBRUSH bgClipBrush = CreateSolidBrush (m_bgClipColor);

	// DC variables for a clipping backbuffer
	HDC hdcClipBuf;
	HBITMAP hbmClipOld;
	HBITMAP hbmClipTemp;

	// This nice little UI Paint function is actually doing some Double Buffering.
	// We're painting all of our controls to a backbuffer first, and then when this is all
	// done, we blit the entire back buffer to the screen, which is hdc in this case.
	// This is done so we don't get any nasty flickering when moving scrollbars or other controls.

	HDC hdcBuf = CreateCompatibleDC (hdc);
	HBITMAP hbmTemp = CreateCompatibleBitmap (hdc, m_iWidth+m_iXpos, m_iHeight+m_iYpos);
	hbmOld = (HBITMAP) SelectObject (hdcBuf, hbmTemp);

	// If Clipping Properties have been set then we need to create a separate back buffer
	// for the clipped area.
	if (m_bUseClipping == true) {
		hdcClipBuf = CreateCompatibleDC (hdc);
		hbmClipTemp = CreateCompatibleBitmap (hdc, m_rctClippingArea.right-m_rctClippingArea.left, m_rctClippingArea.bottom-m_rctClippingArea.top);
		hbmClipOld = (HBITMAP) SelectObject (hdcClipBuf, hbmClipTemp);
	}

	UIControl *pcontrol;
	UIControl *tmpControl;
	int s = 0;

	// FloodFill the window first with our desired background colour.
	// We have to do this otherwise the background appears to be black, and this is because
	// of the CreateCompatibleBitmap function above.
	myRect.top = m_iYpos;
	myRect.left = m_iXpos;
	myRect.bottom = m_iYpos+m_iHeight;
	myRect.right = m_iXpos+m_iWidth;

	clpRect.top = 0;
	clpRect.left = 0;
	clpRect.right = m_iWidth;
	clpRect.bottom = m_iHeight;

	FillRect(hdcBuf, &myRect, bgBrush);

	// Cycle through the internal DynList of direct control pointers
	// and paint each one of them directly to the hdc without first going through the backbuffer.
	for (int d=0;d<m_dlDirectControls.GetNumItems ();d++)
	{		
		m_dlDirectControls.GetItem (&pcontrol, d);			
		pcontrol->GetControlRect (&ctlRect); // Get the bounding rect for the current control
		pcontrol->Paint (hdc); // If the update region intersects with this control then repaint the control.
	}

	// Cycle through the internal DynList of control pointers
	// and paint each one of them to the supplied window handle.
	for (int m=0;m<m_dlControls.GetNumItems ();m++)
	{
		m_dlControlclipping.GetItem (&bClippingUsed, m);
		if (bClippingUsed == false) { // We only want to paint controls that have not been set as clipped controls.
			m_dlControls.GetItem (&pcontrol, m);
			
			pcontrol->GetControlRect (&ctlRect); // Get the bounding rect for the current control			
			pcontrol->Paint (hdcBuf); // If the update region intersects with this control then repaint the control.
		}
	}
	
	// Cycle through all stored controls that do not have clipping applied
	// and paint these to the buffer.
	if (m_bUseStoredControls == true) {
		
		for (s=0;s<m_dlStoredControls.GetNumItems ();s++)
		{
			m_dlStoredControlclipping.GetItem (&bClippingUsed, s);
			if (bClippingUsed == false) { // Get only stored controls that do not have clipping applied.					
				tmpControl = (UIControl *) m_dlStoredControls.GetItem (s);				
				tmpControl->Paint (hdcBuf); // If the update region intersects with this control then repaint the control.				
			}
		}
	}

	// Now we cycle through all controls that are set as clipped controls.
	// Clipped controls are draw onto a separate dc buffer. The entire clipped buffer is then blitted to the
	// primary backbuffer, just before the back buffer is blitted to the screen.
	if (m_bUseClipping == true) {

		// If a background colour for the clipping area has been set, then fill the clip buffer in this colour
		// first.
		if (m_bUseClipBgColor == true) {
			FillRect(hdcClipBuf, &clpRect, bgClipBrush);
		}

		for (int c=0;c<m_dlControls.GetNumItems ();c++)
		{
			m_dlControlclipping.GetItem (&bClippingUsed, c);
			if (bClippingUsed == true) { // This time we're interested in controls that are set as clipped controls.
				m_dlControls.GetItem (&pcontrol, c);
				pcontrol->Paint (hdcClipBuf);
			}
		}

		if (m_bUseStoredControls == true) {

			for (s=0;s<m_dlStoredControls.GetNumItems ();s++)
			{
				m_dlStoredControlclipping.GetItem (&bClippingUsed, s);
				if (bClippingUsed == true) { // Get only stored controls that have clipping applied.					
					tmpControl = (UIControl *) m_dlStoredControls.GetItem (s);
					//if (tmpControl->IsWithinRect (m_rctClippingArea, true) == true) {
					if (tmpControl->IsWithinRect (0, m_rctClippingArea.right - m_rctClippingArea.left, 0, m_rctClippingArea.bottom, true) == true) {
						tmpControl->Paint (hdcClipBuf);
					}
				}
			}
		}

		// Blit the clipped buffer to the primary backbuffer
		BitBlt (hdcBuf, m_rctClippingArea.left, m_rctClippingArea.top, m_rctClippingArea.right-m_rctClippingArea.left, m_rctClippingArea.bottom-m_rctClippingArea.top, hdcClipBuf, 0, 0, SRCCOPY);
	}

	// Blit the entire backbuffer to the screen.
	BitBlt (hdc, m_iXpos, m_iYpos, m_iWidth, m_iHeight, hdcBuf, m_iXpos, m_iYpos, SRCCOPY);

	// Clean up any clipping objects
	if (m_bUseClipping == true) {
		SelectObject (hdcClipBuf, hbmClipOld);
	}

	// Clean up
	DeleteDC (hdcClipBuf);
	DeleteObject (hbmClipTemp);
	DeleteObject (hbmClipOld);

	SelectObject (hdcBuf, hbmOld);
	DeleteDC (hdcBuf);
	DeleteObject (hbmTemp);
	DeleteObject (hbmOld);
	DeleteObject (bgBrush);
	DeleteObject (bgClipBrush);	
}

void UIHandler::NotifyMouseMove (int iXPos, int iYPos)
{
	UIControl *pcontrol;
	
	// Update our internal ints with the position of the mouse.
	// we need these so that when a NotifyMouseUp or Down is received, we can verify if we should actually
	// notify the control by checking if the mousex and y are within the control boundaries.
	m_iMouseX = iXPos;
	m_iMouseY = iYPos;

	// Cycle through the internal list of direct controls, and notify them of mouse movements.
	for (int d=0;d<m_dlDirectControls.GetNumItems ();d++)
	{		
		m_dlDirectControls.GetItem (&pcontrol, d);			
		pcontrol->NotifyMouseMove (m_iMouseX, m_iMouseY);	
	}

	// Cycle through the internal DynList of control pointers
	// and notify each one of them of this event.
	for (int m=0;m<m_dlControls.GetNumItems ();m++)
	{
		m_dlControls.GetItem (&pcontrol, m);
	
		// Commented out because a control needs to know if the mouse has moved out of it's area for highlighting purposes.
		//if (pcontrol->IsMouseRect (iXPos, iYPos) == true) { // Check if the mouse is actually within the control rect
			// we don't need to inform every single control of a mouse move if the mouse is only over one control.
			pcontrol->NotifyMouseMove (m_iMouseX, m_iMouseY);
		//}
	}

	// Now check if we're using stored controls and cycle through each of the stored
	// controls and inform them of the mouse event.
	if (m_bUseStoredControls == true) {
		UIControl *tmpControl;
		for (int s=0;s<m_dlStoredControls.GetNumItems ();s++)
		{			
			tmpControl = (UIControl *) m_dlStoredControls.GetItem (s);
			tmpControl->NotifyMouseMove (m_iMouseX, m_iMouseY);
		}
	}
}

void UIHandler::NotifyMouseDown ()
{
	UIControl *pcontrol;

	// Cycle through the internal list of direct controls, and notify them of mouse events.
	for (int d=0;d<m_dlDirectControls.GetNumItems ();d++)
	{		
		m_dlDirectControls.GetItem (&pcontrol, d);			
		if (pcontrol->IsMouseRect (m_iMouseX, m_iMouseY) == true) { // Check if the mouse is actually within the control rect
			pcontrol->NotifyMouseDown ();
		}
	}

	// Cycle through the internal DynList of control pointers
	// and notify each one of them of this event.
	for (int m=0;m<m_dlControls.GetNumItems ();m++)
	{
		m_dlControls.GetItem (&pcontrol, m);

		
		if (pcontrol->IsMouseRect (m_iMouseX, m_iMouseY) == true) { // Check if the mouse is actually within the control rect
			pcontrol->NotifyMouseDown ();
		}

	}

	// Now check if we're using stored controls and cycle through each of the stored
	// controls and inform them of the mouse event.
	if (m_bUseStoredControls == true) {
		UIControl *tmpControl;
		for (int s=0;s<m_dlStoredControls.GetNumItems ();s++)
		{			
			tmpControl = (UIControl *) m_dlStoredControls.GetItem (s);
			tmpControl->NotifyMouseDown ();
		}
	}
}

void UIHandler::NotifyMouseUp ()
{
	UIControl *pcontrol;


	// Cycle through the internal list of direct controls, and notify them of mouse events.
	for (int d=0;d<m_dlDirectControls.GetNumItems ();d++)
	{		
		m_dlDirectControls.GetItem (&pcontrol, d);			
		//if (pcontrol->IsMouseRect (m_iMouseX, m_iMouseY) == true) { // Check if the mouse is actually within the control rect
			pcontrol->NotifyMouseUp ();
		//}
	}

	// Cycle through the internal DynList of control pointers
	// and notify each one of them of this event.
	for (int m=0;m<m_dlControls.GetNumItems ();m++)
	{
		m_dlControls.GetItem (&pcontrol, m);
		//if (pcontrol->IsMouseRect (m_iMouseX, m_iMouseY) == true) { // Check if the mouse is actually within the control rect
			pcontrol->NotifyMouseUp ();
		//}
		
	}

	// Now check if we're using stored controls and cycle through each of the stored
	// controls and inform them of the mouse event.
	if (m_bUseStoredControls == true) {
		UIControl *tmpControl;
		for (int s=0;s<m_dlStoredControls.GetNumItems ();s++)
		{			
			tmpControl = (UIControl *) m_dlStoredControls.GetItem (s);
			tmpControl->NotifyMouseUp ();
		}
	}
}

void UIHandler::NotifyTimer (unsigned int wParam)
{
	UIControl *pcontrol;

	// Cycle through the internal list of direct controls, and notify them of mouse events.
	for (int d=0;d<m_dlDirectControls.GetNumItems ();d++)
	{		
		m_dlDirectControls.GetItem (&pcontrol, d);			
		pcontrol->NotifyTimer (wParam);
	}

	// Cycle through the internal DynList of control pointers
	// and notify each one of them of this event.
	for (int m=0;m<m_dlControls.GetNumItems ();m++)
	{
		m_dlControls.GetItem (&pcontrol, m);
		pcontrol->NotifyTimer (wParam);

	}

	// Now check if we're using stored controls and cycle through each of the stored
	// controls and inform them of the mouse event.
	if (m_bUseStoredControls == true) {
		UIControl *tmpControl;
		for (int s=0;s<m_dlStoredControls.GetNumItems ();s++)
		{			
			tmpControl = (UIControl *) m_dlStoredControls.GetItem (s);
			tmpControl->NotifyTimer (wParam);
		}
	}
}

void UIHandler::AddCommandButton(HWND hWnd, UIPicButton *picbutton, unsigned int controlID, int xPos, int yPos, char *szCaption, int txOffset, int tyOffset)
{
	picbutton->SetBitmapResources (false, IDB_STDBTNBASE, IDB_STDBTNHIGH, IDB_STDBTNDOWN);
	picbutton->SetBitmapProperties (0, 7, 6, 7, 17, 7, 21, 24);
	picbutton->SetProperties (hWnd, controlID, xPos, yPos, 230);
	picbutton->SetHighlightProperties (true, false);
	picbutton->SetTextProperties (TEXT ("Arial"), 85, 0, txOffset, tyOffset, szCaption, RGB (0, 0, 0));
	
	//AddControl (picbutton);
	AddDirectControl (picbutton);
	
	//m_uihandler.AddControl (&m_stdexitbtn);
}

void UIHandler::AddStdCommandButton(HWND hWnd, UIPicButton *picbutton, unsigned int controlID, int xPos, int yPos, char *szCaption, int txOffset, int tyOffset)
{
	picbutton->SetBitmapResources (false, IDB_STDBTNBASE, IDB_STDBTNHIGH, IDB_STDBTNDOWN);
	picbutton->SetBitmapProperties (0, 7, 6, 7, 17, 7, 21, 24);
	picbutton->SetProperties (hWnd, controlID, xPos, yPos, 230);
	picbutton->SetHighlightProperties (true, false);
	picbutton->SetTextProperties (TEXT ("Arial"), 85, 0, txOffset, tyOffset, szCaption, RGB (0, 0, 0));
	
	AddControl (picbutton);
	//AddDirectControl (picbutton);
	
	//m_uihandler.AddControl (&m_stdexitbtn);
}

void UIHandler::AddCommandBarButton (HWND hWnd, UIPicButton *picbutton, unsigned int controlID, int xPos, int yPos, unsigned int iResNorm, unsigned int iResHigh, unsigned int iResDown)
{
	picbutton->SetBitmapResources (true, iResNorm, iResHigh, iResDown);
	picbutton->SetBitmapProperties (32, 30);
	picbutton->SetProperties (hWnd, controlID, xPos, yPos, 32, 30);
	picbutton->SetHighlightProperties (true, false);	
	
	AddControl (picbutton);
}
void UIHandler::AddCommandBarButtonHorz (HWND hWnd, UIPicButton *picbutton, unsigned int controlID, int xPos, int yPos, unsigned int iResNorm, unsigned int iResHigh, unsigned int iResDown)
{
	picbutton->SetBitmapResources (true, iResNorm, iResHigh, iResDown);
	picbutton->SetBitmapProperties (32, 32);
	picbutton->SetProperties (hWnd, controlID, xPos, yPos, 32, 32);
	picbutton->SetHighlightProperties (true, false);	
	
	AddControl (picbutton);
}

