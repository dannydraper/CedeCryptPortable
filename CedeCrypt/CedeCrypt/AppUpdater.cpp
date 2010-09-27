#include "AppUpdater.h"

AppUpdater::AppUpdater ()
{
	m_bUseDiagnostics = true;

	strcpy_s (m_szWebsite, SIZE_STRING, "http://www.cedesoft.com");
	strcpy_s (m_szVersioninfo, SIZE_STRING, "http://www.cedesoft.com/AppUpdates/cedecrypt.bin");

	m_iVersioninfomax = 70000; // The maximum possible size of the version info file
	m_iCurrentversion = 243; // This is the current version of the application - ALWAYS KEEP THIS UP TO DATE!!!
}

AppUpdater::~AppUpdater ()
{

}

void AppUpdater::SetDiagnostics (Diagnostics *pdiag)
{
	m_pdiag = pdiag;
}

bool AppUpdater::DeserialiseVersionInfo (MemoryBuffer *memFile)
{
	char szWebversion[SIZE_STRING];
	ZeroMemory (szWebversion, SIZE_STRING);

	char szLatestupdates[SIZE_STRING];
	ZeroMemory (szLatestupdates, SIZE_STRING);

	char szNumfiles[SIZE_STRING];
	ZeroMemory (szNumfiles, SIZE_STRING);

	char szLocalfile1[SIZE_STRING];
	ZeroMemory (szLocalfile1, SIZE_STRING);

	char szWebloc1[SIZE_STRING];
	ZeroMemory (szWebloc1, SIZE_STRING);

	char szLocalfile2[SIZE_STRING];
	ZeroMemory (szLocalfile2, SIZE_STRING);

	char szWebloc2[SIZE_STRING];
	ZeroMemory (szWebloc2, SIZE_STRING);

	char szLocalfile3[SIZE_STRING];
	ZeroMemory (szLocalfile3, SIZE_STRING);

	char szWebloc3[SIZE_STRING];
	ZeroMemory (szWebloc3, SIZE_STRING);

	char szLocalfile4[SIZE_STRING];
	ZeroMemory (szLocalfile4, SIZE_STRING);

	char szWebloc4[SIZE_STRING];
	ZeroMemory (szWebloc4, SIZE_STRING);

	int iwebversion = 0;
	int imagicnumber = 0;
	int ilatestupdateslen = 0;
	int inumfiles = 0;
	
	int ilocalfile1len = 0;
	int iwebfile1len = 0;
	
	int ilocalfile2len = 0;
	int iwebfile2len = 0;
	
	int ilocalfile3len = 0;
	int iwebfile3len = 0;
	
	int ilocalfile4len = 0;
	int iwebfile4len = 0;

	int ipointer = 0;

	memcpy (&imagicnumber, (BYTE *) memFile->GetBuffer ()+ipointer, sizeof (int));
	ipointer+=sizeof (int);

	if (imagicnumber == 47715) { // A basic check to make sure this is the file we want

		memcpy (&iwebversion, (BYTE *) memFile->GetBuffer ()+ipointer, sizeof (int));
		ipointer+=sizeof (int);
		
		memcpy (&ilatestupdateslen, (BYTE *) memFile->GetBuffer ()+ipointer, sizeof (int));
		ipointer+=sizeof (int);

		memcpy (szLatestupdates, (char *) memFile->GetBuffer ()+ipointer, ilatestupdateslen);
		ipointer+=ilatestupdateslen;

		memcpy (&inumfiles, (BYTE *) memFile->GetBuffer ()+ipointer, sizeof (int));
		ipointer+=sizeof (int);


		itoa (iwebversion, szWebversion, 10);
		itoa (inumfiles, szNumfiles, 10);


		memcpy (&ilocalfile1len, (BYTE *) memFile->GetBuffer ()+ipointer, sizeof (int));
		ipointer+=sizeof (int);

		memcpy (szLocalfile1, (char *) memFile->GetBuffer ()+ipointer, ilocalfile1len);
		ipointer+=ilocalfile1len;

		memcpy (&iwebfile1len, (BYTE *) memFile->GetBuffer ()+ipointer, sizeof (int));
		ipointer+=sizeof (int);

		memcpy (szWebloc1, (char *) memFile->GetBuffer ()+ipointer, iwebfile1len);
		ipointer+=iwebfile1len;



		memcpy (&ilocalfile2len, (BYTE *) memFile->GetBuffer ()+ipointer, sizeof (int));
		ipointer+=sizeof (int);

		memcpy (szLocalfile2, (char *) memFile->GetBuffer ()+ipointer, ilocalfile2len);
		ipointer+=ilocalfile2len;

		memcpy (&iwebfile2len, (BYTE *) memFile->GetBuffer ()+ipointer, sizeof (int));
		ipointer+=sizeof (int);

		memcpy (szWebloc2, (char *) memFile->GetBuffer ()+ipointer, iwebfile2len);
		ipointer+=iwebfile2len;


		
		memcpy (&ilocalfile3len, (BYTE *) memFile->GetBuffer ()+ipointer, sizeof (int));
		ipointer+=sizeof (int);

		memcpy (szLocalfile3, (char *) memFile->GetBuffer ()+ipointer, ilocalfile3len);
		ipointer+=ilocalfile3len;

		memcpy (&iwebfile3len, (BYTE *) memFile->GetBuffer ()+ipointer, sizeof (int));
		ipointer+=sizeof (int);

		memcpy (szWebloc3, (char *) memFile->GetBuffer ()+ipointer, iwebfile3len);
		ipointer+=iwebfile3len;



		memcpy (&ilocalfile4len, (BYTE *) memFile->GetBuffer ()+ipointer, sizeof (int));
		ipointer+=sizeof (int);

		memcpy (szLocalfile4, (char *) memFile->GetBuffer ()+ipointer, ilocalfile4len);
		ipointer+=ilocalfile4len;

		memcpy (&iwebfile4len, (BYTE *) memFile->GetBuffer ()+ipointer, sizeof (int));
		ipointer+=sizeof (int);

		memcpy (szWebloc4, (char *) memFile->GetBuffer ()+ipointer, iwebfile4len);
		ipointer+=iwebfile4len;	

		ZeroMemory (m_szLatestupdates, SIZE_STRING);
		ZeroMemory (m_szLocalfile1, SIZE_STRING);
		ZeroMemory (m_szWebfile1, SIZE_STRING);

		ZeroMemory (m_szLocalfile2, SIZE_STRING);
		ZeroMemory (m_szWebfile2, SIZE_STRING);

		ZeroMemory (m_szLocalfile3, SIZE_STRING);
		ZeroMemory (m_szWebfile3, SIZE_STRING);

		ZeroMemory (m_szLocalfile4, SIZE_STRING);
		ZeroMemory (m_szWebfile4, SIZE_STRING);

		m_iWebversion = iwebversion;
		m_iNumupdatefiles = inumfiles;

		strcpy_s (m_szLatestupdates, SIZE_STRING, szLatestupdates);
		strcpy_s (m_szLocalfile1, SIZE_STRING, szLocalfile1);
		strcpy_s (m_szWebfile1, SIZE_STRING, szWebloc1);

		strcpy_s (m_szLocalfile2, SIZE_STRING, szLocalfile2);
		strcpy_s (m_szWebfile2, SIZE_STRING, szWebloc2);

		strcpy_s (m_szLocalfile3, SIZE_STRING, szLocalfile3);
		strcpy_s (m_szWebfile3, SIZE_STRING, szWebloc3);

		strcpy_s (m_szLocalfile4, SIZE_STRING, szLocalfile4);
		strcpy_s (m_szWebfile4, SIZE_STRING, szWebloc4);
		
		return true;
	} else {
		OutputText ("Version Info magic number not found! Invalid data.");
		return false;
	}
}

bool AppUpdater::IsUpdateCheckPossible () {
	// A function to check if net access is available

	if (m_inet.DoesWebConnectionExist () == true) {
		return true;
	} else {
		OutputText ("Internet Connection not available.");
		return false;
	}

}

bool AppUpdater::CheckForUpdate ()
{
	MemoryBuffer memVersioninfo;

	if (m_inet.InternetFiletoMemoryBuffer (m_szVersioninfo, &memVersioninfo, m_iVersioninfomax) == true) {
		if (DeserialiseVersionInfo (&memVersioninfo) == true) {
			OutputText ("Internet Update Check successful.");
			OutputInt ("Web version: ", m_iWebversion);
			OutputInt ("Num update files: ", m_iNumupdatefiles);

			OutputText ("Local File 1: ", m_szLocalfile1);
			OutputText ("Web File 1: ", m_szWebfile1);

			OutputText ("Local File 2: ", m_szLocalfile2);
			OutputText ("Web File 2: ", m_szWebfile2);

			OutputText ("Local File 3: ", m_szLocalfile3);
			OutputText ("Web File 3: ", m_szWebfile3);

			OutputText ("Local File 4: ", m_szLocalfile4);
			OutputText ("Web File 4: ", m_szWebfile4);
			return true;
		} else {
			OutputText ("Deserialisation failed.");
			return false;
		}
	} else {
		OutputText ("Version file download failed.");
		return false;
	}
}

bool AppUpdater::IsNewVersionAvailable ()
{
	if (m_iWebversion > m_iCurrentversion) {
		return true;
	} else {
		return false;
	}
}

void AppUpdater::GetProgramFilePaths ()
{
	// First get the full path to the current running process.
	// This is assumed to be where the user installed the app - e.g. C:\Program Files\.....
	ZeroMemory (m_szCurexepathfull, SIZE_STRING);
	GetModuleFileName (NULL, m_szCurexepathfull, SIZE_STRING);

	// Now get the path only - without the filename on the end
	ZeroMemory (m_szCurexepathonly, SIZE_STRING);
	ZeroMemory (m_szCurfileonly, SIZE_STRING);
	GetPathOnly (m_szCurexepathfull, m_szCurexepathonly, m_szCurfileonly, "\\");

	//Now construct the old full path - basically with CedeCrypt.old at the end
	ZeroMemory (m_szCuroldpathfull, SIZE_STRING); // The full path with .old at the end
	ZeroMemory (m_szCurnameonly, SIZE_STRING); // The name of the file only e.g. CedeCrypt.
	ZeroMemory (m_szCurextonly, SIZE_STRING); // the extension of the file e.g. exe
	GetPathOnly (m_szCurfileonly, m_szCurnameonly, m_szCurextonly, ".");

	strcat_s (m_szCuroldpathfull, SIZE_STRING, m_szCurexepathonly);
	strcat_s (m_szCuroldpathfull, SIZE_STRING, m_szCurnameonly);
	strcat_s (m_szCuroldpathfull, SIZE_STRING, "old");

	OutputText ("Full EXE Path: ", m_szCurexepathfull);
	OutputText ("Path Only: ", m_szCurexepathonly);
	OutputText ("File Only: ", m_szCurfileonly);
	OutputText ("Name Only: ", m_szCurnameonly);
	OutputText ("Ext Only: ", m_szCurextonly);
	OutputText ("Old Path: ", m_szCuroldpathfull);
}

bool AppUpdater::FileExists (char *FileName)
{
    FILE* fp = NULL;

    //will not work if you do not have read permissions
    //to the file, but if you don't have read, it
    //may as well not exist to begin with.

    fp = fopen( FileName, "rb" );
    if( fp != NULL )
    {
        fclose( fp );
        return true;
    }

    return false;
}

bool AppUpdater::UpdateSingleFile (char *szLocalfilename, char *szWebfile)
{
	// First construct the old path
	char szOldpath[SIZE_STRING];
	ZeroMemory (szOldpath, SIZE_STRING);

	strcpy_s (szOldpath, SIZE_STRING, m_szCurexepathonly);
	strcat_s (szOldpath, SIZE_STRING, szLocalfilename);
	strcat_s (szOldpath, SIZE_STRING, "old");

	OutputText ("UpdateSingleFile: OldPath: ", szOldpath);

	// Now construct the new path
	char szCurpath[SIZE_STRING];
	ZeroMemory (szCurpath, SIZE_STRING);

	strcpy_s (szCurpath, SIZE_STRING, m_szCurexepathonly);
	strcat_s (szCurpath, SIZE_STRING, szLocalfilename);

	OutputText ("UpdateSingleFile: CurPath: ", szCurpath);


	if (FileExists (szOldpath) == true) {
		if (DeleteFile (szOldpath) == TRUE) {
			OutputText ("UpdateSingleFile: OLD file path deleted ok.", szOldpath);
		} else {
			OutputText ("UpdateSingleFile: Unable to delete existing OLD file! - ", szOldpath);
			return false;
		}
	}

	// Now get the web version of the file
	MemoryBuffer memWebfile;
	if (m_inet.InternetFiletoMemoryBuffer (szWebfile, &memWebfile, 10000000) == false) {
		OutputText ("UpdateSingleFile: Unable to download updated file from the web!");
		return false;
	}

	if (FileExists (szCurpath) == true) {
		if (MoveFile (szCurpath, szOldpath) == TRUE) {
			if (memWebfile.SaveToFile (szCurpath) == false) {
				MoveFile (szOldpath, szCurpath);
				OutputText ("UpdateSingleFile: unable to save newer file to disk!", szCurpath);
				return false;
			}
		} else {
			OutputText ("UpdateSingleFile: Unable to rename file!");
		}
	} else {
		OutputText ("UpdateSingleFile: Local file not present - Update not necessary: ", szCurpath);
	}

	OutputText ("UpdateSingleFile: File Update Success: ", szCurpath);
	return true;
}

bool AppUpdater::UpdateFromInternet ()
{
	int f = 0;
	GetProgramFilePaths ();

	OutputInt ("Number of updated files: ", m_iNumupdatefiles);
	bool updatesuccess = true;

	for (f=0;f<m_iNumupdatefiles;f++) {
		
		if (f==0) {
			if (UpdateSingleFile (m_szLocalfile1, m_szWebfile1) == false) {
				updatesuccess = false;
				return false;
			}
		}

		if (f==1) {
			if (UpdateSingleFile (m_szLocalfile2, m_szWebfile2) == false) {
				updatesuccess = false;
			}
		}

		if (f==2) {
			if (UpdateSingleFile (m_szLocalfile3, m_szWebfile3) == false) {
				updatesuccess = false;
			}
		}

		if (f==3) {
			if (UpdateSingleFile (m_szLocalfile4, m_szWebfile4) == false) {
				updatesuccess = false;
			}
		}

	}

	return updatesuccess;
}

void AppUpdater::GetPathOnly (char *szInpath, char *szOutpath, char *szOutfile, char *szSep)
{
	char szCurchar[SIZE_NAME];
	int seploc = 0;

	for (int c=strlen(szInpath);c>0;c--) {
		
		ZeroMemory (szCurchar, SIZE_NAME);
		strncpy_s (szCurchar, SIZE_NAME, szInpath+c, 1);

		if (strcmp (szCurchar, szSep) == 0) {
			seploc = c+1;
			break;
		}
	}

	strncpy_s (szOutpath, SIZE_STRING, szInpath, seploc);
	strncpy_s (szOutfile, SIZE_STRING, szInpath+seploc, strlen (szInpath)-seploc);
}

bool AppUpdater::ShowNewVersionInfo ()
{
	char szMessage[SIZE_STRING];

	strcpy_s (szMessage, SIZE_STRING, "A new version of CedeCrypt is available! The new version contains the following updates:\n\n");
	strcat_s (szMessage, SIZE_STRING, m_szLatestupdates);
	strcat_s (szMessage, SIZE_STRING, "\n\nWould you like to install this new version?");

	if (MessageBox (NULL, szMessage, "CedeCrypt Update", MB_ICONQUESTION | MB_YESNO) == IDYES) {
		return true;
	} else {
		return false;
	}
}



void AppUpdater::OutputInt (LPCSTR lpszText, int iValue)
{
	if (m_bUseDiagnostics == true) {
		m_pdiag->OutputInt (lpszText, iValue);
	}
}

void AppUpdater::OutputText (LPCSTR lpszText)
{
	if (m_bUseDiagnostics == true) {
		m_pdiag->OutputText (lpszText);
	}
}

void AppUpdater::OutputText (LPCSTR lpszName, LPCSTR lpszValue)
{
	if (m_bUseDiagnostics == true) {
		m_pdiag->OutputText (lpszName, lpszValue);
	}
}