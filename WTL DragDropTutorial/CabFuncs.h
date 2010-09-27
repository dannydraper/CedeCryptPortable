// CabFuncs.h: interface for the CCabFuncs class.
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CABFUNCS_H__873CCDCC_66AD_461B_A848_68FF6DC994CD__INCLUDED_)
#define AFX_CABFUNCS_H__873CCDCC_66AD_461B_A848_68FF6DC994CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

void* DIAMONDAPI cab_Alloc ( ULONG cb );
void  DIAMONDAPI cab_Free ( void* pvData );
int   DIAMONDAPI cab_Open ( char* pszFile, int oflag, int pmode );
UINT  DIAMONDAPI cab_Read ( int hf, void* memory, UINT cb );
UINT  DIAMONDAPI cab_Write ( int hf, void* memory, UINT cb );
int   DIAMONDAPI cab_Close ( int hf );
long  DIAMONDAPI cab_Seek ( int hf, long dist, int seektype );

#endif // !defined(AFX_CABFUNCS_H__873CCDCC_66AD_461B_A848_68FF6DC994CD__INCLUDED_)
