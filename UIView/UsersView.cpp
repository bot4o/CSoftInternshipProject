#include "pch.h"
#include "UsersView.h"


/////////////////////////////////////////////////////////////////////////////
// CUsersViews

// Macros
// ----------------

IMPLEMENT_DYNCREATE(CUsersView, CListView)

// Constructor / Destructor
// ----------------

CUsersView::CUsersView()
{
}

CUsersView::~CUsersView()
{
}


// Methods
// ----------------


// Overrides
// ----------------
void CUsersView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();
	//GetUsers();
	//TODO GetListCtrl().methods
	GetListCtrl().ModifyStyle(0, LVS_REPORT | LVS_SINGLESEL);
	// LVS_REPORT - Setup row list-like items
	// LVS_SINGLESEL - Disable selection of multiple rows

	GetListCtrl().SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	// LVS_EX_FULLROWSELECT - TODO проверете сами
	// LVS_EX_GRIDLINES - TODO проверете сами
	// ... your source code
}
void CUsersView::OnUpdate(CView* /*pSender*/, LPARAM lHint, CObject* pHint)
{
}