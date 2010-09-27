
#include <windows.h>
#include <commctrl.h>
#include <winable.h>
#include <io.h>
#include <shlobj.h>


#include "UIWindow.h"
#include "UIHandler.h"
#include "ControlCreator.h"
#include "UIPicButton.h"
#include "UIBanner.h"
#include "Diagnostics.h"
#include "UILabel.h"
#include "MenuHandler.h"
#include "Stack.h"
#include "PasswordWindow.h"
#include "Encryption.h"
#include "MemoryBuffer.h"
#include "AboutWindow.h"
#include "DirScanner.h"
#include "Licensing.h"
#include "LicenseWindow.h"
#include "FileShredder.h"
#include "ProtectedFoldersWindow.h"
#include "TextWindow.h"
#include "UTEOptionsWindow.h"
#include "InternetHandler.h"
#include "AppUpdater.h"
#include "ToolsOptionsWindow.h"
#include "Constants.h"
#include "PFolderRequestWindow.h"
#include "FileHandler.h"
#include "StandardEncryption.h"
#include "SingleFileInfo.h"
#include "EncryptWindow.h"
#include "EncryptWindowEx.h"
#include "UIToolbar.h"
#include "dataobject.h"
#include "dropsource.h"

#define MODE_FILE	7000
#define MODE_FOLDER	7001
#define MODE_TEXT	7004

#define MODE_ENCRYPT 7002
#define MODE_DECRYPT 7003
#define APP_ENCRYPT 9000
#define APP_DECRYPT	9001

#define ACTIVATE_SUCCESS	300
#define ACTIVATE_FAILED		301
#define ACTIVATE_EXISTS		302
#define ACTIVATE_NOCONNECTION	303
#define DEACTIVATE_SUCCESS	304
#define DEACTIVATE_FAILED	305
#define DEACTIVATE_NOTACTIVATED	306

class MainWindow : public UIWindow
{
	public:
		MainWindow ();
		~MainWindow ();
		
		void Initialise (HWND hWnd, LPSTR lpCmdLine);
		void SetDiagnostics (Diagnostics *pdiag);
		BOOL MySystemShutdown();
		void CheckApplicationUpdate (bool bunattended);
		bool ValidateOS ();
		void InitialisePassword ();

	private:
		// Private Member Variables & objects
		static INT_PTR CALLBACK OptionsDlgProc (HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
		static bool m_sbcancelwaitdlg;
		//bool m_sbcancelwaitdlg;

		BOOL ShowWaitDialog ();

		bool OpenSingleFile ();
		bool SaveSingleFile ();
		bool BrowseForFolder (LPCTSTR szTitle, char *szOutPath);
		void OutputStatusInt (LPCSTR lpszText, int iValue);
		void OutputStatusText (LPCSTR lpszText);
		void OutputStatusText (LPCSTR lpszName, LPCSTR lpszValue);
		bool DoesFileExist (char *szFilepath, char *szFilename);
		void ValidateInput ();
		void DoEncryption ();
		void BrowseSource ();
		void BrowseDestination ();
		void SaveEncryptedFile ();
		unsigned int CalcInputChecksum ();
		unsigned int CalcOutputChecksum ();
		void ParseCommandLine (char *pszCmdline);
		void GetFileList (char *szFolderPath);
		void Test ();
		void CombineKeys (MemoryBuffer *memKeyOne, MemoryBuffer *memKeyTwo, MemoryBuffer *memKeyOut);
		int Resourcetomem (LPCTSTR ResourceName, MemoryBuffer *memResource);
		void PrepareShellInfoPath ();
		void PrepareTempCommandFile (char *pszOrigPath);
		bool FileExists (char *FileName);
		void CheckShellCommand ();
		bool CheckProtectedModeFiles (bool bEncrypt);
		void DoSessionDecryption ();
		bool DoSessionEncryption ();

		bool DoesShellCommandExist ();
		void RSAGenandEncrypt ();

		void RSAGenerateKey ();
		void WritetoKeyCanvas ();
		bool GetKeyfromCanvas ();
		bool GetDistributionfromCanvas ();

		void RSAPrivateFromPublic ();
		void EncryptUsingPublic ();
		void DecryptUsingPrivate ();
		bool IsDirectory (char *szPathname);
		void SecurePassword ();
		void SecurePasswordUsingCanvas ();
		void RecoverPassword ();
		bool GetRecoveryPassword (MemoryBuffer *memPrivateKey, MemoryBuffer *memEncpassword);
		void RecoverCanvasPassword ();
		bool OpenPrivateKey ();
		void ShowLicensePrompt ();
		void UpdateWindowCaption ();
		void CheckDirectCommand ();
		void CheckSessionAction ();

		void CreateFileAssociation ();
		void RemoveFileAssociation ();

		void GetFileExtension (bool &bFound, char *pszFilePath, char *pszOutExtension);
		void SendKeyState(int vkeycode);
		void SetShiftState(int sstate);
		void GetClipboard ();
		void DoTextEncryption ();
		void SaveEncryptedText ();
		void MakeReadable (MemoryBuffer *memInput, MemoryBuffer *memOutput);
		bool CheckEnclosingTags (MemoryBuffer *memReadable);
		bool CheckStartTag (MemoryBuffer *memReadable);
		int GetStartTagLocation (MemoryBuffer *memReadable);
		int GetEndTagLocation (MemoryBuffer *memReadable);
		void MakeBinary (MemoryBuffer *memInput, MemoryBuffer *memOutput);
		void CreateShellExtensionPath ();
		unsigned int GetVirtualKeyCode (char *szChar);
		void RegisterEncryptionHotkey (bool bUseCtrl, bool bUseAlt, LPCSTR szHotkey);
		void RegisterDecryptionHotkey (bool bUseCtrl, bool bUseAlt, LPCSTR szHotkey);

		void DoUpdateCheck (bool unattended);
		static DWORD WINAPI UpdateThreadProc (PVOID pParam);
		static DWORD WINAPI UpdateThreadProcUnat (PVOID pParam);
		void StoreUpdateCurrentDate ();
		bool IsUpdateCheckDateDifferent ();
		void DoAutomaticUpdateCheck ();
		unsigned long GetFileSize (char *szFilepath);
		void InitialisePackage ();
		void AddSingleFileInfo (char *szName, char *szFilepath, unsigned long lFilesize, char *szModified);
		HGLOBAL CopyDragText ();
		HGLOBAL PrepareDropTarget (HWND hwndlv);

		HRESULT CreateDropSource(IDropSource **ppDropSource);
		HRESULT CreateDataObject (FORMATETC *fmtetc, STGMEDIUM *stgmeds, UINT count, IDataObject **ppDataObject);
		void PrepareListView ();
		int LV_GetFirstSelectedIndex (HWND hwndlv);
		void LV_GetAllSelectedFiles (HWND hwndlv);
		int LV_GetSelectedCount (HWND hwndlv);

		void RefreshListView (char *szDir);
		bool PopLastItem (DynList *pdlList, char *szDestitem);
		void InitialiseStartPath ();
		void GetPathOnly (char *szInpath, char *szOutpath, char *szOutfile, char *szSep);
		void SaveSettings ();
		void LoadSettings ();
		void DeleteSelected ();
		int CompareHive (MemoryBuffer *memOne, MemoryBuffer *memTwo);
		unsigned long long GetVaultSize ();
		static DWORD WINAPI DragProc (PVOID pParam);
		void LaunchActivation (bool bDeactivate);
		void DeleteActivator ();
		void DeactivateLicense ();
		bool Findmemstring (char *szString, MemoryBuffer *memBuffer);
		unsigned int ActivateProductKey (char *szProductKey, bool bActivate);
		void BrowserCheck ();

		// The primary encryption class
		Encryption m_crypt;

		// File input and output buffers
		MemoryBuffer m_fileinput;
		MemoryBuffer m_fileinputex;
		MemoryBuffer m_fileoutput;
		MemoryBuffer m_fileoutputex;

		// The UI Handler required for multiple handling of custom controls.
		UIHandler m_uihandler;


		// Test Encryption object
		Encryption m_keycrypt;
		
		//*******************************************
		// MSCryptAPI stuff
		HCRYPTPROV hProvider;
		HCRYPTKEY hGeneratedKey;
		MemoryBuffer m_memExportedPublicKey;
		MemoryBuffer m_memExportedPrivateKey;

		MemoryBuffer m_memDataBuffer; // plaintext data
		MemoryBuffer m_memEncrypted; // encrypted data
		MemoryBuffer m_memCanvasEncrypted; // encrypted data
		MemoryBuffer m_memDecrypted; // decrypted data

		MemoryBuffer m_memImportedPublicKey;
		HCRYPTKEY hImportedPublic;

		MemoryBuffer m_memImportedPrivateKey;
		HCRYPTKEY hImportedPrivate;

		HCRYPTKEY hSecondGeneratedKey;
		MemoryBuffer m_memExportedSecondPrivateKey;

		MemoryBuffer m_memEncpassword;

		MemoryBuffer m_memGenPublic;
		MemoryBuffer m_memGenPrivate;
		MemoryBuffer m_memCanvasPublicKey;
		bool m_bCanvasPublicKeypresent;

		//*******************************************

		// The Control Creater required for fast creation of controls
		ControlCreator m_ccontrols;
		
		//************* Licensing stuff

		Licensing m_license;
		bool m_bvalidlicenseinitialised;

		// The license prompt window
		LicenseWindow m_licensewindow;
		char m_szUserlicense[SIZE_STRING];

		char m_szWindowcaption[SIZE_STRING];
		char m_szWindowcomment[SIZE_STRING];


		//*****************************

		// Recursive Directory Scanner class
		DirScanner m_Dirscanner;

		char m_szVaultpath[SIZE_STRING];

		unsigned long long m_lmaxdemosize;

		// File shredding class for secure
		// deletions
		FileShredder m_fileshred;

		// ******** PROTECTED MODE (SESSION) ***
		// protected folders window
		ProtectedFoldersWindow m_pfolders;
		bool m_bSessionmode;
		bool m_bProtectedfilesexist;
		char m_szSessionpassword[SIZE_NAME];
		bool m_bUsesessionpassword;
		bool m_bSessionpasswordpresent;
		bool m_bDecryptionsessioncheck;
		bool m_bSessionshuttingdown;
		bool m_bShutdownencryptdone;
		// *************************************


		// Text encryption stuff
		MemoryBuffer m_memTextbuffer;
		MemoryBuffer m_memTextoutput;
		MemoryBuffer m_memTextprocessed;		
		MemoryBuffer m_memTextreadable;
		MemoryBuffer m_memTextpasteable;
		MemoryBuffer m_memTextparseable;
		MemoryBuffer m_memTextfinished;

		TextWindow m_textwindow;
		UTEOptionsWindow m_uteoptions;

		HWND m_lastactivewindow;

		// File flags
		unsigned int m_inputchecksum;
		unsigned int m_inputid;
		unsigned int m_checksum;
		unsigned int m_ccryptid;
		unsigned int m_ccryptid2;
		unsigned int m_ccryptid3;
		unsigned int m_ccryptidprev1;

		// The password request window
		PasswordWindow m_passwindow;

		// Flag indicating first time setup
		bool m_bFirstimesetup;

		// File handler
		// Used for checking of duplicate files
		// and proposing new path names
		FileHandler m_filehandler;

		// The about window
		AboutWindow m_aboutwindow;

		// Preferences path
		char m_szPrefpath[SIZE_STRING];

		// Tools Options Window
		ToolsOptionsWindow m_optionswindow;

		// Protected Folders Request window (if the user has turned on the Protected Folders prompt)
		PFolderRequestWindow m_pfrequest;

		// The user password
		char m_szPassword[SIZE_NAME];

		// Loaded hive hash
		MemoryBuffer m_memhashhive;

		// Windows controls
		HWND m_hwndgroup;
		HWND m_hwndgroup1;
		HWND m_hwndgroup2;
		HWND m_hwndgroup3;
		HWND m_hwndoptfile;
		HWND m_hwndoptfolder;
		HWND m_hwndoptpackage;
		HWND m_hwndoptencrypt;
		HWND m_hwndoptdecrypt;
		HWND m_hwndeditsource;
		HWND m_hwndeditdest;
		HWND m_hwndbtnbegin;
		HWND m_hwndbtnsrcbrowse;
		HWND m_hwndbtndstbrowse;
		HWND m_hwndbtnexit;
		HWND m_hwndlststatus;
		HWND m_hwndprogress;
		HWND m_hwndtotalprogress;
		HWND m_hwndphase;
		HWND m_chkkeepopen;
		HWND m_btncancel;
		HWND m_hwndlbltotal;
		HWND m_hwndtxtpath;
		UILabel m_lblintro;
		UIRect m_bgrect;

		// The toolbar
		UIToolbar m_toolbar;

		// The main encryption window
		EncryptWindowEx m_wndenc;

		// The better replacement encryption window
		// a modal dialog instead
		//EncryptWindowEx m_wndencex;

		// The current directory being displayed
		char m_szCurrentfolder[SIZE_STRING];

		// The list of previous directories we've navigated
		DynList m_dlFolderhistory;

		// Our list view control for manipulating
		// files within the encrypted vault

		HWND m_hwndlistview;
		HIMAGELIST m_hLarge;   // image list for icon view 
		HIMAGELIST m_hSmall;   // image list for other views 

		// Our list of SingleFileInfo's
		DynList m_dlListviewfiles;

		// Package Creation (Self-EXE) Memory Buffer
		MemoryBuffer m_memPackage;

		// Application flags
		unsigned int m_filemode;
		unsigned int m_appmode;
		bool m_bcreatepackage;
		bool m_busingownpackagepath;
		bool m_btextmode;
		char m_szInputfile[SIZE_STRING];
		char m_szInputfiletitle[SIZE_STRING];
		char m_szOutputfile[SIZE_STRING];

		char m_szInputfolder[SIZE_STRING];
		char m_szPrivateKey[SIZE_STRING];
		char m_szPrivateKeyTitle[SIZE_STRING];

		char m_szCommandPath[SIZE_STRING];
		char m_szCommandTempFile[SIZE_STRING];
		// Flag to indicate whether we
		// delete the original files
		bool m_bDeleteorig;

		// The name of this particular CedeCrypt distribution
		// this will be either set to CedeSoft or read from
		// the distribution string in the binary resource
		// BINARY3
		char m_szDistributionname[SIZE_STRING];

		// Flag to indicate if we're in shell mode
		bool m_bShellmode;
		bool m_bCommandmode;
		bool m_bCreatewindow;

		// Application mode
		unsigned int m_cmdmode;

		// File Encryption timings
		unsigned int m_starttime;
		unsigned int m_finishtime;
		bool m_busingtimings;

		// Folder pointer
		int m_iFilepointer;

		// A flag to indicate the user wishes
		// to cancel the operation
		bool m_bAwaitingcancel;

		// Flag indicating that recovery mode
		// is activated
		bool m_bRecoverymode;
		bool m_bCanvasRecoverymode;

		//A test stack
		Stack m_stack;

		// Global hwnd
		HWND m_hwnd;		

		// Flag to indicate whether we accept drops
		bool m_bAcceptdrops;

		// The header bitmap image
		UIBanner m_header;

		// The MD5 hash
		char m_szmd5[SIZE_STRING];

		// The registry handler
		RegistryHandler m_registry;

		// The main menu class
		MenuHandler m_mainmenu;

		bool m_bCancelquestionshown;

		// Pointer to the global diagnostics window
		Diagnostics *m_pdiag;

		// List of input files if used in folder
		// mode
		DynList m_dlFilelist;

		// The shell info path - the path for the shell
		// command file if one is present
		char m_szShellInfoPath[SIZE_STRING];

		// Temporary input output buffers
		//MemoryBuffer m_inBuffer;
		//MemoryBuffer m_outBuffer;

		// event notification from base class
		void OnCreate (HWND hWnd);
		void OnNotify (HWND hWnd, WPARAM wParam, LPARAM lParam);
		void OnCommand (HWND hWnd, WPARAM wParam, LPARAM lParam);
		void OnUICommand (HWND hWnd, WPARAM wParam, LPARAM lParam);		
		void OnPaint (HWND hWnd);
		void OnTimer (WPARAM wParam);
		void OnHotKey (HWND hWnd, WPARAM wParam, LPARAM lParam);
		void OnMouseMove (HWND hWnd, int mouseXPos, int mouseYPos);
		void OnLButtonDown (HWND hWnd);
		void OnLButtonDblClick (HWND hWnd, WPARAM wParam, LPARAM lParam);				
		void OnCryptEvent (HWND hWnd, WPARAM wParam, LPARAM lParam);
		void OnLButtonUp (HWND hWnd);
		void OnDropFiles (HWND hWnd, WPARAM wParam, LPARAM lParam);

		LRESULT OnQueryEndSession (HWND hWnd, LPARAM lParam);
		LRESULT OnEndSession (HWND hWnd, WPARAM wParam, LPARAM lParam);
		LRESULT	OnPowerBroadcast (HWND hWnd, WPARAM wParam, LPARAM lParam);
};
