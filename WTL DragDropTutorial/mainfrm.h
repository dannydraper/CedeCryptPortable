// MainFrm.h : interface of the CMainFrame class
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__3254D144_A335_4254_80C1_6B51301FCA9F__INCLUDED_)
#define AFX_MAINFRM_H__3254D144_A335_4254_80C1_6B51301FCA9F__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "WTLCabViewView.h"

class CMainFrame : public CFrameWindowImpl<CMainFrame>,
                   public CUpdateUI<CMainFrame>,
                   public CMessageFilter, public CIdleHandler
{
public:
    DECLARE_FRAME_WND_CLASS(NULL, IDR_MAINFRAME)

    // Construction
    CMainFrame();

    // Maps
    BEGIN_MSG_MAP(CMainFrame)
        MSG_WM_CREATE(OnCreate)
        MSG_WM_DESTROY(OnDestroy)
        MSG_WM_ENDSESSION(OnEndSession)
        COMMAND_ID_HANDLER_EX(ID_FILE_OPEN, OnFileOpen)
        COMMAND_RANGE_HANDLER_EX(ID_FILE_MRU_FIRST, ID_FILE_MRU_LAST, OnMRUMenuItem)
        COMMAND_ID_HANDLER_EX(ID_APP_EXIT, OnFileExit)
        COMMAND_ID_HANDLER_EX(ID_VIEW_TOOLBAR, OnViewToolBar)
        COMMAND_ID_HANDLER_EX(ID_VIEW_STATUS_BAR, OnViewStatusBar)
        COMMAND_ID_HANDLER_EX(ID_APP_ABOUT, OnAppAbout)
        COMMAND_ID_HANDLER_EX(IDC_VIEW_ICONS, OnViewIcons)
        COMMAND_ID_HANDLER_EX(IDC_VIEW_SMALL_ICONS, OnViewSmallIcons)
        COMMAND_ID_HANDLER_EX(IDC_VIEW_LIST, OnViewList)
        COMMAND_ID_HANDLER_EX(IDC_VIEW_DETAILS, OnViewDetails)
        COMMAND_ID_HANDLER_EX(IDC_VIEW_TILES, OnViewTiles)
        NOTIFY_CODE_HANDLER_EX(LVN_BEGINDRAG, OnListBeginDrag)
        CHAIN_MSG_MAP(CUpdateUI<CMainFrame>)
        CHAIN_MSG_MAP(CFrameWindowImpl<CMainFrame>)
        REFLECT_NOTIFICATIONS()
    END_MSG_MAP()

    BEGIN_UPDATE_UI_MAP(CMainFrame)
        UPDATE_ELEMENT(ID_VIEW_TOOLBAR, UPDUI_MENUPOPUP)
        UPDATE_ELEMENT(ID_VIEW_STATUS_BAR, UPDUI_MENUPOPUP)
        UPDATE_ELEMENT(IDC_VIEW_ICONS, UPDUI_TOOLBAR)
        UPDATE_ELEMENT(IDC_VIEW_SMALL_ICONS, UPDUI_TOOLBAR)
        UPDATE_ELEMENT(IDC_VIEW_LIST, UPDUI_TOOLBAR)
        UPDATE_ELEMENT(IDC_VIEW_DETAILS, UPDUI_TOOLBAR)
        UPDATE_ELEMENT(IDC_VIEW_TILES, UPDUI_TOOLBAR)
    END_UPDATE_UI_MAP()

    // Message handlers
    BOOL PreTranslateMessage(MSG* pMsg);
    BOOL OnIdle();

    LRESULT OnCreate ( LPCREATESTRUCT lpcs );
    void    OnDestroy();
    void    OnEndSession ( BOOL bSessionEnding, UINT uReason );

    // Command handlers
    void OnFileOpen ( UINT uCode, int nID, HWND hwndCtrl );
    void OnMRUMenuItem ( UINT uCode, int nID, HWND hwndCtrl );
    void OnFileExit ( UINT uCode, int nID, HWND hwndCtrl );
    void OnViewToolBar ( UINT uCode, int nID, HWND hwndCtrl );
    void OnViewStatusBar ( UINT uCode, int nID, HWND hwndCtrl );
    void OnAppAbout ( UINT uCode, int nID, HWND hwndCtrl );
    void OnViewIcons ( UINT uCode, int nID, HWND hwndCtrl );
    void OnViewSmallIcons ( UINT uCode, int nID, HWND hwndCtrl );
    void OnViewList ( UINT uCode, int nID, HWND hwndCtrl );
    void OnViewDetails ( UINT uCode, int nID, HWND hwndCtrl );
    void OnViewTiles ( UINT uCode, int nID, HWND hwndCtrl );

    // Notify handlers
    LRESULT OnListBeginDrag ( NMHDR* phdr );

protected:
    CWTLCabViewView m_view;
    CRecentDocumentList m_mru;

    CString m_sCurrentCabFilePath;

    void ViewCab ( LPCTSTR szCabFilename, int nMRUID = 0 );
    bool EnumCabContents ( LPCTSTR szCabFilename );
    void CheckViewControls ( int nIDToCheck );

    // FDICopy() callbacks
    static int DIAMONDAPI fdi_Notify ( FDINOTIFICATIONTYPE message, PFDINOTIFICATION pInfo );
    int Notify_CabInfo ( LPCTSTR szCabName, LPCTSTR szDiskName, LPCTSTR szCabPath,
                         USHORT uCabID, USHORT uCabNumber );
    int Notify_PartialFile ( LPCTSTR szFilename, LPCTSTR szStartingCabName,
                             LPCTSTR szStartingDiskName );
    int Notify_Enumerate ( long lCurrPos, USHORT cFilesLeft, USHORT uCabID );
    int Notify_CopyFile ( LPCTSTR szFilename, long cbyUncompressedSize, USHORT uDate,
                          USHORT uTime, USHORT uAttribs, USHORT uFolderIdx );
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__3254D144_A335_4254_80C1_6B51301FCA9F__INCLUDED_)
