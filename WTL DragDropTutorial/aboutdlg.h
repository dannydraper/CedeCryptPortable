// aboutdlg.h : interface of the CAboutDlg class
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ABOUTDLG_H__C4F299E8_A3C0_45B1_B313_B35269DAA793__INCLUDED_)
#define AFX_ABOUTDLG_H__C4F299E8_A3C0_45B1_B313_B35269DAA793__INCLUDED_

class CAboutDlg : public CDialogImpl<CAboutDlg>
{
public:
    enum { IDD = IDD_ABOUTBOX };

    // Maps
    BEGIN_MSG_MAP(CAboutDlg)
        MSG_WM_INITDIALOG(OnInitDialog)
        COMMAND_ID_HANDLER_EX(IDOK, OnCloseCmd)
        COMMAND_ID_HANDLER_EX(IDCANCEL, OnCloseCmd)
    END_MSG_MAP()

    // Message handlers
    BOOL OnInitDialog ( HWND hwndFocus, LPARAM lParam );
    void OnCloseCmd ( UINT uCode, int nID, HWND hwndCtrl );
};

#endif // !defined(AFX_ABOUTDLG_H__C4F299E8_A3C0_45B1_B313_B35269DAA793__INCLUDED_)
