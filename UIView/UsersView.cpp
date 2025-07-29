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
CUsersDocument* CUsersView::GetDocument() const
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CUsersDocument)));
	return (CUsersDocument*)m_pDocument;
}

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
	// LVS_EX_FULLROWSELECT - When a row is selected, the entire row is highlighted, not just the first column. 
	// LVS_EX_GRIDLINES - Draws horizontal and vertical grid lines between items and columns, similar to a spreadsheet (like Excel).
	
	// ... your source code
	GetListCtrl().InsertColumn(0, _T("ID"), LVCFMT_LEFT, 50);
	GetListCtrl().InsertColumn(1, _T("UPDATE_COUNTER"), LVCFMT_LEFT, 50);
	GetListCtrl().InsertColumn(2, _T("NAME"), LVCFMT_LEFT, 200);
	GetListCtrl().InsertColumn(3, _T("EMAIL"), LVCFMT_LEFT,200);
	GetListCtrl().InsertColumn(4, _T("JOB_TITLE"), LVCFMT_LEFT,150);

	CUsersTypedPtrArray& oUsersArray = GetDocument()->m_oUsersArray;
	for (int i = 0; i < oUsersArray.GetSize(); i++)
	{
		USERS* pUser = oUsersArray[i];

		CString strId;
		strId.Format(_T("%d"), pUser->lId);

		CString strUpdateCounter;
		strUpdateCounter.Format(_T("%d"), pUser->lUpdateCounter);
		
		int nItem = GetListCtrl().InsertItem(i, strId);
		GetListCtrl().SetItemText(nItem, 1, strUpdateCounter);
		GetListCtrl().SetItemText(nItem, 2, pUser->szName);
		GetListCtrl().SetItemText(nItem, 3, pUser->szEmail);
		GetListCtrl().SetItemText(nItem, 4, pUser->szJobTitle);
	}
}
void CUsersView::OnUpdate(CView* /*pSender*/, LPARAM lHint, CObject* pHint)
{
	//GetListCtrl().DeleteAllItems();
}