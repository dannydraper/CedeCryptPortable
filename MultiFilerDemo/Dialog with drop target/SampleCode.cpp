void MainWindow::OnDropFiles (HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	int iCount = DragQueryFile ((HDROP) wParam, 0xFFFFFFFF, NULL, NULL);
	int i = 0;
	int r = 0;

	char szFilepath[SIZE_STRING];
	char szCurfilepath[SIZE_STRING];

	m_wndenc.ClearFileInfo (); // Clear any existing file info objects from the encryption window

	DynList dlAllfiles;

	m_pdiag->OutputInt ("Number of dropped files: ", iCount);

	for (i=0;i<iCount;i++) {
		ZeroMemory (szFilepath, SIZE_STRING);

		DragQueryFile ((HDROP) wParam, i, szFilepath, SIZE_STRING);

		if (m_Dirscanner.IsFolder (szFilepath) == true) {
			m_pdiag->OutputText ("DropPath: FOLDER: ", szFilepath);

			dlAllfiles.Clear ();
			m_Dirscanner.ListAllFiles (szFilepath, &dlAllfiles);

			// Now list out all the recursively scanned files
			for (r=0;r<dlAllfiles.GetNumItems ();r++) {
				
				ZeroMemory (szCurfilepath, SIZE_STRING);
				strcpy_s (szCurfilepath, SIZE_STRING, (char *) dlAllfiles.GetItem (r));

				m_pdiag->OutputText ("ScannedFile: ", szCurfilepath);
				
				// Add the item to the encryption window list
				m_wndenc.AddFileInfo (szCurfilepath, szFilepath, true);
			}

		} else {
			m_pdiag->OutputText ("DropPath: FILE: ", szFilepath);

			// Add the item to the encryption window list
			m_wndenc.AddFileInfo (szFilepath, false);
		}
	}

	dlAllfiles.Clear ();

	m_wndenc.DoEncryption (m_szCurrentfolder);
	RefreshListView (m_szCurrentfolder);

}


void MainWindow::LV_GetAllSelectedFiles (HWND hwndlv)
{
	SingleFileInfo *pinfo;
	int iNumitems = ListView_GetItemCount (hwndlv);
	int index = 0;
	LVITEM lvi;
	DynList dlAllfiles;
	int r = 0;
	
	char szFilepath[SIZE_STRING];
	char szCurfilepath[SIZE_STRING];

	m_wndenc.ClearFileInfo ();
	//m_pdiag->OutputInt ("ListView Item count is: ", iNumitems);

	for (index=0;index<iNumitems;index++) {
		lvi.mask = LVIF_STATE;
		lvi.stateMask = LVIS_SELECTED;
		lvi.iItem = index;
		lvi.iSubItem = 0;

		ListView_GetItem (hwndlv, &lvi);

		//m_pdiag->OutputText ("Item Text: ", lvi.pszText);
		if (lvi.state == LVIS_SELECTED) {
			pinfo = (SingleFileInfo *) m_dlListviewfiles.GetItem (index);

			//m_pdiag->OutputText ("Selected: ", pinfo->szFilepath);

			if (m_Dirscanner.IsFolder (pinfo->szFilepath) == true) {
				dlAllfiles.Clear ();
				m_Dirscanner.ListAllFiles (pinfo->szFilepath, &dlAllfiles);

				// Now list out all the recursively scanned files
				for (r=0;r<dlAllfiles.GetNumItems ();r++) {
					
					ZeroMemory (szCurfilepath, SIZE_STRING);
					strcpy_s (szCurfilepath, SIZE_STRING, (char *) dlAllfiles.GetItem (r));

					//m_pdiag->OutputText ("ScannedFile: ", szCurfilepath);
					
					// Add the item to the encryption window list
					m_wndenc.AddFileInfo (szCurfilepath, pinfo->szFilepath, true);
				}

			} else {
				m_wndenc.AddFileInfo (pinfo->szFilepath, false);
			}
		}
	}

	dlAllfiles.Clear ();


}