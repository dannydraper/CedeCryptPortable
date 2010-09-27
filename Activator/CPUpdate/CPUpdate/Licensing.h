#pragma once
#define _CRT_RAND_S
#include <windows.h>
#include <io.h>

#include "MemoryBuffer.h"
#include "RegistryHandler.h"
#include "resource.h"

class Licensing {
	
	public:
		Licensing ();
		~Licensing ();
		void Test ();		
		bool MachineMatch ();
		bool VerifyStore ();
		bool UserKeyPresent ();
		bool ValidateUserKey (char *szInputKey);
		void SaveMachineKey ();
		void SaveUserKey (char *szUserKey);
		void ClearAllKeys ();
		bool LoadUserKey (char *szOutkey);
		char *GetOwner ();
		char *GetUniqueProductID ();
		bool GetTrialMode ();
		int GetTrialDays ();
		int GetMaxClients ();

		void CheckInst ();
		void GetEncSysIdent (char *szOut);
		void SaveEncIdent ();
		void SaveCurrentIdent ();
		int VersionIdent ();
		int CurrentIdent ();
		int IdentVersion (DWORD dwHigh, DWORD dwLow);
		bool GetFirstInst ();
		int GetDaysInTrial ();

		void DecryptString (char *szInstring, char *szOutstring);
		void EncryptString (char *szInstring, char *szOutstring);
		

		void SaveCurrentDate (char *szKeyname);
		bool ReadSavedDate (char *szKeyname, WORD *wDay, WORD *wMonth, WORD *wYear);
		int TrialValid ();
		int GetDaysPassed (WORD wDay1, WORD wMonth1, WORD wYear1, WORD wDay2, WORD wMonth2, WORD wYear2);
		void ShowExpiredMessage ();

		void CheckInstEx ();

		bool CLPasswordPresent ();
		void CLClearPassword ();
		bool CLIsPasswordValid (char *szPassword);
		bool CLSaveNewPassword (char *szPassword);
		bool CheckDLLPath ();
		unsigned long GetWriteableOffset ();
		DWORD GetVolumeSerialNumber ();
		void SetDLLPath (char *szInpath);

	private:
		// Private methods
		void Expand (char *szInval, char *szOutval);
		unsigned long Raise (unsigned long lValue, unsigned long lPower);
		void RepeatExpand (char *szInval, int Length, char *szOutval);
		void FilterH (char *szInval, char *szOutval);
		void UnShiftPChar (char *szCharone, int Shiftamount, char *szOutval);
		void DecString (char *szInstring, char *szKeyhive, char *szOutstring);
		int UnShiftSingle (int inval, int shiftamount);
		void DecCS (char *szInstring, int csum, char *szOutstring);
		int GenCSum (char *szInstring);
		bool IsNumber (char *szInstring);
		void TextFromASC (char *szInstring, char *szOutstring);
		void EncBytes (char *szInstring, char *szKeyhive, MemoryBuffer *poutBuffer);
		void DecBytes (MemoryBuffer *pinBuffer, char *szKeyhive, char *szOutString);
		void ReadMachine (char *szOutstring);
		void GetASC (char *szValue);
		void GenMachineKey (char *szOutstring);
		void AddASC (char *szAppendstring, int ascval);
		void GenRegKeys (char *szIDate, char *szProdID, char *RegOrg, char *RegOwn);
		void GetPathOnly (char *szInpath, char *szOutpath);
		bool LoadMachineKey (char *szOutKey);
		void ShowInt (char *szTitle, int iValue);
		bool PrepareDLLPath ();
		bool FileExists (char *FileName);
		


		// License fields
		char m_szOwner[SIZE_STRING];
		int m_iFirstAuth;
		int m_iWebID;
		int m_iTrial;
		int m_iTrialdays;
		int m_iMaxclients;
		char m_szUniqueID[SIZE_STRING];
		int m_iYear;
		int m_iMonth;
		int m_iDay;
		int m_iVendID1;
		int m_iVendID2;
		int m_iSecondAuth;
		int m_iDistID1;
		int m_iDistID2;
		int m_iFeatureMode;

		bool m_bGenuineOne;
		bool m_bGenuineTwo;
		bool m_bWebsource;
		bool m_bTrial;

		// Trial tracking
		bool m_bFirstinst;
		int m_idaysintrial;

		char m_szDLLPath[SIZE_STRING];
		bool m_bDLLpathprepared;

		RegistryHandler m_registry;

		// Private members
		bool m_bUseDiagnostics;


};
