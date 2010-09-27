#include "UIPicButton.h"

UIPicButton::UIPicButton ()
{
	// Blend settings
	g_blendop.BlendOp = AC_SRC_OVER;
	g_blendop.BlendFlags = 0;
	g_blendop.AlphaFormat = NULL;

	g_bTimeractive = false;
	g_bMouseover = false;
	g_bHighlighted = false;
	g_bDepressed = false;
	g_bHighlightlocked = false;
	
	// Default transparency settings
	m_clTranskey = RGB (255, 0, 255);
	m_bUsingTransparency = false;
	m_bVisible = true;
	m_bTogglemode = false;
	m_bUseRectHighlight = false;
}

UIPicButton::~UIPicButton ()
{
}

unsigned long UIPicButton::GetSize () {
	return sizeof (UIPicButton);
}

void UIPicButton::SetVisible (bool bVisible)
{
	m_bVisible = bVisible;
	InvalidateControl (false);
}

void UIPicButton::SetBitmapResources (bool bSingleBitmap, unsigned int wResbase, unsigned int wReshighlight, unsigned int wResdown)
{
	// Load the bitmap resources. Three resources are required, one for the base button image, one for the
	// highlighted button image, and one for the mouse down button image.
	g_hbmBase = LoadBitmapResource (wResbase);
	g_hbmHighlight = LoadBitmapResource (wReshighlight);
	g_hbmDown = LoadBitmapResource (wResdown);

	// The single bitmap flag tells this class how to draw the button bitmap. A single image is where the entire button
	// image is provided in it's entirity within the provided resources. Therefore the width and height of the button
	// is restricted to that in the resource. If singlebitmap is false then a 3 column tiled bitmap is assumed where the
	// columns 1 and 3 of the provided bitmap are the first and last segments and column 2 is the middle segment which
	// is repeated as many times as the requested width.
	g_bSinglebitmap = bSingleBitmap;
}

void UIPicButton::SetBitmapProperties (int c1xStart, int c1xWidth, int c2xStart, int c2xWidth, int c3xStart, int c3xWidth, int Height, int Width)
{
	g_c1xStart = c1xStart;
	g_c1xWidth = c1xWidth;
	g_c2xStart = c2xStart;
	g_c2xWidth = c2xWidth;
	g_c3xStart = c3xStart;
	g_c3xWidth = c3xWidth;
	g_iBitmapheight = Height;
	g_iBitmapwidth = Width;
}

void UIPicButton::SetBitmapProperties (int Width, int Height)
{
	g_iBitmapwidth = Width;
	g_iBitmapheight = Height;
}

void UIPicButton::SetToggleMode (bool bUsetoggle)
{
	m_bTogglemode = bUsetoggle;
}

void UIPicButton::SetRectHighlight (bool bUseRectHighlight)
{
	m_bUseRectHighlight = bUseRectHighlight;
}

void UIPicButton::SetTransparencyKey (COLORREF clTransparentKey)
{
	m_clTranskey = clTransparentKey;
	// Set our member variable of using transparent to true. This causes the blitting methods to use the
	// TransparentBlt Win32 API call instead of the standard BitBlt call.
	m_bUsingTransparency = true;
}

void UIPicButton::Blit (HDC hdc, int xPos, int yPos, int Width, int Height, HDC srcDC, int srcxPos, int srcyPos, int srcWidth, int srcHeight, int Method, BLENDFUNCTION blendop)
{
	if (Method == BLITMETHOD_STANDARD) {
		if (m_bUsingTransparency == true) {
			//TransparentBlt (hdcTemp, 0, 0, width, height, hdcMem, srcxpos, srcypos, srcwidth, srcheight, transColor);
			TransparentBlt (hdc, xPos, yPos, Width, Height, srcDC, srcxPos, srcyPos, Width, Height, m_clTranskey);	
		} else {
			BitBlt (hdc, xPos, yPos, Width, Height, srcDC, srcxPos, srcyPos, SRCCOPY);
		}
	}
	if (Method == BLITMETHOD_ALPHABLEND) {
		AlphaBlend (hdc, xPos, yPos, Width, Height, srcDC, srcxPos, srcyPos, srcWidth, srcHeight, blendop);
	}
}

void UIPicButton::DrawButton (HDC hdc, HDC hdcMem, int Method, BLENDFUNCTION blendop)
{
	if (g_bSinglebitmap == true) {
		Blit (hdc, g_xPos, g_yPos, g_iBitmapwidth, g_iBitmapheight, hdcMem, 0, 0, g_iBitmapwidth, g_iBitmapheight, Method, blendop);
	} else {
		for (int i=0;i<g_Width/g_iBitmapwidth;i++) {
			if (i==0) {
				Blit (hdc, g_xPos+(i*(g_c1xWidth)), g_yPos, g_c1xWidth, g_iBitmapheight, hdcMem, g_c1xStart, 0, g_c1xWidth, g_iBitmapheight, Method, blendop);
			}

			if (i>0 && i<(g_Width/g_iBitmapwidth)-1) {
				Blit (hdc, g_xPos+(i*(g_c2xWidth)), g_yPos, g_c2xWidth, g_iBitmapheight, hdcMem, g_c2xStart, 0, g_c2xWidth, g_iBitmapheight, Method, blendop);
			}

			if (i==(g_Width/g_iBitmapwidth)-1) {
				Blit (hdc, g_xPos+(i*(g_c3xWidth)), g_yPos, g_c3xWidth, g_iBitmapheight, hdcMem, g_c3xStart, 0, g_c3xWidth, g_iBitmapheight, Method, blendop);
				g_Rect.right = g_xPos+(i*(g_c3xWidth))+g_c3xWidth;
			}
		}
	}
}

void UIPicButton::DrawClippedButton (HDC hdc, HDC hdcMem, int Method, BLENDFUNCTION blendop)
{
	if (g_bSinglebitmap == true) {
		Blit (hdc, g_xPos+1, g_yPos+1, g_iBitmapwidth-2, g_iBitmapheight-2, hdcMem, 1, 1, g_iBitmapwidth, g_iBitmapheight, Method, blendop);
	}
}

void UIPicButton::DrawRect (HDC hdc)
{
	HBRUSH myBrush = CreateSolidBrush (RGB (0, 100, 200));
	RECT controlRect;
	controlRect.left = g_xPos;
	controlRect.right = g_xPos+g_iBitmapwidth;
	controlRect.top = g_yPos;
	controlRect.bottom = g_yPos+g_iBitmapheight;
	
	FillRect (hdc, &controlRect, myBrush);

	DeleteObject (myBrush);
}

void UIPicButton::Paint (HDC hdc)
{
	if (m_bVisible == false) {
		return;
	}

	// Paint the title to the screen
	BITMAP bm;
	HDC hdcMem = CreateCompatibleDC (hdc);
	HBITMAP hbmOld;

	int i = 0;
	int a = 0;

	if (g_bMouseover == true) {
		if (g_mouseDown == true) {
			hbmOld = (HBITMAP) SelectObject (hdcMem, g_hbmDown);
			GetObject (g_hbmDown, sizeof (bm), &bm);
		} else {
			hbmOld = (HBITMAP) SelectObject (hdcMem, g_hbmHighlight);
			GetObject (g_hbmHighlight, sizeof (bm), &bm);
		}
		
	} else {
		hbmOld = (HBITMAP) SelectObject (hdcMem, g_hbmBase);	
		GetObject (g_hbmBase, sizeof (bm), &bm);
	}
	
	if (g_bMouseover == true) {
		if (g_bUsehighlight == true) {
			if (g_bUsefade == true) {
				g_bHighlighted = true;
				if (g_mouseDown == true) {
					g_bDepressed = true;
					DrawButton (hdc, hdcMem, BLITMETHOD_STANDARD, g_blendop);
				} else {
					if (g_bDepressed == true) {
						DrawButton (hdc, hdcMem, BLITMETHOD_STANDARD, g_blendop);
						g_bDepressed = false;
					} else {
						DrawButton (hdc, hdcMem, BLITMETHOD_ALPHABLEND, g_blendop);	
					}
					
				}
			} else {
				g_bHighlighted = true;
				if (m_bUseRectHighlight == true) {
					DrawRect (hdc);
					DrawClippedButton (hdc, hdcMem, BLITMETHOD_STANDARD, g_blendop);
				} else {
					DrawButton (hdc, hdcMem, BLITMETHOD_STANDARD, g_blendop);
				}
			}
		}
	} else {
		if (g_bHighlighted==false) {
			DrawButton (hdc, hdcMem, BLITMETHOD_STANDARD, g_blendop);
		} else {
			if (g_bUsefade == true) {
				DrawButton (hdc, hdcMem, BLITMETHOD_ALPHABLEND, g_blendop);
			} else {
				DrawButton (hdc, hdcMem, BLITMETHOD_STANDARD, g_blendop);
			}
		}		
	}

	if (g_bUsingtext == true) {
		PaintText (hdc);
	}

	SelectObject (hdcMem, hbmOld);
	DeleteObject (hbmOld);
	DeleteDC (hdcMem);
}

void UIPicButton::Fade ()
{
	g_iAlpha++;
	if (g_iAlpha > 50) {
		g_iAlpha = 0;
		StopFade ();
		if (g_bMouseover == false) {
			g_bHighlighted = false;
		}
	}
	
	g_blendop.SourceConstantAlpha = g_iAlpha;
	InvalidateRect (g_hWnd, &g_Rect, false);
}

void UIPicButton::NotifyTimer (unsigned int wParam)
{
	if (g_bUsefade == true) {
		if (wParam == TMR_FADE+g_controlID) {
			Fade ();
		}
	}
}

void UIPicButton::LockHighlight (bool bLock)
{
	g_bHighlightlocked = bLock;
}

void UIPicButton::StartFade ()
{
	g_iAlpha = 0;
	if (g_bTimeractive == false) {
		g_bTimeractive = true;
		SetTimer (g_hWnd, TMR_FADE+g_controlID, 3, NULL);
		return;
	}
	if (g_bTimeractive == true) {
		KillTimer (g_hWnd, TMR_FADE+g_controlID);
		SetTimer (g_hWnd, TMR_FADE+g_controlID, 3, NULL);
		return;
	}
}

void UIPicButton::StopFade ()
{
	g_bTimeractive = false;
	KillTimer (g_hWnd, TMR_FADE+g_controlID);
}

void UIPicButton::NotifyMouseMove (int iXPos, int iYPos)
{
	g_mouseXPos = iXPos;
	g_mouseYPos = iYPos;
	
	if (m_bVisible == false) {
		return;
	}

	if (IsMouseRect () == true) {
		if (g_bMouseover == false) {
			g_bMouseover = true;
			InvalidateRect (g_hWnd, &g_Rect, false);
			PostMessage (g_hWnd, WM_UIHIGHLIGHT, (WPARAM) g_controlID, 0);
			if (g_bUsefade == true) {
				StartFade ();
			}
		}

		if (g_mouseDown == true) {
			// A drag is in progress
		}
	} else {

		if (m_bTogglemode == false) {
			if (g_mouseDown == true) {
				g_mouseDown = false;
			}
		}

		if (m_bTogglemode == false) {
			if (g_bHighlightlocked == false) {
				if (g_bMouseover == true) {
					g_bMouseover = false;
					InvalidateRect (g_hWnd, &g_Rect, false);
					if (g_bUsefade == true) {
						StartFade ();
					}
				}
			}
		} else {
			if (g_mouseDown == false) {
				if (g_bMouseover == true) {
					g_bMouseover = false;
					InvalidateRect (g_hWnd, &g_Rect, false);
				}
			}
		}
	}
}

void UIPicButton::ReleaseToggle ()
{
	g_bMouseover = false;
	g_mouseDown = false;
	InvalidateRect (g_hWnd, &g_Rect, false);
}

void UIPicButton::NotifyMouseDown ()
{
	if (m_bVisible == false) {
		return;
	}

	if (IsMouseRect () == true) {
		if (m_bTogglemode == true) {
			if (g_mouseDown == true) {
				g_mouseDown = false;
			} else {
				g_mouseDown = true;
			}
		} else {
			g_mouseDown = true;
		}		
		InvalidateRect (g_hWnd, &g_Rect, false);
	}
}

void UIPicButton::NotifyMouseUp ()
{
	if (m_bVisible == false) {
		return;
	}
	
	if (m_bTogglemode == false) {
		g_mouseDown = false;
	}

	if (IsMouseRect () == true) {
		InvalidateRect (g_hWnd, &g_Rect, false);
		if (m_bTogglemode == false) {
			PostMessage (g_hWnd, WM_UICOMMAND, (WPARAM) g_controlID, 0);
		} else {
			if (g_mouseDown == true) {
				PostMessage (g_hWnd, WM_UICOMMAND, (WPARAM) g_controlID, 1);
			} else {
				PostMessage (g_hWnd, WM_UICOMMAND, (WPARAM) g_controlID, 0);
			}
		}
	}
}