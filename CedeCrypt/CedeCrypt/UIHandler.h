#pragma once
#include <windows.h>
#include "UIControl.h"
#include "UIPicButton.h"
#include "DynList.h"

class UIHandler
{
	public:
		UIHandler ();
		~UIHandler ();

		void SetWindowProperties (int xPos, int yPos, int Width, int Height, COLORREF bgColor);
		DynList *GetStoredControls ();
		void AddControl (UIControl *pUIControl);
		void AddDirectControl (UIControl *pUIControl);
		void AddControl (UIControl *pUIControl, bool bUseClipping);
		void StoreControl (UIControl *pUIControl, bool bUseClipping);
		void ClearStoredControls ();
		void SetClippingArea (RECT rctClipping);
		void SetClippingArea (RECT rctClipping, COLORREF bgColor);
		void GetClippingRect (LPRECT lpRect);
		void PaintControls (HWND hWnd);
		void PainttoHDC (HDC hdc);

		void AddCommandButton(HWND hWnd, UIPicButton *picbutton, unsigned int controlID, int xPos, int yPos, char *szCaption, int txOffset, int tyOffset);
		void AddCommandBarButton (HWND hWnd, UIPicButton *picbutton, unsigned int controlID, int xPos, int yPos, unsigned int iResNorm, unsigned int iResHigh, unsigned int iResDown);
		void AddStdCommandButton(HWND hWnd, UIPicButton *picbutton, unsigned int controlID, int xPos, int yPos, char *szCaption, int txOffset, int tyOffset);
		void AddCommandBarButtonHorz (HWND hWnd, UIPicButton *picbutton, unsigned int controlID, int xPos, int yPos, unsigned int iResNorm, unsigned int iResHigh, unsigned int iResDown);
		void NotifyMouseMove (int iXPos, int iYPos);
		void NotifyMouseDown ();
		void NotifyMouseUp ();
		void NotifyTimer (unsigned int wParam);

		


	private:
		// Private Members and Objects
		// A list storing the object for every control contained in this instance of the UI Handler.
		// Multiple windows may have different instances of the UIHandler.
		DynList m_dlControls;
		DynList m_dlStoredControls; // A Dynlist storing the actual control object for stored controls. - can be useful for controls that are dynamic at runtime.
		// A list storing boolean values specifying whether the associated control in the above
		// array is a clipped control. If it is, then it has to be handled slightly differently.
		DynList m_dlControlclipping;
		DynList m_dlStoredControlclipping; // A Dynlist storing boolean values for stored controls, and whether they are clipped controls.		

		// Dynlist storing a list of controls which are to be painted directly to the hdc with no double buffering.
		DynList m_dlDirectControls;

		UIControl *m_pwndtitle;
	
		int m_iXpos;
		int m_iYpos;
		int m_iWidth;
		int m_iHeight;
		int m_iMouseX;
		int m_iMouseY;
		COLORREF m_bgColor;
		COLORREF m_bgClipColor;

		RECT m_rctClippingArea;
		bool m_bUseClipping;
		bool m_bUseClipBgColor;

		bool m_bUseStoredControls;

		// Private methods
		void ShowInt (int iInttoShow);
		
};