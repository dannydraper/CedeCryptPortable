// CabFuncs.cpp: callbacks passed to the CAB functions
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CabFuncs.h"

/////////////////////////////////////////////////////////////////////////////

void* DIAMONDAPI cab_Alloc ( ULONG cb )
{
    return new BYTE[cb];
}

void DIAMONDAPI cab_Free ( void* pvData )
{
    delete[] (BYTE*) pvData;
}

int DIAMONDAPI cab_Open ( char* pszFile, int oflag, int pmode )
{
    return _open ( pszFile, oflag, pmode );
}

UINT DIAMONDAPI cab_Read ( int hf, void* memory, UINT cb )
{
    return (unsigned) _read ( hf, memory, cb );
}

UINT DIAMONDAPI cab_Write ( int hf, void* memory, UINT cb )
{
    return (unsigned) _write ( hf, memory, cb );
}

int DIAMONDAPI cab_Close ( int hf )
{
    return _close(hf);
}

long DIAMONDAPI cab_Seek ( int hf, long dist, int seektype )
{
    return _lseek ( hf, dist, seektype );
}
