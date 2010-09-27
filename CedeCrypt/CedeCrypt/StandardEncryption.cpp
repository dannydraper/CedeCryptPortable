// A standard encryption class. Written by Danny Draper (c) 2008. Provides standard encryption using Crypto API algorithms
// To support all standard algorithms such as AES, 3DES, DES, etc...

#include "StandardEncryption.h"

StandardEncryption::StandardEncryption ()
{
	m_hwnddiaglist = NULL;
	m_bOutputwindowset = false;

	m_lMagicone = 14772895557;
	m_lMagictwo = 74922457155;
	m_lMagicthree = 55555777777;
	m_lMagicfour = 77777774444;

	m_Currentalg = CALG_AES_256;
	m_dwKeylength = 0x01000000;
}

StandardEncryption::~StandardEncryption ()
{
}

void StandardEncryption::SetOutputHWND (HWND hwndOutput)
{
	m_hwnddiaglist = hwndOutput;
	m_bOutputwindowset = true;
}

void StandardEncryption::SetProgressHWND (HWND hwnd)
{
	m_hwndprogress = hwnd;
}

void StandardEncryption::SetSpeedLabelHWND (HWND hwnd)
{
	m_hwndspeed = hwnd;
}

void StandardEncryption::SetAlgorithm (unsigned int iAlgID) 
{
	if (iAlgID == ALG_AES256) {
		m_Currentalg = CALG_AES_256;
		m_dwKeylength = 0x01000000;
	}

	if (iAlgID == ALG_3DES) {
		m_Currentalg = CALG_3DES;
		m_dwKeylength = 0x00A80000;
	}

	if (iAlgID == ALG_DES) {
		m_Currentalg = CALG_DES;
		m_dwKeylength = 0x00380000;	
	}

}

void StandardEncryption::OutputText (LPCSTR lpszText)
{
	if (m_bOutputwindowset == false) {
		return;
	}

	char szText[SIZE_STRING];
	ZeroMemory (szText, SIZE_STRING);

	sprintf_s (szText, SIZE_STRING, "%d: ", GetTickCount ());

	strcat_s (szText, SIZE_STRING, lpszText);
	SendMessage (m_hwnddiaglist, LB_ADDSTRING, 0, (LPARAM) (LPCTSTR) &szText);

	int lCount = SendMessage (m_hwnddiaglist, LB_GETCOUNT, 0, 0);
	SendMessage (m_hwnddiaglist, LB_SETCURSEL, lCount-1, 0);
}

void StandardEncryption::OutputInt (LPCSTR lpszText, int iValue)
{
	
	char szInteger[SIZE_INTEGER];
	ZeroMemory (szInteger, SIZE_INTEGER);
	sprintf_s (szInteger, SIZE_INTEGER, "%d", iValue);

	char szText[SIZE_STRING];
	ZeroMemory (szText, SIZE_STRING);


	sprintf_s (szText, SIZE_STRING, "%d: ", GetTickCount ());

	strcat_s (szText, SIZE_STRING, lpszText);

	strcat_s (szText, SIZE_STRING, szInteger);

	SendMessage (m_hwnddiaglist, LB_ADDSTRING, 0, (LPARAM) (LPCTSTR) &szText);

	int lCount = SendMessage (m_hwnddiaglist, LB_GETCOUNT, 0, 0);
	SendMessage (m_hwnddiaglist, LB_SETCURSEL, lCount-1, 0);
}

void StandardEncryption::MyHandleError(char *s)
{
	DWORD dwError = GetLastError ();

	char szOutput[SIZE_STRING];
	
	ZeroMemory (szOutput, SIZE_STRING);
    sprintf_s (szOutput, SIZE_STRING, "An error occurred in running the cipher routine. \n");
    OutputText (szOutput);

	ZeroMemory (szOutput, SIZE_STRING);
	sprintf_s (szOutput, SIZE_STRING, "%s\n",s);
    OutputText (szOutput);

	ZeroMemory (szOutput, SIZE_STRING);
	sprintf_s (szOutput, SIZE_STRING, "Error number %i.\n", dwError);
    OutputText (szOutput);

} // end MyHandleError

bool StandardEncryption::GetMD5Hash (char *szPassword, char *szOutbuf)
{
	HCRYPTPROV hCryptProv; 
    HCRYPTHASH hHash; 
    BYTE bHash[0x7f]; 
    DWORD dwHashLen= 16; // The MD5 algorithm always returns 16 bytes. 
    DWORD cbContent= strlen (szPassword);
    BYTE* pbContent= (BYTE*) szPassword;

	char szFinal[SIZE_STRING];
	ZeroMemory (szFinal, SIZE_STRING);

	char szCurchar[SIZE_NAME];
	
    if(CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT | CRYPT_MACHINE_KEYSET)) {
		if(CryptCreateHash(hCryptProv, CALG_MD5, 0, 0, &hHash)) {
			if(CryptHashData(hHash, pbContent, cbContent, 0)) {
				if(CryptGetHashParam(hHash, HP_HASHVAL, bHash, &dwHashLen, 0)) {
					
					for (int i=0;i<16;i++) {
						ZeroMemory (szCurchar, SIZE_NAME);
						sprintf_s (szCurchar, SIZE_NAME, "%02x", bHash[i]);

						strcat_s (szFinal, SIZE_STRING, szCurchar);
					}

				} else {
					OutputText ("GetMD5Hash: Error getting hash param!");
					return false;
				}
			} else {
				OutputText ("GetMD5Hash: Error Hashing data!");
				return false;
			}
		} else {
			OutputText ("GetMD5Hash: Error Creating Hash!");
			return false;
		}
	} else {
		OutputText ("GetMD5Hash: Error Aquiring Context!");
		return false;
	}

	ZeroMemory (szOutbuf, SIZE_STRING);
	strcpy_s (szOutbuf, SIZE_STRING, szFinal);

	CryptDestroyHash(hHash); 
    CryptReleaseContext(hCryptProv, 0); 	
    return true; 
}

bool StandardEncryption::EncryptBuffer (MemoryBuffer *memSource, PCHAR szPassword, bool bEncrypt)
{
	HCRYPTPROV hCryptProv; 
	HCRYPTHASH hHash; 
	HCRYPTKEY hKey;
	DWORD dwBufferlen;
	DWORD dwBufsize;
	MemoryBuffer memOutput;

	// Get the handle to the default provider. 
	if(CryptAcquireContext(&hCryptProv, NULL, NULL, PROVIDER , 0)) {
	   //printf("A cryptographic provider has been acquired. \n");
	} else {
	   MyHandleError("Error during CryptAcquireContext!"); 
	   return false;
	}

	if(!szPassword ) { 
		return false;
	} else { 
		// Create a hash object. 
		if(CryptCreateHash(hCryptProv, CALG_MD5, 0, 0, &hHash)) {
			//printf("A hash object has been created. \n");
		} else { 
			 MyHandleError("Error during CryptCreateHash!");
			 return false;
		}

		//-------------------------------------------------------------------
		// Hash the password.
		if(CryptHashData(hHash, (BYTE *)szPassword, strlen(szPassword), 0)) {
			//printf("The password has been added to the hash. \n");
		} else {
			MyHandleError("Error during CryptHashData."); 
			return false;
		}

		//-------------------------------------------------------------------
		// Derive a session key from the hash object. 
		if(CryptDeriveKey(hCryptProv, m_Currentalg, hHash, m_dwKeylength, &hKey)) {
			//printf("An encryption key is derived from the password hash. \n"); 
		} else {
			MyHandleError("Error during CryptDeriveKey!");
			return false;
		}
		//-------------------------------------------------------------------
		// Destroy hash object. 
		if(hHash) {
			if(!(CryptDestroyHash(hHash))) {
			   MyHandleError("Error during CryptDestroyHash"); 
			   return false;
			}
			hHash = 0;
		}

		// Encrypt / Decrypt data.
		if (bEncrypt == true) {
			// First get the size of the buffer needed.
			dwBufferlen = memSource->GetSize ();
			dwBufsize = memSource->GetSize ();

			CryptEncrypt (hKey, 0, TRUE, 0, NULL, &dwBufferlen, dwBufsize);

			if (dwBufferlen > 0) {
				dwBufsize = dwBufferlen;
				memOutput.SetSize (dwBufferlen);
				memOutput.Write (memSource->GetBuffer (), 0, memSource->GetSize ());

				if (!CryptEncrypt (hKey, 0, FALSE, 0, (BYTE *) memOutput.GetBuffer (), &dwBufferlen, dwBufsize)) {
					MyHandleError ("Error during Encrypt.");
					return false;
				} else {
					memSource->Clear ();
					memSource->SetSize (memOutput.GetSize ());
					memSource->Write (memOutput.GetBuffer (), 0, memOutput.GetSize ());
					memOutput.Clear ();
				}
			} else {
				OutputText ("Unable to obtain encrypted buffer size.");
				return false;
			}		
		} else {
			dwBufferlen = memSource->GetSize ();

			memOutput.SetSize (dwBufferlen);
			memOutput.Write (memSource->GetBuffer (), 0, memSource->GetSize ());

			if (!CryptDecrypt (hKey, 0, FALSE, 0, (BYTE *) memOutput.GetBuffer (), &dwBufferlen)) {
				MyHandleError ("Error during Decrypt.");
				return false;
			} else {
				memSource->Clear ();
				memSource->SetSize (dwBufferlen);
				memSource->Write (memOutput.GetBuffer (), 0, dwBufferlen);
				memOutput.Clear ();
			}
		}

		//-------------------------------------------------------------------
		// Destroy the session key. 
		if(hKey)
		{
			if(!(CryptDestroyKey(hKey))) {
				MyHandleError("Error during CryptDestroyKey");
				return false;
			}
				
		}

		//-------------------------------------------------------------------
		// Release the provider handle. 

		if(hCryptProv)
		{
			if(!(CryptReleaseContext(hCryptProv, 0))) {
				MyHandleError("Error during CryptReleaseContext");
				return false;
			}
				
		}
			return true;
		}
}

unsigned long StandardEncryption::GetFileSize (char *FileName)
{
	// Check if the file exists		
	struct _finddata_t c_file;
	long hFile;

	// Build the list of files in the source path
	if( (hFile = _findfirst(FileName, &c_file )) == -1L ) {
		return 0;
	} else {
		return c_file.size;
	}

	return 0;
}

unsigned int StandardEncryption::IsFileEncrypted (char *szFilename)
{
	unsigned long lSigone = 0;
	unsigned long lSigtwo = 0;
	unsigned long lSigthree = 0;
	unsigned long lSigfour = 0;

	bool bSigmatch = true;

	// If the file is not large enough to hold the signature then return false;
	if (GetFileSize (szFilename) < (sizeof (unsigned long) * 4)) {
		OutputText ("File not large enough to be encrypted.");
		return CRYPTRES_NOTENCRYPTED;
	}

	FILE *hSource;

	if (hSource = fopen (szFilename, "rb")) {
		// Return nothing
	} else {
		OutputText ("Unable to open file for reading!");
		return CRYPTRES_FAILED;
	}

	fread (&lSigone, 1, sizeof (unsigned long), hSource);	
	fread (&lSigtwo, 1, sizeof (unsigned long), hSource);
	fread (&lSigthree, 1, sizeof (unsigned long), hSource);
	fread (&lSigfour, 1, sizeof (unsigned long), hSource);


	if (lSigone != m_lMagicone) {
		bSigmatch = false;
	}

	if (lSigtwo != m_lMagictwo) {
		bSigmatch = false;
	}

	if (lSigthree != m_lMagicthree) {
		bSigmatch = false;
	}

	if (lSigfour != m_lMagicfour) {
		bSigmatch = false;
	}


	if(hSource)
	{
		if(fclose(hSource)) {
			OutputText ("Unable to close file!");			
		}			
	}

	if (bSigmatch == true) {
		return CRYPTRES_ENCRYPTED;
	} else {
		return CRYPTRES_NOTENCRYPTED;
	}
}

void StandardEncryption::SetupBlankFile (char *szDestination)
{
	int fd = 0;
	// Creates a blank file including creating all of the folders to the destination
	char szDestpathonly[SIZE_STRING];
	ZeroMemory (szDestpathonly, SIZE_STRING);

	char szDestfileonly[SIZE_STRING];
	ZeroMemory (szDestfileonly, SIZE_STRING);

	// Get just the path of the destination specified
	m_Dirscanner.GetPathOnly (szDestination, szDestpathonly, szDestfileonly, "\\");

	// Now recursively create each folder of the destination path
	m_Dirscanner.CreateFolderEx (szDestpathonly);

	fd = _sopen (szDestination, _O_CREAT, _SH_DENYNO, _S_IREAD | _S_IWRITE);
	_close (fd);
}

bool StandardEncryption::EncryptFileEx3 (char *szSource, char *szDestination, char *szPassword, bool bEncrypt) {

	char szDestpathonly[SIZE_STRING];
	ZeroMemory (szDestpathonly, SIZE_STRING);

	char szDestfileonly[SIZE_STRING];
	ZeroMemory (szDestfileonly, SIZE_STRING);

	// Get just the path of the destination specified
	m_Dirscanner.GetPathOnly (szDestination, szDestpathonly, szDestfileonly, "\\");

	// Now recursively create each folder of the destination path
	m_Dirscanner.CreateFolderEx (szDestpathonly);

	// Now encrypt as normal
	return EncryptFile (szSource, szDestination, szPassword, bEncrypt);

}

bool StandardEncryption::EncryptFileEx2 (char *szSource, char *szDestination, char *szPassword, bool bEncrypt) {
	
	char szNewsource[SIZE_STRING];
	bool bEncres = false;
	DWORD dwLastError = 0;

	if (strcmp (szSource, szDestination) == 0) {
	
		ZeroMemory (szNewsource, SIZE_STRING);
		strcpy_s (szNewsource, SIZE_STRING, szSource);
		strcat_s (szNewsource, SIZE_STRING, "src");

		// Now move the source file to the new source file
		if (MoveFile (szSource, szNewsource) != 0) {

			bEncres = EncryptFile (szNewsource, szSource, szPassword, bEncrypt);

			if (bEncres == true) {
				if (DeleteFile (szNewsource) == 0) {
					dwLastError = GetLastError ();
					OutputInt ("EncryptFileEx2: DeleteFile FAILED! Err: ", dwLastError);
				}
			} else {

				if (DeleteFile (szSource) == 0) {
					dwLastError = GetLastError ();
					OutputInt ("EncryptFileEx2: DeleteFile (Recovery) FAILED! Err: ", dwLastError);
				} else {					
					if (MoveFile (szNewsource, szSource) == 0) {
						dwLastError = GetLastError ();
						OutputInt ("EncryptFileEx2: MoveFile (Recovery) FAILED! Err: ", dwLastError);
					}
				}
			}			

		} else {
			dwLastError = GetLastError ();
			OutputInt ("EncryptFileEx2: MoveFile FAILED! Err: ", dwLastError);
			return false;
		}

	} else {
		// The file paths are not the same, so just encrypt anyway.
		return EncryptFile (szSource, szDestination, szPassword, bEncrypt);
	}

	return bEncres;
}

bool StandardEncryption::EncryptFileEx (char *szSource, char *szDestination, char *szPassword, bool bEncrypt) {
	// This function will call the encryptfile function, but this function in particular will handle
	// same name source and destination files. - If the destination file is the same as the source file
	// then the destination filepath is renamed with -enc, and then after encryption the original file
	// is deleted and replaced with the encrypted file.

	char szNewdest[SIZE_STRING];
	bool bEncres = false;
	DWORD dwLastError = 0;
	//int retrcount = 0;

	if (strcmp (szSource, szDestination) == 0) {
		
		ZeroMemory (szNewdest, SIZE_STRING);
		strcpy_s (szNewdest, SIZE_STRING, szDestination);
		strcat_s (szNewdest, SIZE_STRING, "enc");

		// Now encrypt the old file to the new one
		bEncres = EncryptFile (szSource, szNewdest, szPassword, bEncrypt);

		// Now we delete the old file, first setting it's attributes to normal
		if (SetFileAttributes (szSource, FILE_ATTRIBUTE_NORMAL) == 0) {
			dwLastError = GetLastError ();
			OutputInt ("EncryptFileEx: SetFileAttributes FAILED! Err: ", dwLastError);
		}

		//retrcount = 0;
		if (DeleteFile (szSource) == 0) {
			dwLastError = GetLastError ();
			OutputInt ("EncryptFileEx: DeleteFile FAILED! Err: ", dwLastError);
			//retrcount++;

			//if (retrcount > 300) {
			//	break;
			//}
		}

		//retrcount = 0;
		// Now move the new file to the same path as the old one
		
		for (int i=0;i<50;i++) {
			if (MoveFile (szNewdest, szSource) == 0) {
				dwLastError = GetLastError ();
				OutputInt ("EncryptFileEx: MoveFile FAILED! Err: ", dwLastError);
				//retrcount++;

				//if (retrcount > 300) {
				//	break;
				//}
			} else {
				break;
			} 
		}
	
		return bEncres;
	} else {
		// The file paths are not the same, so just encrypt anyway.

		return EncryptFile (szSource, szDestination, szPassword, bEncrypt);
	}
}

void StandardEncryption::InitEncryptFile ()
{
	ZeroMemory (m_szLastaverage, SIZE_STRING);
}

bool StandardEncryption::EncryptFile(PCHAR szSource, PCHAR szDestination, PCHAR szPassword, bool bEncrypt) {
	//-------------------------------------------------------------------
	// Parameters passed are:
	//  szSource, the name of the input, a plaintext file.
	//  szDestination, the name of the output, an encrypted file to be 
	//   created.
	//  szPassword, either NULL if a password is not to be used or the 
	//   string that is the password.
	//-------------------------------------------------------------------
	// Declare and initialize local variables.

	FILE *hSource; 
	FILE *hDestination; 

	HCRYPTPROV hCryptProv; 
	HCRYPTKEY hKey;
	HCRYPTKEY hXchgKey; 
	HCRYPTHASH hHash; 

	PBYTE pbKeyBlob; 
	DWORD dwKeyBlobLen; 

	PBYTE pbBuffer; 
	DWORD dwBlockLen; 
	DWORD dwBufferLen; 
	DWORD dwCount; 
 
	unsigned long ltemp = 0;

	char szSpeed[SIZE_STRING];
	char szAverage[SIZE_STRING];
	
	//-------------------------------------------------------------------
	// Open source file. 

	if(hSource = fopen(szSource,"rb")) {
	   //printf("The source plaintext file, %s, is open. \n", szSource);
	} else {
	   MyHandleError("Error opening source plaintext file!");
	   return false;
	} 
	//-------------------------------------------------------------------
	// Open destination file. 

	if(hDestination = fopen(szDestination,"wb")) {
		 //printf("Destination file %s is open. \n", szDestination);
	} else {
		MyHandleError("Error opening destination ciphertext file!"); 
		return false;
	}
	// Get the handle to the default provider. 
	if(CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_AES , CRYPT_VERIFYCONTEXT)) {
	//if(CryptAcquireContext(&hCryptProv, NULL, MS_ENHANCED_PROV, PROV_RSA_FULL , 0)) {
	   //printf("A cryptographic provider has been acquired. \n");
	} else {
	   MyHandleError("Error during CryptAcquireContext!");
	   return false;
	}
	//-------------------------------------------------------------------
	// Create the session key.
	if(!szPassword ) { 
		return false;
	} else { 
		//-------------------------------------------------------------------
		// The file will be encrypted with a session key derived from a
		// password.
		// The session key will be recreated when the file is decrypted
		// only if the password used to create the key is available.

		//-------------------------------------------------------------------
		// Create a hash object. 
		if(CryptCreateHash(hCryptProv, CALG_MD5, 0, 0, &hHash)) {
			//printf("A hash object has been created. \n");
		} else { 
			 MyHandleError("Error during CryptCreateHash!");
			 return false;
		}

		//-------------------------------------------------------------------
		// Hash the password.
		if(CryptHashData(hHash, (BYTE *)szPassword, strlen(szPassword), 0)) {
			//printf("The password has been added to the hash. \n");
		} else {
			MyHandleError("Error during CryptHashData."); 
			return false;
		}

		//-------------------------------------------------------------------
		// Derive a session key from the hash object. 
		if(CryptDeriveKey(hCryptProv, ENCRYPT_ALGORITHM, hHash, KEYLENGTH, &hKey)) {
			//printf("An encryption key is derived from the password hash. \n"); 
		} else {
			MyHandleError("Error during CryptDeriveKey!");
			return false;
		}
		//-------------------------------------------------------------------
		// Destroy hash object. 
		if(hHash) {
			if(!(CryptDestroyHash(hHash))) {
			   MyHandleError("Error during CryptDestroyHash"); 
			   return false;
			}
			hHash = 0;
		}
	} 

	//-------------------------------------------------------------------
	// The session key is now ready. If it is not a key derived from a 
	// password, the session key encrypted with the encrypter's private 
	// key has been written to the destination file.
	 
	//-------------------------------------------------------------------
	// Determine the number of bytes to encrypt at a time. 
	// This must be a multiple of ENCRYPT_BLOCK_SIZE.
	// ENCRYPT_BLOCK_SIZE is set by a #define statement.

	dwBlockLen = 1000 - 1000 % ENCRYPT_BLOCK_SIZE; 

	//-------------------------------------------------------------------
	// Determine the block size. If a block cipher is used, 
	// it must have room for an extra block. 

	if(ENCRYPT_BLOCK_SIZE > 1) 
		dwBufferLen = dwBlockLen + ENCRYPT_BLOCK_SIZE; 
	else 
		dwBufferLen = dwBlockLen; 
    
	//-------------------------------------------------------------------
	// Allocate memory. 
	if(pbBuffer = (BYTE *)malloc(dwBufferLen)) {
		//printf("Memory has been allocated for the buffer. \n");
	} else { 
		MyHandleError("Out of memory."); 
		return false;
	}
	
	// Write / Read the header information from the file.
	// If we are encrypting then we need to write header information
	// if decrypting we need to skip past the header information providing
	// header information exists.
	
	if (bEncrypt == true) {
		fwrite (&m_lMagicone, 1, sizeof (unsigned long), hDestination);
		fwrite (&m_lMagictwo, 1, sizeof (unsigned long), hDestination);
		fwrite (&m_lMagicthree, 1, sizeof (unsigned long), hDestination);
		fwrite (&m_lMagicfour, 1, sizeof (unsigned long), hDestination);
	} else {
		ltemp = 0;
		fread (&ltemp, 1, sizeof (unsigned long), hSource);
		fread (&ltemp, 1, sizeof (unsigned long), hSource);
		fread (&ltemp, 1, sizeof (unsigned long), hSource);
		fread (&ltemp, 1, sizeof (unsigned long), hSource);
	}

	//-------------------------------------------------------------------
	// In a do loop, encrypt the source file, 
	// and write to the source file. 
	m_lastprogressvalue = 0;
	m_bmaxprogressredone = false;
	m_ispeedtrigger = 0;
	unsigned long ltimereading = 0;
	unsigned long long lbytesreading = 0;
	int ispeed = 0;
	int iaverage = 0;
	m_lastbytesreading = 0;
	m_lasttimereading = 0;
	do 
	{ 
		//-------------------------------------------------------------------
		// Read up to dwBlockLen bytes from the source file. 
		dwCount = fread(pbBuffer, 1, dwBlockLen, hSource); 
		if(ferror(hSource)) { 
			MyHandleError("Error reading plaintext!");
			return false;
		}
	 
		//-------------------------------------------------------------------
		// Encrypt / Decrypt data.
		if (bEncrypt == true) {
			if(!CryptEncrypt(hKey, 0, feof(hSource), 0, pbBuffer, &dwCount, dwBufferLen)) {
			   MyHandleError("Error during Encrypt.");
			   return false;
			}		
		} else {
			if(!CryptDecrypt(hKey, 0, feof(hSource), 0, pbBuffer, &dwCount)) {
			   MyHandleError("Error during Decrypt.");
			   return false;
			}
		}

		//-------------------------------------------------------------------
		// Write data to the destination file. 
		fwrite(pbBuffer, 1, dwCount, hDestination); 
		ltotalbytesprocessed+=dwCount;

		//OutputInt ("ltotalprocessed: ", ltotalbytesprocessed);
		int idivvalue = 1000;

		if (ltotalbytes > 0 && ltotalbytes <= 1000) {
			idivvalue = 1;
		}

		if (ltotalbytes > 1000 && ltotalbytes <= 10000) {
			idivvalue = 10;
		}

		if (ltotalbytes > 10000 && ltotalbytes <= 100000) {
			idivvalue = 100;
		}

		if (ltotalbytes > 100000 && ltotalbytes <= 1000000) {
			idivvalue = 1000;
		}

		if (ltotalbytes > 1000000 && ltotalbytes <= 10000000) {
			idivvalue = 10000;
		}

		if (ltotalbytes > 10000000 && ltotalbytes <= 100000000) {
			idivvalue = 100000;
		}

		if (ltotalbytes > 100000000 && ltotalbytes <= 1000000000) {
			idivvalue = 1000000;
		}

		if (ltotalbytes > 1000000000 && ltotalbytes <= 10000000000) {
			idivvalue = 10000000;
		}

		if (ltotalbytes > 10000000000 && ltotalbytes <= 100000000000) {
			idivvalue = 100000000;
		}

		if (ltotalbytes > 100000000000 && ltotalbytes <= 1000000000000) {
			idivvalue = 1000000000;
		}

		if (ltotalbytes > 1000000000000 && ltotalbytes <= 10000000000000) {
			idivvalue = 10000000000;
		}

		if (ltotalbytes > 10000000000000 && ltotalbytes <= 100000000000000) {
			idivvalue = 100000000000;
		}

		if (ltotalbytes > 100000000000000 && ltotalbytes <= 1000000000000000) {
			idivvalue = 1000000000000;
		}


		unsigned int progressvalue = (40000 / (ltotalbytes / idivvalue)) * ((ltotalbytesprocessed) / idivvalue);
		
		//if (m_bmaxprogressredone == false) {
		//	if ((progressvalue-m_lastprogressvalue) > 0) {			
		//		SendMessage(m_hwndprogress, PBM_SETRANGE, 0L, MAKELONG (0, 40000-((progressvalue-m_lastprogressvalue)*20)));
		//		m_bmaxprogressredone = true;
		//		//OutputInt ("max progress now at: ", 40000-((progressvalue-m_lastprogressvalue)*20));
		//	}
		//}
		if (progressvalue != m_lastprogressvalue) {
			//OutputInt ("progressvalue: ", progressvalue);
			SendMessage (m_hwndprogress, PBM_SETPOS, progressvalue, 0L);
		}

		m_lastprogressvalue = progressvalue;

		//m_ispeedtrigger++;
		if ((GetTickCount () - m_lasttickcount) > 100) {
			//m_ispeedtrigger = 0;
			m_lasttickcount = GetTickCount ();
			
			ltimereading = GetTickCount ();
			lbytesreading = ltotalbytesprocessed;


			if ((ltimereading-m_lasttimereading) < 1000) {
				ispeed = (1000000 / ((ltimereading-m_lasttimereading)*1000)) * (ltotalbytesprocessed-m_lastbytesreading);

				ZeroMemory (szSpeed, SIZE_STRING);

				if (ispeed > 0 && ispeed <= 1000) {
					sprintf_s (szSpeed, "Speed: %i Bytes/sec (%i %% complete)", ispeed, ((100*progressvalue) / 40000));
				}

				if (ispeed > 1000 && ispeed <= 1000000) {
					sprintf_s (szSpeed, "Speed: %i KB/sec (%i %% complete)", ispeed / 1000, ((100*progressvalue) / 40000));
				}

				if (ispeed > 1000000) {
					sprintf_s (szSpeed, "Speed: %i MB/sec (%i %% complete)", ispeed / 1000000, ((100*progressvalue) / 40000));
				}
				
				m_addedspeed+=ispeed;
				m_iaveragetrigger++;
				

				if (m_iaveragetrigger > 120) {
					iaverage = m_addedspeed / m_iaveragetrigger;
					
					m_addedspeed = 0;
					m_iaveragetrigger = 0;
					
					ZeroMemory (szAverage, SIZE_STRING);

					if (iaverage > 0 && iaverage <= 1000) {
						if ((((ltotalbytes-ltotalbytesprocessed) / iaverage) / 60) == 0) {
							sprintf_s (szAverage, "      Average Speed: %i Bytes/sec      Time Remaining: Less than a minute", iaverage);
						} else {
							sprintf_s (szAverage, "      Average Speed: %i Bytes/sec      Time Remaining: %i mins", iaverage, ((ltotalbytes-ltotalbytesprocessed) / iaverage) / 60);
						}
						
					}

					if (iaverage > 1000 && iaverage <= 1000000) {
						if ((((ltotalbytes-ltotalbytesprocessed) / iaverage) / 60) == 0) {
							sprintf_s (szAverage, "      Average Speed: %i KB/sec      Time Remaining: Less than a minute",  iaverage / 1000);
						} else {
							sprintf_s (szAverage, "      Average Speed: %i KB/sec      Time Remaining: %i mins", iaverage / 1000, ((ltotalbytes-ltotalbytesprocessed) / iaverage) / 60);
						}
						
					}

					if (iaverage > 1000000) {
						if ((((ltotalbytes-ltotalbytesprocessed) / iaverage) / 60) == 0) {
							sprintf_s (szAverage, "      Average Speed: %i MB/sec      Time Remaining: Less than a minute", iaverage / 1000000);
						} else {
							sprintf_s (szAverage, "      Average Speed: %i MB/sec      Time Remaining: %i mins", iaverage / 1000000, ((ltotalbytes-ltotalbytesprocessed) / iaverage) / 60);
						}
						
					}
					
					ZeroMemory (m_szLastaverage, SIZE_STRING);
					strcpy_s (m_szLastaverage, SIZE_STRING, szAverage);
				}

				strcat_s (szSpeed, SIZE_STRING, m_szLastaverage);

				SetDlgItemText (m_hwndspeed, ID_LBLSPEED, szSpeed);
			} else {
				//SetDlgItemText (m_hwndspeed, ID_LBLSPEED, "Calculating speed...");
			}


			m_lastbytesreading = ltotalbytesprocessed;
			m_lasttimereading = ltimereading;
		}

		if(ferror(hDestination)) { 
			//MyHandleError("Error writing ciphertext.");
			return false;
		}
	}
	while(!feof(hSource)); 
	//-------------------------------------------------------------------
	// End the do loop when the last block of the source file has been
	// read, encrypted, and written to the destination file.

	//-------------------------------------------------------------------
	// Close files.
	//SendMessage (m_hwndprogress, PBM_SETPOS, 40000, 0L);

	if(hSource)
	{
		if(fclose(hSource)) {
			MyHandleError("Error closing source file");
			return false;
		}
			
	}
	if(hDestination)
	{
		if(fclose(hDestination)) {
			MyHandleError("Error closing destination file");
			return false;
		}
			
	}

	//-------------------------------------------------------------------
	// Free memory. 

	if(pbBuffer) {
		free(pbBuffer); 
	}
		 
	 
	//-------------------------------------------------------------------
	// Destroy the session key. 
	if(hKey)
	{
		if(!(CryptDestroyKey(hKey))) {
			MyHandleError("Error during CryptDestroyKey");
			return false;
		}
			
	}

	//-------------------------------------------------------------------
	// Release the provider handle. 

	if(hCryptProv)
	{
		if(!(CryptReleaseContext(hCryptProv, 0))) {
			MyHandleError("Error during CryptReleaseContext");
			return false;
		}
			
	}
	return true;
} // end Encryptfile