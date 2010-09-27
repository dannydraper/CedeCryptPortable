// MainFrm.cpp : implmentation of the CMainFrame class
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "aboutdlg.h"
#include "WTLCabViewView.h"
#include "MainFrm.h"
#include "CabFuncs.h"
#include "MyDialogs.h"
#include "DragDropSource.h"

struct __declspec(uuid("DE5BF786-477A-11d2-839D-00C04FD918D0")) IDragSourceHelper;


/////////////////////////////////////////////////////////////////////////////
// Construction

CMainFrame::CMainFrame()
{
}


/////////////////////////////////////////////////////////////////////////////
// Message handlers

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
    if ( CFrameWindowImpl<CMainFrame>::PreTranslateMessage(pMsg) )
        return TRUE;

    return m_view.PreTranslateMessage ( pMsg );
}

BOOL CMainFrame::OnIdle()
{
    UIUpdateToolBar();
    return FALSE;
}

LRESULT CMainFrame::OnCreate ( LPCREATESTRUCT lpcs )
{
HWND hWndToolBar = CreateSimpleToolBarCtrl(m_hWnd, IDR_MAINFRAME, FALSE, ATL_SIMPLE_TOOLBAR_PANE_STYLE);

    CreateSimpleReBar ( ATL_SIMPLE_REBAR_NOBORDER_STYLE );
    AddSimpleReBarBand ( hWndToolBar );

    CreateSimpleStatusBar();

    m_hWndClient = m_view.Create ( m_hWnd, rcDefault );
    m_view.Init();

    // Init MRU list
CMenuHandle mainMenu = GetMenu();
CMenuHandle fileMenu = mainMenu.GetSubMenu(0);

    m_mru.SetMaxEntries(9);
    m_mru.SetMenuHandle ( fileMenu );
    m_mru.ReadFromRegistry ( APP_SETTINGS_KEY );

    // Init UI stuff
    UIAddToolBar ( hWndToolBar );
    UISetCheck ( ID_VIEW_TOOLBAR, 1 );
    UISetCheck ( ID_VIEW_STATUS_BAR, 1 );

    // Start in tile mode on XP+. On pre-XP, start in report mode and disable
    // the tile view command.
    if ( g_bXPOrLater )
        {
        m_view.SetViewMode ( LV_VIEW_TILE );
        CheckViewControls ( IDC_VIEW_TILES );
        }
    else
        {
        UIEnable ( IDC_VIEW_TILES, false );
        CMenuHandle(GetMenu()).EnableMenuItem ( IDC_VIEW_TILES, MF_BYCOMMAND|MF_GRAYED );
        m_view.SetViewMode ( LV_VIEW_DETAILS );
        CheckViewControls ( IDC_VIEW_DETAILS );
        }

    // Register object for message filtering and idle updates
CMessageLoop* pLoop = _Module.GetMessageLoop();

    ATLASSERT(pLoop != NULL);
    pLoop->AddMessageFilter(this);
    pLoop->AddIdleHandler(this);

    return 0;
}

void CMainFrame::OnDestroy()
{
    // Unregister object for message filtering and idle updates
CMessageLoop* pLoop = _Module.GetMessageLoop();

    ATLASSERT(pLoop != NULL);
    pLoop->RemoveMessageFilter(this);
    pLoop->RemoveIdleHandler(this);

    // Save the MRU
    m_mru.WriteToRegistry ( APP_SETTINGS_KEY );

    // Clean up temp files
std::vector<CString>::const_iterator it;

    for ( it = g_vecsTempFiles.begin(); it != g_vecsTempFiles.end(); it++ )
        DeleteFile ( *it );

    SetMsgHandled(false);
}

void CMainFrame::OnEndSession ( BOOL bSessionEnding, UINT uReason )
{
    if ( bSessionEnding )
        {
        // Save the MRU
        m_mru.WriteToRegistry ( APP_SETTINGS_KEY );

        // Clean up temp files
        std::vector<CString>::const_iterator it;

        for ( it = g_vecsTempFiles.begin(); it != g_vecsTempFiles.end(); it++ )
            DeleteFile ( *it );
        }
}


/////////////////////////////////////////////////////////////////////////////
// Command handlers

void CMainFrame::OnFileOpen ( UINT uCode, int nID, HWND hwndCtrl )
{
CMyFileDialog dlg ( true, _T("cab"), 0U, OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,
                    IDS_OPENFILE_FILTER, *this );

    if ( IDOK == dlg.DoModal ( *this ) )
        ViewCab ( dlg.m_szFileName );

    // Some interesting CABs to look at, if you have the Win 98 CABs lying around:
    // win98_21.cab - 6 files, all continued to next cab
    // win98_22.cab - has files from prev cab and continued in next cab
    // driver14.cab - only has a file from the prev cab
}

void CMainFrame::OnMRUMenuItem ( UINT uCode, int nID, HWND hwndCtrl )
{
CString sFile;

    // Read the selected filename from the MRU list, and open that file.
    if ( m_mru.GetFromList ( nID, sFile ) )
        ViewCab ( sFile, nID );
}

void CMainFrame::OnFileExit ( UINT uCode, int nID, HWND hwndCtrl )
{
    PostMessage ( WM_CLOSE );
}

void CMainFrame::OnViewToolBar ( UINT uCode, int nID, HWND hwndCtrl )
{
static BOOL bVisible = TRUE;            // initially visible
CReBarCtrl rebar = m_hWndToolBar;
int nBandIndex = rebar.IdToIndex ( ATL_IDW_BAND_FIRST );    // toolbar is first band

    bVisible = !bVisible;
    rebar.ShowBand ( nBandIndex, bVisible );
    UISetCheck ( ID_VIEW_TOOLBAR, bVisible );
    UpdateLayout();
}

void CMainFrame::OnViewStatusBar ( UINT uCode, int nID, HWND hwndCtrl )
{
BOOL bVisible = !::IsWindowVisible ( m_hWndStatusBar );

    ::ShowWindow ( m_hWndStatusBar, bVisible ? SW_SHOWNOACTIVATE : SW_HIDE );
    UISetCheck ( ID_VIEW_STATUS_BAR, bVisible );
    UpdateLayout();
}

void CMainFrame::OnAppAbout ( UINT uCode, int nID, HWND hwndCtrl )
{
CAboutDlg dlg;

    dlg.DoModal ( *this );
}

void CMainFrame::OnViewIcons ( UINT uCode, int nID, HWND hwndCtrl )
{
    m_view.SetViewMode ( LV_VIEW_ICON );
    CheckViewControls ( nID );
}

void CMainFrame::OnViewSmallIcons ( UINT uCode, int nID, HWND hwndCtrl )
{
    m_view.SetViewMode ( LV_VIEW_SMALLICON );
    CheckViewControls ( nID );
}

void CMainFrame::OnViewList ( UINT uCode, int nID, HWND hwndCtrl )
{
    m_view.SetViewMode ( LV_VIEW_LIST );
    CheckViewControls ( nID );
}

void CMainFrame::OnViewDetails ( UINT uCode, int nID, HWND hwndCtrl )
{
    m_view.SetViewMode ( LV_VIEW_DETAILS );
    CheckViewControls ( nID );
}

void CMainFrame::OnViewTiles ( UINT uCode, int nID, HWND hwndCtrl )
{
    ATLASSERT(g_bXPOrLater);

    m_view.SetViewMode ( LV_VIEW_TILE );
    CheckViewControls ( nID );
}


/////////////////////////////////////////////////////////////////////////////
// Notify handlers

LRESULT CMainFrame::OnListBeginDrag ( NMHDR* phdr )
{
NMLISTVIEW* pnmlv = (NMLISTVIEW*) phdr;
std::vector<CDraggedFileInfo> vec;
CComObjectStack2<CDragDropSource> dropsrc;
DWORD dwEffect = 0;
HRESULT hr;
CComPtr<IDragSourceHelper> pdsh;

    // Get a list of the files being dragged (minus files that we can't extract
    // from the current CAB).
    if ( !m_view.GetDraggedFileInfo ( vec ) )
        {
        ATLTRACE("Error: Couldn't get list of files dragged (or only partial files were dragged)\n");
        return 0;   // do nothing
        }

    // Init the drag/drop data object.
    if ( !dropsrc.Init ( m_sCurrentCabFilePath, vec ) )
        {
        ATLTRACE("Error: Couldn't init drop source object\n");
        return 0;   // do nothing
        }

    // On 2K+, init a drag source helper object that will do the fancy drag
    // image when the user drags into Explorer (or another target that supports
    // the drag/drop helper).
    hr = pdsh.CoCreateInstance ( CLSID_DragDropHelper );

    if ( SUCCEEDED(hr) )
        {
        CComQIPtr<IDataObject> pdo;

        if ( pdo = dropsrc.GetUnknown() )
            pdsh->InitializeFromWindow ( m_view, &pnmlv->ptAction, pdo );
        }

    // Start the drag/drop!
    hr = dropsrc.DoDragDrop ( DROPEFFECT_COPY, &dwEffect );

    if ( FAILED(hr) )
        ATLTRACE("Error: DoDragDrop() failed, error: 0x%08X\n", hr);
    else
        {
        // If we found any files continued into other CABs, update the UI.
        const std::vector<CDraggedFileInfo>& vecResults = dropsrc.GetDragResults();
        std::vector<CDraggedFileInfo>::const_iterator it;

        for ( it = vecResults.begin(); it != vecResults.end(); it++ )
            {
            if ( it->bPartialFile )
                m_view.UpdateContinuedFile ( *it );
            }
        }

    return 0;
}


/////////////////////////////////////////////////////////////////////////////
// Other methods

void CMainFrame::ViewCab ( LPCTSTR szCabFilename, int nMRUID )
{
    if ( EnumCabContents ( szCabFilename ) )
        {
        m_sCurrentCabFilePath = szCabFilename;

        // If this CAB file was already in the MRU list, move it to the
        // top of the list. Otherwise, add it to the list.
        if ( 0 == nMRUID )
            m_mru.AddToList ( szCabFilename );
        else
            m_mru.MoveToTop ( nMRUID );
        }
    else
        {
        // We couldn't read the contents of this CAB file, so remove it from
        // the MRU list if it was in there.
        if ( 0 != nMRUID )
            m_mru.RemoveFromList ( nMRUID );
        }
}

bool CMainFrame::EnumCabContents ( LPCTSTR szCabFilename )
{
CWaitCursor w;
TCHAR szCabDir[MAX_PATH], szCabTitle[MAX_PATH];
ERF erf = {0};
FDICABINETINFO info = {0};
int hf = 0;
HFDI hfdi = NULL;

    // Check that the passed-in file exists.
    if ( !PathFileExists ( szCabFilename ) )
        {
        AtlMessageBox ( *this, IDS_FILE_NOT_FOUND, IDS_MSGBOX_TITLE, MB_ICONERROR );
        return false;
        }

    // The CAB API takes the CAB file path as separate dir/name parts,
    // so split up the path.
    lstrcpyn ( szCabDir, szCabFilename, countof(szCabDir) );
    PathRemoveFileSpec ( szCabDir );
    PathAddBackslash ( szCabDir );
    lstrcpyn ( szCabTitle, PathFindFileName ( szCabFilename ), countof(szCabTitle) );

    // Init the CAB decompression engine.
    hfdi = FDICreate ( cab_Alloc, cab_Free, cab_Open, cab_Read, cab_Write,
                       cab_Close, cab_Seek, 0, &erf );

    if ( NULL == hfdi )
        {
        CString sMsg;

        sMsg.Format ( IDS_FDICREATE_FAILED, erf.erfOper );
        AtlMessageBox ( *this, (LPCTSTR) sMsg, IDS_MSGBOX_TITLE, MB_ICONERROR );
        return false;
        }

    // Open the CAB file for reading.
    hf = cab_Open ( T2A(LPTSTR(szCabFilename)), _O_BINARY|_O_RDONLY, 0 );

    if ( -1 == hf )
        {
        AtlMessageBox ( *this, IDS_OPEN_CAB_ERROR, IDS_MSGBOX_TITLE, MB_ICONERROR );
        FDIDestroy ( hfdi );
        return false;
        }

    // Check that it's a valid CAB file.
    if ( !FDIIsCabinet ( hfdi, hf, &info ) )
        {
        cab_Close ( hf );
        AtlMessageBox ( *this, IDS_INVALID_CAB_ERROR, IDS_MSGBOX_TITLE, MB_ICONERROR );
        FDIDestroy ( hfdi );
        return false;
        }

    cab_Close ( hf );

    m_view.Clear();
    m_view.SetRedraw ( false );

    // Enum the contents of the CAB.
    if ( !FDICopy ( hfdi, T2A(szCabTitle), T2A(szCabDir), 0, fdi_Notify, NULL, this ) )
        {
        CString sMsg;

        m_view.SetRedraw();

        sMsg.Format ( IDS_FDICOPY_FAILED, erf.erfOper );
        AtlMessageBox ( *this, (LPCTSTR) sMsg, IDS_MSGBOX_TITLE, MB_ICONERROR );
        FDIDestroy ( hfdi );
        return false;
        }

    FDIDestroy ( hfdi );

    // We're done, so fix up the column widths and update the frame's title
    // text to show the CAB filename.
    m_view.SetColumnWidth ( 0, LVSCW_AUTOSIZE_USEHEADER );
    m_view.SetColumnWidth ( 1, LVSCW_AUTOSIZE_USEHEADER );
    m_view.SetColumnWidth ( 2, LVSCW_AUTOSIZE_USEHEADER );
    m_view.SetColumnWidth ( 3, LVSCW_AUTOSIZE_USEHEADER );
    m_view.SetColumnWidth ( 4, LVSCW_AUTOSIZE_USEHEADER );
    m_view.SetColumnWidth ( 5, 150 );
    m_view.SetRedraw();
    m_view.RedrawWindow();

    // Show the CAB name in the frame window title.
CString sTitle;

    sTitle.Format ( _T("%s - %s"), (LPCTSTR) _S(IDR_MAINFRAME), szCabTitle );
    SetWindowText ( sTitle );

    return true;
}

void CMainFrame::CheckViewControls ( int nIDToCheck )
{
    ATLASSERT(nIDToCheck >= IDC_VIEW_ICONS && nIDToCheck <= IDC_VIEW_TILES);

    // Uncheck all 5 view toolbar buttons, then check the one whose ID was
    // passed in.
    UISetCheck ( IDC_VIEW_ICONS, false );
    UISetCheck ( IDC_VIEW_SMALL_ICONS, false );
    UISetCheck ( IDC_VIEW_LIST, false );
    UISetCheck ( IDC_VIEW_DETAILS, false );
    UISetCheck ( IDC_VIEW_TILES, false );
    UISetCheck ( nIDToCheck, true );

    // CUpdateUI doesn't support radio checks in menus, so we handle
    // the menu separately.
    CheckMenuRadioItem ( GetMenu(), IDC_VIEW_ICONS, IDC_VIEW_TILES,
                         nIDToCheck, MF_BYCOMMAND );
}


/////////////////////////////////////////////////////////////////////////////
// FDICopy() callbacks

int DIAMONDAPI CMainFrame::fdi_Notify (
    FDINOTIFICATIONTYPE message, PFDINOTIFICATION pInfo )
{
CMainFrame* pThis = (CMainFrame*) pInfo->pv;

    switch ( message )
        {
        case fdintCABINET_INFO:
            return pThis->Notify_CabInfo ( A2CT(pInfo->psz1), A2CT(pInfo->psz2),
                                           A2CT(pInfo->psz3), pInfo->setID,
                                           pInfo->iCabinet );
        break;

        case fdintPARTIAL_FILE:
            return pThis->Notify_PartialFile ( A2CT(pInfo->psz1), A2CT(pInfo->psz2),
                                               A2CT(pInfo->psz3) );
        break;

        case fdintCOPY_FILE:
            return pThis->Notify_CopyFile ( A2CT(pInfo->psz1), pInfo->cb,
                                            pInfo->date, pInfo->time,
                                            pInfo->attribs, pInfo->iFolder );
        break;

        case fdintENUMERATE:
            return pThis->Notify_Enumerate ( pInfo->cb, pInfo->iFolder, pInfo->setID );
        break;
        }

    return 0;
}

int CMainFrame::Notify_CabInfo (
    LPCTSTR szCabName, LPCTSTR szDiskName, LPCTSTR szCabDir,
    USHORT uCabID, USHORT uCabNumber )
{
    ATLTRACE("fdi_Notify: fdintCABINET_INFO\n");
    ATLTRACE(" >> cab name: %s\n", szCabName);
    ATLTRACE(" >> disk name: %s\n", szDiskName);
    ATLTRACE(" >> cab dir: %s\n", szCabDir);
    ATLTRACE(" >> cab ID: %hu\n", uCabID);
    ATLTRACE(" >> cab #: %hu\n", uCabNumber);

    return 0;   // proceed with FDICopy
}

int CMainFrame::Notify_PartialFile (
    LPCTSTR szFilename, LPCTSTR szStartingCabName, LPCTSTR szStartingDiskName )
{
    ATLTRACE("fdi_Notify: fdintPARTIAL_FILE\n");
    ATLTRACE(" >> file name: %s\n", szFilename);
    ATLTRACE(" >> cab name: %s\n", szStartingCabName);
    ATLTRACE(" >> disk name: %s\n", szStartingDiskName);

    m_view.AddPartialFile ( szFilename, szStartingCabName, szStartingDiskName );

    return 0;   // proceed with FDICopy
}

int CMainFrame::Notify_Enumerate (
    long lCurrPos, USHORT cFilesLeft, USHORT uCabID )
{
    ATLTRACE("fdi_Notify: fdintENUMERATE\n");
    ATLTRACE(" >> current pos: %ld\n", lCurrPos);
    ATLTRACE(" >> # files left: %hu\n", cFilesLeft);
    ATLTRACE(" >> cab ID: %hu\n", uCabID);
    
    return 0;   // proceed with FDICopy
}

int CMainFrame::Notify_CopyFile (
    LPCTSTR szFilename, long cbyUncompressedSize, USHORT uDate, USHORT uTime,
    USHORT uAttribs, USHORT uFolderIdx )
{
    ATLTRACE("fdi_Notify: fdintCOPY_FILE\n");
    ATLTRACE(" >> file name: %s\n", szFilename);
    ATLTRACE(" >> uncomp. size: %ld\n", cbyUncompressedSize);
    ATLTRACE(" >> file date: %hu\n", uDate);
    ATLTRACE(" >> file time: %hu\n", uTime);
    ATLTRACE(" >> file attribs: %hu\n", uAttribs);
    ATLTRACE(" >> folder index: %hu\n", uFolderIdx);

    m_view.AddFile ( szFilename, cbyUncompressedSize, uDate, uTime, uAttribs );

    return 0;   // don't extract this file, we just wanted the attributes
}
