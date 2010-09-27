#pragma once

#define IDT_SCROLLUP				140
#define IDT_SCROLLDOWN				141

#include "UIControl.h"
#include "Diagnostics.h"

class UIVScrollbar : public UIControl
{
	public:
		UIVScrollbar ();
		~UIVScrollbar ();
		void Paint (HDC hdc);
		void SetDiagnostics (Diagnostics *pdiag);
		void OutputInt (LPCSTR lpszText, int iValue);
		void OutputText (LPCSTR lpszText);
		void OutputText (LPCSTR lpszName, LPCSTR lpszValue);
		void NotifyMouseMove (int iXPos, int iYPos);
		void NotifyMouseWheel (short zDelta);
		void NotifyTimer (unsigned int wParam);
		void NotifyMouseDown ();
		void NotifyMouseUp ();
		void SetAlwaysatend ();
		void SetMaxValue (int iMax);
		void InitialiseDefault (HWND hWnd, int cID, int xPos, int yPos, int Width, int Height);
		void SetBitmapResources (unsigned int wResbase);
		void SetBitmapUpArrow (int xPos, int yPos, int Width, int Height);
		void SetBitmapUpArrowHigh (int xPos, int yPos, int Width, int Height);
		void SetBitmapUpArrowDown (int xPos, int yPos, int Width, int Height);
		void SetBitmapDownArrowHigh (int xPos, int yPos, int Width, int Height);
		void SetBitmapDownArrowDown (int xPos, int yPos, int Width, int Height);
		void SetBitmapDownArrow (int xPos, int yPos, int Width, int Height);
		void SetBitmapScrollTrack (int xPos, int yPos, int Width, int Height);
		void SetBitmapScrollbarHighTop (int xPos, int yPos, int Width, int Height);
		void SetBitmapScrollbarHighBottom (int xPos, int yPos, int Width, int Height);
		void SetBitmapScrollbarHighMiddle (int xPos, int yPos, int Width, int Height);
		void SetBitmapScrollbarHighHandle (int xPos, int yPos, int Width, int Height);
		void SetBitmapScrollbarNormTop (int xPos, int yPos, int Width, int Height);
		void SetBitmapScrollbarNormBottom (int xPos, int yPos, int Width, int Height);
		void SetBitmapScrollbarNormMiddle (int xPos, int yPos, int Width, int Height);
		void SetBitmapScrollbarNormHandle (int xPos, int yPos, int Width, int Height);
		void SetBitmapScrollbarDownTop (int xPos, int yPos, int Width, int Height);
		void SetBitmapScrollbarDownBottom (int xPos, int yPos, int Width, int Height);
		void SetBitmapScrollbarDownMiddle (int xPos, int yPos, int Width, int Height);
		void SetBitmapScrollbarDownHandle (int xPos, int yPos, int Width, int Height);

	private:

		int m_iUpArrowxPos;
		int m_iUpArrowyPos;
		int m_iUpArrowWidth;
		int m_iUpArrowHeight;
		
		int m_iUpArrowHighxPos;
		int m_iUpArrowHighyPos;
		int m_iUpArrowHighWidth;
		int m_iUpArrowHighHeight;

		int m_iUpArrowDownxPos;
		int m_iUpArrowDownyPos;
		int m_iUpArrowDownWidth;
		int m_iUpArrowDownHeight;

		int m_iDownArrowxPos;
		int m_iDownArrowyPos;
		int m_iDownArrowWidth;
		int m_iDownArrowHeight;
		
		int m_iDownArrowHighxPos;
		int m_iDownArrowHighyPos;
		int m_iDownArrowHighWidth;
		int m_iDownArrowHighHeight;
		
		int m_iDownArrowDownxPos;
		int m_iDownArrowDownyPos;
		int m_iDownArrowDownWidth;
		int m_iDownArrowDownHeight;

		int m_iScrollTrackxPos;
		int m_iScrollTrackyPos;
		int m_iScrollTrackWidth;
		int m_iScrollTrackHeight;
		
		int m_iScrollBarHighTopxPos;
		int m_iScrollBarHighTopyPos;
		int m_iScrollBarHighTopWidth;
		int m_iScrollBarHighTopHeight;

		int m_iScrollBarHighBottomxPos;
		int m_iScrollBarHighBottomyPos;
		int m_iScrollBarHighBottomWidth;
		int m_iScrollBarHighBottomHeight;

		int m_iScrollBarHighMiddlexPos;
		int m_iScrollBarHighMiddleyPos;
		int m_iScrollBarHighMiddleWidth;
		int m_iScrollBarHighMiddleHeight;

		int m_iScrollBarHighHandlexPos;
		int m_iScrollBarHighHandleyPos;
		int m_iScrollBarHighHandleWidth;
		int m_iScrollBarHighHandleHeight;

		int m_iScrollBarNormTopxPos;
		int m_iScrollBarNormTopyPos;
		int m_iScrollBarNormTopWidth;
		int m_iScrollBarNormTopHeight;

		int m_iScrollBarNormBottomxPos;
		int m_iScrollBarNormBottomyPos;
		int m_iScrollBarNormBottomWidth;
		int m_iScrollBarNormBottomHeight;

		int m_iScrollBarNormMiddlexPos;
		int m_iScrollBarNormMiddleyPos;
		int m_iScrollBarNormMiddleWidth;
		int m_iScrollBarNormMiddleHeight;

		int m_iScrollBarNormHandlexPos;
		int m_iScrollBarNormHandleyPos;
		int m_iScrollBarNormHandleWidth;
		int m_iScrollBarNormHandleHeight;

		int m_iScrollBarDownTopxPos;
		int m_iScrollBarDownTopyPos;
		int m_iScrollBarDownTopWidth;
		int m_iScrollBarDownTopHeight;

		int m_iScrollBarDownBottomxPos;
		int m_iScrollBarDownBottomyPos;
		int m_iScrollBarDownBottomWidth;
		int m_iScrollBarDownBottomHeight;

		int m_iScrollBarDownMiddlexPos;
		int m_iScrollBarDownMiddleyPos;
		int m_iScrollBarDownMiddleWidth;
		int m_iScrollBarDownMiddleHeight;

		int m_iScrollBarDownHandlexPos;
		int m_iScrollBarDownHandleyPos;
		int m_iScrollBarDownHandleWidth;
		int m_iScrollBarDownHandleHeight;

		// Internal Scroll Bar Tracking variables
		int m_iUpperBound;
		int m_iLowerBound;

		int m_ioriginyPos;
		int m_iBarorigin;
		int m_ioriginDiff;
		int m_iBaryPos;
		bool m_bBarPosInitialised;
		int m_iBarHeight;
		int m_iInitialBarYPos;
		int m_iScrollpos;
		int m_iScrollstep;
		int m_iScrollmax;
		RECT m_rctBar;
		RECT m_rctUpArrow;
		RECT m_rctDownArrow;
		RECT m_rctUpperTrack;
		RECT m_rctLowerTrack;

		bool m_bMouseover;
		bool m_bHighlighted;
		int m_iScrollInterval;

		bool m_bMouseoverUpArrow;
		bool m_bMousedownUpArrow;
		bool m_bHighlightedUpArrow;

		bool m_bMouseoverUpperTrack;
		bool m_bMouseoverLowerTrack;

		bool m_bMouseoverDownArrow;
		bool m_bMousedownDownArrow;
		bool m_bHighlightedDownArrow;

		bool m_bHighlightlocked;
		bool m_bScrollTimerActive;
		bool m_bStartatend;

		Diagnostics *m_pdiag;
		bool m_bUseDiagnostics;
		///////////////// METHOD DEFINITIONS ///////////////////
		void InitialiseScrollValues ();
};
