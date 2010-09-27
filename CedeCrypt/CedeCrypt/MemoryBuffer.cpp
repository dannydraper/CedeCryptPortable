// A memory buffer class. Written by Danny Draper (c) 2007. Provides safe memory buffer storage.

#include "MemoryBuffer.h"

MemoryBuffer::MemoryBuffer ()
{
	m_bCleared = true;
	m_bSizeset = false;
	m_iBuffersize = 0;
	m_iAppendpointer = 0;
}

MemoryBuffer::~MemoryBuffer ()
{
}



void MemoryBuffer::SetSize (unsigned long iSize)
{
	if (m_bSizeset == false) {
		m_bSizeset = true;
		m_bCleared = false;
		m_buffer = (BYTE *) calloc (iSize, 1);
		m_iBuffersize = iSize;
	} else {
		m_buffer = (BYTE *) realloc (m_buffer, iSize);
		m_iBuffersize = iSize;
		m_iAppendpointer = 0;
		m_bCleared = false;
		ZeroMemory (m_buffer, iSize);
	}
}

unsigned long MemoryBuffer::GetSize ()
{
	return m_iBuffersize;
}

BYTE MemoryBuffer::GetByte (unsigned long lIndex)
{
	if (m_bSizeset == true) {
		if (m_iAppendpointer > 0) {
			if (lIndex >=0 && lIndex < m_iAppendpointer) {
				return m_buffer[lIndex];
			} else {
				return 0;
			}
		} else {
			if (lIndex >=0 && lIndex < m_iBuffersize) {
				return m_buffer[lIndex];
			} else {
				return 0;
			}
		}
	} else {
		return 0;
	}
}

void MemoryBuffer::SetByte (unsigned long lIndex, BYTE bByte)
{
	if (m_bSizeset == true) {
		if (m_iAppendpointer > 0) {
			if (lIndex >=0 && lIndex < m_iAppendpointer) {
				m_buffer[lIndex] = bByte;
			}
		} else {
			if (lIndex >=0 && lIndex < m_iBuffersize) {
				m_buffer[lIndex] = bByte;
			}
		}
	}
}

bool MemoryBuffer::Write (void *pItem, unsigned long iOffset, unsigned long itemSize)
{
	if (m_bSizeset == true) { // Make sure memory is allocated
		if (iOffset >=0 && iOffset < m_iBuffersize) { // Make sure the offset is within bounds.
			if (itemSize <= (m_iBuffersize-iOffset)) { // Make sure the item size will fit within the space available
				memcpy (m_buffer+iOffset, pItem, itemSize);
				return true;
			} else {
				return false;
			}
		} else {
			return false;
		}
	} else {
		return false;
	}
}

bool MemoryBuffer::Append (void *pItem, unsigned long itemSize)
{
	if (m_bSizeset == true) { // Make sure memory is allocated
		if (itemSize <= (m_iBuffersize-m_iAppendpointer)) {
			memcpy (m_buffer+m_iAppendpointer, pItem, itemSize);
			m_iAppendpointer+=itemSize;
			return true;
		} else {
			return false;
		}
	} else {
		return false;	
	}
}

bool MemoryBuffer::Compress (int iCompressionLevel)
{
	// Compress the MemoryBuffer using ZLIB compression routines.

	unsigned long lcompressedsize = 0;
	unsigned long lsourcesize = 0;
	unsigned long lsignature = 778217754; // The signature indicating this buffer is compressed;
	int izerr = 0;
	BYTE *compressedbuffer;

	int icomplevel = 0;

	icomplevel = iCompressionLevel;

	// Set some compression level boundaries just incase we're supplied something we can't do.
	if (iCompressionLevel > 9) {
		icomplevel = 9;
	}

	if (iCompressionLevel < 1) {
		icomplevel = 1;
	}

	if (m_bSizeset == true) {
		if (m_iAppendpointer > 0) {
			lcompressedsize = m_iAppendpointer + (m_iAppendpointer / 1000) + 12;
			lsourcesize = m_iAppendpointer;
		} else {
			lcompressedsize = m_iBuffersize + (m_iBuffersize / 1000) + 12;
			lsourcesize = m_iBuffersize;
		}

		compressedbuffer = (BYTE *) calloc (lcompressedsize, 1);

		// Now compress the buffer
		izerr = compress2 ((BYTE *) compressedbuffer, &lcompressedsize, (BYTE *) m_buffer, lsourcesize, icomplevel);

		if (izerr != Z_OK) {
			// Compressed failed
			free (compressedbuffer);
			return false;
		} else {
			// Compression successful.

			Clear ();
			SetSize ((sizeof (unsigned long) *2) + lcompressedsize);
			Write (&lsignature, 0, sizeof (unsigned long));
			Write (&lsourcesize, sizeof (unsigned long), sizeof (unsigned long));
			Write (compressedbuffer, (sizeof (unsigned long)*2), lcompressedsize);

			free (compressedbuffer);

			return true;
		}

	} else {
		return false;
	}

	return false;
}

bool MemoryBuffer::IsCompressed ()
{
	// Check if this memory buffer is compressed by looking at the signature at the beginning
	// of the memory buffer.
	unsigned long lsignature = 0;

	if (m_bSizeset == true) {
		if (m_iAppendpointer > 0) {
			if (m_iAppendpointer > sizeof (unsigned long)) {
				memcpy (&lsignature, m_buffer, sizeof (unsigned long));

				if (lsignature == 778217754) {
					return true;
				} else {
					return false;
				}
			} else {
				return false;
			}
		} else {
			if (m_iBuffersize > sizeof (unsigned long)) {
				memcpy (&lsignature, m_buffer, sizeof (unsigned long));

				if (lsignature == 778217754) {
					return true;
				} else {
					return false;
				}

			} else {
				return false;
			}
		}
	} else {
		return false;
	}
}

bool MemoryBuffer::Decompress ()
{
	// Decompress this memory buffer using zlib decompression routines.

	unsigned long lsignature = 0;
	unsigned long lcompressedsize = 0;
	unsigned long loriginalsize = 0;
	BYTE *decompressedbuffer;
	int izerr = 0;

	if (m_bSizeset == true) {
		if (m_iAppendpointer > 0) {
			if (m_iAppendpointer > sizeof (unsigned long)) {
				memcpy (&lsignature, m_buffer, sizeof (unsigned long));

				if (lsignature == 778217754) {
					lcompressedsize = m_iAppendpointer-(sizeof (unsigned long) *2);
				} else {
					return false;
				}
			} else {
				return false;
			}
		} else {
			if (m_iBuffersize > sizeof (unsigned long)) {
				memcpy (&lsignature, m_buffer, sizeof (unsigned long));

				if (lsignature == 778217754) {
					lcompressedsize = m_iBuffersize-(sizeof (unsigned long)*2);
				} else {
					return false;
				}

			} else {
				return false;
			}
		}


		memcpy (&loriginalsize, m_buffer+sizeof (unsigned long), sizeof (unsigned long));

		decompressedbuffer = (BYTE *) calloc (loriginalsize, 1);

		izerr = uncompress ((BYTE *) decompressedbuffer, &loriginalsize, (BYTE *) m_buffer+(sizeof (unsigned long)*2), lcompressedsize);

		if (izerr != Z_OK) {
			free (decompressedbuffer);
			return false;
		} else {
			Clear ();
			SetSize (loriginalsize);
			Write (decompressedbuffer, 0, loriginalsize);
			free (decompressedbuffer);
			return true;
		}

	} else {
		return false;
	}
}

bool MemoryBuffer::SaveToFile (LPCSTR lpszFilename)
{
	FILE *stream;

	if (m_bSizeset == true) {
		if (m_iAppendpointer > 0) {
			if ((stream = fopen (lpszFilename, "w+b")) == NULL) {
				_fcloseall ();
				return false;
			} else {
				fwrite (m_buffer, m_iAppendpointer, 1, stream);
				_fcloseall ();
				return true;
			}			
		} else {
			if ((stream = fopen (lpszFilename, "w+b")) == NULL) {
				_fcloseall ();
				return false;
			} else {
				fwrite (m_buffer, m_iBuffersize, 1, stream);
				_fcloseall ();
				return true;
			}

		}
	} else {
		return false;
	}
}

bool MemoryBuffer::SaveToClipboard (HWND hwnd)
{
	HANDLE hMem;
	LPBYTE lpbyteMem;
	LPTSTR  lptstrCopy;
	DWORD dwErr;

	if (m_bSizeset == true) {
		if (m_iAppendpointer > 0) {

			hMem = GlobalAlloc (GMEM_MOVEABLE, m_iAppendpointer);
			lptstrCopy = (LPTSTR) GlobalLock (hMem);
			memcpy (lptstrCopy, m_buffer, m_iAppendpointer);			
			GlobalUnlock (hMem);

		} else {
			hMem = GlobalAlloc (GMEM_MOVEABLE, m_iBuffersize);			    
			lptstrCopy = (LPTSTR) GlobalLock (hMem);
			memcpy (lptstrCopy, m_buffer, m_iBuffersize);
			GlobalUnlock (hMem);
		}

		// Now put the data in the clipboard
		if (OpenClipboard (NULL) != 0) {
			
			if (EmptyClipboard() == 0) {
				MessageBox (NULL, "EmptyClipboard Failed.", "Error", MB_OK);					
				return false;
			}

			if (SetClipboardData (CF_TEXT, hMem) == NULL) {
				dwErr = GetLastError ();
				MessageBox (NULL, "SetClipboardData Failed.", "Error", MB_OK);					
				return false;
			}

			if (CloseClipboard () == 0) {
				dwErr = GetLastError ();
				MessageBox (NULL, "CloseClipboard Failed.", "Error", MB_OK);					
				return false;
			}						

		} else {
			dwErr = GetLastError ();
			MessageBox (NULL, "OpenClipboard Failed.", "Error", MB_OK);								
			return false;
		}
		
		return true;
	} else {
		return false;
	}
}

bool MemoryBuffer::ReadFromFile (LPCSTR lpszFilename)
{
	FILE *stream2;
	int fh = 0;
	int filelen = 0;

	// Open a file
	if( _sopen_s(&fh, lpszFilename, _O_BINARY | _O_RDONLY, _SH_DENYNO, _S_IREAD | _S_IWRITE ) == 0 )
	{
		filelen = _filelength( fh );
		_close( fh );

		if( (stream2 = fopen( lpszFilename, "rb" )) == NULL ) {
			_fcloseall( );
			

			errno_t err;			
			_get_errno( &err );
			
			char szErr[512];
			ZeroMemory (szErr, 512);

			sprintf_s(szErr, 512, "errno = %d\n", err );
			//sprintf_s(szErr, 512, "fyi, ENOENT = %d\n", ENOENT );			

			MessageBox (NULL, strerror (err), "Error", MB_OK);

			return false;
		} else {
			// Allocate the memory we need
			SetSize (filelen);

			fread( m_buffer, sizeof(BYTE), filelen, stream2);
			m_iAppendpointer = filelen;
			m_iBuffersize = filelen;

			_fcloseall( );
			return true;
		}
	    // All other files are closed:
	    _fcloseall( );
	} else {
		

		//MessageBox (NULL, "Second Error!", "Error", MB_OK);
		return false;
	}
}

bool MemoryBuffer::ReadFromResource (LPCTSTR ResourceName) {
	
	HRSRC hResource;
	hResource = FindResource (GetModuleHandle (NULL), ResourceName, "BINARY");
	if (hResource == NULL) {
		return false;
	} else {
		HGLOBAL hLoad = LoadResource (GetModuleHandle (NULL), hResource);
		void *data = LockResource (hLoad);
		size_t sz = SizeofResource (GetModuleHandle (NULL), hResource);
		
		SetSize (sz);
		memcpy (m_buffer, data, sz);

		return true;
	}
}

bool MemoryBuffer::SwapBytes (unsigned long iIndex1, unsigned long iIndex2)
{
	BYTE bValue1;
	BYTE bValue2;

	if (m_bSizeset == true) {
		if (m_iAppendpointer > 0) {
			if (iIndex1 >= 0 && iIndex1 <=m_iAppendpointer) {
				if (iIndex2 >= 0 && iIndex2 <=m_iAppendpointer) {
					if (iIndex2 != iIndex1) {
						bValue1 = m_buffer[iIndex1];
						bValue2 = m_buffer[iIndex2];
						m_buffer[iIndex1] = bValue2;
						m_buffer[iIndex2] = bValue1;
						return true;
					} else {
						return false;
					}
				} else {
					return false;
				}
			} else {
				return false;
			}
		} else {
			if (iIndex1 >= 0 && iIndex1 <=m_iBuffersize) {
				if (iIndex2 >= 0 && iIndex2 <=m_iBuffersize) {
					if (iIndex2 != iIndex1) {
						bValue1 = m_buffer[iIndex1];
						bValue2 = m_buffer[iIndex2];
						m_buffer[iIndex1] = bValue2;
						m_buffer[iIndex2] = bValue1;
						return true;
					} else {
						return false;
					}
				} else {
					return false;
				}
			} else {
				return false;
			}
		}
	} else {
		return false;
	}
}

void MemoryBuffer::QuickCrypt (bool Encrypt)
{
	// Build a really small and simple key
	int ikey[20];
	ikey[0] = 20;
	ikey[1] = 10;
	ikey[2] = 87;
	ikey[3] = 45;
	ikey[4] = 32;
	ikey[5] = 14;
	ikey[6] = 35;
	ikey[7] = 67;
	ikey[8] = 77;
	ikey[9] = 89;
	ikey[10] = 62;
	ikey[11] = 74;
	ikey[12] = 72;
	ikey[13] = 42;
	ikey[14] = 12;
	ikey[15] = 93;
	ikey[16] = 72;
	ikey[17] = 54;
	ikey[18] = 39;
	ikey[19] = 55;
	ikey[20] = 41;
	
	int ikeypointer = 0;
	unsigned long size = 0;
	unsigned long b = 0;
	BYTE bCurrent;

	if (m_bSizeset == true) {
		if (m_iAppendpointer > 0) {
			size = m_iAppendpointer;
		} else {
			size = m_iBuffersize;
		}

		for (b=0;b<size;b++) {
			bCurrent = GetByte (b);

			if (Encrypt == true) {
				bCurrent += ikey[ikeypointer];
			} else {
				bCurrent -= ikey[ikeypointer];
			}

			if (ikeypointer > 20) {
				ikeypointer = 0;
			}

			SetByte (b, bCurrent);
		}
	}
}

unsigned long MemoryBuffer::GetAppendPointer ()
{
	return m_iAppendpointer;
}

void *MemoryBuffer::GetBuffer ()
{
	return (void *) m_buffer;
}

bool MemoryBuffer::IsSizeset ()
{
	return m_bSizeset;
}

void MemoryBuffer::Clear ()
{
	if (m_bCleared == false) {
		m_bCleared = true;
		m_bSizeset = false;
		m_iBuffersize = 0;
		m_iAppendpointer = 0;
		// Free the buffer
		free (m_buffer);	
	}
}