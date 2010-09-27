#include "UIToolbar.h"

UIToolbar::UIToolbar ()
{

}

UIToolbar::~UIToolbar ()
{
	
}

void UIToolbar::SetDiagnostics (Diagnostics *pdiag)
{
	m_pdiag = pdiag;
}

void UIToolbar::CreateToolbar (UIHandler *phandler, HWND hwndParent)
{
	m_hwnd = hwndParent;

	int tby = 28;
	int tbx = 170;


//	#define CID_BTNPARENT					423
//#define CID_BTNDELETE					424
//#define CID_BTNICONVIEW					425
//#define CID_BTNLISTVIEW					426
//#define CID_BTNDETAILVIEW				427
//#define CID_BTNADDFILES					428
//#define CID_BTNEXTRACTFILES				429

	m_btnParent.SetBitmapResources (true, IDB_BTNPARENTNORM, IDB_BTNPARENTHIGH, IDB_BTNPARENTDOWN);
	m_btnParent.SetBitmapProperties (35, 32);
	m_btnParent.SetProperties (hwndParent, CID_BTNPARENT, tbx, tby, 35, 32);
	m_btnParent.SetHighlightProperties (true, false);	
	phandler->AddControl (&m_btnParent);

	tbx+=40;
	m_btnDelete.SetBitmapResources (true, IDB_BTNDELETENORM, IDB_BTNDELETEHIGH, IDB_BTNDELETEDOWN);
	m_btnDelete.SetBitmapProperties (35, 32);
	m_btnDelete.SetProperties (hwndParent, CID_BTNDELETE, tbx, tby, 35, 32);
	m_btnDelete.SetHighlightProperties (true, false);	
	phandler->AddControl (&m_btnDelete);

	tbx+=40;
	m_btnIconview.SetBitmapResources (true, IDB_BTNICONVIEWNORM, IDB_BTNICONVIEWHIGH, IDB_BTNICONVIEWDOWN);
	m_btnIconview.SetBitmapProperties (35, 32);
	m_btnIconview.SetProperties (hwndParent, CID_BTNICONVIEW, tbx, tby, 35, 32);
	m_btnIconview.SetHighlightProperties (true, false);	
	phandler->AddControl (&m_btnIconview);

	tbx+=40;
	m_btnListview.SetBitmapResources (true, IDB_BTNLISTVIEWNORM, IDB_BTNLISTVIEWHIGH, IDB_BTNLISTVIEWDOWN);
	m_btnListview.SetBitmapProperties (35, 32);
	m_btnListview.SetProperties (hwndParent, CID_BTNLISTVIEW, tbx, tby, 35, 32);
	m_btnListview.SetHighlightProperties (true, false);	
	phandler->AddControl (&m_btnListview);

	tbx+=40;
	m_btnDetailview.SetBitmapResources (true, IDB_BTNDETAILVIEWNORM, IDB_BTNDETAILVIEWHIGH, IDB_BTNDETAILVIEWDOWN);
	m_btnDetailview.SetBitmapProperties (35, 32);
	m_btnDetailview.SetProperties (hwndParent, CID_BTNDETAILVIEW, tbx, tby, 35, 32);
	m_btnDetailview.SetHighlightProperties (true, false);	
	phandler->AddControl (&m_btnDetailview);

	tbx+=40;
	m_btnAddfiles.SetBitmapResources (true, IDB_BTNADDNORM, IDB_BTNADDHIGH, IDB_BTNADDDOWN);
	m_btnAddfiles.SetBitmapProperties (35, 32);
	m_btnAddfiles.SetProperties (hwndParent, CID_BTNADDFILES, tbx, tby, 35, 32);
	m_btnAddfiles.SetHighlightProperties (true, false);	
	//phandler->AddControl (&m_btnAddfiles);

	tbx+=40;
	m_btnExtractfiles.SetBitmapResources (true, IDB_BTNEXTRACTNORM, IDB_BTNEXTRACTHIGH, IDB_BTNEXTRACTDOWN);
	m_btnExtractfiles.SetBitmapProperties (35, 32);
	m_btnExtractfiles.SetProperties (hwndParent, CID_BTNEXTRACTFILES, tbx, tby, 35, 32);
	m_btnExtractfiles.SetHighlightProperties (true, false);	
	//phandler->AddControl (&m_btnExtractfiles);
}
