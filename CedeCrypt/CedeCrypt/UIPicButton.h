#pragma once
#include "UIControl.h"

#define BLITMETHOD_STANDARD		0
#define BLITMETHOD_ALPHABLEND	1
#define TMR_FADE				100

class UIPicButton : public UIControl
{
	public:
		UIPicButton ();
		~UIPicButton ();
		void Paint (HDC hdc);

		void NotifyMouseMove (int iXPos, int iYPos);
		void NotifyMouseDown ();
		void NotifyMouseUp ();
		void NotifyTimer (unsigned int wParam);

		void SetVisible (bool bVisible);
		unsigned long GetSize ();
		void SetBitmapResources (bool bSingleBitmap, unsigned int wResbase, unsigned int wReshighlight, unsigned int wResdown);
		void SetBitmapProperties (int c1xStart, int c1xWidth, int c2xStart, int c2xWidth, int c3xStart, int c3xWidth, int Height, int Width);
		void SetBitmapProperties (int Width, int Height);
		void SetTransparencyKey (COLORREF clTransparentKey);
		void LockHighlight (bool bLock);
		void SetToggleMode (bool bUsetoggle);
		void SetRectHighlight (bool bUseRectHighlight);
		void ReleaseToggle ();
	private:
		void DrawRect (HDC hdc);
		void DrawClippedButton (HDC hdc, HDC hdcMem, int Method, BLENDFUNCTION blendop);
		// Bitmap properties
		int g_c1xStart;
		int g_c1xWidth;
		int g_c2xStart;
		int g_c2xWidth;
		int g_c3xStart;
		int g_c3xWidth;

		bool g_bMouseover;
		bool g_bHighlighted;
		bool g_bDepressed;
		bool g_bHighlightlocked;
		bool m_bVisible;

		bool g_bSinglebitmap;
		// Blend variables for highlighting
		BLENDFUNCTION g_blendop;
		int g_iAlpha;
		bool g_bTimeractive;
		bool m_bTogglemode;
		bool m_bUseRectHighlight;

		//Transparency key for transparent blitting.
		// A default transparent key will be set upon instance creation. However the default blitting will be used until the
		// SetTransparencyKey method is used. This tells the object to use a different method of blitting.
		COLORREF m_clTranskey;
		bool m_bUsingTransparency;		

		///////////////// METHOD DEFINITIONS ///////////////////
		void Blit (HDC hdc, int xPos, int yPos, int Width, int Height, HDC srcDC, int srcxPos, int srcyPos, int srcWidth, int srcHeight, int Method, BLENDFUNCTION blendop);
		void DrawButton (HDC hdc, HDC hdcMem, int Method, BLENDFUNCTION blendop);
		void Fade ();
		void StartFade ();
		void StopFade ();
};