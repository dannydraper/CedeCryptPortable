#include "dataobject.h"

//
//	Constructor
//
CDataObject::CDataObject(FORMATETC *fmtetc, STGMEDIUM *stgmed, int count, EncryptWindowEx *pwnd) 
{
	pwndenc = pwnd;

	m_lRefCount  = 1;
	m_nNumFormats = count;
	
	m_pFormatEtc  = new FORMATETC[count];
	m_pStgMedium  = new STGMEDIUM[count];

	for(int i = 0; i < count; i++)
	{
		m_pFormatEtc[i] = fmtetc[i];
		m_pStgMedium[i] = stgmed[i];
	}
}

//
//	Destructor
//
CDataObject::~CDataObject()
{
	// cleanup
	if(m_pFormatEtc) delete[] m_pFormatEtc;
	if(m_pStgMedium) delete[] m_pStgMedium;

	OutputDebugString("oof\n");
}

//
//	IUnknown::AddRef
//
ULONG __stdcall CDataObject::AddRef(void)
{
    // increment object reference count
    return InterlockedIncrement(&m_lRefCount);
}

//
//	IUnknown::Release
//
ULONG __stdcall CDataObject::Release(void)
{
    // decrement object reference count
	LONG count = InterlockedDecrement(&m_lRefCount);
		
	if(count == 0)
	{
		delete this;
		return 0;
	}
	else
	{
		return count;
	}
}

HRESULT __stdcall CDataObject::EndOperation (HRESULT hResult, IBindCtx *pbcReserved, DWORD dwEffects)
{
	//MessageBox (NULL, "EncOperation", "", MB_OK);
	return S_OK;
}

HRESULT __stdcall CDataObject::GetAsyncMode (BOOL *pfIsOpAsync)
{
	//MessageBox (NULL, "GetAsyncMode", "", MB_OK);
	*pfIsOpAsync = VARIANT_TRUE;
	return S_OK;
}

HRESULT __stdcall CDataObject::InOperation (BOOL *pfInAsyncOp)
{
	//MessageBox (NULL, "InOperation", "", MB_OK);
	*pfInAsyncOp = VARIANT_TRUE;
	return S_OK;
}

HRESULT __stdcall CDataObject::SetAsyncMode (BOOL fDoOpAsync)
{
	//MessageBox (NULL, "SetAsyncMode", "", MB_OK);
	return S_OK;
}

HRESULT __stdcall CDataObject::StartOperation (IBindCtx *pbcReserved)
{
	//MessageBox (NULL, "StartOperation", "", MB_OK);
	pwndenc->DoDecryption ();

	return S_OK;
}

//
//	IUnknown::QueryInterface
//
HRESULT __stdcall CDataObject::QueryInterface(REFIID iid, void **ppvObject)
{
    // check to see what interface has been requested
    if(iid == IID_IDataObject || iid == IID_IUnknown || iid == IID_IAsyncOperation)
    {
        AddRef();
        *ppvObject = this;
        return S_OK;
    }
    else
    {
        *ppvObject = 0;
        return E_NOINTERFACE;
    }
}

HGLOBAL DupMem(HGLOBAL hMem)
{
	// lock the source memory object
	DWORD   len    = GlobalSize(hMem);
	PVOID   source = GlobalLock(hMem);
	
	// create a fixed "global" block - i.e. just
	// a regular lump of our process heap
	PVOID   dest   = GlobalAlloc(GMEM_FIXED, len);

	memcpy(dest, source, len);

	GlobalUnlock(hMem);

	return dest;
}

int CDataObject::LookupFormatEtc(FORMATETC *pFormatEtc)
{
	for(int i = 0; i < m_nNumFormats; i++)
	{
		if((pFormatEtc->tymed    &  m_pFormatEtc[i].tymed)   &&
			pFormatEtc->cfFormat == m_pFormatEtc[i].cfFormat && 
			pFormatEtc->dwAspect == m_pFormatEtc[i].dwAspect)
		{
			return i;
		}
	}

	return -1;

}

//
//	IDataObject::GetData
//
HRESULT __stdcall CDataObject::GetData (FORMATETC *pFormatEtc, STGMEDIUM *pMedium)
{
	int idx;

	//
	// try to match the requested FORMATETC with one of our supported formats
	//
	if((idx = LookupFormatEtc(pFormatEtc)) == -1)
	{
		return DV_E_FORMATETC;
	}

	//
	// found a match! transfer the data into the supplied storage-medium
	//
	pMedium->tymed			 = m_pFormatEtc[idx].tymed;
	pMedium->pUnkForRelease  = 0;
	
	switch(m_pFormatEtc[idx].tymed)
	{
	case TYMED_HGLOBAL:

		pMedium->hGlobal = DupMem(m_pStgMedium[idx].hGlobal);
		//return S_OK;
		break;

	default:
		return DV_E_FORMATETC;
	}

	return S_OK;
}

//
//	IDataObject::GetDataHere
//
HRESULT __stdcall CDataObject::GetDataHere (FORMATETC *pFormatEtc, STGMEDIUM *pMedium)
{
	// GetDataHere is only required for IStream and IStorage mediums
	// It is an error to call GetDataHere for things like HGLOBAL and other clipboard formats
	//
	//	OleFlushClipboard 
	//
	return DATA_E_FORMATETC;
}

//
//	IDataObject::QueryGetData
//
//	Called to see if the IDataObject supports the specified format of data
//
HRESULT __stdcall CDataObject::QueryGetData (FORMATETC *pFormatEtc)
{
	return (LookupFormatEtc(pFormatEtc) == -1) ? DV_E_FORMATETC : S_OK;
}

//
//	IDataObject::GetCanonicalFormatEtc
//
HRESULT __stdcall CDataObject::GetCanonicalFormatEtc (FORMATETC *pFormatEct, FORMATETC *pFormatEtcOut)
{
	// Apparently we have to set this field to NULL even though we don't do anything else
	pFormatEtcOut->ptd = NULL;
	return E_NOTIMPL;
}

//
//	IDataObject::SetData
//
HRESULT __stdcall CDataObject::SetData (FORMATETC *pFormatEtc, STGMEDIUM *pMedium,  BOOL fRelease)
{
	return E_NOTIMPL;
}

//
//	IDataObject::EnumFormatEtc
//
HRESULT __stdcall CDataObject::EnumFormatEtc (DWORD dwDirection, IEnumFORMATETC **ppEnumFormatEtc)
{
	if(dwDirection == DATADIR_GET)
	{
		// for Win2k+ you can use the SHCreateStdEnumFmtEtc API call, however
		// to support all Windows platforms we need to implement IEnumFormatEtc ourselves.
		return CreateEnumFormatEtc(m_nNumFormats, m_pFormatEtc, ppEnumFormatEtc);
	}
	else
	{
		// the direction specified is not support for drag+drop
		return E_NOTIMPL;
	}
}

//
//	IDataObject::DAdvise
//
HRESULT __stdcall CDataObject::DAdvise (FORMATETC *pFormatEtc, DWORD advf, IAdviseSink *pAdvSink, DWORD *pdwConnection)
{
	return OLE_E_ADVISENOTSUPPORTED;
}

//
//	IDataObject::DUnadvise
//
HRESULT __stdcall CDataObject::DUnadvise (DWORD dwConnection)
{
	return OLE_E_ADVISENOTSUPPORTED;
}

//
//	IDataObject::EnumDAdvise
//
HRESULT __stdcall CDataObject::EnumDAdvise (IEnumSTATDATA **ppEnumAdvise)
{
	return OLE_E_ADVISENOTSUPPORTED;
}

//
//	Helper function
//
