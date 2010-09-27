//
//	DATAOBJECT.CPP
//
//	Implementation of the IDataObject COM interface
//
//	By J Brown 2004 
//
//	www.catch22.net
//

#define STRICT

#include <windows.h>
#include <shlobj.h>
#include "EncryptWindowEx.h"

// defined in enumformat.cpp
HRESULT CreateEnumFormatEtc(UINT nNumFormats, FORMATETC *pFormatEtc, IEnumFORMATETC **ppEnumFormatEtc);

class CDataObject : public IDataObject, public IAsyncOperation
{
public:
	//
    // IUnknown members
	//
    HRESULT __stdcall QueryInterface (REFIID iid, void ** ppvObject);
    ULONG   __stdcall AddRef (void);
    ULONG   __stdcall Release (void);
		
    //
	// IDataObject members
	//
    HRESULT __stdcall GetData				(FORMATETC *pFormatEtc,  STGMEDIUM *pMedium);
    HRESULT __stdcall GetDataHere			(FORMATETC *pFormatEtc,  STGMEDIUM *pMedium);
    HRESULT __stdcall QueryGetData			(FORMATETC *pFormatEtc);
	HRESULT __stdcall GetCanonicalFormatEtc (FORMATETC *pFormatEct,  FORMATETC *pFormatEtcOut);
    HRESULT __stdcall SetData				(FORMATETC *pFormatEtc,  STGMEDIUM *pMedium,  BOOL fRelease);
	HRESULT __stdcall EnumFormatEtc			(DWORD      dwDirection, IEnumFORMATETC **ppEnumFormatEtc);
	HRESULT __stdcall DAdvise				(FORMATETC *pFormatEtc,  DWORD advf, IAdviseSink *pAdvSink, DWORD *pdwConnection);
	HRESULT __stdcall DUnadvise				(DWORD      dwConnection);
	HRESULT __stdcall EnumDAdvise			(IEnumSTATDATA **ppEnumAdvise);
	

	HRESULT __stdcall EndOperation (HRESULT hResult, IBindCtx *pbcReserved, DWORD dwEffects);
	HRESULT __stdcall GetAsyncMode (BOOL *pfIsOpAsync);
	HRESULT __stdcall InOperation (BOOL *pfInAsyncOp);
	HRESULT __stdcall SetAsyncMode (BOOL fDoOpAsync);
	HRESULT __stdcall StartOperation (IBindCtx *pbcReserved);

	//
    // Constructor / Destructor
	//
    CDataObject(FORMATETC *fmt, STGMEDIUM *stgmed, int count, EncryptWindowEx *pwnd);
    ~CDataObject();
	HRESULT CreateDataObject(FORMATETC *fmtetc, STGMEDIUM *stgmeds, UINT count, IDataObject **ppDataObject);

private:

	int LookupFormatEtc(FORMATETC *pFormatEtc);

    //
	// any private members and functions
	//
    LONG	   m_lRefCount;
	EncryptWindowEx *pwndenc;

	FORMATETC *m_pFormatEtc;
	STGMEDIUM *m_pStgMedium;
	LONG	   m_nNumFormats;

};