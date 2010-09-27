// Communication encryption class designed and written by Danny Draper (c) 2007. I should probably write
// a paper article on how this encryption algorithm works. It's really simple in theory. The basic idea
// Is to create a massive hive of numbers which are generated from a passphrase. The hive must be created
// such that a single change in a single byte of the password creates a totally different key hive. So
// even the smallest change in the password has a massive effect on the numbers contained in the hive.
// However the same hive must be able to be recreated by our counterpart on the other end. 

// The security here is based on the password. The password is used to create the number hive, and the number
// hive is used to encrypt the data. So the encryption is essentially based on the users password.
// The password must always be kept secret by the server and the client, and the server and clients
// absolutely do not transmit this password to each other.

// Another thing about security. The same data encrypted more than once will always appear random. So, if I
// encrypt a whole load of aaaaaaa's once, and then do it again, the output of the two versions will be
// completely different. This is to prevent against differential analysis attacks while the data is in
// transit.

// NOTES about this Encryption class:

/*

This class includes code for doing Multi-Threaded Encryption. At the moment I've tried it with only 2
threads going simultaneously. The problem with it at the moment is that after encrypting data,
the decryption fails, and the plain text data is not recovered. This is most likely a problem with the
ForwardIntEx routine not being able to handle steps greater than 1, and the Single Cipher method 
accesses the key hive in 2 byte steps.

So for now, I've stuck with Single threaded encryption. 

*/

#include "Encryption.h"

Encryption::Encryption ()
{
	m_bUseDiagnostics = true;
	ZeroMemory (m_szPhrase, SIZE_STRING);
	m_seedChecksum = 0;
	m_bHivecreated = false;
	m_iAlgorithm = ALG_CYBERCEDE;
}

Encryption::~Encryption ()
{
}

void Encryption::SetDiagnostics (Diagnostics *pdiag)
{
	m_pdiag = pdiag;
}

void Encryption::SetAlgorithm (unsigned int iAlgID)
{
	/*
	ALG_CYBERCEDE
	ALG_AES256
	ALG_AES512
	ALG_DES
	ALG_3DES	
	*/
	m_iAlgorithm = iAlgID;
}

unsigned int Encryption::GetAlgorithm ()
{
	return m_iAlgorithm;
}

unsigned long Encryption::GetRand (int iMax)
{
	//errno_t err;
	//double max = iMax;
	//unsigned int number;

	//err = rand_s(&number);

	//return (double) number / (double) UINT_MAX * max;
	return (double) rand() / (RAND_MAX + 1) * iMax;
}

unsigned long Encryption::Raise (unsigned long lValue, unsigned long lPower)
{
	unsigned long lRes = lValue;
	for (int l=1;l<lPower;l++) {
		lRes = lRes * lValue;
	}
	return lRes;
}

unsigned int Encryption::ForwardInt (unsigned int iValue, unsigned int iAmount)
{
	// This is used by the genseed function to scale integers. It has a default
	// limit of 10, and doesn't like zero's.
	unsigned int rVal = 0;
	if (iValue > 0 && iValue < 10) {
		rVal = iValue;
		for (int r=0;r<iAmount;r++) {
			if (rVal < 9) {
				rVal++;
			} else {
				rVal = 1;
			}
		}
		return rVal;
	} else {
		if (iValue == 0) {
			return iValue++;
		} else {
			return iValue;
		}
		
	}
}

unsigned int Encryption::ForwardInt2 (unsigned int iValue, unsigned int iLimit)
{
	// Slow method of doing integer wrapping given a limit. The forward Int Ex
	// method is better.
	unsigned int i = 0;
	unsigned int counter = 0;

	for (i=0;i<iValue;i++) {
		counter++;

		if (counter > iLimit) {
			counter = 0;
		}
	}

	return counter;
}

unsigned int Encryption::ForwardIntEx (unsigned int iValue, unsigned int iLimit)
{
	// This is a superior forward int function to the above forward int 2
	// the wrap around is calculated based on the number of division of the
	// iValue by the iLimit. The remainder is then returned as the wrap around.
	// I'll have to work out a way to forward integers given a step amount, at
	// the moment it is defaulted to one.

	// After further research it appears what I've been doing here is calculating the
	// Modulo of iValue given iLimit. Essentially I'm doing iValue Mod iLimit.
	

	unsigned int iMul;

	if (iValue <= iLimit) {
		return iValue;
	}

	if (iValue > iLimit) {
		iMul = iValue - ((iValue / (iLimit+1)) * (iLimit+1));
		return iMul;
	}

	return 0;
}

void Encryption::GenSeed (char *szPhrase)
{
	// The first step to doing encryption is for a shared private key to be given to this
	// function. Both parties must be using the same passphrase, as the entire cipher is based
	// on this phrase. A very big number is then created from this pass phrase. Well kinda big
	// but not huge, maybe like 1000 characters. It does depend on the size of the phrase.
	// Next, we have to ensure that the tinyiest change in the pass phrase creates a massive
	// change in the seed. This is done by calculating a checksum on the seed, and then adding
	// the checksum to each byte value in the string. 

	int s = 0;
	int iAscii = 0;
	char szAscii[SIZE_STRING];
	char szReplace[1];

	ZeroMemory (m_szPhrase, SIZE_STRING);
	strcpy (m_szPhrase, szPhrase);

	// Set the checksum to zero
	m_seedChecksum = 1;

	// Set the size of the buffer holding the initial seed
	m_memSeed.SetSize (strlen (m_szPhrase)*SIZE_STRING);

	for (s=0;s<strlen (m_szPhrase);s++) {
		
		// Get the ascii code for the current letter in the seed phrase.
		// and multiply this by the current position in the phrase
		iAscii = m_szPhrase[s]*s;

		// Convert the given above value into a base 7 string
		ZeroMemory (szAscii, SIZE_STRING);
		itoa (iAscii, szAscii, 7);

		// Append this string literal to the seed buffer
		m_memSeed.Append ((char *) szAscii, strlen (szAscii));

		// Add this value to the seed checksum
		m_seedChecksum += iAscii;
	}

	// Now replace each occurance of 0 with a forwarded integer value
	// starting from 1 with the amount of the position in the buffer.
	
	for (s=0;s<m_memSeed.GetAppendPointer ();s++) {
		if (strncmp ((char *) m_memSeed.GetBuffer ()+s, "0", 1) == 0) {
			ZeroMemory (szReplace, 1);
			itoa (ForwardInt (1, s), szReplace, 10);
			m_memSeed.Write (szReplace, s, 1);
		}
	}
	
	//OutputSeed ();
	//OutputInt ("Checksum: ", m_seedChecksum);
}

void Encryption::ProcessZero (MemoryBuffer *pbuffer)
{
	// Replace all Zero's in the input buffer by single digit numbers
	// representing the index in the buffer.

	char szReplace[SIZE_STRING];
	int s=0;
	for (s=0;s<pbuffer->GetAppendPointer ();s++) {
		if (strncmp ((char *) pbuffer->GetBuffer ()+s, "0", 1) == 0) {
			ZeroMemory (szReplace, SIZE_STRING);
			itoa (ForwardInt (1, s), szReplace, 10);
			pbuffer->Write (szReplace, s, 1);
		}
	}
}

void Encryption::SeedChecksum ()
{
	// This function computes the checksum of the input passphrase
	// and then adds this checksum value to each byte value in the current seed string.

	MemoryBuffer memNewseed;

	char szChecksum [SIZE_STRING];
	ZeroMemory (szChecksum, SIZE_STRING);

	char szSeedpart [SIZE_STRING];
	ZeroMemory (szSeedpart, SIZE_STRING);

	char szNewseed [SIZE_STRING];
	ZeroMemory (szNewseed, SIZE_STRING);

	unsigned int iSeedpart;

	itoa (m_seedChecksum, szChecksum, 10);

	memNewseed.SetSize (strlen (szChecksum) * SIZE_STRING);

	for (int s=0;s<m_memSeed.GetAppendPointer ()-strlen(szChecksum);s++) {
		ZeroMemory (szSeedpart, SIZE_STRING);
		strncpy (szSeedpart, (char *) m_memSeed.GetBuffer ()+s, strlen (szChecksum));

		iSeedpart = atoi (szSeedpart);
		iSeedpart+=m_seedChecksum;

		ZeroMemory (szNewseed, SIZE_STRING);
		itoa (iSeedpart, szNewseed, 10);

		memNewseed.Append ((char *) szNewseed, strlen (szNewseed));
	}

	m_memSeed.Clear ();
	m_memSeed.SetSize (memNewseed.GetAppendPointer ());
	m_memSeed.Append (memNewseed.GetBuffer (), memNewseed.GetAppendPointer ());
	memNewseed.Clear ();

	//OutputSeed ();
}

void Encryption::ShuffleSeed ()
{
	int s = 0;

	for (s=0;s<m_memSeed.GetAppendPointer () / 2;s++) {
		m_memSeed.SwapBytes (s, m_memSeed.GetAppendPointer ()-1-s);
	}

	for (s=0;s<m_memSeed.GetAppendPointer () / 4;s++) {
		m_memSeed.SwapBytes (s, m_memSeed.GetAppendPointer ()-1-s);
	}

	//OutputSeed ();
}

void Encryption::ShuffleHive ()
{
	int s = 0;

	for (s=0;s<m_memHive.GetAppendPointer () / 2;s++) {
		m_memHive.SwapBytes (s, m_memHive.GetAppendPointer ()-1-s);
	}
}

void Encryption::ScaleSeed ()
{
	int s = 0;
	unsigned int iCurrent = 0;
	char szChar[SIZE_STRING];
	
	for (s=0;s<m_memSeed.GetAppendPointer ();s++) {
		ZeroMemory (szChar, 1);
		strncpy (szChar, (char *) m_memSeed.GetBuffer ()+s, 1);
		iCurrent = atoi (szChar);
		iCurrent = ForwardInt (iCurrent, s);
		ZeroMemory (szChar, SIZE_STRING);
		itoa (iCurrent, szChar, 10);
		m_memSeed.Write (szChar, s, 1);
	}

	//OutputSeed ();
}

void Encryption::OutputSeed ()
{
	char szOutput[SIZE_LARGESTRING];
	ZeroMemory (szOutput, SIZE_LARGESTRING);

	strncpy (szOutput, (char *) m_memSeed.GetBuffer (), m_memSeed.GetAppendPointer ());

	OutputText (szOutput);
	//OutputInt ("m_memSeed Size: ", m_memSeed.GetSize ());
	//OutputInt ("m_memSeed Append: ", m_memSeed.GetAppendPointer ());
}

void Encryption::GenHive ()
{
	// This generates the first pass of the main key hive.
	// This is only done once, because the initial hive comes from the seed
	// after this, we then use expand hive to create the final key hive.
	int s = 0;

	char szMult[2];
	char szPower[1];
	char szProduct[SIZE_STRING];

	unsigned long lMult = 0;
	unsigned long lPower = 0;
	unsigned long lProduct = 0;

	MemoryBuffer memHive;
	memHive.SetSize (m_memSeed.GetAppendPointer ()*SIZE_STRING);

	for (s=0;s<m_memSeed.GetAppendPointer ()-3;s++) {

		// Copy 3 bytes at the current position into the Multiplier buffer
		ZeroMemory (szMult, 2);
		ZeroMemory (szPower, 1);
		strncpy (szMult, (char *) m_memSeed.GetBuffer ()+s, 2);
		strncpy (szPower, (char *) m_memSeed.GetBuffer ()+s+2, 1);

		lMult = atoi (szMult);
		lPower = atoi (szPower);

		ZeroMemory (szProduct, SIZE_STRING);
		lProduct = Raise (lMult, lPower);
		ultoa (lProduct, szProduct, 10);
		
		//OutputInt ("lProduct: ", lProduct);
		//OutputText ("szProduct: ", szProduct);

		memHive.Append ((char *) szProduct, strlen (szProduct));
	}

	m_memHive.SetSize (memHive.GetAppendPointer ());
	m_memHive.Append (memHive.GetBuffer (), memHive.GetAppendPointer ());
	memHive.Clear ();
}

unsigned long Encryption::ExpandHive (MemoryBuffer *phive)
{
	MemoryBuffer tmpHive;

	int s = 0;
	char szMult[2];
	char szPower[1];
	char szProduct[SIZE_STRING];

	unsigned long lMult = 0;
	unsigned long lPower = 0;
	unsigned long lProduct = 0;

	tmpHive.SetSize (phive->GetAppendPointer ()*SIZE_STRING);

	for (s=0;s<phive->GetAppendPointer ()-3;s++) {
		// Copy 3 bytes at the current position into the Multiplier buffer
		ZeroMemory (szMult, 2);
		ZeroMemory (szPower, 1);
		strncpy (szMult, (char *) phive->GetBuffer ()+s, 2);
		strncpy (szPower, (char *) phive->GetBuffer ()+s+2, 1);

		lMult = atoi (szMult);
		lPower = atoi (szPower);

		ZeroMemory (szProduct, SIZE_STRING);
		lProduct = Raise (lMult, lPower);
		ultoa (lProduct, szProduct, 10);

		tmpHive.Append ((char *) szProduct, strlen (szProduct));
	}

	phive->Clear ();
	phive->SetSize (tmpHive.GetAppendPointer ());
	phive->Append (tmpHive.GetBuffer (), tmpHive.GetAppendPointer ());
	tmpHive.Clear ();
	return phive->GetAppendPointer ();
}

bool Encryption::CreateKeyHive (LPCSTR lpszPassword)
{
	if (strlen (lpszPassword) > 0) {
		ZeroMemory (m_szPassword, SIZE_STRING);
		strcpy_s (m_szPassword, SIZE_STRING, lpszPassword);
	}

	//if (m_iAlgorithm == ALG_CYBERCEDE) {
		if (strlen (lpszPassword) > 4 && strlen (lpszPassword) < 41) {
			// begin generation of the secret hive
			GenSeed ((char *) lpszPassword);
			SeedChecksum ();
			ScaleSeed ();
			ShuffleSeed ();
			GenHive ();
			ProcessZero (&m_memHive);

			// Now expand the hive by raising each 2 digits by the power of the 3rd.
			// keep doing this until the hive is greater than 10K
			while (m_memHive.GetAppendPointer () < 10000) {
				ExpandHive (&m_memHive);
				ProcessZero (&m_memHive); // Replace all zero's with index locations.
				ShuffleHive (); // Shuffle the hive
			}	

			m_bHivecreated = true;
			return true;
		} else {
			return false;
		}
	//} else {
		//return true;
	//}
	
}

bool Encryption::CreateKeySeed (LPCSTR lpszPassword, MemoryBuffer *memDest)
{
	if (strlen (lpszPassword) > 4 && strlen (lpszPassword) < 41) {
		// begin generation of the secret hive
		m_memHive.Clear ();

		GenSeed ((char *) lpszPassword);
		SeedChecksum ();
		ScaleSeed ();
		ShuffleSeed ();
		GenHive ();
		ProcessZero (&m_memHive);

		memDest->SetSize (m_memHive.GetSize ());
		memDest->Write (m_memHive.GetBuffer (), 0, m_memHive.GetSize ());

		return true;
	} else {
		return false;
	}
}

unsigned long Encryption::GetHiveValue (unsigned long iOffset, int iNumdigits)
{
	char szSingle[SIZE_STRING];

	if (iNumdigits > 0 && iNumdigits < 8) {
		if (iOffset >= 0 && iOffset < m_memHive.GetAppendPointer ()-iNumdigits) {
			ZeroMemory (szSingle, SIZE_STRING);
			strncpy (szSingle, (char *) m_memHive.GetBuffer ()+iOffset, iNumdigits);
			return atol (szSingle);
		} else {
			return 0;
		}	
	} else {
		return 0;
	}
}

bool Encryption::DoPartCipher (HWND hWnd, MemoryBuffer *pinbuffer, unsigned long istartoffset, unsigned long ilength, unsigned long rndHive, unsigned int msgprogress, bool Encrypt)
{
	// MULTI-THREADED FUNCTION
	// This function is designed to execute in a seperate thread. Part of the multi-threaded
	// encryption. It only encrypts a part of the input buffer given by the offset and length.
	// the output is placed in a member output buffer which all threads have access to.
	// On a Core Duo or HT processor, this will maximise the usage of both CPU cores, or windows.

	int algsize = 100;
	unsigned long hivepointer = 0;
	unsigned long inputoffset = 0; // not to be confused with start offset
	unsigned long outputoffset = sizeof (unsigned long);
	int algpointer = 0;
	unsigned long i = 0;
	unsigned long iprogress = 0;
	unsigned long lpercentage = 0;

	int cTrans = 0; // Transposition value
	int cAlg = 0; // Transposition multiplier
	unsigned long cProduct = 0; // Product of Trans * cAlg
	bool cShift = 0; // The transposition shift - up or down
	BYTE cByte = 0; // The current byte we're encrypting.

	if (m_bHivecreated == false) {
		OutputText ("DoPartCipher: ", "Key hive not created.");
		return false;
	} else {
		if (m_tmpBuffer.GetSize () == 0) {
			OutputText ("DoPartCipher: ", "Result buffer size not set");
			return false;
		} else {
			
			// Remember here we're not generated a random value to get the hive value
			// this time it is given to us by the part manager.
			// For decryption we must be told to be offset by the length of any header values

			// We need to now set the initial hive pointer and alg pointer
			// values given the start offset we're about to process.
			hivepointer = ForwardIntEx (istartoffset, m_memHive.GetAppendPointer ()-2);
			algpointer = ForwardIntEx (istartoffset, algsize);

			// Start the encryption process for this part			
			for (i=istartoffset;i<istartoffset+ilength;i++) {
				cTrans = GetHiveValue (hivepointer, 2);
				cAlg = GetHiveValue (rndHive+algpointer, 1);

				if (cAlg <= 5) {
					cShift = true; // true means we transpose up
				} else {
					cShift = false; // false means we transpose down
				}

				// Now transpose the byte given the values we have.
				//if (Encrypt == true) {
					cByte = pinbuffer->GetByte (i);
				//} else {
				//	cByte = pinbuffer->GetByte (i+outputoffset);
				//}
				
				cProduct = cTrans * cAlg;

				if (Encrypt == true) {
					if (cShift == true) {
						cByte+=cProduct;
					} else {
						cByte-=cProduct;
					}
				} else {
					if (cShift == true) {
						cByte-=cProduct;
					} else {
						cByte+=cProduct;
					}
				}
				
				if (Encrypt == true) {
					m_tmpBuffer.Write (&cByte, i+outputoffset, sizeof (BYTE));
				} else {
					m_tmpBuffer.Write (&cByte, i, sizeof (BYTE));
				}
				

				hivepointer+=2;
				algpointer++;
				iprogress++;
				if (iprogress > 50000) {
					iprogress = 0;
					lpercentage = (i * 50) / (istartoffset+ilength);
					//OutputInt ("cipher: ", i);
					PostMessage (hWnd, CRYPT_MSG, msgprogress, (LPARAM) lpercentage);
				}

				if (hivepointer >= m_memHive.GetAppendPointer ()-2) {
					hivepointer = 0;
				}

				if (algpointer >= algsize) {
					algpointer = 0;
				}

			}
			PostMessage (hWnd, CRYPT_MSG, msgprogress, (LPARAM) -1);
			return true;
		}
	}
}

unsigned int Encryption::GetNumDigits (unsigned long value)
{
	char szValue[SIZE_STRING];
	ZeroMemory (szValue, SIZE_STRING);

	ltoa (value, szValue, 10);

	return strlen (szValue);
}

bool Encryption::DoResequence (HWND hWnd, MemoryBuffer *pinbuffer, bool bEncrypt)
{
	// Single threaded resequencing function. The job of this function is to process the input buffer
	// and treat the encryption hive values as swap values. The input buffer bytes are then swapped with
	// bytes at the index given by the hive at the current index of the input file. This will form the first
	// part of the encryption phase. The next part is to apply transposition to the encrypted file.
	unsigned long i = 0;
	unsigned long h = 0;
	unsigned int numdigits = 0;
	unsigned long iprogress = 0;
	unsigned long ipercentage = 0;

	BYTE bTemp;
	BYTE bOne;
	BYTE bTwo;
	
	unsigned int inputlimit = 0;
	long hivepointer = 0;
	unsigned long swapindex = 0;

	if (m_bHivecreated == false) {
		OutputText ("Encryption: ", "Key hive not created.");
		return false;
	} else {
	
		// We need to know what values to set the length of the exchange indexes. ie - how far of an offset
		// in the input file can we use to swap byte values. Larger files can use larger indexes, smaller files
		// smaller indexes, so we need to determine this now.
		numdigits = GetNumDigits (pinbuffer->GetSize ());

		if (numdigits >= 1) {
			
			if (bEncrypt == true) {
				
				for (i=0;i<pinbuffer->GetSize ();i++) {
					// Get the index of the byte we want to swap with
					swapindex = GetHiveValue (hivepointer, GetNumDigits (pinbuffer->GetSize ())-1);
					
					BYTE bOne = pinbuffer->GetByte (i);
					BYTE bTwo = pinbuffer->GetByte (swapindex);

					pinbuffer->SetByte (swapindex, bOne);
					pinbuffer->SetByte (i, bTwo);

					hivepointer++;
					if (hivepointer > m_memHive.GetSize ()-GetNumDigits (pinbuffer->GetSize ())) {
						hivepointer = 0;
					}

					iprogress++;
					if (iprogress > 10000) {
						iprogress = 0;
						ipercentage = (i * 10) / (pinbuffer->GetSize ());
						PostMessage (hWnd, CRYPT_MSG, CRYPT_RPROGRESSSINGLE, (LPARAM) ipercentage*10);
					}

				}
			} else {
				
				// First we need to work out where the hive pointer would
				// be as if we had processed the file normally
				for (i=0;i<pinbuffer->GetSize ();i++) {
					hivepointer++;
					if (hivepointer > m_memHive.GetSize ()-GetNumDigits (pinbuffer->GetSize ())) {
						hivepointer = 0;
					}
				}
				
				hivepointer--;

				// Now we need to work backwards through the file
				// reordering the bytes back as they were using our encryption hive
				for (i=pinbuffer->GetSize ();i>0;i--) {
										
					// Get the index of the byte we want to swap with
					swapindex = GetHiveValue (hivepointer, GetNumDigits (pinbuffer->GetSize ())-1);

					BYTE bOne = pinbuffer->GetByte (i-1);
					BYTE bTwo = pinbuffer->GetByte (swapindex);

					pinbuffer->SetByte (swapindex, bOne);
					pinbuffer->SetByte (i-1, bTwo);					

					hivepointer--;
					if (hivepointer < 0) {						
						hivepointer = m_memHive.GetSize ()-GetNumDigits (pinbuffer->GetSize ());
					}
					
					iprogress++;
					if (iprogress > 10000) {
						iprogress = 0;
						ipercentage = ((pinbuffer->GetSize ()-i) * 10) / (pinbuffer->GetSize ());
						PostMessage (hWnd, CRYPT_MSG, CRYPT_RPROGRESSSINGLE, (LPARAM) ipercentage*10);
					}

				}
			}	

			PostMessage (hWnd, CRYPT_MSG, CRYPT_RPROGRESSSINGLE, (LPARAM) -1);
			OutputText ("Encryption: ", "Resequence complete.");						
			return true;
		} else {
			return false;
		}
	}
}

bool Encryption::DoCipher (HWND hWnd, MemoryBuffer *pinbuffer, MemoryBuffer *poutbuffer, bool bEncrypt)
{
	// Single threaded encryption function. Operates in a seperate thread, but a single thread
	// only. Unfortunately on a Core Duo, or HT processor, it will only max out one of the CPU
	// windows.

	MemoryBuffer tmpBuffer;
	unsigned long rndHive = 0;
	int algsize = 100;
	unsigned long hivepointer = 0;
	unsigned long inputoffset = 0;
	unsigned long iprogress = 0;
	unsigned long ipercentage = 0;
	int algpointer = 0;
	unsigned long i = 0;

	if (m_iAlgorithm == ALG_CYBERCEDE) {
			int cTrans = 0; // Our transposition value
			int cAlg = 0; // The algorithm we use alongside the transposition value
			unsigned long cProduct = 0; // This value is the current result of the transposition multiplication
			bool cShift = 0;  // The transposition shift - up or down
			BYTE cByte = 0; // The current byte we're working on.

			if (m_bHivecreated == false) {
				OutputText ("Encryption: ", "Key hive not created.");
				return false;
			} else {
			
				tmpBuffer.SetSize ((pinbuffer->GetSize ()+sizeof (unsigned long))*2);
				OutputInt ("DoCipher: ", pinbuffer->GetSize ()*2);

				// Generate a random number between 0 and the size of the hive
				// minus 100 (we use 100 single digits for determining the transposition
				// shift mechanism and transposition multiplier

				if (bEncrypt == true) {
					rndHive = GetRand (m_memHive.GetAppendPointer ()-algsize);
					tmpBuffer.Append (&rndHive, sizeof (unsigned long));
				} else {
					memcpy (&rndHive, pinbuffer->GetBuffer (), sizeof (unsigned long));
					inputoffset+=sizeof (unsigned long);
				}

				for (i=0;i<pinbuffer->GetSize ()-inputoffset;i++) {
								
					cTrans = GetHiveValue (hivepointer, 2);
					cAlg = GetHiveValue (rndHive+algpointer, 1);

					if (cAlg <= 5) {
						cShift = true; // true means we transpose up
					} else {
						cShift = false; // false means we transpose down
					}

					// Now transpose the current byte given the current hive value
					// algorithm value and shift direction.
					cByte = pinbuffer->GetByte (i+inputoffset);
					cProduct = cTrans * cAlg;

					if (bEncrypt == true) {
						if (cShift == true) {
							cByte+=cProduct;
						} else {
							cByte-=cProduct;
						}
					} else {
						if (cShift == true) {
							cByte-=cProduct;
						} else {
							cByte+=cProduct;
						}
					}
					

					tmpBuffer.Append (&cByte, sizeof (BYTE));

					hivepointer+=2;
					algpointer++;

					if (hivepointer >= m_memHive.GetAppendPointer ()-2) {
						hivepointer = 0;
					}

					if (algpointer >= algsize) {
						algpointer = 0;
					}

					iprogress++;
					if (iprogress > 10000) {
						iprogress = 0;
						ipercentage = (i * 10) / (pinbuffer->GetSize ()-inputoffset);
						PostMessage (hWnd, CRYPT_MSG, CRYPT_PROGRESSSINGLE, (LPARAM) ipercentage*10);
					}
				
				}

				poutbuffer->SetSize (tmpBuffer.GetAppendPointer ());
				poutbuffer->Append (tmpBuffer.GetBuffer (), tmpBuffer.GetAppendPointer ());
				tmpBuffer.Clear ();
				PostMessage (hWnd, CRYPT_MSG, CRYPT_PROGRESSSINGLE, (LPARAM) -1);
				return true;
			}
	}

	if (m_iAlgorithm == ALG_AES256 || m_iAlgorithm == ALG_3DES || m_iAlgorithm == ALG_DES) {
		poutbuffer->SetSize (pinbuffer->GetSize ());
		poutbuffer->Write (pinbuffer->GetBuffer (), 0, pinbuffer->GetSize ());

		m_enc.SetAlgorithm (m_iAlgorithm);

		if (m_enc.EncryptBuffer (poutbuffer, m_szPassword, bEncrypt) == true) {
			PostMessage (hWnd, CRYPT_MSG, CRYPT_PROGRESSSINGLE, (LPARAM) 100);
			PostMessage (hWnd, CRYPT_MSG, CRYPT_PROGRESSSINGLE, (LPARAM) -1);
			return true;
		} else {
			PostMessage (hWnd, CRYPT_MSG, CRYPT_PROGRESSSINGLE, (LPARAM) 100);
			PostMessage (hWnd, CRYPT_MSG, CRYPT_PROGRESSSINGLE, (LPARAM) -1);
			return false;
		}
	}

	return true;
}

void Encryption::Test2 ()
{
	if (m_bHivecreated == true) {
	}
	for (;;) {
		Beep (1000, 100);
		Sleep (200);
	}
}

DWORD WINAPI Encryption::ThreadProc (PVOID pParam)
{
	Encryption *pcrypt;

	pcrypt = (Encryption *) pParam;

	pcrypt->Test2 ();

	return 0;
}

DWORD WINAPI Encryption::CipherProc1 (PVOID pParam)
{
	// MULTI-THREADED FUNCTION
	// First thread encryption Procedure. This executes in a separate thread.
	Encryption *pcrypt;
	pcrypt = (Encryption *) pParam;

	unsigned int iBefore = GetTickCount ();

	if (pcrypt->DoPartCipher (pcrypt->m_hwnd, pcrypt->m_pinbuffer_thall, pcrypt->m_istartoffset_th1, pcrypt->m_ilength_th1, pcrypt->m_rndHive_thall, CRYPT_PROGRESS1, pcrypt->m_bEncrypt_thall) == true) {
		pcrypt->OutputText ("CipherProc 1 completed.");
	} else {
		pcrypt->OutputText ("CipherProc 1 failed.");
	}

	pcrypt->OutputInt ("Proc1: Time Taken: ", GetTickCount ()-iBefore);

	return 0;
}

void Encryption::DoCipher1 ()
{
	// MULTI-THREADED FUNCTION
	// Spawns the first encryption thread.
	HANDLE hThread;
	DWORD dwThreadID;

	hThread = CreateThread (NULL, 0, CipherProc1, (void *) this, 0, &dwThreadID);
}

DWORD WINAPI Encryption::CipherProc2 (PVOID pParam)
{
	// MULTI-THREADED FUNCTION
	Encryption *pcrypt;
	pcrypt = (Encryption *) pParam;

	unsigned int iBefore = GetTickCount ();

	if (pcrypt->DoPartCipher (pcrypt->m_hwnd, pcrypt->m_pinbuffer_thall, pcrypt->m_istartoffset_th2, pcrypt->m_ilength_th2, pcrypt->m_rndHive_thall, CRYPT_PROGRESS2, pcrypt->m_bEncrypt_thall) == true) {
		pcrypt->OutputText ("CipherProc 2 completed.");
	} else {
		pcrypt->OutputText ("CipherProc 2 failed.");
	}

	pcrypt->OutputInt ("Proc2: Time Taken: ", GetTickCount ()-iBefore);

	return 0;
}

void Encryption::DoCipher2 ()
{
	// MULTI-THREADED FUNCTION
	HANDLE hThread;
	DWORD dwThreadID;

	hThread = CreateThread (NULL, 0, CipherProc2, (void *) this, 0, &dwThreadID);
}

DWORD WINAPI Encryption::CipherProcSingle (PVOID pParam)
{
	// The single threaded encryption process executes
	// here, within a separate thread.
	// Note that this is a static member function, and static member functions do not have access
	// to the non static methods or attributes of the class. So, upon creating this thread
	// we have passed the encryptions *this pointer as a parameter, so that when the thread
	// is executed, we can still get to the member methods and variables through the provided
	// pointer.
	Encryption *pcrypt;
	pcrypt = (Encryption *) pParam;

	if (pcrypt->m_iAlgorithm == ALG_CYBERCEDE) {
		
		if (pcrypt->m_bEncrypt_thall == true) {
			if (pcrypt->DoResequence (pcrypt->m_hwnd, pcrypt->m_pinbuffer_thall, pcrypt->m_bEncrypt_thall) == true) {
				if (pcrypt->DoCipher (pcrypt->m_hwnd, pcrypt->m_pinbuffer_thall, pcrypt->m_outbuffer, pcrypt->m_bEncrypt_thall) == true) {
					pcrypt->OutputText ("CipherProcSingle completed. ");
				} else {
					pcrypt->OutputText ("CipherProcSingle transposition failed.");
				}		
			} else {
				pcrypt->OutputText ("CipherProcSingle resequencing failed.");
			}
		} else {
			if (pcrypt->DoCipher (pcrypt->m_hwnd, pcrypt->m_pinbuffer_thall, pcrypt->m_outbuffer, pcrypt->m_bEncrypt_thall) == true) {
				if (pcrypt->DoResequence (pcrypt->m_hwnd, pcrypt->m_outbuffer, pcrypt->m_bEncrypt_thall) == true) {
					pcrypt->OutputText ("CipherProcSingle completed.");
				} else {
					pcrypt->OutputText ("CipherProcSingle resequencing failed.");
				}
			} else {
				pcrypt->OutputText ("CipherProcSingle transposition failed.");
			}
		}	

	} else {

		if (pcrypt->m_bEncrypt_thall == true) {			
			if (pcrypt->DoCipher (pcrypt->m_hwnd, pcrypt->m_pinbuffer_thall, pcrypt->m_outbuffer, pcrypt->m_bEncrypt_thall) == true) {
				pcrypt->OutputText ("CipherProcSingle completed. - - CIPHER SUCCESS");
			} else {
				pcrypt->OutputText ("CipherProcSingle transposition failed. - CIPHER FAILURE");
			}					
		} else {
			if (pcrypt->DoCipher (pcrypt->m_hwnd, pcrypt->m_pinbuffer_thall, pcrypt->m_outbuffer, pcrypt->m_bEncrypt_thall) == true) {				
				pcrypt->OutputText ("CipherProcSingle completed.");
			} else {
				pcrypt->OutputText ("CipherProcSingle transposition failed.");
			}
		}		
	}

	/*
	if (pcrypt->DoCipher (pcrypt->m_hwnd, pcrypt->m_pinbuffer_thall, pcrypt->m_outbuffer, pcrypt->m_bEncrypt_thall) == true) {
		pcrypt->OutputText ("CipherProcSingle completed.");
	} else {
		pcrypt->OutputText ("CipherProcSingle failed.");
	}
	*/
	return 0;
}

void Encryption::DoCipherThread ()
{
	// This actually spawns the single thread for single threaded encryption.
	HANDLE hThread;
	DWORD dwThreadID;

	hThread = CreateThread (NULL, 0, CipherProcSingle, (void *) this, 0, &dwThreadID);
}

void Encryption::DoSingleCipher (HWND hWnd, MemoryBuffer *pinbuffer, MemoryBuffer *poutbuffer, bool bEncrypt)
{
	// SINGLE THREADED FUNCTION
	// This will spawn the single thread for single threaded encryption.
	// This is the parent function for single threaded encryption.

	if (m_iAlgorithm == ALG_CYBERCEDE) {
		if (m_bHivecreated == false) {
			OutputText ("SingleCipher: Hive not created!");
			return;
		}
	}

	m_hwnd = hWnd;
	m_outbuffer = poutbuffer;
	m_pinbuffer_thall = pinbuffer;
	m_bEncrypt_thall = bEncrypt;
	m_bsinglethreaddone = false;
	m_progresssingle = 0;
	m_lastpercentage = 0;

	// Start the single cipher thread
	DoCipherThread ();
}

void Encryption::DoMultiCipher (HWND hWnd, MemoryBuffer *pinbuffer, MemoryBuffer *poutbuffer, bool bEncrypt)
{
	// MULTI-THREADED FUNCTION
	// This is the function that spawns the smaller threads for multi-threaded encryption.
	// This can be thought of as the Multi Cipher parent function.
	// The idea is that this function delegates the whole encryption task to the two threads
	// that actually perform the encryption.

	if (m_bHivecreated == false) {
		OutputText ("MultiCipher: Hive not created!");
		return;
	}

	m_hwnd = hWnd;
	m_outbuffer = poutbuffer;

	// First we make the input buffer accessible by the separate threads
	unsigned int algsize = 100;
	//unsigned long part1offset = 0;
	//unsigned long part1length = 0;
	//unsigned long part2offset = 0;
	//unsigned long part2length = 0;
	unsigned long partlength = 0;

	m_pinbuffer_thall = pinbuffer;
	m_bEncrypt_thall = bEncrypt;

	// Now we generate a random hive value that all the ciper parts will need
	if (bEncrypt == true) {
		m_rndHive_thall = GetRand (m_memHive.GetAppendPointer ()-algsize);
	} else {
		memcpy (&m_rndHive_thall, pinbuffer->GetBuffer (), sizeof (unsigned long));
	}
	

	// Now we need to set the size of the working buffer
	if (bEncrypt == true) {
		m_tmpBuffer.SetSize ((pinbuffer->GetSize ()+sizeof (unsigned long)));
	} else {
		m_tmpBuffer.SetSize ((pinbuffer->GetSize ()-sizeof (unsigned long)));
	}
	

	// Now we need to write the random hive value to the working buffer
	if (bEncrypt == true) {
		m_tmpBuffer.Write (&m_rndHive_thall, 0, sizeof (unsigned long));
	}

	// Now start delegating
	partlength = pinbuffer->GetSize () / 2;

	if (bEncrypt == true) {
		m_ilength_th1 = partlength;
		m_istartoffset_th1 = 0;

		m_istartoffset_th2 = partlength;
		m_ilength_th2 = pinbuffer->GetSize () - m_istartoffset_th2;
	} else {
		m_ilength_th1 = partlength;
		m_istartoffset_th1 = sizeof (unsigned long);

		m_istartoffset_th2 = partlength+sizeof (unsigned long);
		m_ilength_th2 = pinbuffer->GetSize () - m_istartoffset_th2;
	}
	

	OutputInt ("partlength: ", partlength);
	OutputInt ("m_istartoffset_th1: ", m_istartoffset_th1);
	OutputInt ("m_ilength_th1: ", m_ilength_th1);
	OutputInt ("m_istartoffset_th2: ", m_istartoffset_th2);
	OutputInt ("m_ilength_th2: ", m_ilength_th2);

	m_lastpercentage = 0;
	m_bthread1done = false;
	m_bthread2done = false;

	// Now Start Multi-Threaded Encryption!!
	DoCipher1 ();
	DoCipher2 ();
}

void Encryption::NotifyEvent (HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	// As we're executing in a seperate thread, the only way for the main window
	// to know if we're done is either by Callback functions through pointers
	// or to Post Window messages back to our main window which is then
	// passed to this class. 

	if (wParam == CRYPT_PROGRESSSINGLE) {
		if (lParam != -1) {
			m_progresssingle = (unsigned long ) lParam;
		} else {
			if (m_bEncrypt_thall == true) {
				m_bsinglethreaddone = true;
				PostMessage (hWnd, CRYPT_MSG, CRYPT_COMPLETE, 0);
			} else {
				if (m_iAlgorithm != ALG_CYBERCEDE) {
					m_bsinglethreaddone = true;
					PostMessage (hWnd, CRYPT_MSG, CRYPT_COMPLETE, 0);
				}
			}
		}

		if (m_progresssingle != m_lastpercentage) {
			m_lastpercentage = m_progresssingle;
			OutputInt ("SingleCipher Progress: ", m_progresssingle);
		
			PostMessage (hWnd, CRYPT_MSG, CRYPT_USERPROGRESS, (LPARAM) m_progresssingle);
		}
	}

	
	if (wParam == CRYPT_RPROGRESSSINGLE) {
		if (lParam != -1) {
			m_progresssingle = (unsigned long ) lParam;
		} else {
			if (m_bEncrypt_thall == false) {
				m_bsinglethreaddone = true;
				PostMessage (hWnd, CRYPT_MSG, CRYPT_COMPLETE, 0);
			} else {
				if (m_iAlgorithm != ALG_CYBERCEDE) {
					m_bsinglethreaddone = true;
					PostMessage (hWnd, CRYPT_MSG, CRYPT_COMPLETE, 0);
				}
			}
		}

		if (m_progresssingle != m_lastpercentage) {
			m_lastpercentage = m_progresssingle;
			OutputInt ("SingleSequence Progress: ", m_progresssingle);
		
			PostMessage (hWnd, CRYPT_MSG, CRYPT_RUSERPROGRESS, (LPARAM) m_progresssingle);
		}
	}
	/*
	if (wParam == CRYPT_PROGRESS1) {
		if (lParam != -1) {
			m_progress1 = lParam;
		} else {
			m_bthread1done = true;
		}
	}

	if (wParam == CRYPT_PROGRESS2) {
		if (lParam != -1) {
			m_progress2 = lParam;
		} else {
			m_bthread2done = true;
		}
		
	}

	unsigned long fPercentage = (m_progress1 + m_progress2);
	
	if (fPercentage != m_lastpercentage) {
		m_lastpercentage = fPercentage;
		OutputInt ("MultiCipher Progress: ", fPercentage);
	}

	if (m_bthread1done == true && m_bthread2done == true) {
		m_outbuffer->SetSize (m_tmpBuffer.GetSize ());
		m_outbuffer->Append (m_tmpBuffer.GetBuffer (), m_tmpBuffer.GetSize ());
		m_tmpBuffer.Clear ();
		OutputText ("MultiCipher: OutputBuffer written.");
	}
	*/
}

void Encryption::Test ()
{

	//Beep (1000, 200);
	//HANDLE hThread;
	//DWORD dwThreadID;

	//hThread = CreateThread (NULL, 0, ThreadProc, (void *) this, 0, &dwThreadID);
	//MessageBox (NULL, "Thread", "Info", MB_OK);

	unsigned int i = 0;

	unsigned int iBefore = GetTickCount ();

	//OutputInt ("ForwardInt2: ", ForwardInt2 (80000000, 10000));
	OutputInt ("ForwardEx: ", ForwardIntEx (80000000, 10000));

	OutputInt ("Time Taken: ", GetTickCount ()-iBefore);
	

}

void Encryption::OutputInt (LPCSTR lpszText, int iValue)
{
	if (m_bUseDiagnostics == true) {
		m_pdiag->OutputInt (lpszText, iValue);
	}
}

void Encryption::OutputText (LPCSTR lpszText)
{
	if (m_bUseDiagnostics == true) {
		m_pdiag->OutputText (lpszText);
	}
}

void Encryption::OutputText (LPCSTR lpszName, LPCSTR lpszValue)
{
	if (m_bUseDiagnostics == true) {
		m_pdiag->OutputText (lpszName, lpszValue);
	}
}