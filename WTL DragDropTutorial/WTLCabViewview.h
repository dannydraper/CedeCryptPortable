// WTLCabViewView.h : interface of the CWTLCabViewView class
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_WTLCABVIEWVIEW_H__9300C31D_E4FF_4E4B_A98D_670F63106D35__INCLUDED_)
#define AFX_WTLCABVIEWVIEW_H__9300C31D_E4FF_4E4B_A98D_670F63106D35__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define VIEW_STYLES (LVS_REPORT|LVS_SHOWSELALWAYS|LVS_SHAREIMAGELISTS|LVS_AUTOARRANGE)
#define VIEW_EX_STYLES (WS_EX_CLIENTEDGE)

class CWTLCabViewView : public CWindowImpl<CWTLCabViewView, CListViewCtrl,
                                             CWinTraitsOR<VIEW_STYLES,VIEW_EX_STYLES> >
{
public:
    DECLARE_WND_SUPERCLASS(NULL, CListViewCtrl::GetWndClassName())

    // Construction
    CWTLCabViewView();

    // Maps
    BEGIN_MSG_MAP(CWTLCabViewView)
        MSG_WM_DESTROY(OnDestroy)
        REFLECTED_NOTIFY_CODE_HANDLER_EX(LVN_COLUMNCLICK, OnColumnClick)
        REFLECTED_NOTIFY_CODE_HANDLER_EX(LVN_MARQUEEBEGIN, OnMarqueeBegin)
        REFLECTED_NOTIFY_CODE_HANDLER_EX(LVN_KEYDOWN, OnKeyDown)
        REFLECTED_NOTIFY_CODE_HANDLER_EX(LVN_DELETEITEM, OnDeleteItem)
        DEFAULT_REFLECTION_HANDLER()
    END_MSG_MAP()

    // Message handlers
    BOOL PreTranslateMessage ( MSG* pMsg );

    void OnDestroy();

    // Notify handlers
    LRESULT OnColumnClick ( NMHDR* phdr );
    LRESULT OnMarqueeBegin ( NMHDR* phdr );
    LRESULT OnKeyDown ( NMHDR* phdr );
    LRESULT OnDeleteItem ( NMHDR* phdr );

    // Operations
    void Init();
    void Clear();
    void SetViewMode ( int nMode );
    void AddFile ( LPCTSTR szFilename, long cbyUncompressedSize, USHORT uDate,
                   USHORT uTime, USHORT uAttribs );
    void AddPartialFile ( LPCTSTR szFilename, LPCTSTR szStartingCabName,
                          LPCTSTR szStartingDiskName );
    void UpdateContinuedFile ( const CDraggedFileInfo& info );
    bool GetDraggedFileInfo ( std::vector<CDraggedFileInfo>& vec );

protected:
    CImageList m_imlSmall, m_imlLarge, m_imlTiles, m_imlState;
    CComPtr<IImageList> m_TileIml;

    int m_nSortedCol;   // -1 if list hasn't been sorted yet
    bool m_bSortAscending;

    void InitImageLists();
    void InitColumns();

    static int CALLBACK SortCallback ( LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort );
    int SortCallback ( const CCompressedFileInfo& info1, const CCompressedFileInfo& info2 ) const;
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WTLCABVIEWVIEW_H__9300C31D_E4FF_4E4B_A98D_670F63106D35__INCLUDED_)
