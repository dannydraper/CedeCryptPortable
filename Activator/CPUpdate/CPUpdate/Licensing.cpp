#include "Licensing.h"

Licensing::Licensing ()
{
	m_bUseDiagnostics = true;

	ZeroMemory (m_szOwner, SIZE_STRING);
	m_iFirstAuth = 0;
	m_iWebID = 0;
	m_bTrial = true;
	m_iTrialdays = 0;
	m_iMaxclients = 0;
	ZeroMemory (m_szUniqueID, SIZE_STRING);
	m_iYear = 0;
	m_iMonth = 0;
	m_iDay = 0;
	m_iVendID1 = 0;
	m_iVendID2 = 0;
	m_iSecondAuth = 0;
	m_iDistID1 = 0;
	m_iDistID2 = 0;
	m_iFeatureMode = 0;
	m_bFirstinst = false;
	m_idaysintrial = 0;
	m_bDLLpathprepared = false;
}

Licensing::~Licensing ()
{
}

unsigned long Licensing::Raise (unsigned long lValue, unsigned long lPower)
{
	unsigned long lRes = lValue;
	for (int l=1;l<lPower;l++) {
		lRes = lRes * lValue;
	}
	return lRes;
}

void Licensing::Expand (char *szInval, char *szOutval)
{
	//strcpy_s (szOutval, SIZE_STRING, "Output Test");
	char szExp1[SIZE_NAME];
	char szExp2[SIZE_NAME];
	unsigned long lExp1 = 0;
	unsigned long lExp2 = 0;

	unsigned long lPower = 0;

	char szCurResult[SIZE_STRING];

	for (int c=0;c<strlen(szInval);c++) {
		if (c>0) {

			ZeroMemory (szExp1, SIZE_NAME);
			ZeroMemory (szExp2, SIZE_NAME);
			strncpy_s (szExp1, SIZE_NAME, szInval+(c-1), 1);
			strncpy_s (szExp2, SIZE_NAME, szInval+c, 1);

			lExp1 = atol (szExp1);
			lExp2 = atol (szExp2);

			//OutputInt ("Long Exp1: ", lExp1);
			//OutputInt ("Long Exp2: ", lExp2);

			lPower = Raise (lExp1, lExp2);

			ZeroMemory (szCurResult, SIZE_STRING);
			ltoa (lPower, szCurResult, 10);

			//OutputText ("szCurResult: ", szCurResult);

			strcat_s (szOutval, SIZE_STRING, szCurResult);
		}
	}
}

void Licensing::RepeatExpand (char *szInval, int Length, char *szOutval)
{
	char szResult[SIZE_STRING];
	ZeroMemory (szResult, SIZE_STRING);

	Expand (szInval, szResult);

	char szNewResult[SIZE_STRING];

	while (strlen(szResult) < Length)
	{
		ZeroMemory (szNewResult, SIZE_STRING);
		Expand (szResult, szNewResult);

		strcpy_s (szResult, SIZE_STRING, szNewResult);
	}

	strcpy_s (szOutval, SIZE_STRING, szResult);
}

void Licensing::FilterH (char *szInval, char *szOutval)
{
	char szCurrent [SIZE_NAME];
	for (int c=0;c<strlen (szInval);c++) 
	{
		strncpy_s (szCurrent, SIZE_NAME, szInval+c, 1);

		if (strcmp (szCurrent, "-") != 0) {
			strcat_s (szOutval, SIZE_STRING, szCurrent);
		}
	}
}

void Licensing::UnShiftPChar (char *szCharone, int Shiftamount, char *szOutval)
{
	int curcode = szCharone[0];

	for (int c=0;c<Shiftamount;c++)
	{
		curcode--;

		if (curcode > 57 && curcode < 65) {
			curcode = 57;
		}

		if (curcode < 48) {
			curcode = 90;
		}
	}

	szOutval[0] = curcode;
}

void Licensing::DecString (char *szInstring, char *szKeyhive, char *szOutstring)
{
	char szCurKey[SIZE_NAME];
	int curkey = 0;

	char szCurChar[SIZE_NAME];
	char szEncChar[SIZE_NAME];

	for (int c=0;c<strlen (szInstring);c++) {
		
		ZeroMemory (szCurKey, SIZE_NAME);
		strncpy_s (szCurKey, SIZE_NAME, szKeyhive+c, 2);
		
		curkey = atoi (szCurKey);

		ZeroMemory (szCurChar, SIZE_NAME);
		strncpy_s (szCurChar, SIZE_NAME, szInstring+c, 1);

		ZeroMemory (szEncChar, SIZE_NAME);
		UnShiftPChar (szCurChar, curkey, szEncChar);

		strcat_s (szOutstring, SIZE_STRING, szEncChar);
	}
}

void Licensing::EncBytes (char *szInstring, char *szKeyhive, MemoryBuffer *poutBuffer)
{
	char szCurchar[SIZE_NAME];
	int iCurchar = 0;

	char szCurKey[SIZE_NAME];
	int curkey = 0;

	BYTE bEnc = 0;
	
	for (int c=0;c<strlen(szInstring);c++) {
		// Get the current byte value
		ZeroMemory (szCurchar, SIZE_NAME);
		strncpy_s (szCurchar, SIZE_NAME, szInstring+c, 1);
		iCurchar = szCurchar[0];

		// Get the current key value
		ZeroMemory (szCurKey, SIZE_NAME);
		strncpy_s (szCurKey, SIZE_NAME, szKeyhive+c, 3);
		curkey = atoi (szCurKey);

		bEnc = iCurchar+curkey;

		//OutputInt ("bEnc: ", bEnc);

		poutBuffer->SetByte (c, bEnc);
	}
}

void Licensing::DecBytes (MemoryBuffer *pinBuffer, char *szKeyhive, char *szOutString)
{
	BYTE bCurEnc = 0;
	BYTE bDec = 0;

	char szCurKey[SIZE_NAME];
	int curkey = 0;

	char szCurDec[SIZE_NAME];

	for (int c=0;c<pinBuffer->GetSize ();c++) {
		// Get the current key value
		ZeroMemory (szCurKey, SIZE_NAME);
		strncpy_s (szCurKey, SIZE_NAME, szKeyhive+c, 3);
		curkey = atoi (szCurKey);
		
		bCurEnc = pinBuffer->GetByte (c);
		bDec = bCurEnc-curkey;

		ZeroMemory (szCurDec, SIZE_NAME);
		szCurDec[0] = bDec;

		strcat_s (szOutString, SIZE_STRING, szCurDec);
	}
}

int Licensing::UnShiftSingle (int inval, int shiftamount)
{
	int curval = inval;

	for (int c=0;c<shiftamount;c++) {
		curval--;

		if (curval < 0) {
			curval = 9;
		}
	}

	return curval;
}

void Licensing::DecCS (char *szInstring, int csum, char *szOutstring)
{
	char szCurchar[SIZE_NAME];
	char szNewchar[SIZE_NAME];
	int inewchar = 0;
	int icurchar = 0;

	for (int c=0;c<strlen (szInstring);c++) {
		
		ZeroMemory (szCurchar, SIZE_NAME);
		strncpy_s (szCurchar, SIZE_NAME, szInstring+c, 1);

		icurchar = atoi (szCurchar);
		inewchar = UnShiftSingle (icurchar, csum);
		
		itoa (inewchar, szNewchar, 10);

		strcat_s (szOutstring, SIZE_STRING, szNewchar);
	}
}

int Licensing::GenCSum (char *szInstring)
{
	int cs = 0;

	char szCurchar[SIZE_NAME];
	int icurchar = 0;

	for (int c=0;c<strlen (szInstring);c++) {
		ZeroMemory (szCurchar, SIZE_NAME);

		strncpy_s (szCurchar, SIZE_NAME, szInstring+c, 1);
		icurchar = atoi (szCurchar);

		cs+=icurchar;
	}

	return cs;
}

bool Licensing::IsNumber (char *szInstring)
{
	char szCurchar[SIZE_NAME];
	int iCurasc = 0;

	for (int c=0;c<strlen (szInstring);c++) {
		
		ZeroMemory (szCurchar, SIZE_NAME);
		strncpy_s (szCurchar, SIZE_NAME, szInstring+c, 1);

		iCurasc = szCurchar[0];

		if (iCurasc < 48 || iCurasc > 57) {
			return false;
		}
	}

	return true;
}

void Licensing::TextFromASC (char *szInstring, char *szOutstring)
{
	char szCurasc[SIZE_NAME];
	int iCurasc = 0;
	char szRechar[SIZE_NAME];

	for (int c=0;c<strlen (szInstring);c+=3) {
		
		ZeroMemory (szCurasc, SIZE_NAME);
		strncpy_s (szCurasc, SIZE_NAME, szInstring+c, 3);
		
		iCurasc = atoi (szCurasc);

		if (iCurasc > 0 && iCurasc < 254) {
			
			ZeroMemory (szRechar, SIZE_NAME);			
			szRechar[0] = iCurasc;

			strcat_s (szOutstring, SIZE_STRING, szRechar);
		}

	}
}

bool Licensing::FileExists (char *FileName)
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

bool Licensing::PrepareDLLPath ()
{
	// Function to prepare the DLL path which will live under C:\Windows

	//char szSysRoot[SIZE_STRING];
	//ZeroMemory (szSysRoot, SIZE_STRING);

	//if (GetEnvironmentVariable ("APPDATA", szSysRoot, SIZE_STRING) == 0) {
	//	// retrieving environment variable failed.
	//	//m_diag.OutputText ("Failed to retrieve APPDATA environment variable!");		
	//	return false;
	//} 

	//strcpy_s (m_szDLLPath, SIZE_STRING, szSysRoot);
	
	//strcpy_s (m_szDLLPath, SIZE_STRING, "C:\\Data\\Work\\DJD\\Programming\\Win32Work\\CedeCryptPortable\\CedeCrypt\\release\\CedeCrypt.exe");
	m_bDLLpathprepared = true;
	
	return true;
}

void Licensing::SetDLLPath (char *szInpath)
{
	ZeroMemory (m_szDLLPath, SIZE_STRING);
	strcpy_s (m_szDLLPath, SIZE_STRING, szInpath);
	m_bDLLpathprepared = true;
}

bool Licensing::CheckDLLPath ()
{
	if (m_bDLLpathprepared == false) {
		PrepareDLLPath ();
	}

	if (FileExists (m_szDLLPath) == false) {
		return false;
	} else {
		return true;
	} 
}

void Licensing::ReadMachine (char *szOutstring)
{

	char szMachineKey [SIZE_STRING];
	ZeroMemory (szMachineKey, SIZE_STRING);
	GenMachineKey (szMachineKey);

	char szIDateKey[SIZE_STRING];
	ZeroMemory (szIDateKey, SIZE_STRING);
	
	char szProdIDKey[SIZE_STRING];
	ZeroMemory (szProdIDKey, SIZE_STRING);

	char szRegOrgKey[SIZE_STRING];
	ZeroMemory (szRegOrgKey, SIZE_STRING);

	char szRegOwnKey[SIZE_STRING];
	ZeroMemory (szRegOwnKey, SIZE_STRING);

	GenRegKeys (szIDateKey, szProdIDKey, szRegOrgKey, szRegOwnKey);

	DWORD dwIDate;
	dwIDate = m_registry.ReadDword (szMachineKey, szIDateKey);

	char szIDate[SIZE_STRING];
	ZeroMemory (szIDate, SIZE_STRING);
	itoa (dwIDate, szIDate, 10);

	char szProdID[SIZE_STRING];
	ZeroMemory (szProdID, SIZE_STRING);
	strcpy_s (szProdID, SIZE_STRING, m_registry.ReadString (szMachineKey, szProdIDKey));

	char szRegOrg[SIZE_STRING];
	ZeroMemory (szRegOrg, SIZE_STRING);
	strcpy_s (szRegOrg, SIZE_STRING, m_registry.ReadString (szMachineKey, szRegOrgKey));

	char szRegOwn[SIZE_STRING];
	ZeroMemory (szRegOwn, SIZE_STRING);
	strcpy_s (szRegOwn, SIZE_STRING, m_registry.ReadString (szMachineKey, szRegOwnKey));

	//OutputText ("szIDate: ", szIDate);
	//OutputText ("ProdID: ", szProdID);
	//OutputText ("RegOrg: ", szRegOrg);
	//OutputText ("RegOwn: ", szRegOwn);


	char szFullID[SIZE_STRING];
	ZeroMemory (szFullID, SIZE_STRING);

	strcat_s (szFullID, SIZE_STRING, szIDate);
	strcat_s (szFullID, SIZE_STRING, szProdID);
	strcat_s (szFullID, SIZE_STRING, szRegOrg);
	strcat_s (szFullID, SIZE_STRING, szRegOwn);

	//OutputText ("Full ID: ", szFullID);

	char szExpandedKey[SIZE_STRING];
	ZeroMemory (szExpandedKey, SIZE_STRING);
	RepeatExpand ("2347", 77, szExpandedKey);

	DecString (szFullID, szExpandedKey, szOutstring);	
}

void Licensing::GetASC (char *szValue)
{

	char szCurchar[SIZE_NAME];
	int icurchar = 0;

	char szCurasc[SIZE_NAME];

	char szFinal[SIZE_STRING];
	ZeroMemory (szFinal, SIZE_STRING);

	char szCrlf[SIZE_NAME];
	ZeroMemory (szCrlf, SIZE_NAME);
	szCrlf[0] = 13;
	szCrlf[1] = 10;

	for (int c=0;c<strlen (szValue);c++) {
		ZeroMemory (szValue, SIZE_NAME);
		strncpy_s (szCurchar, SIZE_NAME, szValue+c, 1);	
		icurchar = szCurchar[0];

		ZeroMemory (szCurasc, SIZE_NAME);
		itoa (icurchar, szCurasc, 10);



		strcat_s (szFinal, SIZE_STRING, szCurasc);
		strcat_s (szFinal, SIZE_STRING, szCrlf);
	}

	MemoryBuffer tmpBuf;
	tmpBuf.SetSize (strlen (szFinal));
	
	tmpBuf.Write ((char *) szFinal, 0, strlen (szFinal));

	//tmpBuf.SaveToFile ("D:\\szFinal.txt");
}

void Licensing::AddASC (char *szAppendstring, int ascval)
{
	char szAscchar[SIZE_NAME];
	ZeroMemory (szAscchar, SIZE_NAME);
	szAscchar[0] = ascval;

	strcat_s (szAppendstring, SIZE_STRING, szAscchar);
}

#pragma region asc machine key generation

void Licensing::GenMachineKey (char *szOutstring)
{
	AddASC (szOutstring, 83);
	AddASC (szOutstring, 79);
	AddASC (szOutstring, 70);
	AddASC (szOutstring, 84);
	AddASC (szOutstring, 87);
	AddASC (szOutstring, 65);
	AddASC (szOutstring, 82);
	AddASC (szOutstring, 69);
	AddASC (szOutstring, 92);
	AddASC (szOutstring, 77);
	AddASC (szOutstring, 105);
	AddASC (szOutstring, 99);
	AddASC (szOutstring, 114);
	AddASC (szOutstring, 111);
	AddASC (szOutstring, 115);
	AddASC (szOutstring, 111);
	AddASC (szOutstring, 102);
	AddASC (szOutstring, 116);
	AddASC (szOutstring, 92);
	AddASC (szOutstring, 87);
	AddASC (szOutstring, 105);
	AddASC (szOutstring, 110);
	AddASC (szOutstring, 100);
	AddASC (szOutstring, 111);
	AddASC (szOutstring, 119);
	AddASC (szOutstring, 115);
	AddASC (szOutstring, 32);
	AddASC (szOutstring, 78);
	AddASC (szOutstring, 84);
	AddASC (szOutstring, 92);
	AddASC (szOutstring, 67);
	AddASC (szOutstring, 117);
	AddASC (szOutstring, 114);
	AddASC (szOutstring, 114);
	AddASC (szOutstring, 101);
	AddASC (szOutstring, 110);
	AddASC (szOutstring, 116);
	AddASC (szOutstring, 86);
	AddASC (szOutstring, 101);
	AddASC (szOutstring, 114);
	AddASC (szOutstring, 115);
	AddASC (szOutstring, 105);
	AddASC (szOutstring, 111);
	AddASC (szOutstring, 110);
}

void Licensing::GenRegKeys (char *szIDate, char *szProdID, char *RegOrg, char *RegOwn)
{
	// InstallDate
	AddASC (szIDate,73);
	AddASC (szIDate,110);
	AddASC (szIDate,115);
	AddASC (szIDate,116);
	AddASC (szIDate,97);
	AddASC (szIDate,108);
	AddASC (szIDate,108);
	AddASC (szIDate,68);
	AddASC (szIDate,97);
	AddASC (szIDate,116);
	AddASC (szIDate,101);

	// ProductId
	AddASC (szProdID,80);
	AddASC (szProdID,114);
	AddASC (szProdID,111);
	AddASC (szProdID,100);
	AddASC (szProdID,117);
	AddASC (szProdID,99);
	AddASC (szProdID,116);
	AddASC (szProdID,73);
	AddASC (szProdID,100);

	// RegisteredOrganization
	AddASC (RegOrg,82);
	AddASC (RegOrg,101);
	AddASC (RegOrg,103);
	AddASC (RegOrg,105);
	AddASC (RegOrg,115);
	AddASC (RegOrg,116);
	AddASC (RegOrg,101);
	AddASC (RegOrg,114);
	AddASC (RegOrg,101);
	AddASC (RegOrg,100);
	AddASC (RegOrg,79);
	AddASC (RegOrg,114);
	AddASC (RegOrg,103);
	AddASC (RegOrg,97);
	AddASC (RegOrg,110);
	AddASC (RegOrg,105);
	AddASC (RegOrg,122);
	AddASC (RegOrg,97);
	AddASC (RegOrg,116);
	AddASC (RegOrg,105);
	AddASC (RegOrg,111);
	AddASC (RegOrg,110);

	// RegisteredOwner
	AddASC (RegOwn,82);
	AddASC (RegOwn,101);
	AddASC (RegOwn,103);
	AddASC (RegOwn,105);
	AddASC (RegOwn,115);
	AddASC (RegOwn,116);
	AddASC (RegOwn,101);
	AddASC (RegOwn,114);
	AddASC (RegOwn,101);
	AddASC (RegOwn,100);
	AddASC (RegOwn,79);
	AddASC (RegOwn,119);
	AddASC (RegOwn,110);
	AddASC (RegOwn,101);
	AddASC (RegOwn,114);
}

#pragma endregion

DWORD Licensing::GetVolumeSerialNumber ()
{
	char szVolname[SIZE_STRING];
	ZeroMemory (szVolname, SIZE_STRING);

	DWORD dwSerial = 0;

	DWORD dwMaxcomponent = 0;
	DWORD dwFlags = 0;
	
	char szFilesystem[SIZE_STRING];
	ZeroMemory (szFilesystem, SIZE_STRING);

	char szModule[SIZE_STRING];
	ZeroMemory (szModule, SIZE_STRING);
	GetModuleFileName (NULL, szModule, SIZE_STRING);

	char szDrive[SIZE_NAME];
	ZeroMemory (szDrive, SIZE_NAME);
	strncpy_s (szDrive, SIZE_NAME, szModule, 3);

	if (GetVolumeInformation (szDrive, szVolname, SIZE_STRING, &dwSerial, &dwMaxcomponent, &dwFlags, szFilesystem, SIZE_STRING) != 0) {
		
		return dwSerial;
	} else {
		
		return 0;
	}

	return 0;
}

void Licensing::SaveMachineKey ()
{
	char szExpandedKey[SIZE_STRING];
	ZeroMemory (szExpandedKey, SIZE_STRING);
	RepeatExpand ("2347", 77, szExpandedKey);

	char szModule[SIZE_STRING];
	ZeroMemory (szModule, SIZE_STRING);
	GetModuleFileName (NULL, szModule, SIZE_STRING);

	//OutputText ("Module: ", szModule);
	//MessageBox (NULL, szModule, "Module", MB_OK);

	if (m_bDLLpathprepared == false) {
		PrepareDLLPath ();
	}

	unsigned long lwriteoffset = GetWriteableOffset ();

	// Open the DLL
	MemoryBuffer memFile;
	memFile.ReadFromFile (m_szDLLPath);
	
	// Generate the machine key
	DWORD dwSerial = GetVolumeSerialNumber ();
	char szMachineID [SIZE_STRING];
	ZeroMemory (szMachineID, SIZE_STRING);
	//ReadMachine (szMachineID);
	sprintf_s (szMachineID, SIZE_STRING, "%X", dwSerial);

	// Encode the machine key
	MemoryBuffer memEncoded;
	memEncoded.SetSize (strlen (szMachineID));
	EncBytes (szMachineID, szExpandedKey, &memEncoded);

	// Write the size to the file
	int iencsize = memEncoded.GetSize ();
	memFile.Write (&iencsize, lwriteoffset+1700, sizeof (int));

	// Write the encoded key to the file
	memFile.Write (memEncoded.GetBuffer (), lwriteoffset+1900, memEncoded.GetSize ());
	memFile.SaveToFile (m_szDLLPath);	
}


bool Licensing::LoadMachineKey (char *szOutKey)
{
	char szExpandedKey[SIZE_STRING];
	ZeroMemory (szExpandedKey, SIZE_STRING);
	RepeatExpand ("2347", 77, szExpandedKey);

	char szModule[SIZE_STRING];
	ZeroMemory (szModule, SIZE_STRING);
	GetModuleFileName (NULL, szModule, SIZE_STRING);

	//OutputText ("Module: ", szModule);
	//MessageBox (NULL, szModule, "Module", MB_OK);

	if (m_bDLLpathprepared == false) {
		PrepareDLLPath ();
	}

	unsigned long lwriteoffset = GetWriteableOffset ();

	// Open the DLL
	MemoryBuffer memFile;
	if (memFile.ReadFromFile (m_szDLLPath) == true) {
		// Get the size of the encoded key from the file
		int iencsize = 0;
		memcpy (&iencsize, (BYTE *) memFile.GetBuffer ()+lwriteoffset+1700, sizeof (int));

		// Now read the encoded machine key
		MemoryBuffer memEncoded;
		memEncoded.SetSize (iencsize);
		memEncoded.Write ((BYTE *) memFile.GetBuffer ()+lwriteoffset+1900, 0, iencsize);

		//Now decode the key
		DecBytes (&memEncoded, szExpandedKey, szOutKey);

		return true;
	} else {
		return false;
	}
}

bool Licensing::MachineMatch ()
{
	// Get the machine key from disk
	char szDiskKey[SIZE_STRING];
	ZeroMemory (szDiskKey, SIZE_STRING);
	if (LoadMachineKey (szDiskKey) == true) {
		// Generate the actual machine key machine key
		DWORD dwSerial = GetVolumeSerialNumber ();
		char szMachineID [SIZE_STRING];
		ZeroMemory (szMachineID, SIZE_STRING);
		sprintf_s (szMachineID, SIZE_STRING, "%X", dwSerial);

		// Check for a match
		if (strcmp (szDiskKey, szMachineID) == 0) {
			return true;
		} else {
			return false;
		}
	} else {
		return false;
	}

	return false;
}

void Licensing::GetPathOnly (char *szInpath, char *szOutpath)
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

unsigned long Licensing::GetWriteableOffset ()
{
	// This function scans the CedeCrypt portable executable and obtains the writeable offset
	// by scanning for the 7 magic numbers which are the headers for the cp canvas resource.
	// the writeable offset is then returned. It is then up to the calling function to specify
	// a sub-offset to actually write the data, the offset returned here will only return the
	// beginning of the resource

	unsigned long lk1 = 14777324;
	unsigned long lk2 = 77782415;
	unsigned long lk3 = 77777777;
	unsigned long lk4 = 44444444;
	unsigned long lk5 = 77777777;
	unsigned long lk6 = 11881188;
	unsigned long lk7 = 11881188;

	unsigned long lc1 = 0;
	unsigned long lc2 = 0;
	unsigned long lc3 = 0;
	unsigned long lc4 = 0;
	unsigned long lc5 = 0;
	unsigned long lc6 = 0;
	unsigned long lc7 = 0;

	if (m_bDLLpathprepared == false) {
		PrepareDLLPath ();
	}

	MemoryBuffer memFile;
	memFile.ReadFromFile (m_szDLLPath);

	unsigned int msize = sizeof (unsigned long) * 7;

	unsigned long b = 0;
	unsigned int ipointer = 0;

	
	if (memFile.GetSize () > 0) {
		for (b=0;b<memFile.GetSize ()-msize;b++) {
			ipointer = 0;

			memcpy (&lc1, (BYTE *) memFile.GetBuffer ()+b+ipointer, sizeof (unsigned long));
			ipointer+=sizeof (unsigned long);
			memcpy (&lc2, (BYTE *) memFile.GetBuffer ()+b+ipointer, sizeof (unsigned long));
			ipointer+=sizeof (unsigned long);
			memcpy (&lc3, (BYTE *) memFile.GetBuffer ()+b+ipointer, sizeof (unsigned long));
			ipointer+=sizeof (unsigned long);
			memcpy (&lc4, (BYTE *) memFile.GetBuffer ()+b+ipointer, sizeof (unsigned long));
			ipointer+=sizeof (unsigned long);
			memcpy (&lc5, (BYTE *) memFile.GetBuffer ()+b+ipointer, sizeof (unsigned long));
			ipointer+=sizeof (unsigned long);
			memcpy (&lc6, (BYTE *) memFile.GetBuffer ()+b+ipointer, sizeof (unsigned long));
			ipointer+=sizeof (unsigned long);
			memcpy (&lc7, (BYTE *) memFile.GetBuffer ()+b+ipointer, sizeof (unsigned long));
			ipointer+=sizeof (unsigned long);

			if (lc1 == lk1 && lc2 == lk2 && lc3 == lk3 && lc4 == lk4 && lc5 == lk5 && lc6 == lk6 && lc7 == lk7) {
				return b+msize+10;
			}
			
		}
	}
	
	

	return 0;
}

void Licensing::SaveUserKey (char *szUserKey)
{
	char szExpandedKey[SIZE_STRING];
	ZeroMemory (szExpandedKey, SIZE_STRING);
	RepeatExpand ("2347", 77, szExpandedKey);

	//char szModule[SIZE_STRING];
	//ZeroMemory (szModule, SIZE_STRING);
	//GetModuleFileName (NULL, szModule, SIZE_STRING);

	//OutputText ("Module: ", szModule);
	//MessageBox (NULL, szModule, "Module", MB_OK);

	if (m_bDLLpathprepared == false) {
		PrepareDLLPath ();
	}

	// Open the DLL
	MemoryBuffer memFile;
	memFile.ReadFromFile (m_szDLLPath);
	
	// Encode the user key
	MemoryBuffer memEncoded;
	memEncoded.SetSize (strlen (szUserKey));
	EncBytes (szUserKey, szExpandedKey, &memEncoded);

	unsigned long lwriteoffset = GetWriteableOffset ();

	if (lwriteoffset > 200) {
		// Write the size to the file
		int iencsize = memEncoded.GetSize ();
		memFile.Write (&iencsize, lwriteoffset + 10, sizeof (int));

		// Write the encoded key to the file
		memFile.Write (memEncoded.GetBuffer (), lwriteoffset + 70, memEncoded.GetSize ());
		memFile.SaveToFile (m_szDLLPath);
	}
}

void Licensing::ClearAllKeys ()
{
	char szModule[SIZE_STRING];
	ZeroMemory (szModule, SIZE_STRING);
	GetModuleFileName (NULL, szModule, SIZE_STRING);

	//OutputText ("Module: ", szModule);
	//MessageBox (NULL, szModule, "Module", MB_OK);

	if (m_bDLLpathprepared == false) {
		PrepareDLLPath ();
	}

	unsigned long lwriteoffset = GetWriteableOffset ();

	// Open the DLL
	MemoryBuffer memFile;
	memFile.ReadFromFile (m_szDLLPath);
	
	// Clear all the key bytes
	for (int c=lwriteoffset+9;c<lwriteoffset+4000;c++) {
		memFile.SetByte (c, 0);
	}
	
	memFile.SaveToFile (m_szDLLPath);
}

bool Licensing::LoadUserKey (char *szOutkey)
{
	char szExpandedKey[SIZE_STRING];
	ZeroMemory (szExpandedKey, SIZE_STRING);
	RepeatExpand ("2347", 77, szExpandedKey);

	char szModule[SIZE_STRING];
	ZeroMemory (szModule, SIZE_STRING);
	GetModuleFileName (NULL, szModule, SIZE_STRING);

	//OutputText ("Module: ", szModule);
	//MessageBox (NULL, szModule, "Module", MB_OK);

	if (m_bDLLpathprepared == false) {
		PrepareDLLPath ();
	}

	unsigned long lwriteoffset = GetWriteableOffset ();

	// Open the DLL
	MemoryBuffer memFile;
	if (memFile.ReadFromFile (m_szDLLPath) == true) {
		// Get the size of the encoded key from the file
		int iencsize = 0;
		memcpy (&iencsize, (BYTE *) memFile.GetBuffer ()+lwriteoffset+10, sizeof (int));

		// Now read the encoded user key
		MemoryBuffer memEncoded;
		memEncoded.SetSize (iencsize);
		memEncoded.Write ((BYTE *) memFile.GetBuffer ()+lwriteoffset+70, 0, iencsize);

		//Now decode the key
		DecBytes (&memEncoded, szExpandedKey, szOutkey);

		return true;
	} else {
		return false;
	}
}

bool Licensing::UserKeyPresent ()
{
	char szModule[SIZE_STRING];
	ZeroMemory (szModule, SIZE_STRING);
	GetModuleFileName (NULL, szModule, SIZE_STRING);

	//OutputText ("Module: ", szModule);
	//MessageBox (NULL, szModule, "Module", MB_OK);

	if (m_bDLLpathprepared == false) {
		PrepareDLLPath ();
	}

	unsigned long lwriteoffset = GetWriteableOffset ();

	BYTE bCur = 0;
	int numvalid = 0;

	// Open the DLL
	MemoryBuffer memFile;
	if (memFile.ReadFromFile (m_szDLLPath) == true) {
		// Get the size of the encoded key from the file
		for (int b=lwriteoffset+70;b<lwriteoffset+700;b++) {
			bCur = memFile.GetByte (b);

			if (bCur > 0 && bCur < 255) {
				numvalid++;
			}
		}				
		
		if (numvalid > 12) {
			return true;
		} else {
			return false;
		}
	} else {
		return false;
	}
}

bool Licensing::CLPasswordPresent ()
{
	char szModule[SIZE_STRING];
	ZeroMemory (szModule, SIZE_STRING);
	GetModuleFileName (NULL, szModule, SIZE_STRING);

	if (m_bDLLpathprepared == false) {
		PrepareDLLPath ();
	}

	BYTE bCur = 0;
	int numvalid = 0;

	// Open the DLL
	MemoryBuffer memFile;
	if (memFile.ReadFromFile (m_szDLLPath) == true) {
		// Get the size of the encoded key from the file
		for (int b=43632;b<43824;b++) {
			bCur = memFile.GetByte (b);

			if (bCur > 0 && bCur < 255) {
				numvalid++;
			}
		}				
		
		if (numvalid > 2) {
			memFile.Clear ();
			return true;
		} else {
			memFile.Clear ();
			return false;
		}
	} else {
		memFile.Clear ();
		return false;
	}
}

void Licensing::CLClearPassword ()
{
	char szModule[SIZE_STRING];
	ZeroMemory (szModule, SIZE_STRING);
	GetModuleFileName (NULL, szModule, SIZE_STRING);

	if (m_bDLLpathprepared == false) {
		PrepareDLLPath ();
	}

	// Open the DLL
	MemoryBuffer memFile;
	memFile.ReadFromFile (m_szDLLPath);
	
	// Clear all the key bytes
	for (int c=43616;c<43824;c++) {
		memFile.SetByte (c, 0);
	}
	
	memFile.SaveToFile (m_szDLLPath);

	memFile.Clear ();
}

bool Licensing::CLIsPasswordValid (char *szPassword)
{
	char szModule[SIZE_STRING];
	ZeroMemory (szModule, SIZE_STRING);
	GetModuleFileName (NULL, szModule, SIZE_STRING);

	//OutputText ("Module: ", szModule);
	//MessageBox (NULL, szModule, "Module", MB_OK);

	if (m_bDLLpathprepared == false) {
		PrepareDLLPath ();
	}

	char szLocalPassword[SIZE_STRING];
	char szSavedPassword[SIZE_STRING];

	// Open the DLL
	MemoryBuffer memFile;
	if (memFile.ReadFromFile (m_szDLLPath) == true) {
		// Get the size of the encoded key from the file
		int iencsize = 0;
		memcpy (&iencsize, (BYTE *) memFile.GetBuffer ()+43616, sizeof (int));

		// Now read the encoded user key
		MemoryBuffer memEncoded;
		memEncoded.SetSize (iencsize);
		memEncoded.Write ((BYTE *) memFile.GetBuffer ()+43632, 0, iencsize);

		//Now decode the password
		memEncoded.QuickCrypt (false);	

		ZeroMemory (szLocalPassword, SIZE_STRING);
		strcpy_s (szLocalPassword, SIZE_STRING, szPassword);

		ZeroMemory (szSavedPassword, SIZE_STRING);
		memcpy (szSavedPassword, (char *) memEncoded.GetBuffer (), iencsize);

		if (strcmp (szLocalPassword, szSavedPassword) == 0) {
			memFile.Clear ();
			return true;
		} else {
			memFile.Clear ();
			return false;
		}

	} else {
		memFile.Clear ();
		return false;
	}

}

bool Licensing::CLSaveNewPassword (char *szPassword)
{
	char szModule[SIZE_STRING];
	ZeroMemory (szModule, SIZE_STRING);
	GetModuleFileName (NULL, szModule, SIZE_STRING);

	//OutputText ("Module: ", szModule);
	//MessageBox (NULL, szModule, "Module", MB_OK);

	if (m_bDLLpathprepared == false) {
		PrepareDLLPath ();
	}

	// Open the DLL
	MemoryBuffer memFile;
	memFile.ReadFromFile (m_szDLLPath);
	
	char szLocalPassword[SIZE_STRING];
	ZeroMemory (szLocalPassword, SIZE_STRING);
	strcpy_s (szLocalPassword, SIZE_STRING, szPassword);

	// Encode the user key
	MemoryBuffer memEncoded;
	memEncoded.SetSize (strlen (szLocalPassword));
	memEncoded.Write (szLocalPassword, 0, strlen (szLocalPassword));
	memEncoded.QuickCrypt (true);

	// Write the size to the file
	int iencsize = memEncoded.GetSize ();
	memFile.Write (&iencsize, 43616, sizeof (int));

	// Write the encoded key to the file
	memFile.Write (memEncoded.GetBuffer (), 43632, memEncoded.GetSize ());
	memFile.SaveToFile (m_szDLLPath);

	memEncoded.Clear ();
	memFile.Clear ();

	return true;
}

bool Licensing::VerifyStore ()
{
	char szModule[SIZE_STRING];
	ZeroMemory (szModule, SIZE_STRING);
	GetModuleFileName (NULL, szModule, SIZE_STRING);

	//OutputText ("Module: ", szModule);
	//MessageBox (NULL, szModule, "Module", MB_OK);

	if (m_bDLLpathprepared == false) {
		PrepareDLLPath ();
	}

	BYTE bCur = 0;
	int numvalid = 0;

	// Open the DLL
	MemoryBuffer memFile;
	if (memFile.ReadFromFile (m_szDLLPath) == true) {
		// Get the size of the encoded key from the file
		if (memFile.GetSize () > 48000 && memFile.GetSize () < 59000) {
			return true;
		} else {
			return false;
		}
	} else {
		return false;
	}
}

bool Licensing::ValidateUserKey (char *szInputKey)
{
	char szExpandedKey[SIZE_STRING];
	ZeroMemory (szExpandedKey, SIZE_STRING);

	RepeatExpand ("2347", 77, szExpandedKey);


	if (strlen (szInputKey) < 20) {
		return false;
	}

	char szRemoveH [SIZE_STRING];
	ZeroMemory (szRemoveH, SIZE_STRING);

	FilterH (szInputKey, szRemoveH);

	char szDecString[SIZE_STRING];
	ZeroMemory (szDecString, SIZE_STRING);

	DecString (szRemoveH, szExpandedKey, szDecString);

	if (IsNumber (szDecString) == false) {
		//MessageBox (NULL, "Invalid License Key", "Licensing Error", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	char szCSum[SIZE_STRING];
	ZeroMemory (szCSum, SIZE_STRING);

	strcpy_s (szCSum, SIZE_STRING, szDecString+(strlen (szDecString)-5));
	
	char szRemCSum[SIZE_STRING];
	ZeroMemory (szRemCSum, SIZE_STRING);
	strncpy_s (szRemCSum, SIZE_STRING, szDecString, strlen(szDecString)-5);
	
	int icsum = atoi (szCSum);

	char szDSum[SIZE_STRING];
	ZeroMemory (szDSum, SIZE_STRING);

	DecCS (szRemCSum, icsum, szDSum);

	int resum = GenCSum (szDSum);

	if (icsum != resum) {
		//MessageBox (NULL, "Invalid License Key", "Licensing Error", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	char szOwnerASC [SIZE_STRING];
	ZeroMemory (szOwnerASC, SIZE_STRING);

	strcpy_s (szOwnerASC, SIZE_STRING, szDSum+62);

	char szOwnerPlain [SIZE_STRING];
	ZeroMemory (szOwnerPlain, SIZE_STRING);

	TextFromASC (szOwnerASC, szOwnerPlain);

	char szNoOwner [SIZE_STRING];
	ZeroMemory (szNoOwner, SIZE_STRING);

	strncpy_s (szNoOwner, SIZE_STRING, szDSum, 62);

	//OutputText ("License Owner: ", szOwnerPlain);
	strcpy_s (m_szOwner, SIZE_STRING, szOwnerPlain);

	char szFirstAuth[SIZE_NAME];
	ZeroMemory (szFirstAuth, SIZE_NAME);
	strncpy_s (szFirstAuth, SIZE_NAME, szNoOwner, 4);
	//OutputText ("First Auth: ", szFirstAuth);
	m_iFirstAuth = atoi (szFirstAuth);
	if (m_iFirstAuth >= 2347 && m_iFirstAuth <= 4077) {
		m_bGenuineOne = true;
	} else {
		m_bGenuineTwo = false;
	}
	//2347-4077

	char szWebID[SIZE_NAME];
	ZeroMemory (szWebID, SIZE_NAME);
	strncpy_s (szWebID, SIZE_NAME, szNoOwner+4, 2);
	//OutputText ("Web ID: ", szWebID);
	m_iWebID = atoi (szWebID);
	if (m_iWebID >= 0 && m_iWebID <= 47) {
		m_bWebsource = true;
	} else {
		m_bWebsource = false;
	}

	char szMode[SIZE_NAME];
	ZeroMemory (szMode, SIZE_NAME);
	strncpy_s (szMode, SIZE_NAME, szNoOwner+6, 3);
	//OutputText ("Trial / Full: ", szMode);
	m_iTrial = atoi (szMode);
	if (m_iTrial >= 0 && m_iTrial <= 747) {
		m_bTrial = true;
	} else {
		m_bTrial = false;
	}

	char szTrialDays[SIZE_NAME];
	ZeroMemory (szTrialDays, SIZE_NAME);
	strncpy_s (szTrialDays, SIZE_NAME, szNoOwner+9, 2);
	//OutputText ("Trial Days: ", szTrialDays);
	m_iTrialdays = atoi (szTrialDays);

	char szMaxClients[SIZE_NAME];
	ZeroMemory (szMaxClients, SIZE_NAME);
	strncpy_s (szMaxClients, SIZE_NAME, szNoOwner+11, 5);
	//OutputText ("Max Clients: ", szMaxClients);
	m_iMaxclients = atoi (szMaxClients);

	char szUniqueID[SIZE_NAME];
	ZeroMemory (szUniqueID, SIZE_NAME);
	strncpy_s (szUniqueID, SIZE_NAME, szNoOwner+16, 14);
	//OutputText ("Unique ID: ", szUniqueID);
	strcpy_s (m_szUniqueID, SIZE_STRING, szUniqueID);

	char szExpYear[SIZE_NAME];
	ZeroMemory (szExpYear, SIZE_NAME);
	strncpy_s (szExpYear, SIZE_NAME, szNoOwner+30, 4);
	//OutputText ("Exp Year: ", szExpYear);
	m_iYear = atoi (szExpYear);

	char szExpMonth[SIZE_NAME];
	ZeroMemory (szExpMonth, SIZE_NAME);
	strncpy_s (szExpMonth, SIZE_NAME, szNoOwner+34, 2);
	//OutputText ("Exp Month: ", szExpMonth);
	m_iMonth = atoi (szExpMonth);

	char szExpDay[SIZE_NAME];
	ZeroMemory (szExpDay, SIZE_NAME);
	strncpy_s (szExpDay, SIZE_NAME, szNoOwner+36, 2);
	//OutputText ("Exp Day: ", szExpDay);
	m_iDay = atoi (szExpDay);

	char szVendOne[SIZE_NAME];
	ZeroMemory (szVendOne, SIZE_NAME);
	strncpy_s (szVendOne, SIZE_NAME, szNoOwner+38, 4);
	//OutputText ("Vend ID1: ", szVendOne);
	m_iVendID1 = atoi (szVendOne);

	char szVendTwo[SIZE_NAME];
	ZeroMemory (szVendTwo, SIZE_NAME);
	strncpy_s (szVendTwo, SIZE_NAME, szNoOwner+42, 4);
	//OutputText ("Vend ID2: ", szVendTwo);
	m_iVendID2 = atoi (szVendTwo);

	char szSecondAuth[SIZE_NAME];
	ZeroMemory (szSecondAuth, SIZE_NAME);
	strncpy_s (szSecondAuth, SIZE_NAME, szNoOwner+46, 4);
	//OutputText ("Second Auth: ", szSecondAuth);
	m_iSecondAuth = atoi (szSecondAuth);
	if (m_iSecondAuth >= 7077 && m_iSecondAuth <= 9999) {
		m_bGenuineTwo = true;
	} else {
		m_bGenuineTwo = false;
	}
	//7077-9999

	char szDistOne[SIZE_NAME];
	ZeroMemory (szDistOne, SIZE_NAME);
	strncpy_s (szDistOne, SIZE_NAME, szNoOwner+50, 4);
	//OutputText ("Dist ID1: ", szDistOne);
	m_iDistID1 = atoi (szDistOne);

	char szDistTwo[SIZE_NAME];
	ZeroMemory (szDistTwo, SIZE_NAME);
	strncpy_s (szDistTwo, SIZE_NAME, szNoOwner+54, 4);
	//OutputText ("Dist ID2: ", szDistTwo);
	m_iDistID2 = atoi (szDistTwo);

	char szFeatures[SIZE_NAME];
	ZeroMemory (szFeatures, SIZE_NAME);
	strncpy_s (szFeatures, SIZE_NAME, szNoOwner+58, 4);
	//OutputText ("Feature Mode: ", szFeatures);
	m_iFeatureMode = atoi (szFeatures);

	//OutputText ("Full Key: ", szNoOwner);

	return true;
}

void Licensing::EncryptString (char *szInstring, char *szOutstring)
{
	int ilen = 0;
	int iChar = 0;
	int iStage = 0;
	BOOL enDone = FALSE;
	char cChar;
	char sztOutstring[255];
	ZeroMemory (sztOutstring, 255);

	char sztInstring[255];
	ZeroMemory (sztInstring, 255);
	
	strcpy (sztInstring, szInstring);
	ilen = strlen (sztInstring);	

	char szChar[8];
	ZeroMemory (szChar, 8);
	
	for (int c=0;c<ilen;c++) {
		enDone = FALSE;
		ZeroMemory (szChar, 1);
		//szChar[0] = sztInstring[c];
		strncpy (szChar, sztInstring+c, 1);
	
		iChar = szChar[0];
		//cChar = iChar+1;

		if (iStage == 0 && enDone == FALSE) {
			cChar = iChar + 24;
			iStage = 1;
			enDone = TRUE;
		}
		if (iStage == 1 && enDone == FALSE) {
			cChar = iChar - 17;
			iStage = 2;
			enDone = TRUE;
		}
		if (iStage == 2 && enDone == FALSE) {
			cChar = iChar + 25;
			iStage = 3;
			enDone = TRUE;
		}
		if (iStage == 3 && enDone == FALSE) {
			cChar = iChar - 13;
			iStage = 4;
			enDone = TRUE;
		}
		if (iStage == 4 && enDone == FALSE) {
			cChar = iChar + 29;
			iStage = 5;
			enDone = TRUE;
		}
		if (iStage == 5 && enDone == FALSE) {
			cChar = iChar - 16;
			iStage = 6;
			enDone = TRUE;
		}
		if (iStage == 6 && enDone == FALSE) {
			cChar = iChar - 27;
			iStage = 7;
			enDone = TRUE;	
		}
		if (iStage == 7 && enDone == FALSE) {
			cChar = iChar + 18;
			iStage = 0;
			enDone = TRUE;
		}

		sztOutstring[c] = cChar;
	}
		
	strcpy (szOutstring, sztOutstring);
}

// Decode the given string encoded using the function above.

void Licensing::DecryptString (char *szInstring, char *szOutstring)
{
	int ilen = 0;
	int iChar = 0;
	int iStage = 0;
	BOOL enDone = FALSE;
	char cChar;
	char sztOutstring[255];
	ZeroMemory (sztOutstring, 255);

	char sztInstring[255];
	ZeroMemory (sztInstring, 255);
	
	strcpy (sztInstring, szInstring);
	ilen = strlen (sztInstring);	

	char szChar[8];
	ZeroMemory (szChar, 8);
	
	for (int c=0;c<ilen;c++) {
		enDone = FALSE;
		ZeroMemory (szChar, 1);
		//szChar[0] = sztInstring[c];
		strncpy (szChar, sztInstring+c, 1);
	
		iChar = szChar[0];
		//cChar = iChar+1;
		if (iChar != 10) {
		if (iStage == 0 && enDone == FALSE) {
			cChar = iChar - 24;
			iStage = 1;
			enDone = TRUE;
		}
		if (iStage == 1 && enDone == FALSE) {
			cChar = iChar + 17;
			iStage = 2;
			enDone = TRUE;
		}
		if (iStage == 2 && enDone == FALSE) {
			cChar = iChar - 25;
			iStage = 3;
			enDone = TRUE;
		}
		if (iStage == 3 && enDone == FALSE) {
			cChar = iChar + 13;
			iStage = 4;
			enDone = TRUE;
		}
		if (iStage == 4 && enDone == FALSE) {
			cChar = iChar - 29;
			iStage = 5;
			enDone = TRUE;
		}
		if (iStage == 5 && enDone == FALSE) {
			cChar = iChar + 16;
			iStage = 6;
			enDone = TRUE;
		}
		if (iStage == 6 && enDone == FALSE) {
			cChar = iChar + 27;
			iStage = 7;
			enDone = TRUE;	
		}
		if (iStage == 7 && enDone == FALSE) {
			cChar = iChar - 18;
			iStage = 0;
			enDone = TRUE;
		}
		sztOutstring[c] = cChar;
		} 						
	}
		
	strcpy (szOutstring, sztOutstring);	
}

bool Licensing::GetFirstInst ()
{
	return m_bFirstinst;
}

int Licensing::GetDaysInTrial ()
{
	return m_idaysintrial;
}

int Licensing::IdentVersion (DWORD dwHigh, DWORD dwLow) {
	FILETIME ftRTime;
	FILETIME ftSTime;
	SYSTEMTIME stRTime;
	SYSTEMTIME stSTime;
	
	long sDaysPassed = 0;
	long rDaysPassed = 0;
	
	//ShowInt ("dwHigh: ", dwHigh);
	//ShowInt ("dwLow: ", dwLow);

	ftRTime.dwLowDateTime = dwLow;
	ftRTime.dwHighDateTime = dwHigh;
	
	GetSystemTimeAsFileTime (&ftSTime);
	
	if (FileTimeToSystemTime (&ftRTime, &stRTime) != 0) {
		if (FileTimeToSystemTime (&ftSTime, &stSTime) != 0) {
			//ShowInt ("Sys Year:", stSTime.wYear);
			//ShowInt ("Sys Month:", stSTime.wMonth);
			//ShowInt ("Sys Day:", stSTime.wDay);
			//ShowInt ("Ret Year:", stRTime.wYear);
			//ShowInt ("Ret Month:", stRTime.wMonth);
			//ShowInt ("Ret Day:", stRTime.wDay);



			sDaysPassed = ((stSTime.wYear - 1980) * 365);
			sDaysPassed += ((stSTime.wMonth - 1) * 30);
			sDaysPassed += stSTime.wDay;
			//OutputDebugLong ("sDaysPassed:", sDaysPassed);
			rDaysPassed = ((stRTime.wYear - 1980) * 365);
			rDaysPassed += ((stRTime.wMonth - 1) * 30);
			rDaysPassed += stRTime.wDay;
			//OutputDebugLong ("rDaysPassed:", rDaysPassed);
			
			//ShowInt ("DaysPassed: ", sDaysPassed);
			//ShowInt ("rDaysPassed: ", rDaysPassed);
			//ShowInt ("Trial Days: ", m_iTrialdays);

			m_idaysintrial = sDaysPassed - rDaysPassed;

			if ((sDaysPassed - rDaysPassed) > m_iTrialdays) {
				return 47;
			} else {
				return 0;
			}
		} else {
			//OutputDebugString ("S Sys Time Conversion failed.");
			return 47;
		}
	} else {
		//OutputDebugString ("R Sys Time Conversion failed.");
		return 47;
	}
}

void Licensing::ShowInt (char *szTitle, int iValue)
{
	char szString[SIZE_STRING];
	ZeroMemory (szString, SIZE_STRING);

	sprintf_s (szString, SIZE_STRING, "%s: %d", szTitle, iValue);
	MessageBox (NULL, szString, "ShowInt", MB_OK);
}

int Licensing::CurrentIdent () {
	
	char szEncIdent[255];
	char szDecIdent[255];
	DWORD dwRHigh;
	DWORD dwRLow;
	FILETIME ftRTime;
	FILETIME ftSysTime;
	long ldiff = 0;
	int iv;
	//ReadRegStringValue (LPCSTR szHive, LPCSTR szKey)
	
	if (m_registry.DoesValueExist ("Software\\CedeSoft\\CedeCrypt", "Version", REG_BINARY) == true) {
		//OutputDebugString ("Ident Exists");
		ZeroMemory (szEncIdent, 255);
		ZeroMemory (szDecIdent, 255);

		MemoryBuffer memIdent;
		memIdent.SetSize (sizeof (DWORD) *2);

		m_registry.ReadBinary ("Software\\CedeSoft\\CedeCrypt", "Version", &memIdent, sizeof (DWORD) *2);

		memcpy (szEncIdent, memIdent.GetBuffer (), sizeof (DWORD) *2);

		//strcpy (szEncIdent, m_registry.ReadString ("Software\\CedeSoft\\CedeCom", "Version"));
		if (strlen (szEncIdent) == 0) {
			MessageBox (NULL, "Your trial has expired. Please purchase the full version to continue.", "CedeCrypt Trial", MB_OK | MB_ICONSTOP);
			return 47;
		}
		DecryptString (szEncIdent, szDecIdent);
		
		memcpy (&dwRLow, szDecIdent, sizeof (DWORD));
		memcpy (&dwRHigh, szDecIdent+sizeof (DWORD), sizeof (DWORD));
		ftRTime.dwLowDateTime = dwRLow;
		ftRTime.dwHighDateTime = dwRHigh;
		GetSystemTimeAsFileTime (&ftSysTime);
		ldiff = CompareFileTime (&ftRTime, &ftSysTime);
		//OutputDebugLong ("ldiff: ", ldiff);
		if (ldiff >= 0) {
			MessageBox (NULL, "Your trial has expired. Please purchase the full version to continue.", "CedeCrypt Trial", MB_OK | MB_ICONSTOP);
			return 47;
		} else {
			SaveCurrentIdent ();
			return 0;
		}
		
	} else {
		MessageBox (NULL, "Your trial has expired. Please purchase the full version to continue.", "CedeCrypt Trial", MB_OK | MB_ICONSTOP);
		return 47;
	}
}

int Licensing::VersionIdent () {
	
	char szEncIdent[255];
	char szDecIdent[255];
	DWORD dwRHigh;
	DWORD dwRLow;
	FILETIME ftRTime;
	FILETIME ftSysTime;
	long ldiff = 0;
	int iv;
	//ReadRegStringValue (LPCSTR szHive, LPCSTR szKey)
	
	if (m_registry.DoesValueExist ("Software\\CedeSoft\\CedeCrypt", "CCIdent", REG_BINARY) == true) {
		//OutputDebugString ("Ident Exists");
		ZeroMemory (szEncIdent, 255);
		ZeroMemory (szDecIdent, 255);
		
		MemoryBuffer memIdent;
		memIdent.SetSize (sizeof (DWORD) *2);

		m_registry.ReadBinary ("Software\\CedeSoft\\CedeCrypt", "CCIdent", &memIdent, sizeof (DWORD) *2);

		memcpy (szEncIdent, memIdent.GetBuffer (), sizeof (DWORD) *2);

		//strcpy (szEncIdent, m_registry.ReadString ("Software\\CedeSoft\\CedeCom", "CCIdent"));
		DecryptString (szEncIdent, szDecIdent);
		
		memcpy (&dwRLow, szDecIdent, sizeof (DWORD));
		memcpy (&dwRHigh, szDecIdent+sizeof (DWORD), sizeof (DWORD));
		//OutputDebugDword ("dwRLow: ", dwRLow);
		//OutputDebugDword ("dwRHigh: ", dwRHigh);
		ftRTime.dwLowDateTime = dwRLow;
		ftRTime.dwHighDateTime = dwRHigh;
		GetSystemTimeAsFileTime (&ftSysTime);
		ldiff = CompareFileTime (&ftRTime, &ftSysTime);
		//OutputDebugLong ("ldiff: ", ldiff);
		if (ldiff >= 0) {
			MessageBox (NULL, "The system clock has been set back possibly in an attempt to circumvent the CedeCrypt trial. Please correct this anomaly.", "CedeCrypt Trial", MB_OK | MB_ICONSTOP);
			return 47;
		} else {
			//ShowInt ("dwRHigh: ", dwRHigh);
			//ShowInt ("dwRLow: ", dwRLow);
			iv = IdentVersion (dwRHigh, dwRLow);
			if (iv == 47) {
				MessageBox (NULL, "Your trial has expired. Please purchase the full version to continue.", "CedeCrypt Trial", MB_OK | MB_ICONSTOP);
				return 47;
			} else {
				return 0;
			}
		}
		
	} else {
		MessageBox (NULL, "Your trial has expired. Please purchase the full version to continue.", "CedeCrypt Trial", MB_OK | MB_ICONSTOP);
		return 47;
	}
}

void Licensing::SaveCurrentIdent () {
	char szEncIdent[255];
	
	ZeroMemory (szEncIdent, 255);
	GetEncSysIdent (szEncIdent);
	
	MemoryBuffer memIdent;
	memIdent.SetSize (strlen (szEncIdent));

	memIdent.Write ((char *) szEncIdent, 0, strlen (szEncIdent));

	m_registry.WriteBinary ("Software\\CedeSoft\\CedeCrypt", "Version", &memIdent);

	//OutputDebugString (szEncIdent);	
	//m_registry.WriteString ("Software\\CedeSoft\\CedeCom", "Version", szEncIdent);
}

void Licensing::GetEncSysIdent (char *szOut) {
	DWORD dwHigh;
	DWORD dwLow;
	char szEncString[255];	
	

	FILETIME ftSysTime;
	GetSystemTimeAsFileTime (&ftSysTime);
	dwHigh = ftSysTime.dwHighDateTime;
	dwLow = ftSysTime.dwLowDateTime;
	
	
	ZeroMemory (szEncString, 255);
	memcpy (szEncString, &dwLow, sizeof (DWORD));
	memcpy (szEncString+sizeof (DWORD), &dwHigh, sizeof (DWORD));
	
	EncryptString (szEncString, szOut);
}

void Licensing::SaveEncIdent () {
	char szEncIdent[255];
	
	ZeroMemory (szEncIdent, 255);
	GetEncSysIdent (szEncIdent);
	
	//OutputDebugString (szEncIdent);
	MemoryBuffer memIdent;
	memIdent.SetSize (strlen (szEncIdent));

	memIdent.Write ((char *) szEncIdent, 0, strlen (szEncIdent));

	//m_registry.WriteString ("Software\\CedeSoft\\CedeCom", "CCIdent", szEncIdent);

	m_registry.WriteBinary ("Software\\CedeSoft\\CedeCrypt", "CCIdent", &memIdent);
}

void Licensing::CheckInst () {	

	if (m_registry.DoesKeyExist ("Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\MSCLegacySoftware") == false) {		
		m_registry.WriteString ("Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\MSCLegacySoftware", "CTVersion", "5.1.2247");
		SaveEncIdent ();
		SaveCurrentIdent ();
		m_bFirstinst = true;
	}
}

void Licensing::CheckInstEx () {
	if (m_registry.DoesKeyExist ("Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\MSCLegacySoftware") == false) {		
		m_registry.WriteString ("Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\MSCLegacySoftware", "CTVersion", "5.1.2247");
		
		SaveCurrentDate ("CNVersion");
		SaveCurrentDate ("CNIdent");

		m_bFirstinst = true;
	}
}

int Licensing::GetDaysPassed (WORD wDay1, WORD wMonth1, WORD wYear1, WORD wDay2, WORD wMonth2, WORD wYear2)
{
	// For each date supplied we get the number of days passed since
	// 1980 for date1 and date2 then we compare the big number

	// NOTE DATE2 is expected to be the later date.

	unsigned long lDaysPassed1; // days passed since 1980 on date1
	unsigned long lDaysPassed2; // days passed since 1980 on date2

	lDaysPassed1 = (wYear1 - 1980) * 365;
	lDaysPassed1 += (wMonth1 - 1) * 30;
	lDaysPassed1 += wDay1;

	lDaysPassed2 = (wYear2 - 1980) * 365;
	lDaysPassed2 += (wMonth2 - 1) * 30;
	lDaysPassed2 += wDay2;

	//OutputInt ("Days passed on date1: ", lDaysPassed1);
	//OutputInt ("Days passed on date2: ", lDaysPassed2);

	return lDaysPassed2 - lDaysPassed1;
}

void Licensing::ShowExpiredMessage ()
{
	MessageBox (NULL, "Your trial has expired. Please purchase the full version to continue.", "CedeCrypt Trial", MB_OK | MB_ICONSTOP);
}

int Licensing::TrialValid ()
{
	char szKeyloc[SIZE_STRING];
	ZeroMemory (szKeyloc, SIZE_STRING);
	strcpy_s (szKeyloc, SIZE_STRING, "SOFTWARE\\CedeSoft\\CedeCrypt");

	SYSTEMTIME systime;
	GetLocalTime (&systime);

	WORD lDay = systime.wDay;
	WORD lMonth = systime.wMonth;
	WORD lYear = systime.wYear;

	WORD vDay = 0;
	WORD vMonth = 0;
	WORD vYear = 0;

	WORD iDay = 0;
	WORD iMonth = 0;
	WORD iYear = 0;

	// First check if both keys are present else invalidate the trial
	if (m_registry.DoesValueExist (szKeyloc, "CNVersion", REG_BINARY) == true) {
		
		if (m_registry.DoesValueExist (szKeyloc, "CNIdent", REG_BINARY) == true) {
		
			// First check that the current date is later than the date last run
			// just to make sure the user hasn't tried to extend the trial by setting back their clock
			if (ReadSavedDate ("CNVersion", &vDay, &vMonth, &vYear) == true) {
				
				// Now the days passed on the CNVersion date must be greater or equal to zero compared
				// to the local date
				if (GetDaysPassed (vDay, vMonth, vYear, lDay, lMonth, lYear) >= 0) {
					// Now we're good to validate the trial
					// Now save the local date time in the CNVersion for when the app is next opened.
					SaveCurrentDate ("CNVersion");

					if (ReadSavedDate ("CNIdent", &iDay, &iMonth, &iYear) == true) {
						// CNIdent is the original install date. We compare this with the local date
						//OutputInt ("ins Day: ", iDay);
						//OutputInt ("ins Month: ", iMonth);
						//OutputInt ("int Year: ", iYear);

						if (GetDaysPassed (iDay, iMonth, iYear, lDay, lMonth, lYear) > m_iTrialdays) {
							m_idaysintrial = GetDaysPassed (iDay, iMonth, iYear, lDay, lMonth, lYear);
							ShowExpiredMessage ();
							return 49;
						} else {
							m_idaysintrial = GetDaysPassed (iDay, iMonth, iYear, lDay, lMonth, lYear);
							//OutputInt ("daysintrial: ", m_idaysintrial);
							return 0;
						}
					} else {
						// Possible tampering witht he CNIdent key
						ShowExpiredMessage ();
						return 48;
					}

				} else {
					// Possible tampering with the clock
					MessageBox (NULL, "The system clock has been set back possibly in an attempt to circumvent the CedeCrypt trial. Please correct this anomaly.", "CedeCrypt Trial", MB_OK | MB_ICONSTOP);
					return 47;
				}
			} else {
				// Possible tampering with the CNVersion key
				ShowExpiredMessage ();
				return 46;
			}
		
		} else {
			ShowExpiredMessage ();
			return 45;
		}

	} else {
		ShowExpiredMessage ();
		return 44;
	}
}

void Licensing::SaveCurrentDate (char *szKeyname)
{
	MemoryBuffer memLocaltime;

	SYSTEMTIME systime;
	GetLocalTime (&systime);

	WORD wday = systime.wDay;
	WORD wmonth = systime.wMonth;
	WORD wyear = systime.wYear;

	int csum = wday+wmonth+wyear;

	//OutputInt ("Local Day: ", wday);
	//OutputInt ("Local Month: ", wmonth);
	//OutputInt ("Local Year: ", wyear);

	memLocaltime.SetSize ((sizeof (WORD) * 3) + sizeof (int));

	memLocaltime.Append (&wday, sizeof (WORD));
	memLocaltime.Append (&wmonth, sizeof (WORD));
	memLocaltime.Append (&wyear, sizeof (WORD));
	memLocaltime.Append (&csum, sizeof (int));

	memLocaltime.QuickCrypt (true);

	m_registry.WriteBinary ("SOFTWARE\\CedeSoft\\CedeCrypt", szKeyname, &memLocaltime);

}

bool Licensing::ReadSavedDate (char *szKeyname, WORD *wDay, WORD *wMonth, WORD *wYear)
{
	MemoryBuffer memLoadeddate;

	m_registry.ReadBinary ("SOFTWARE\\CedeSoft\\CedeCrypt", szKeyname, &memLoadeddate, 10);

	memLoadeddate.QuickCrypt (false);

	WORD lwDay = 0;
	WORD lwMonth = 0;
	WORD lwYear = 0;
	int lsum = 0;
	int csum = 0;
	int ipointer = 0;
	
	memcpy (&lwDay, (BYTE *) memLoadeddate.GetBuffer ()+ipointer, sizeof (WORD));
	ipointer+=sizeof (WORD);

	memcpy (&lwMonth, (BYTE *) memLoadeddate.GetBuffer ()+ipointer, sizeof (WORD));
	ipointer+=sizeof (WORD);

	memcpy (&lwYear, (BYTE *) memLoadeddate.GetBuffer ()+ipointer, sizeof (WORD));
	ipointer+=sizeof (WORD);

	memcpy (&csum, (BYTE *) memLoadeddate.GetBuffer ()+ipointer, sizeof (int));
	ipointer+=sizeof (int);

	lsum = lwDay+lwMonth+lwYear;

	if (lsum != csum) {
		// Checksums stored and calculated did not match
		// possible tampering with the date
		return false;
	} else {
		*wDay = lwDay;
		*wMonth = lwMonth;
		*wYear = lwYear;
		return true;
	}
}

void Licensing::Test ()
{
	//OutputText ("Hello, this is a licensing test.");

	//Trial Key
	//FISIAC6H2-T934H58N7-DQW7UY92S-P4GSRIVUF-CRA2SSNUC-XTWPSFCQB-3SRQUEBES-YMHBCBDUE-ERAUU2TD6-2P3QE6KYH-B7IUFIVER-SNZBEFOK4-9RTC

	//Full Key	
	//GKYK8C0O1-QA34H58N7-DQW7UY92S-P4GSRIVUF-CRA2SSNUC-XTSHUFCQB-3SRQUEBES-YUHAFCENE-EU9RQ3V76-1L3RF5MZG-HDJSJJX9O-PI1B

}

char *Licensing::GetOwner ()
{
	return m_szOwner;
}

char *Licensing::GetUniqueProductID ()
{
	return m_szUniqueID;
}

bool Licensing::GetTrialMode ()
{
	return m_bTrial;
}

int Licensing::GetMaxClients ()
{
	return m_iMaxclients;
}

int Licensing::GetTrialDays ()
{
	return m_iTrialdays;
}
