#include "pch.h"
#include "UsersView.h"
#include "DialogModes.h"
#include "UsersDialog.h"
#include "UsersAutoCleanArray.h"

/////////////////////////////////////////////////////////////////////////////
// CUsersViews

// Macros
// ----------------
IMPLEMENT_DYNCREATE(CUsersView, CListView)

BEGIN_MESSAGE_MAP(CUsersView, CListView)
	ON_NOTIFY_REFLECT(NM_DBLCLK, &CUsersView::OnListDoubleClick)
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
void CUsersView::OnListDoubleClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	int nSelectedIndex = GetListCtrl().GetNextItem(-1, LVNI_SELECTED);
	if (nSelectedIndex == -1)
	{
		return;
	}

	CUsersTypedPtrArray& oUsersArray = GetDocument()->GetUsers();
	long lID = oUsersArray[nSelectedIndex]->lId;

	USERS* oRefferedUser = nullptr;
	for (int i = 0; i < oUsersArray.GetSize(); i++)
	{
		if (lID == oUsersArray[i]->lId)
		{
			oRefferedUser = oUsersArray[i];
		}
	}

	CUsersDialog oUsersDialog(*oRefferedUser, Modes::PreviewMode);

	INT_PTR nResult = -1;
	nResult = oUsersDialog.DoModal();

	if (nResult == 1)
	{
		GetDocument()->UpdateUser(lID, *oRefferedUser);
	}
}
void CUsersView::OnNMRClick(NMHDR* pNMHDR, LRESULT* pResult)
{

	CPoint oPoint;
	GetCursorPos(&oPoint);

	CMenu oMenu;
	oMenu.LoadMenu(IDR_POPUP_USERS_VIEW);
	CMenu* pPopup = oMenu.GetSubMenu(0);

	int nSelectedIndex = GetListCtrl().GetNextItem(-1, LVNI_SELECTED);
	if (nSelectedIndex == -1)
	{
		pPopup->EnableMenuItem(ID_POPUP_USERS_DELETE, MF_GRAYED);
		pPopup->EnableMenuItem(ID_POPUP_USERS_UPDATE, MF_GRAYED);

	}

	if (pPopup)
	{
		pPopup->TrackPopupMenu(TPM_RIGHTBUTTON, oPoint.x, oPoint.y, this);
	}

	
	*pResult = 0;
}
void CUsersView::OnContextInsert()
{
	USERS oUser = USERS();
	CUsersDialog oUsersDialog(oUser, Modes::InsertMode);
	
	INT_PTR nResult = -1;
	nResult = oUsersDialog.DoModal();

	if (nResult == 1)
	{
		GetDocument()->AddUser(oUser);
	}
}
void CUsersView::OnContextEdit()
{
	int nSelectedIndex = GetListCtrl().GetNextItem(-1, LVNI_SELECTED);
	if (nSelectedIndex == -1)
	{
		AfxMessageBox(_T("No user selected."));
		return;
	}	
	CUsersTypedPtrArray& oUsersArray = GetDocument()->GetUsers();
	long lID = oUsersArray[nSelectedIndex]->lId;

	USERS* oRefferedUser = nullptr;
	for (int i = 0; i < oUsersArray.GetSize(); i++)
	{
		if (lID == oUsersArray[i]->lId)
		{
			 oRefferedUser = oUsersArray[i];
		}
	}

	CUsersDialog oUsersDialog(*oRefferedUser, Modes::UpdateMode);

	INT_PTR nResult = -1;
	nResult = oUsersDialog.DoModal();

	if (nResult == 1)
	{
		GetDocument()->UpdateUser(lID, *oRefferedUser);
	}
}
void CUsersView::OnContextDelete()
{
	int nSelectedIndex = GetListCtrl().GetNextItem(-1, LVNI_SELECTED);
	if (nSelectedIndex == -1)
	{
		AfxMessageBox(_T("No user selected."));
		return;
	}

	CUsersTypedPtrArray& oUsersArray = GetDocument()->GetUsers();
	long lID = oUsersArray[nSelectedIndex]->lId;

	USERS* oRefferedUser = nullptr;
	for (int i = 0; i < oUsersArray.GetSize(); i++)
	{
		if (lID == oUsersArray[i]->lId)
		{
			oRefferedUser = oUsersArray[i];
		}
	}

	GetDocument()->DeleteUser(lID);
}
void CUsersView::OnContextLoad()
{
	GetDocument()->LoadAllUsers();
	CUsersTypedPtrArray& oUsersArray = GetDocument()->GetUsers();

	GetListCtrl().DeleteAllItems();

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
	GetListCtrl().ModifyStyle(0, LVS_REPORT | LVS_SINGLESEL);
	GetListCtrl().SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

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
	CListView::OnInitialUpdate();
}


void CUsersView::OnUpdate(CView* /*pSender*/, LPARAM lHint, CObject* pHint)
{
	OnContextLoad();
}
