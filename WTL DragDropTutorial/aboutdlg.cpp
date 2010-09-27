// aboutdlg.cpp : implementation of the CAboutDlg class
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "aboutdlg.h"

BOOL CAboutDlg::OnInitDialog ( HWND hwndFocus, LPARAM lParam )
{
    CenterWindow(GetParent());
    return TRUE;
}

void CAboutDlg::OnCloseCmd ( UINT uCode, int nID, HWND hwndCtrl )
{
    EndDialog ( nID );
}
