// WTLCabViewView.cpp : implementation of the CWTLCabViewView class
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "WTLCabViewView.h"

/////////////////////////////////////////////////////////////////////////////
// Construction

CWTLCabViewView::CWTLCabViewView() : m_nSortedCol(-1), m_bSortAscending(true)
{
}


/////////////////////////////////////////////////////////////////////////////
// Message handlers

BOOL CWTLCabViewView::PreTranslateMessage ( MSG* pMsg )
{
    return FALSE;
}

void CWTLCabViewView::OnDestroy()
{
    // Destroy the state image list. The other image lists shouldn't be destroyed
    // since they are copies of the system image list. Destroying those on Win 9x
    // is a Bad Thing to do.
    if ( m_imlState )
        m_imlState.Destroy();

    SetMsgHandled(false);
}


/////////////////////////////////////////////////////////////////////////////
// Notify handlers

LRESULT CWTLCabViewView::OnColumnClick ( NMHDR* phdr )
{
CWaitCursor w;
int nCol = ((NMLISTVIEW*) phdr)->iSubItem;

    // If the user clicked the column that is already sorted, reverse the sort
    // direction. Otherwise, go back to ascending order.
    if ( nCol == m_nSortedCol )
        m_bSortAscending = !m_bSortAscending;
    else
        m_bSortAscending = true;

    if ( g_bXPOrLater )
        {
        HDITEM hdi = { HDI_FORMAT };
        CHeaderCtrl wndHdr = GetHeader();

        // Remove the sort arrow indicator from the previously-sorted column.
        if ( -1 != m_nSortedCol )
            {
            wndHdr.GetItem ( m_nSortedCol, &hdi );
            hdi.fmt &= ~(HDF_SORTDOWN | HDF_SORTUP);
            wndHdr.SetItem ( m_nSortedCol, &hdi );
            }

        // Add the sort arrow to the new sorted column.
        hdi.mask = HDI_FORMAT;
        wndHdr.GetItem ( nCol, &hdi );
        hdi.fmt |= m_bSortAscending ? HDF_SORTUP : HDF_SORTDOWN;
        wndHdr.SetItem ( nCol, &hdi );
        }

    // Store the column being sorted, and do the sort
    m_nSortedCol = nCol;

    SortItems ( SortCallback, (LPARAM)(DWORD_PTR) this );

    // Have the list ctrl indicate the sorted column by changing its color.
    if ( g_bXPOrLater )
        SetSelectedColumn ( nCol );

    return 0;   // retval ignored
}

LRESULT CWTLCabViewView::OnMarqueeBegin ( NMHDR* phdr )
{
    // Don't allow the marquee if the list is empty.
    return (GetItemCount() == 0);
}

LRESULT CWTLCabViewView::OnKeyDown ( NMHDR* phdr )
{
NMLVKEYDOWN* pnmlv = (NMLVKEYDOWN*) phdr;

    // Select all items if the user presses ^A
    if ( 'A' == pnmlv->wVKey && (GetKeyState(VK_CONTROL) & 0x8000) )
        SetItemState ( -1, LVIS_SELECTED, LVIS_SELECTED );

    return 0;   // retval ignored
}

LRESULT CWTLCabViewView::OnDeleteItem ( NMHDR* phdr )
{
NMLISTVIEW* pnmlv = (NMLISTVIEW*) phdr;

    if ( 0 != pnmlv->lParam )
        delete (CCompressedFileInfo*) pnmlv->lParam;

    return 0;   // retval ignored
}


/////////////////////////////////////////////////////////////////////////////
// Operations

void CWTLCabViewView::Init()
{
    InitColumns();
    InitImageLists();
    SetExtendedListViewStyle ( LVS_EX_HEADERDRAGDROP, LVS_EX_HEADERDRAGDROP );

    // On XP, set some additional properties of the list ctrl.
    if ( g_bXPOrLater )
        {
        // Turning on LVS_EX_DOUBLEBUFFER also enables the transparent
        // selection marquee.
        SetExtendedListViewStyle ( LVS_EX_DOUBLEBUFFER, LVS_EX_DOUBLEBUFFER );

        // Each tile will have 2 additional lines (3 lines total).
        LVTILEVIEWINFO lvtvi = { sizeof(LVTILEVIEWINFO), LVTVIM_COLUMNS };

        lvtvi.cLines = 2;
        lvtvi.dwFlags = LVTVIF_AUTOSIZE;
        SetTileViewInfo ( &lvtvi );
        }
}

void CWTLCabViewView::Clear()
{
    DeleteAllItems();

    if ( -1 != m_nSortedCol )
        {
        if ( g_bXPOrLater )
            {
            // Remove the sort arrow indicator from the sorted column.
            HDITEM hdi = { HDI_FORMAT };
            CHeaderCtrl wndHdr = GetHeader();

            wndHdr.GetItem ( m_nSortedCol, &hdi );
            hdi.fmt &= ~(HDF_SORTDOWN | HDF_SORTUP);
            wndHdr.SetItem ( m_nSortedCol, &hdi );

            // Remove the sorted column color from the list.
            SetSelectedColumn(-1);
            }

        m_nSortedCol = -1;
        m_bSortAscending = true;
        }
}

void CWTLCabViewView::SetViewMode ( int nMode )
{
    ATLASSERT(nMode >= LV_VIEW_ICON && nMode <= LV_VIEW_TILE);

    if ( g_bXPOrLater )
        {
        if ( LV_VIEW_TILE == nMode )
            SetImageList ( m_imlTiles, LVSIL_NORMAL );
        else
            SetImageList ( m_imlLarge, LVSIL_NORMAL );

        SetView ( nMode );
        }
    else
        {
        DWORD dwViewStyle;

        ATLASSERT(LV_VIEW_TILE != nMode);

        switch ( nMode )
            {
            case LV_VIEW_ICON:      dwViewStyle = LVS_ICON; break;
            case LV_VIEW_SMALLICON: dwViewStyle = LVS_SMALLICON; break;
            case LV_VIEW_LIST:      dwViewStyle = LVS_LIST; break;
            case LV_VIEW_DETAILS:   dwViewStyle = LVS_REPORT; break;
            DEFAULT_UNREACHABLE;
            }

        ModifyStyle ( LVS_TYPEMASK, dwViewStyle );
        }
}

void CWTLCabViewView::AddFile (
    LPCTSTR szFilename, long cbyUncompressedSize, USHORT uDate,
    USHORT uTime, USHORT uAttribs )
{
int nIdx;
TCHAR szSize[64], szDate[64], szTime[64];
CString sDateTime, sAttrs;
FILETIME ft = {0}, ftLocal = {0};
SYSTEMTIME st = {0};

    // Get the file size with the size in KB or MB or whatever suffix.
    StrFormatByteSize ( cbyUncompressedSize, szSize, countof(szSize) );

    // Format the modified date/time using the user's locale settings.
    DosDateTimeToFileTime ( uDate, uTime, &ft );
    FileTimeToLocalFileTime ( &ft, &ftLocal );
    FileTimeToSystemTime ( &ftLocal, &st );

    GetDateFormat ( LOCALE_USER_DEFAULT, DATE_SHORTDATE, &st, NULL, szDate, countof(szDate) );
    GetTimeFormat ( LOCALE_USER_DEFAULT, TIME_NOSECONDS, &st, NULL, szTime, countof(szTime) );

    sDateTime.Format ( _T("%s %s"), szDate, szTime );

    // Make a string that shows the attributes.
    if ( uAttribs & _A_RDONLY )
        sAttrs += 'R';
    if ( uAttribs & _A_HIDDEN )
        sAttrs += 'H';
    if ( uAttribs & _A_SYSTEM )
        sAttrs += 'S';
    if ( uAttribs & _A_ARCH )
        sAttrs += 'A';

    // Get the file's type description and the index of its icon in the system imagelist.
SHFILEINFO info = {0};

    SHGetFileInfo ( szFilename, FILE_ATTRIBUTE_NORMAL, &info, sizeof(info),
                    SHGFI_USEFILEATTRIBUTES|SHGFI_TYPENAME|SHGFI_SYSICONINDEX );

    // Add a new list item for this file.
    nIdx = InsertItem ( GetItemCount(), szFilename, info.iIcon );
    SetItemText ( nIdx, 1, info.szTypeName );
    SetItemText ( nIdx, 2, szSize );
    SetItemText ( nIdx, 3, sDateTime );
    SetItemText ( nIdx, 4, sAttrs );

CCompressedFileInfo* pInfo = new CCompressedFileInfo;

    pInfo->sFilename = szFilename;
    pInfo->sFileType = info.szTypeName;
    pInfo->dwFileSize = cbyUncompressedSize;
    pInfo->ftDateTime = ft;
    pInfo->uAttribs = uAttribs;

    SetItemData ( nIdx, (DWORD_PTR) pInfo );

    // On XP+, set up the additional tile view text for the item.
    if ( g_bXPOrLater )
        {
        UINT aCols[] = { 1, 2 };
        LVTILEINFO lvti = { sizeof(LVTILEINFO), nIdx, countof(aCols), aCols };

        SetTileInfo ( &lvti );
        }
}

void CWTLCabViewView::AddPartialFile (
    LPCTSTR szFilename, LPCTSTR szStartingCabName, LPCTSTR szStartingDiskName )
{
int nIdx;
SHFILEINFO info = {0};
CString sSplitFileText;

    sSplitFileText.Format ( IDS_FROM_PREV_CAB, szStartingCabName );

    // Get the file's type description and the index of its icon in the system imagelist.
    SHGetFileInfo ( szFilename, FILE_ATTRIBUTE_NORMAL, &info, sizeof(info),
                    SHGFI_USEFILEATTRIBUTES|SHGFI_TYPENAME|SHGFI_SYSICONINDEX );

    // Add a new list item for this file.
    nIdx = InsertItem ( GetItemCount(), szFilename, info.iIcon );
    SetItemText ( nIdx, 1, info.szTypeName );
    SetItemText ( nIdx, 5, sSplitFileText );

CCompressedFileInfo* pInfo = new CCompressedFileInfo;

    pInfo->sFilename = szFilename;
    pInfo->sFileType = info.szTypeName;
    pInfo->location = CCompressedFileInfo::from_prev_cab;
    pInfo->sOtherCabName = szStartingCabName;
    pInfo->bExtractable = false;

    SetItemData ( nIdx, (DWORD_PTR) pInfo );

    // Set the stage image to the icon that shows the file begins in
    // an earlier CAB.
    SetItemState ( nIdx, INDEXTOSTATEIMAGEMASK(1), LVIS_STATEIMAGEMASK );

    // On XP+, set up the additional tile view text for the item.
    if ( g_bXPOrLater )
        {
        UINT aCols[] = { 5 };
        LVTILEINFO lvti = { sizeof(LVTILEINFO), nIdx, countof(aCols), aCols };

        SetTileInfo ( &lvti );
        }
}

void CWTLCabViewView::UpdateContinuedFile ( const CDraggedFileInfo& info )
{
CString sSplitFileText;

    // Update the Split File column to show what CAB this file is continued in.
    sSplitFileText.Format ( IDS_TO_NEXT_CAB, (LPCTSTR) info.sCabName );

    SetItemText ( info.nListIdx, 5, sSplitFileText );

    // Update the item's data to keep track of the fact that the file is
    // continued in another CAB.
CCompressedFileInfo* pInfo;

    pInfo = (CCompressedFileInfo*) GetItemData ( info.nListIdx );
    ATLASSERT(NULL != pInfo);

    pInfo->location = CCompressedFileInfo::to_next_cab;
    pInfo->sOtherCabName = info.sCabName;

    // Set the stage image to the icon that shows the file continues in another CAB.
    SetItemState ( info.nListIdx, INDEXTOSTATEIMAGEMASK(2), LVIS_STATEIMAGEMASK );

    if ( info.bCabMissing )
        {
        // Set a flag indicating that the file can't be extracted, and set the
        // LVIS_CUT style so the icon is grayed out.
        pInfo->bExtractable = false;
        SetItemState ( info.nListIdx, LVIS_CUT, LVIS_CUT );
        }
}

bool CWTLCabViewView::GetDraggedFileInfo ( std::vector<CDraggedFileInfo>& vec )
{
int nIdx = -1;

    while ( (nIdx = GetNextItem ( nIdx, LVNI_SELECTED )) != -1 )
        {
        CCompressedFileInfo* pInfo;

        pInfo = (CCompressedFileInfo*) GetItemData ( nIdx );
        ATLASSERT(NULL != pInfo);

        if ( pInfo->bExtractable )
            vec.push_back ( CDraggedFileInfo ( pInfo->sFilename, nIdx ) );
        else
            ATLTRACE("Skipping partial/unavailable file <%s>\n", (LPCTSTR) pInfo->sFilename);
        }

    return !vec.empty();
}


/////////////////////////////////////////////////////////////////////////////
// Other methods

void CWTLCabViewView::InitColumns()
{
    InsertColumn ( 0, _S(IDS_COLHDR_NAME), LVCFMT_LEFT, 100, 0 );
    InsertColumn ( 1, _S(IDS_COLHDR_TYPE), LVCFMT_LEFT, 100, 1 );
    InsertColumn ( 2, _S(IDS_COLHDR_SIZE), LVCFMT_LEFT, 150, 2 );
    InsertColumn ( 3, _S(IDS_COLHDR_DATE), LVCFMT_LEFT, 100, 3 );
    InsertColumn ( 4, _S(IDS_COLHDR_ATTRS), LVCFMT_LEFT, 100, 4 );
    InsertColumn ( 5, _S(IDS_COLHDR_SPLIT), LVCFMT_LEFT, 100, 5 );
}

void CWTLCabViewView::InitImageLists()
{
SHFILEINFO info = {0};

    // Get the large/small system image lists
    m_imlSmall = (HIMAGELIST) SHGetFileInfo ( _T("alyson.com"), FILE_ATTRIBUTE_NORMAL,
                                &info, sizeof(info),
                                SHGFI_USEFILEATTRIBUTES|SHGFI_ICON|SHGFI_SMALLICON|SHGFI_SYSICONINDEX );

    m_imlLarge = (HIMAGELIST) SHGetFileInfo ( _T("willow.com"), FILE_ATTRIBUTE_NORMAL,
                                &info, sizeof(info),
                                SHGFI_USEFILEATTRIBUTES|SHGFI_ICON|SHGFI_SYSICONINDEX );

    SetImageList ( m_imlLarge, LVSIL_NORMAL );
    SetImageList ( m_imlSmall, LVSIL_SMALL );

    // Create a state image list
HICON hiPrev = AtlLoadIconImage ( IDI_PREV_CAB, LR_DEFAULTCOLOR, 16, 16 );
HICON hiNext = AtlLoadIconImage ( IDI_NEXT_CAB, LR_DEFAULTCOLOR, 16, 16 );

    m_imlState.Create ( 16, 16, ILC_COLOR8|ILC_MASK, 2, 1 );

    m_imlState.AddIcon ( hiPrev );
    m_imlState.AddIcon ( hiNext );

    DestroyIcon ( hiPrev );
    DestroyIcon ( hiNext );

    SetImageList ( m_imlState, LVSIL_STATE );

    // On pre-XP, we're done.
    if ( !g_bXPOrLater )
        return;

    // Get the 48x48 system image list. 
HRESULT hr, (WINAPI* pfnGetImageList)(int, REFIID, void**) = NULL;
HMODULE hmod = GetModuleHandle ( _T("shell32") );

    if ( NULL != hmod )
        (FARPROC&) pfnGetImageList = GetProcAddress ( hmod, "SHGetImageList" );

    if ( NULL == pfnGetImageList )
        (FARPROC&) pfnGetImageList = GetProcAddress ( hmod, MAKEINTRESOURCEA(727) );  // see Q316931

    if ( NULL == pfnGetImageList )
        {
        // Couldn't get the address of SHGetImageList(), so just use the 32x32 imglist.
        m_imlTiles = m_imlLarge;
        return;
        }

    hr = pfnGetImageList ( SHIL_EXTRALARGE, IID_IImageList, (void**) &m_TileIml );

    if ( FAILED(hr) )
        {
        // Couldn't get the 48x48 image list, so fall back to using the 32x32 one.
        m_imlTiles = m_imlLarge;
        return;
        }

    // HIMAGELIST and IImageList* are interchangeable, so this cast is kosher.
    m_imlTiles = (HIMAGELIST)(IImageList*) m_TileIml;
}

int CALLBACK CWTLCabViewView::SortCallback (
    LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort )
{
    ATLASSERT(NULL != lParam1);
    ATLASSERT(NULL != lParam2);
    ATLASSERT(NULL != lParamSort);

CCompressedFileInfo& info1 = *(CCompressedFileInfo*) lParam1;
CCompressedFileInfo& info2 = *(CCompressedFileInfo*) lParam2;
CWTLCabViewView* pThis = (CWTLCabViewView*) lParamSort;

    return pThis->SortCallback ( info1, info2 );
}

int CWTLCabViewView::SortCallback (
    const CCompressedFileInfo& info1, const CCompressedFileInfo& info2 ) const
{
int nRet = 0;
LPCTSTR sz1, sz2;

    ATLASSERT(-1 != m_nSortedCol);
    ATLASSERT(!info1.sFilename.IsEmpty());
    ATLASSERT(!info2.sFilename.IsEmpty());

    // Default to comparing the file names, unless something in the switch
    // changes these pointers.
    sz1 = info1.sFilename;
    sz2 = info2.sFilename;

    switch ( m_nSortedCol )
        {
        case 0:     // file name
        break;

        case 1:     // type description
            ATLASSERT(!info1.sFileType.IsEmpty());
            ATLASSERT(!info2.sFileType.IsEmpty());

            sz1 = info1.sFileType;
            sz2 = info2.sFileType;
        break;

        case 2:     // uncompressed size
            if ( info1.dwFileSize < info2.dwFileSize )
                nRet = -1;
            else if ( info1.dwFileSize > info2.dwFileSize )
                nRet = 1;
        break;

        case 3:     // modified date/time
            {
            LONG lRet = CompareFileTime ( &info1.ftDateTime, &info2.ftDateTime );

            if ( 0 != lRet )
                nRet = lRet;
            }
        break;

        case 4:     // file attributes
            if ( info1.uAttribs < info2.uAttribs )
                nRet = -1;
            else if ( info1.uAttribs > info2.uAttribs )
                nRet = 1;
        break;

        case 5:     // prev/next CAB name
            if ( info1.location < info2.location )
                nRet = -1;
            else if ( info1.location > info2.location )
                nRet = 1;
            else if ( info1.location != CCompressedFileInfo::in_this_cab )
                {
                ATLASSERT(!info1.sOtherCabName.IsEmpty());
                ATLASSERT(!info2.sOtherCabName.IsEmpty());

                nRet = lstrcmpi ( info1.sOtherCabName, info2.sOtherCabName );
                }
        break;

        DEFAULT_UNREACHABLE;
        }

    // If the primary comparison in the switch returned equality (nRet is still 0),
    // then compare sz1/sz2 so equal items will still be sorted by file names.
    if ( 0 == nRet )
        nRet = lstrcmpi ( sz1, sz2 );

    return m_bSortAscending ? nRet : -nRet;
}
