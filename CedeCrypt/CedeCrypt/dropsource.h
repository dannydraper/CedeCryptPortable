//
//	DROPSOURCE.CPP
//
//	Implementation of the IDropSource COM interface
//
//	By J Brown 2004 
//
//	www.catch22.net
//

#define STRICT

#include <windows.h>
#include "EncryptWindow.h"
#include "EncryptWindowEx.h"

class CDropSource : public IDropSource
{
public:
	//
    // IUnknown members
	//
    HRESULT __stdcall QueryInterface	(REFIID iid, void ** ppvObject);
    ULONG   __stdcall AddRef			(void);
    ULONG   __stdcall Release			(void);
		
    //
	// IDropSource members
	//
    HRESULT __stdcall QueryContinueDrag	(BOOL fEscapePressed, DWORD grfKeyState);
	HRESULT __stdcall GiveFeedback		(DWORD dwEffect);
	
	//
    // Constructor / Destructor
	//
    CDropSource(EncryptWindowEx *pwnd);
    ~CDropSource();
	HRESULT CreateDropSource(IDropSource **ppDropSource);
	void SetEncryptWindow (EncryptWindow *pwnd);

private:

    //
	// private members and functions
	//
    LONG	   m_lRefCount;
	EncryptWindow *pwndenc;
	EncryptWindowEx *pwndencex;
};
