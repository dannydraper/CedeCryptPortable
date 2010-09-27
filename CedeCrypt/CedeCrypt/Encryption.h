#pragma once
#define _CRT_RAND_S
#include <windows.h>
#include <io.h>
#include "Diagnostics.h"
#include "MemoryBuffer.h"
#include "resource.h"
#include "StandardEncryption.h"

class Encryption {
	
	public:
		Encryption ();
		~Encryption ();
		static DWORD WINAPI ThreadProc (PVOID pParam);
		static DWORD WINAPI CipherProc1 (PVOID pParam);
		void DoCipher1 ();
		static DWORD WINAPI CipherProc2 (PVOID pParam);
		void DoCipher2 ();
		void DoCipherThread ();
		static DWORD WINAPI CipherProcSingle (PVOID pParam);
		void DoMultiCipher (HWND hWnd, MemoryBuffer *pinbuffer, MemoryBuffer *poutbuffer, bool bEncrypt);
		void DoSingleCipher (HWND hWnd, MemoryBuffer *pinbuffer, MemoryBuffer *poutbuffer, bool bEncrypt);
		void SetDiagnostics (Diagnostics *pdiag);
		void GenSeed (char *szPhrase);
		unsigned long GetRand (int iMax);
		unsigned int ForwardInt (unsigned int iValue, unsigned int iAmount);
		unsigned int ForwardInt2 (unsigned int iValue, unsigned int iLimit);
		unsigned int ForwardIntEx (unsigned int iValue, unsigned int iLimit);
		unsigned long GetHiveValue (unsigned long iOffset, int iNumdigits);
		void SeedChecksum ();
		void OutputSeed ();
		void ScaleSeed ();
		void ShuffleSeed ();
		void ShuffleHive ();
		void GenHive ();
		void Test ();
		void Test2 ();
		unsigned long ExpandHive (MemoryBuffer *phive);
		bool CreateKeyHive (LPCSTR lpszPassword);
		bool CreateKeySeed (LPCSTR lpszPassword, MemoryBuffer *memDest);
		unsigned int GetNumDigits (unsigned long value);
		bool DoResequence (HWND hWnd, MemoryBuffer *pinbuffer, bool bEncrypt);
		bool DoCipher (HWND hWnd, MemoryBuffer *pinbuffer, MemoryBuffer *poutbuffer, bool bEncrypt);
		bool DoPartCipher (HWND hWnd, MemoryBuffer *pinbuffer, unsigned long istartoffset, unsigned long ilength, unsigned long rndHive, unsigned int msgprogress, bool Encrypt);
		void ProcessZero (MemoryBuffer *pbuffer);
		unsigned long Raise (unsigned long lValue, unsigned long lPower);
		void NotifyEvent (HWND hWnd, WPARAM wParam, LPARAM lParam);
		void SetAlgorithm (unsigned int iAlgID);
		unsigned int GetAlgorithm ();

		// Multithreading public access
		
		unsigned long m_istartoffset_th1;
		unsigned long m_ilength_th1;
		
		unsigned long m_istartoffset_th2;
		unsigned long m_ilength_th2;

		MemoryBuffer *m_pinbuffer_thall;
		MemoryBuffer *m_outbuffer;
		unsigned long m_rndHive_thall;
		bool m_bEncrypt_thall;
		HWND m_hwnd;

		unsigned int m_iAlgorithm;
		char m_szPassword[SIZE_STRING];

		StandardEncryption m_enc;

		MemoryBuffer m_memHive;

	private:
		// Private methods
		void OutputInt (LPCSTR lpszText, int iValue);
		void OutputText (LPCSTR lpszText);
		void OutputText (LPCSTR lpszName, LPCSTR lpszValue);


		// Private members
		bool m_bUseDiagnostics;
		Diagnostics *m_pdiag;

		char m_szPhrase [SIZE_STRING];
		MemoryBuffer m_memSeed;
		
		unsigned int m_seedChecksum;
		bool m_bHivecreated;

		// Multithreaded Vars
		MemoryBuffer m_tmpBuffer;
		unsigned int m_progress1;
		unsigned int m_progress2;
		unsigned int m_progresssingle;
		unsigned long m_lastpercentage;
		
		bool m_bthread1done;
		bool m_bthread2done;
		bool m_bsinglethreaddone;
};