#include "pch.h"
#include "afxdialogex.h"
#include "ProjectsDialog.h"
#include "TasksDialog.h"

#include "../UIView/ProjectsView.h"

/////////////////////////////////////////////////////////////////////////////
// CProjectsDialog

// Macros
// ----------------
#define MODAL_OK 1
#define MODAL_CANCEL 2
#define MANAGER_TITLE _T("Ръководител")

IMPLEMENT_DYNAMIC(CProjectsDialog, CDialogEx)

BEGIN_MESSAGE_MAP(CProjectsDialog, CDialogEx)
//	ON_BN_CLICKED(IDC_BTN_ADD_TASK, &CProjectsDialog::OnBnClickedBtnAddTask)
//	ON_BN_CLICKED(IDC_BTN_DELETE_TASK, &CProjectsDialog::OnBnClickedBtnAddTask2)
//	ON_BN_CLICKED(IDC_BTN_EDIT_TASK, &CProjectsDialog::OnBnClickedBtnAddTask3)

	ON_LBN_SELCHANGE(IDC_LIST1, &CProjectsDialog::OnLbnSelchangeList1)
	ON_BN_CLICKED(IDC_BTN_ADD_TASK, &CProjectsDialog::OnBnClickedBtnAddTask)
	ON_BN_CLICKED(IDC_BTN_DELETE_TASK, &CProjectsDialog::OnBnClickedBtnDeleteTask)
	ON_BN_CLICKED(IDC_BTN_EDIT_TASK, &CProjectsDialog::OnBnClickedBtnEditTask)
END_MESSAGE_MAP()

// Constructor / Destructor
// ----------------
CProjectsDialog::CProjectsDialog(CProjectDetails& oProjectDetails, Modes oActionMode, CUsersTypedPtrArray& oUsersArray,
	CTasksTypedPtrArray& oTasksArray, CWnd* pParent)
	: CDialogEx(IDD_PROJECTS_DIALOG, pParent), m_oProject(oProjectDetails.GetProject()), m_oProjectTasksArray(oProjectDetails.GetTasks()), m_oActionMode(oActionMode), m_oUsersArray(oUsersArray), m_oTasksArray(oTasksArray)
{

}

CProjectsDialog::~CProjectsDialog()
{

}

// Methods
// ----------------
void CProjectsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDB_NAME, m_edbName);
	DDX_Control(pDX, IDC_EDB_DESC, m_edbDescription);
	DDX_Control(pDX, IDC_CMB_PROJECT_MANAGER, m_cmbProjectManager);
	DDX_Control(pDX, IDC_STT_STATE_RESULT, m_sttState);
	DDX_Control(pDX, IDC_STT_TOTAL_EFFORT_RESULT, m_sttTotalEffort);
	DDX_Control(pDX, IDC_LIST1, m_lsbTasks);
	DDX_Control(pDX, IDC_BTN_EDIT_TASK, m_btnEdit);
	DDX_Control(pDX, IDC_BTN_DELETE_TASK, m_btnDelete);
	DDX_Control(pDX, IDC_BTN_ADD_TASK, m_btnAdd);
}

bool CProjectsDialog::ValidateDialog(const PROJECTS& oValidateProject)
{
	if (_tcslen(oValidateProject.szName) == 0 || _tcslen(oValidateProject.szDescription) == 0 || oValidateProject.lProjectManagerId == -1)
	{
		AfxMessageBox(_T("Please fill each field"));
		return false;
	}
	if (m_oProject.lId == oValidateProject.lId &&
		_tcscmp(m_oProject.szName, oValidateProject.szName) == 0 &&
		_tcscmp(m_oProject.szDescription, oValidateProject.szDescription) == 0 &&
		m_oProject.lProjectManagerId == oValidateProject.lProjectManagerId &&
		m_oProject.sState == oValidateProject.sState &&
		m_oProject.sTotalEffort == oValidateProject.sTotalEffort)
	{
		AfxMessageBox(_T("No changes were made"));
		return false;
	}
	return true;
}

void CProjectsDialog::SetDialogElementsText()
{
	//Name:
	m_edbName.SetWindowTextW(m_oProject.szName);
	//Description:
	m_edbDescription.SetWindowTextW(m_oProject.szDescription);
	//Manager:
	for (int i = 0; i < m_oUsersArray.GetSize(); i++)
	{
		USERS* oCurUser = m_oUsersArray[i];
		CString strManagerTitle = MANAGER_TITLE;
		if (oCurUser->szJobTitle == strManagerTitle)
		{
			CString strProjectManager = oCurUser->szName;
			int nIndex = m_cmbProjectManager.AddString(strProjectManager);
			m_cmbProjectManager.SetItemData(nIndex, (DWORD_PTR)oCurUser->lId);
			if (m_cmbProjectManager.GetItemData(i) == m_oProject.lProjectManagerId)
			{
				m_cmbProjectManager.SetCurSel(i);
			}
		}
	}
	//State; Total Effort; Tasks.
	short sState = (short)ProjectStates::None;
	CString stateStr = _T("None");
	m_sttState.SetWindowTextW(stateStr);
	if (m_oActionMode != Modes::InsertMode)
	{
		SetTaskInfo();
	}

	return;
}
void CProjectsDialog::InsertTaskToProject(TASKS& oTask)
{
	m_oProjectTasksArray.Add(&oTask);

	short sTotalEffort = 0;
	int nIndex = m_lsbTasks.AddString(oTask.szName);
	m_lsbTasks.SetItemData(nIndex, (DWORD_PTR)oTask.lId); //insert: id = 0

	//project State check
	short sState = (short)ProjectStates::Finished;
	bool bHasNonFinishedTask = false;
	for (int i = 0; i < m_oProjectTasksArray.GetSize(); i++)
	{
		sTotalEffort += m_oProjectTasksArray[i]->sEffort;
		if (m_oProjectTasksArray[i]->sState != (short)TaskStates::Finished)
		{
			bHasNonFinishedTask = true;
		}
	}
	if (bHasNonFinishedTask)
	{
		sState = (short)ProjectStates::Active;
	}

	CString stateStr, effortStr;
	switch (sState)
	{
	case (short)ProjectStates::Active:
		stateStr = _T("Active");
		break;
	case (short)ProjectStates::Finished:
		stateStr = _T("Finished");
		break;
	default:
		stateStr = _T("None");
		break;
	}

	effortStr.Format(_T("%d"), sTotalEffort);

	m_sttState.SetWindowTextW(stateStr);
	m_sttTotalEffort.SetWindowTextW(effortStr);
}
void CProjectsDialog::DeleteTaskFromProject(const int selIndex)
{
	m_oProjectTasksArray.RemoveAt(selIndex);
	m_lsbTasks.DeleteString(selIndex);

	short sTotalEffort = 0;
	short sState = (short)ProjectStates::Finished;

	bool bHasNonFinishedTask = false;

	for (int i = 0; i < m_oProjectTasksArray.GetSize(); i++)
	{
		sTotalEffort += m_oProjectTasksArray[i]->sEffort;
		if (m_oProjectTasksArray[i]->sState != (short)TaskStates::Finished)
		{
			bHasNonFinishedTask = true;
		}
	}
	if (bHasNonFinishedTask)
	{
		sState = (short)ProjectStates::Active;
	}

	CString stateStr, effortStr;
	switch (sState)
	{
	case (short)ProjectStates::Active:
		stateStr = _T("Active");
		break;
	case (short)ProjectStates::Finished:
		stateStr = _T("Finished");
		break;
	default:
		stateStr = _T("None");
		break;
	}

	effortStr.Format(_T("%d"), sTotalEffort);

	m_sttState.SetWindowTextW(stateStr);
	m_sttTotalEffort.SetWindowTextW(effortStr);

}
void CProjectsDialog::UpdateTaskFromProject(const int nSelIndex, TASKS& oTask)
{
	m_oProjectTasksArray[nSelIndex] = &oTask;

	short sTotalEffort = 0;
	short sState = (short)ProjectStates::Finished;

	bool bHasNonFinishedTask = false;

	for (int i = 0; i < m_oProjectTasksArray.GetSize(); i++)
	{
		sTotalEffort += m_oProjectTasksArray[i]->sEffort;
		if (m_oProjectTasksArray[i]->sState != (short)TaskStates::Finished)
		{
			bHasNonFinishedTask = true;
		}
	}
	if (bHasNonFinishedTask)
	{
		sState = (short)ProjectStates::Active;
	}

	CString stateStr, effortStr;
	switch (sState)
	{
	case (short)ProjectStates::Active:
		stateStr = _T("Active");
		break;
	case (short)ProjectStates::Finished:
		stateStr = _T("Finished");
		break;
	default:
		stateStr = _T("None");
		break;
	}

	effortStr.Format(_T("%d"), sTotalEffort);

	m_sttState.SetWindowTextW(stateStr);
	m_sttTotalEffort.SetWindowTextW(effortStr);
}
void CProjectsDialog::SetTaskInfo()
{
	m_lsbTasks.ResetContent();
	m_oProjectTasksArray.RemoveAll();

	short sTotalEffort = 0;
	short sState = (short)ProjectStates::Finished;
	//Fill list box and total effort:
	for (int i = 0; i < m_oTasksArray.GetSize(); i++)
	{
		TASKS* oNewTask = m_oTasksArray[i];
		if (oNewTask->lProjectId == m_oProject.lId)
		{
			m_oProjectTasksArray.Add(oNewTask);
			int nIndex = m_lsbTasks.AddString(oNewTask->szName);
			m_lsbTasks.SetItemData(nIndex, (DWORD_PTR)oNewTask->lId);

			sTotalEffort += oNewTask->sEffort;
		}
	}
	//project State check
	bool bHasNonFinishedTask = false;

	for (int i = 0; i < m_oProjectTasksArray.GetSize(); i++)
	{
		if (m_oProjectTasksArray.GetSize() == 0)
		{
			sState = (short)ProjectStates::None;
		}
		if (m_oProjectTasksArray[i]->sState != (short)TaskStates::Finished)
		{
			bHasNonFinishedTask = true;
			break;
		}
	}
	if (bHasNonFinishedTask)
	{
		sState = (short)ProjectStates::Active;
	}

	CString stateStr, effortStr;
	switch (sState)
	{
	case (short)ProjectStates::Active:
		stateStr = _T("Active");
		break;
	case (short)ProjectStates::Finished:
		stateStr = _T("Finished");
		break;
	default:
		stateStr = _T("None");
		break;
	}

	effortStr.Format(_T("%d"), sTotalEffort);

	m_sttState.SetWindowTextW(stateStr);
	m_sttTotalEffort.SetWindowTextW(effortStr);
}

BOOL CProjectsDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_btnEdit.EnableWindow(FALSE);
	m_btnDelete.EnableWindow(FALSE);

	switch (m_oActionMode)
	{
	case Modes::InsertMode:
		SetDialogElementsText();
		break;
	case Modes::UpdateMode:
		SetDialogElementsText();
		break;
	case Modes::PreviewMode:
		m_edbName.EnableWindow(FALSE);
		m_edbDescription.EnableWindow(FALSE);
		m_lsbTasks.EnableWindow(FALSE);
		m_cmbProjectManager.EnableWindow(FALSE);
		m_btnAdd.EnableWindow(FALSE);
		m_btnDelete.EnableWindow(FALSE); 
		m_btnEdit.EnableWindow(FALSE);
		SetDialogElementsText();
		break;
	default:
		break;
	}

	return TRUE;
}

void CProjectsDialog::OnOK()
{
	if (m_oActionMode != Modes::PreviewMode) {
		PROJECTS oValidateProject;
		CString strTotalEffort;
		CString strName, strDescription, strProjectManager, strState;
		long lProjectManagerId = 0;
		short sState, sTotalEffort = 0;

		m_edbName.GetWindowText(strName);
		m_edbDescription.GetWindowText(strDescription);
		m_sttTotalEffort.GetWindowText(strTotalEffort);
		m_sttState.GetWindowText(strState);
		int nIndex = m_cmbProjectManager.GetCurSel();
		lProjectManagerId = m_cmbProjectManager.GetItemData(nIndex);

		sState = _ttoi(strState);
		sTotalEffort = _ttoi(strTotalEffort);

		oValidateProject = PROJECTS();
		_tcscpy_s(oValidateProject.szName, strName);
		_tcscpy_s(oValidateProject.szDescription, strName);
		oValidateProject.lProjectManagerId = lProjectManagerId;
		oValidateProject.sState = sState;
		oValidateProject.sTotalEffort = sTotalEffort;

		if (!ValidateDialog(oValidateProject))
		{
			return;
		}

		_tcscpy_s(m_oProject.szName, strName);
		_tcscpy_s(m_oProject.szDescription, strDescription);
		m_oProject.lProjectManagerId = lProjectManagerId;
		m_oProject.sState = sState ;
		m_oProject.sTotalEffort  = sTotalEffort;
	}

	return CDialogEx::OnOK();
}

void CProjectsDialog::OnAbort()
{
	m_oProjectTasksArray.RemoveAll();
	return CDialogEx::OnCancel();
}

void CProjectsDialog::OnCancel()
{
	m_oProjectTasksArray.RemoveAll();
	return CDialogEx::OnCancel();
}

void CProjectsDialog::OnLbnSelchangeList1()
{
	int selIndex = m_lsbTasks.GetCurSel();
	BOOL hasSelection = (selIndex != LB_ERR);

	m_btnEdit.EnableWindow(hasSelection);
	m_btnDelete.EnableWindow(hasSelection);
}

void CProjectsDialog::OnBnClickedBtnAddTask()
{
	TASKS* oTask = new TASKS();
	CTasksDialog oTasksDialog(*oTask, Modes::InsertMode, m_oUsersArray, m_oProject);
	int nResult = oTasksDialog.DoModal();
	if (nResult == MODAL_OK)
	{
		InsertTaskToProject(*oTask);
		return;
	}
}

void CProjectsDialog::OnBnClickedBtnDeleteTask()
{
	int selIndex = m_lsbTasks.GetCurSel();

	int result = AfxMessageBox(
		_T("Do you want to delete this task?"),
		MB_YESNO | MB_ICONQUESTION      // Buttons + icon
	);
	if (result == IDYES)
	{
		AfxMessageBox(_T("Task deleted"));
		DeleteTaskFromProject(selIndex);
	}
	OnLbnSelchangeList1();
}

void CProjectsDialog::OnBnClickedBtnEditTask()
{
	int nSelIndex = m_lsbTasks.GetCurSel();
	TASKS* oTask = m_oProjectTasksArray[nSelIndex];

	CTasksDialog oTasksDialog(*oTask, Modes::UpdateMode, m_oUsersArray, m_oProject);
	int nResult = oTasksDialog.DoModal();
	if (nResult == MODAL_OK)
	{
		UpdateTaskFromProject(nSelIndex, *oTask);
	}
	OnLbnSelchangeList1();
}
