#include "pch.h"
#include "UsersView.h"


/////////////////////////////////////////////////////////////////////////////
// CUsersViews

// Macros
// ----------------
IMPLEMENT_DYNCREATE(CUsersView, CListView)

BEGIN_MESSAGE_MAP(CUsersView, CListView)
	ON_NOTIFY_REFLECT(NM_RCLICK, &CUsersView::OnNMRClick)
	ON_COMMAND(ID_POPUP_USERS_UPDATE, &CUsersView::OnContextEdit)
	ON_COMMAND(ID_POPUP_USERS_DELETE, &CUsersView::OnContextDelete)
	ON_COMMAND(ID_POPUP_USERS_INSERT, &CUsersView::OnContextInsert)
	ON_COMMAND(ID_POPUP_USERS_LOAD, &CUsersView::OnContextLoad)
END_MESSAGE_MAP()

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

void CUsersView::OnNMRClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	CPoint oPoint;
	GetCursorPos(&oPoint);

	CMenu oMenu;
	oMenu.LoadMenu(IDR_POPUP_USERS_VIEW);
	CMenu* pPopup = oMenu.GetSubMenu(0);

	if (pPopup)
	{
		pPopup->TrackPopupMenu(TPM_RIGHTBUTTON, oPoint.x, oPoint.y, this);
	}

	*pResult = 0;
}
void CUsersView::OnContextInsert()
{
	int mode = false;
	USERS pUser;

	CUsersDialog oUsersDialog(&pUser, mode);
	oUsersDialog.DoModal();

	GetDocument()->AddUser(pUser);
}
void CUsersView::OnContextEdit()
{
	bool mode = true;
	int nSelectedIndex = GetListCtrl().GetNextItem(-1, LVNI_SELECTED);
	if (nSelectedIndex == -1)
	{
		AfxMessageBox(_T("No user selected."));
		return;
	}	

	CString strID = GetListCtrl().GetItemText(nSelectedIndex - 1, 0);
	int nID = _ttoi(strID);

	CUsersTypedPtrArray& oUsersArray = GetDocument()->GetUsers();
	USERS* pUser = oUsersArray[nID];

	CUsersDialog oUsersDialog(pUser, mode);
	oUsersDialog.DoModal();

	GetDocument()->UpdateUser(nID, *pUser);
}
void CUsersView::OnContextDelete()
{
	int nSelectedIndex = GetListCtrl().GetNextItem(-1, LVNI_SELECTED);
	if (nSelectedIndex == -1)
	{
		AfxMessageBox(_T("No user selected."));
		return;
	}

	CString strID = GetListCtrl().GetItemText(nSelectedIndex - 1, 0);
	int nID = _ttoi(strID);

	GetDocument()->DeleteUser(nID);
}
void CUsersView::OnContextLoad()
{
	CUsersTypedPtrArray& oUsersArray = GetDocument()->GetUsers();
	GetDocument()->LoadAllUsers();
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

	CUsersTypedPtrArray& oUsersArray = GetDocument()->GetUsers();
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
	//Refresh
	//GetListCtrl().DeleteAllItems();
}
