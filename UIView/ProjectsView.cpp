#include "pch.h"
#include "Resource.h"
#include "framework.h"
#include "AutoCleanArray.h"
#include "DialogModes.h"
#include "ProjectsView.h"
#include "ProjectsDialog.h"
#include "ProjectDetails.h"


#define MODAL_OK 1
#define MODAL_CANCEL 2

/////////////////////////////////////////////////////////////////////////////
// CProjectsViews

// Macros
// ----------------
IMPLEMENT_DYNCREATE(CProjectsView, CListView)

BEGIN_MESSAGE_MAP(CProjectsView, CListView)
	ON_NOTIFY_REFLECT(NM_DBLCLK, &CProjectsView::OnListDoubleClick)
	ON_NOTIFY_REFLECT(NM_RCLICK, &CProjectsView::OnNMRClick)
	ON_COMMAND(ID_POPUP_CRUD_UPDATE, &CProjectsView::OnContextEdit)
	ON_COMMAND(ID_POPUP_CRUD_DELETE, &CProjectsView::OnContextDelete)
	ON_COMMAND(ID_POPUP_CRUD_INSERT, &CProjectsView::OnContextInsert)
	ON_COMMAND(ID_POPUP_CRUD_LOAD, &CProjectsView::OnContextLoad)
END_MESSAGE_MAP()

// Constructor / Destructor
// ----------------

CProjectsView::CProjectsView()
{
}

CProjectsView::~CProjectsView()
{
}

// Methods
// ----------------
CProjectsAndTasksDocument* CProjectsView::GetDocument() const
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CProjectsAndTasksDocument)));
	return (CProjectsAndTasksDocument*)m_pDocument;
}

void CProjectsView::OnListDoubleClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	CUsersTypedPtrArray& oUsersArray = GetDocument()->GetUsers();
	CTasksTypedPtrArray& oTasksArray = GetDocument()->GetTasks();
	CProjectsTypedPtrArray& oProjectsArray = GetDocument()->GetProjects();

	int nSelectedIndex = GetListCtrl().GetNextItem(-1, LVNI_SELECTED);
	if (nSelectedIndex == -1)
	{
		return;
	}

	long lID = oProjectsArray[nSelectedIndex]->lId;

	PROJECTS* oRefferedProject = nullptr;
	for (int i = 0; i < oProjectsArray.GetSize(); i++)
	{
		if (lID == oProjectsArray[i]->lId)
		{
			oRefferedProject = oProjectsArray[i];
		}
	}

	CProjectDetails oProjectDetails = CProjectDetails(*oRefferedProject, CTasksTypedPtrArray());

	CProjectsDialog oProjectsDialog(oProjectDetails, Modes::PreviewMode, oUsersArray, oTasksArray);

	INT_PTR nResult = -1;
	nResult = oProjectsDialog.DoModal();

	if (nResult == MODAL_OK)
	{
		GetDocument()->UpdateProjectWithTasks(lID, oProjectDetails);
	}
}

void CProjectsView::OnNMRClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	CPoint oPoint;
	GetCursorPos(&oPoint);

	CMenu oMenu;
	oMenu.LoadMenu(IDR_POPUP_CRUD_VIEW);
	CMenu* pPopup = oMenu.GetSubMenu(0);

	int nSelectedIndex = GetListCtrl().GetNextItem(-1, LVNI_SELECTED);
	if (nSelectedIndex == -1)
	{
		pPopup->EnableMenuItem(ID_POPUP_CRUD_DELETE, MF_GRAYED);
		pPopup->EnableMenuItem(ID_POPUP_CRUD_UPDATE, MF_GRAYED);
	}
	if (pPopup)
	{
		pPopup->TrackPopupMenu(TPM_RIGHTBUTTON, oPoint.x, oPoint.y, this);
	}
	*pResult = 0;
}

void CProjectsView::OnContextInsert()
{
	CUsersTypedPtrArray& oUsersArray = GetDocument()->GetUsers();
	CTasksTypedPtrArray& oTasksArray = GetDocument()->GetTasks();

	CProjectDetails oProjectDetails = CProjectDetails(PROJECTS(), CTasksTypedPtrArray());

	CProjectsDialog oProjectsDialog(oProjectDetails, Modes::InsertMode, oUsersArray, oTasksArray);

	INT_PTR nResult = -1;
	nResult = oProjectsDialog.DoModal();

	if (nResult == MODAL_OK)
	{
		GetDocument()->AddProjectWithTasks(oProjectDetails);
	}
}

void CProjectsView::OnContextEdit()
{
	CUsersTypedPtrArray& oUsersArray = GetDocument()->GetUsers();
	CTasksTypedPtrArray& oTasksArray = GetDocument()->GetTasks();

	int nSelectedIndex = GetListCtrl().GetNextItem(-1, LVNI_SELECTED);
	if (nSelectedIndex == -1)
	{
		AfxMessageBox(_T("No user selected."));
		return;
	}
	CProjectsTypedPtrArray& oProjectsArray = GetDocument()->GetProjects();
	long lID = oProjectsArray[nSelectedIndex]->lId;

	PROJECTS oRefferedProject = PROJECTS();
	for (int i = 0; i < oProjectsArray.GetSize(); i++)
	{
		if (lID == oProjectsArray[i]->lId)
		{
			oRefferedProject = *oProjectsArray[i];
		}
	}
	CTasksTypedPtrArray* oNewArray = new CTasksTypedPtrArray();
	CProjectDetails oProjectDetails = CProjectDetails(oRefferedProject, *oNewArray);

	CProjectsDialog oProjectsDialog(oProjectDetails, Modes::UpdateMode, oUsersArray, oTasksArray);

	INT_PTR nResult = -1;
	nResult = oProjectsDialog.DoModal();

	if (nResult == MODAL_OK)
	{
		GetDocument()->UpdateProjectWithTasks(lID, oProjectDetails);
	}
}

void CProjectsView::OnContextDelete()
{
	int nSelectedIndex = GetListCtrl().GetNextItem(-1, LVNI_SELECTED);
	if (nSelectedIndex == -1)
	{
		AfxMessageBox(_T("No user selected."));
		return;
	}

	CProjectsTypedPtrArray& oProjectsArray = GetDocument()->GetProjects();
	long lID = oProjectsArray[nSelectedIndex]->lId;

	int result = AfxMessageBox(
		_T("Whoa! You sure you want to delete this project and its tasks?"),
		MB_YESNO | MB_ICONASTERISK      // Buttons + icon
	);
	if (result == IDYES)
	{
		PROJECTS* oProject = oProjectsArray[nSelectedIndex];
		CTasksTypedPtrArray& oTasksArray = GetDocument()->GetTasks();
		CTasksTypedPtrArray oProjectTasksArray;
		for (int i = 0; i < oTasksArray.GetSize(); i++)
		{
			if (oTasksArray[i]->lProjectId == lID)
			{
				oProjectTasksArray.Add(oTasksArray[i]);
			}
		}
		CProjectDetails oProjectDetails = CProjectDetails(*oProject, oProjectTasksArray);

		AfxMessageBox(_T("Project deleted"));
		GetDocument()->DeleteProjectWithTasks(lID, oProjectDetails);
	}
}

void CProjectsView::OnContextLoad()
{
	GetListCtrl().DeleteAllItems();

	GetDocument()->LoadAllProjects();
	CProjectsTypedPtrArray& oProjectsArray = GetDocument()->GetProjects();

	for (int i = 0; i < oProjectsArray.GetSize(); i++)
	{
		PROJECTS* pProject = oProjectsArray[i];

		CString strId;
		strId.Format(_T("%d"), pProject->lId);

		CString strUpdateCounter;
		strUpdateCounter.Format(_T("%d"), pProject->lUpdateCounter);

		CString strProjectManagerId;
		strProjectManagerId.Format(_T("%d"), pProject->lProjectManagerId);

		CString strState;
		strState.Format(_T("%d"), pProject->sState);

		CString strTotalEffort;
		strTotalEffort.Format(_T("%d"), pProject->sTotalEffort);

		int nItem = GetListCtrl().InsertItem(i, strId);
		GetListCtrl().SetItemText(nItem, 1, strUpdateCounter);
		GetListCtrl().SetItemText(nItem, 2, pProject->szName);
		GetListCtrl().SetItemText(nItem, 3, pProject->szDescription);
		GetListCtrl().SetItemText(nItem, 4, strProjectManagerId);
		GetListCtrl().SetItemText(nItem, 5, strState);
		GetListCtrl().SetItemText(nItem, 6, strTotalEffort);
	}
}

// Overrides
// ----------------
void CProjectsView::OnInitialUpdate()
{
	GetListCtrl().ModifyStyle(0, LVS_REPORT | LVS_SINGLESEL);
	GetListCtrl().SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	GetListCtrl().InsertColumn(0, _T("ID"), LVCFMT_LEFT, 50);
	GetListCtrl().InsertColumn(1, _T("UPDATE_COUNTER"), LVCFMT_LEFT, 50);
	GetListCtrl().InsertColumn(2, _T("NAME"), LVCFMT_LEFT, 200);
	GetListCtrl().InsertColumn(3, _T("DESCRIPTION"), LVCFMT_LEFT, 200);
	GetListCtrl().InsertColumn(4, _T("PROJECT_MANAGER_ID"), LVCFMT_LEFT, 150);
	GetListCtrl().InsertColumn(5, _T("STATE"), LVCFMT_LEFT, 150);
	GetListCtrl().InsertColumn(6, _T("TOTAL_EFFORT"), LVCFMT_LEFT, 150);



	OnContextLoad();

	CListView::OnInitialUpdate();
}

void CProjectsView::OnUpdate(CView* /*pSender*/, LPARAM lHint, CObject* pHint)
{
	if (pHint == nullptr)
		return;

	Modes lDialogMode = (Modes)lHint;
	PROJECTS* pProject = (PROJECTS*)pHint;
	if (pProject == nullptr)
		return;


	CString strId;
	strId.Format(_T("%d"), pProject->lId);

	CString strUpdateCounter;
	strUpdateCounter.Format(_T("%d"), pProject->lUpdateCounter);

	CString strProjectManagerId;
	strProjectManagerId.Format(_T("%d"), pProject->lProjectManagerId);

	CString strState;
	strState.Format(_T("%d"), pProject->sState);

	CString strTotalEffort;
	strTotalEffort.Format(_T("%d"), pProject->sTotalEffort);

	switch (lDialogMode)
	{
	case Modes::InsertMode:
	{
		int nItem = GetListCtrl().InsertItem(GetListCtrl().GetItemCount(), strId);
		GetListCtrl().SetItemText(nItem, 1, strUpdateCounter);
		GetListCtrl().SetItemText(nItem, 2, pProject->szName);
		GetListCtrl().SetItemText(nItem, 3, pProject->szDescription);
		GetListCtrl().SetItemText(nItem, 4, strProjectManagerId);
		GetListCtrl().SetItemText(nItem, 5, strState);
		GetListCtrl().SetItemText(nItem, 6, strTotalEffort);
		break;
	}
	case Modes::UpdateMode:
	{
		int nSelectedIndex = GetListCtrl().GetNextItem(-1, LVNI_SELECTED);
		if (nSelectedIndex == -1)
		{
			AfxMessageBox(_T("No user selected."));
			return;
		}
		GetListCtrl().SetItemText(nSelectedIndex, 1, strUpdateCounter);
		GetListCtrl().SetItemText(nSelectedIndex, 2, pProject->szName);
		GetListCtrl().SetItemText(nSelectedIndex, 3, pProject->szDescription);
		GetListCtrl().SetItemText(nSelectedIndex, 4, strProjectManagerId);
		GetListCtrl().SetItemText(nSelectedIndex, 5, strState);
		GetListCtrl().SetItemText(nSelectedIndex, 6, strTotalEffort);
		break;
	}
	case Modes::DeleteMode:
	{
		int nSelectedIndex = GetListCtrl().GetNextItem(-1, LVNI_SELECTED);
		if (nSelectedIndex == -1)
		{
			AfxMessageBox(_T("No user selected."));
			return;
		}
		GetListCtrl().DeleteItem(nSelectedIndex);

		break;
	}
	default:
		AfxMessageBox(_T("Wrong mode for the view"));
		break;
	}
}
