#include "pch.h"
#include "Resource.h"
#include "framework.h"
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
CProjectsDocument* CProjectsView::GetDocument() const
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CProjectsDocument)));
	return (CProjectsDocument*)m_pDocument;
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

	PROJECTS oRefferedProject;
	for (int i = 0; i < oProjectsArray.GetSize(); i++)
	{
		if (lID == oProjectsArray[i]->lId)
		{
			oRefferedProject = *oProjectsArray[i];
		}
	}
	CTasksTypedPtrArray oProjectsTasksArray;
	for (int i = 0; i < oTasksArray.GetSize(); i++)
	{
		if (oTasksArray[i]->lProjectId == oRefferedProject.lId)
		{
			oProjectsTasksArray.Add(oTasksArray[i]);
		}
	}

	CProjectDetails oProjectDetails = CProjectDetails(oRefferedProject, oProjectsTasksArray);

	CProjectsDialog oProjectsDialog(oProjectDetails, Modes::PreviewMode, oUsersArray);

	INT_PTR nResult = -1;
	nResult = oProjectsDialog.DoModal();
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

	CProjectDetails oProjectDetails = CProjectDetails(PROJECTS(), CTasksTypedPtrArray());

	CProjectsDialog oProjectsDialog(oProjectDetails, Modes::InsertMode, oUsersArray);

	INT_PTR nResult = -1;
	nResult = oProjectsDialog.DoModal();
	if (nResult == MODAL_OK)
	{
		GetDocument()->AddProjectWithTasks(oProjectDetails);
	}
}

void CProjectsView::OnContextEdit()
{
	CProjectsTypedPtrArray& oProjectsArray = GetDocument()->GetProjects();
	//CTasksTypedPtrArray& oTasksArray = GetDocument()->GetTasks();
	CUsersTypedPtrArray& oUsersArray = GetDocument()->GetUsers();

	int nSelectedIndex = GetListCtrl().GetNextItem(-1, LVNI_SELECTED);
	if (nSelectedIndex == -1)
	{
		AfxMessageBox(_T("No user selected."));
		return;
	}

	PROJECTS oRefferedProject = *oProjectsArray[nSelectedIndex];
	long lID = oRefferedProject.lId;
	CTasksTypedPtrArray oProjectTasksArray;
	GetDocument()->GetProjectTasks(lID, oProjectTasksArray);
	CProjectDetails oProjectDetails = CProjectDetails(oRefferedProject, oProjectTasksArray);

	CProjectsDialog oProjectsDialog(oProjectDetails, Modes::UpdateMode, oUsersArray);

	INT_PTR nResult = -1;
	nResult = oProjectsDialog.DoModal();

	if (nResult == MODAL_OK)
	{
		GetDocument()->UpdateProjectWithTasks(lID, oProjectDetails);
	}
}

void CProjectsView::OnContextDelete()
{
	CProjectsTypedPtrArray& oProjectsArray = GetDocument()->GetProjects();

	int nSelectedIndex = GetListCtrl().GetNextItem(-1, LVNI_SELECTED);
	if (nSelectedIndex == -1)
	{
		AfxMessageBox(_T("No user selected."));
		return;
	}

	long lID = oProjectsArray[nSelectedIndex]->lId;

	int result = AfxMessageBox(
		_T("Whoa! You sure you want to delete this project and its tasks?"),
		MB_YESNO | MB_ICONASTERISK
	);
	if (result == IDYES)
	{
		PROJECTS* oProject = oProjectsArray[nSelectedIndex];
		CTasksTypedPtrArray oProjectTasksArray;
		GetDocument()->GetProjectTasks(lID, oProjectTasksArray);

		CProjectDetails oProjectDetails = CProjectDetails(*oProject, oProjectTasksArray);

		GetDocument()->DeleteProjectWithTasks(lID, oProjectDetails);
		AfxMessageBox(_T("Project deleted"));
	}
}

void CProjectsView::OnContextLoad()
{
	GetListCtrl().DeleteAllItems();

	GetDocument()->LoadAllProjects();
	CProjectsTypedPtrArray& oProjectsArray = GetDocument()->GetProjects();

	for (int i = 0; i < oProjectsArray.GetSize(); i++)
	{
		PROJECTS* oProject = oProjectsArray[i];

		SetItemToListCtrl(i, *oProject);
	}
}
void CProjectsView::SetItemToListCtrl(int nIndex, const PROJECTS& oProject)
{
	CString strId;
	strId.Format(_T("%d"), oProject.lId);
	CString strUpdateCounter;
	strUpdateCounter.Format(_T("%d"), oProject.lUpdateCounter);
	CString strProjectManagerId;
	strProjectManagerId.Format(_T("%d"), oProject.lProjectManagerId);
	CString strState;
	strState.Format(_T("%d"), oProject.sState);
	CString strTotalEffort;
	strTotalEffort.Format(_T("%d"), oProject.sTotalEffort);

	int nItem = GetListCtrl().InsertItem(nIndex, strId);

	GetListCtrl().SetItemText(nItem, 1, strUpdateCounter);
	GetListCtrl().SetItemText(nItem, 2, oProject.szName);
	GetListCtrl().SetItemText(nItem, 3, oProject.szDescription);
	GetListCtrl().SetItemText(nItem, 4, strProjectManagerId);
	GetListCtrl().SetItemText(nItem, 5, strState);
	GetListCtrl().SetItemText(nItem, 6, strTotalEffort);
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
	Modes lDialogMode = (Modes)lHint;
	if (pHint == nullptr)
		return;
	CProjectDetails* oProjectDetails = (CProjectDetails*)pHint;
	if (oProjectDetails == nullptr)
		return;

	PROJECTS* oProject = new PROJECTS(oProjectDetails->GetProject());

	CString strId;
	strId.Format(_T("%d"), oProject->lId);
	CString strUpdateCounter;
	strUpdateCounter.Format(_T("%d"), oProject->lUpdateCounter);
	CString strProjectManagerId;
	strProjectManagerId.Format(_T("%d"), oProject->lProjectManagerId);
	CString strState;
	strState.Format(_T("%d"), oProject->sState);
	CString strTotalEffort;
	strTotalEffort.Format(_T("%d"), oProject->sTotalEffort);

	switch (lDialogMode)
	{
	case Modes::InsertMode:
	{
		CProjectsTypedPtrArray& oProjectsArray = GetDocument()->GetProjects();
		oProjectsArray.Add(oProject);

		int nItem = GetListCtrl().GetItemCount();
		SetItemToListCtrl(nItem, *oProject);

		break;
	}
	case Modes::UpdateMode:
	{
		CProjectsTypedPtrArray& oProjectsArray = GetDocument()->GetProjects();

		int nSelectedIndex = GetListCtrl().GetNextItem(-1, LVNI_SELECTED);
		if (nSelectedIndex == -1)
		{
			AfxMessageBox(_T("No user selected."));
			return;
		}

		oProjectsArray[nSelectedIndex] = oProject;
		GetListCtrl().DeleteItem(nSelectedIndex);
		SetItemToListCtrl(nSelectedIndex, *oProject);
		break;
	}
	case Modes::DeleteMode:
	{
		CProjectsTypedPtrArray& oProjectsArray = GetDocument()->GetProjects();

		for (int i = 0; i < oProjectsArray.GetSize(); i++)
		{
			if (oProject->lId == oProjectsArray[i]->lId)
			{
				oProjectsArray.RemoveAt(i);
			}
		}
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
