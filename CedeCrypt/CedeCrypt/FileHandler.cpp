#include "FileHandler.h"

FileHandler::FileHandler ()
{

}

FileHandler::~FileHandler ()
{

}

void FileHandler::SplitFileParts (char *szInpath, char *szOutNoext, char *szExtonly)
{
	

	char szCurchar[SIZE_NAME];
	int seploc = 0;

	for (int c=strlen(szInpath);c>0;c--) {
		
		ZeroMemory (szCurchar, SIZE_NAME);
		strncpy_s (szCurchar, SIZE_NAME, szInpath+c, 1);

		if (strcmp (szCurchar, ".") == 0) {
			seploc = c;
			break;
		}
	}

	ZeroMemory (szOutNoext, SIZE_STRING);
	ZeroMemory (szExtonly, SIZE_STRING);

	strncpy_s (szOutNoext, SIZE_STRING, szInpath, seploc);
	strcpy_s (szExtonly, SIZE_STRING, szInpath+seploc);
}

bool FileHandler::FileExists (char *FileName)
{
	// Check if the file exists		
	struct _finddata_t c_file;
	long hFile;

	// Build the list of files in the source path
	if( (hFile = _findfirst(FileName, &c_file )) == -1L ) {
		return false;
	} else {
		return true;
	}

	return false;
}

bool FileHandler::CheckDuplicatedFile (char *szDestpath, char *szNewDestpath)
{
	if (strlen (szDestpath) == 0) {
		return false;
	}

	char szNoext[SIZE_STRING];
	char szExt[SIZE_STRING];

	char szProposedpath[SIZE_STRING];
	char szNum[SIZE_NAME];

	int f = 1;

	// First check if the dest path exists
	if (FileExists (szDestpath) == false) {
		// Just give back the original path, since it's ok.
		ZeroMemory (szNewDestpath, SIZE_STRING);
		strcpy_s (szNewDestpath, SIZE_STRING, szDestpath);
		return true;
	} else {
		// The destination file does exist, so we need to propose a new one
		
		// Split the original path into path and ext
		SplitFileParts (szDestpath, szNoext, szExt);

		for (f=1;f<1000;f++) {
			// first create a new proposed destination path

			ZeroMemory (szProposedpath, SIZE_STRING);			
			strcpy_s (szProposedpath, SIZE_STRING, szNoext);

			// Append a number
			ZeroMemory (szNum, SIZE_NAME);
			itoa (f, szNum, 10);

			// Complete the proposed path
			strcat_s (szProposedpath, SIZE_STRING, szNum);
			strcat_s (szProposedpath, SIZE_STRING, szExt);

			// Now check it does not exist
			if (FileExists (szProposedpath) == false) {
				ZeroMemory (szNewDestpath, SIZE_STRING);
				strcpy_s (szNewDestpath, SIZE_STRING, szProposedpath);
				return true;
			}
		}

		// If we got here, then we could not find a proposed path for over 1000 files
		// so return false
		return false;
	}
}

