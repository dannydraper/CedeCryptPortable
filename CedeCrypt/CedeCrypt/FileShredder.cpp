#include "FileShredder.h"

FileShredder::FileShredder ()
{
	m_bUseDiagnostics = true;
}

FileShredder::~FileShredder ()
{
	
}

void FileShredder::ShowInt (int iInttoShow) {
	char szMsg[255];
	ZeroMemory (szMsg, 255);
	sprintf_s (szMsg, 255, "Value of int: %d", iInttoShow);
	MessageBox (NULL, szMsg, "ShowInt", MB_OK);
}

void FileShredder::SetDiagnostics (Diagnostics *pdiag)
{
	m_pdiag = pdiag;
}

void FileShredder::InitRandom ()
{
	// Initialise a 10K randomised buffer
	BYTE bRand;

	m_randombuffer.SetSize (RAND_SIZE);
	for (int r=0;r<RAND_SIZE;r++) {
		bRand = GetRand (255);
		m_randombuffer.SetByte (r, bRand);
	}
}

BYTE FileShredder::GetRandomAlphaNum ()
{
	int iType = 0;
	int iRand = 0;

	iType = GetRand (3);
	

	if (iType == 0) {
		iRand = GetRand (9);
		return 48+iRand;
	}

	if (iType == 1) {
		iRand = GetRand (25);
		return 65+iRand;
	}

	if (iType == 2) {
		iRand = GetRand (25);
		return 97+iRand;	
	}
	return 65;
}

void FileShredder::GenFileRandomName (char *szOutname)
{
	for (int a=0;a<SIZE_NAME-32;a++) {
		szOutname[a] = GetRandomAlphaNum ();
	}
	//strncat_s (szOutname, SIZE_NAME, ".dat", 4);
}

void FileShredder::GetPathOnly (char *szInpath, char *szOutpath)
{
	char szCurchar[SIZE_NAME];
	int seploc = 0;

	for (int c=strlen(szInpath);c>0;c--) {
		
		ZeroMemory (szCurchar, SIZE_NAME);
		strncpy_s (szCurchar, SIZE_NAME, szInpath+c, 1);

		if (strcmp (szCurchar, "\\") == 0) {
			seploc = c+1;
			break;
		}
	}

	strncpy_s (szOutpath, SIZE_STRING, szInpath, seploc);
}

unsigned long FileShredder::GetFileSize (char *pszFilename)
{
	struct _finddata_t c_file;
	long hFile;
		
	if( (hFile = _findfirst(pszFilename, &c_file )) == -1L )
		return 0;
	else
	{
		return c_file.size;

		_findclose( hFile );
	}
}

int FileShredder::GetRand (int iMax)
{
	//errno_t err;
	//double max = iMax;
	//unsigned int number;

	//err = rand_s(&number); // This doesn't work on Windows 2000!

	//return (double) number / (double) UINT_MAX * max;

	return (double) rand() / (RAND_MAX + 1) * iMax;
}

void FileShredder::FillBuffer (BYTE bByte, bool bComplement, bool bRandom, unsigned long lBufSize)
{
	// Fill our write buffer with a load of bytes, or it's complement or random characters
	// as specified by DOD 5220.22-M.
	int b = 0;
	m_writebuffer.SetSize (lBufSize);
	BYTE bComp;
	BYTE bRand;

	bComp = ~bByte;

	if (bRandom == true) {
		for (b=0;b<lBufSize;b+=RAND_SIZE) {
			m_writebuffer.Write (m_randombuffer.GetBuffer (), b, RAND_SIZE);
		}
	} else {
		for (b=0;b<lBufSize;b++)
		{
			if (bComplement == true) {
				m_writebuffer.SetByte (b, bComp);
			} else {
				m_writebuffer.SetByte (b, bByte);
			}
		}
	}	
}

void FileShredder::OverwriteFile(char *pszFilename, BYTE bByte, bool bComplement, bool bRandom, unsigned long lSize)
{
	HANDLE hFile;
	DWORD dwWrittenbytes = 0;
	DWORD dwNeededbytes = 0;

	// Set the write buffer size to double the size of the file - just to make sure we clear everything
	dwNeededbytes = lSize;

	OutputInt ("OverwriteFile: Bytes needed: ", dwNeededbytes);

	hFile = CreateFile (pszFilename, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_WRITE_THROUGH, NULL);

	if (hFile == INVALID_HANDLE_VALUE) {
		OutputText ("OverwriteFile: CreateFile returned INVALID_HANDLE_VALUE");
		return;
	}

	// Fill our buffer with the specified pattern
	FillBuffer (bByte, bComplement, bRandom, lSize);

	if (WriteFile (hFile, (BYTE *) m_writebuffer.GetBuffer (), dwNeededbytes, &dwWrittenbytes, NULL) == 0) {
		OutputInt ("OverwriteFile: WriteFile failed! Error: ", GetLastError ());
	}

	// Cleanup
	CloseHandle (hFile);
	m_writebuffer.Clear ();
}

void FileShredder::SecureDelete (char *pszFilename)
{

	unsigned long lneededsize = GetFileSize (pszFilename) * 2;

	OverwriteFile (pszFilename, 100, false, false, lneededsize); // Overwrite using a single char
	//OutputText ("FileShredder: Overwrite Pass 1 Complete.");
	OverwriteFile (pszFilename, 100, true, false, lneededsize); // Then the complement
	//OutputText ("FileShredder: Overwrite Pass 2 Complete.");
	OverwriteFile (pszFilename, 177, false, false, lneededsize); // Then another char
	//OutputText ("FileShredder: Overwrite Pass 3 Complete.");
	OverwriteFile (pszFilename, 177, true, false, lneededsize); // Then it's complement
	//OutputText ("FileShredder: Overwrite Pass 4 Complete.");
	OverwriteFile (pszFilename, 177, true, true, lneededsize); // And finally a random pattern
	//OutputText ("FileShredder: Overwrite Pass 5 Complete.");

	// Rename the file lots of times, then delete it the windows way
	NameScramble (pszFilename);

	//ShowInt (0);
}

void FileShredder::NameScramble (char *pszFilename)
{
	char szPathOnly[SIZE_STRING];
	ZeroMemory (szPathOnly, SIZE_STRING);

	char szRandomName[SIZE_NAME];
	char szLastname[SIZE_STRING];

	char szPrevpath[SIZE_STRING];
	char szNewpath[SIZE_STRING];

	GetPathOnly (pszFilename, szPathOnly);

	OutputText (pszFilename);

	ZeroMemory (szPrevpath, SIZE_STRING);
	ZeroMemory (szNewpath, SIZE_STRING);

	strcpy_s (szPrevpath, SIZE_STRING, pszFilename);
	
	strncpy_s (szNewpath, SIZE_STRING, szPathOnly, strlen (szPathOnly));

	ZeroMemory (szRandomName, SIZE_NAME);
	GenFileRandomName (szRandomName);

	strncat_s (szNewpath, SIZE_STRING, szRandomName, strlen(szRandomName));

	// Do the first rename
	if (MoveFile (szPrevpath, szNewpath) == 0) {
		OutputInt ("MoveFile failed! Error: ", GetLastError ());
	}
	//OutputText (szPrevpath, szNewpath);

	ZeroMemory (szLastname, SIZE_STRING);
	strcpy_s (szLastname, SIZE_STRING, szRandomName);

	for (int n=0;n<7;n++) {
		ZeroMemory (szRandomName, SIZE_NAME);
		GenFileRandomName (szRandomName);

		//OutputText ("Random Name: ", szRandomName);

		// Do the rename
		ZeroMemory (szPrevpath, SIZE_STRING);
		ZeroMemory (szNewpath, SIZE_STRING);
		
		strncpy_s (szPrevpath, SIZE_STRING, szPathOnly, strlen (szPathOnly));
		strncat_s (szPrevpath, SIZE_STRING, szLastname, strlen (szLastname));

		strncpy_s (szNewpath, SIZE_STRING, szPathOnly, strlen (szPathOnly));
		strncat_s (szNewpath, SIZE_STRING, szRandomName, strlen (szRandomName));

		if (MoveFile (szPrevpath, szNewpath) == 0) {
			OutputInt ("MoveFile failed! Error: ", GetLastError ());
		}

		//OutputText (szPrevpath, szNewpath);

		// Save the last name we used
		ZeroMemory (szLastname, SIZE_STRING);
		strcpy_s (szLastname, SIZE_STRING, szRandomName);
	}

	// Finally delete the last path used - which will be the file still present on the hd
	DeleteFile (szNewpath);
}

void FileShredder::OutputInt (LPCSTR lpszText, int iValue)
{
	if (m_bUseDiagnostics == true) {
		m_pdiag->OutputInt (lpszText, iValue);
	}
}

void FileShredder::OutputText (LPCSTR lpszText)
{
	if (m_bUseDiagnostics == true) {
		m_pdiag->OutputText (lpszText);
	}
}

void FileShredder::OutputText (LPCSTR lpszName, LPCSTR lpszValue)
{
	if (m_bUseDiagnostics == true) {
		m_pdiag->OutputText (lpszName, lpszValue);
	}
}