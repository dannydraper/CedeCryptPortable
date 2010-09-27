#pragma once
#include <windows.h>
#include <malloc.h>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <share.h>

class MemoryBuffer {
	
	public:
		MemoryBuffer ();
		~MemoryBuffer ();		
		bool Write (void *pItem, unsigned long iOffset, unsigned long itemSize);
		bool Append (void *pItem, unsigned long itemSize);
		void *GetBuffer ();
		void SetSize (unsigned long iSize);
		unsigned long GetSize ();
		void Clear ();
		BYTE GetByte (unsigned long lIndex);
		void SetByte (unsigned long lIndex, BYTE bByte);
		bool SaveToFile (LPCSTR lpszFilename);
		bool ReadFromFile (LPCSTR lpszFilename);
		bool SaveToClipboard (HWND hwnd);
		unsigned long GetAppendPointer ();
		bool IsSizeset ();
		bool SwapBytes (unsigned long iIndex1, unsigned long iIndex2);
		void QuickCrypt (bool Encrypt);

	private:

		BYTE *m_buffer;
		bool m_bCleared;
		bool m_bSizeset;
		unsigned long m_iBuffersize;
		unsigned long m_iAppendpointer;
};
