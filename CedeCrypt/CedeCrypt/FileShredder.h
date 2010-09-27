#pragma once
#define _CRT_RAND_S
#include <windows.h>
#include <malloc.h>
#include <stdio.h>
#include "Diagnostics.h"

#define RAND_SIZE	10000

class FileShredder {
	
	public:
		FileShredder ();
		~FileShredder ();		
		void OutputInt (LPCSTR lpszText, int iValue);
		void OutputText (LPCSTR lpszText);
		void OutputText (LPCSTR lpszName, LPCSTR lpszValue);

		void SetDiagnostics (Diagnostics *pdiag);
		

		//**************************************
		void SecureDelete (char *pszFilename);
		void OverwriteFile (char *pszFilename, BYTE bByte, bool bComplement, bool bRandom, unsigned long lSize);
		void FillBuffer (BYTE bByte, bool bComplement, bool bRandom, unsigned long lBufSize);
		int GetRand (int iMax);
		unsigned long GetFileSize (char *pszFilename);
		void InitRandom ();
		void GetPathOnly (char *szInpath, char *szOutpath);
		void GenFileRandomName (char *szOutname);
		BYTE GetRandomAlphaNum ();
		void NameScramble (char *pszFilename);
	private:
		void ShowInt (int iInttoShow);

		// Flag indicating if we're using diagnostics
		bool m_bUseDiagnostics;
		Diagnostics *m_pdiag;

		MemoryBuffer m_writebuffer;
		MemoryBuffer m_randombuffer;


		//************************************


};