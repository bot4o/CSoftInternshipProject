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

IMPLEMENT_DYNAMIC(CProjectsDialog, CDialogEx)

BEGIN_MESSAGE_MAP(CProjectsDialog, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_ADD_TASK, &CProjectsDialog::OnBnClickedBtnAddTask)
//	ON_CBN_SELCHANGE(IDC_CMB_PROJECT_MANAGER, &CProjectsDialog::OnCbnSelchangeCmbProjectManager)
END_MESSAGE_MAP()

// Constructor / Destructor
// ----------------
CProjectsDialog::CProjectsDialog(CProjectDetails oProjectDetails, Modes oActionMode, CUsersTypedPtrArray& oUsersArray,
	CTasksTypedPtrArray& oTasksArray, CWnd* pParent)
	: CDialogEx(IDD_PROJECTS_DIALOG, pParent), m_oProject(oProjectDetails.GetProject()), m_oNewTasksArray(oProjectDetails.GetTasks()), m_oActionMode(oActionMode), m_oUsersArray(oUsersArray), m_oTasksArray(oTasksArray)
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
}

bool CProjectsDialog::ValidateDialog(const PROJECTS& oValidateProject)
{
	if (m_oProject.lId == oValidateProject.lId &&
		_tcscmp(m_oProject.szName, oValidateProject.szName) == 0 &&
		_tcscmp(m_oProject.szDescription, oValidateProject.szDescription) == 0 &&
		m_oProject.lProjectManagerId == oValidateProject.lProjectManagerId &&
		m_oProject.sState == oValidateProject.sState &&
		m_oProject.sTotalEffort == oValidateProject.sTotalEffort)
	{
		return false;
	}
	return true;
}

void CProjectsDialog::SetDialogElementsText()
{
	m_edbName.SetWindowTextW(m_oProject.szName);
	m_edbDescription.SetWindowTextW(m_oProject.szDescription);

	for (int i = 0; i < m_oUsersArray.GetSize(); i++)
	{
		USERS* oCurUser = m_oUsersArray[i];
		CString strProjectManager = oCurUser->szName;
		int nIndex = m_cmbProjectManager.AddString(strProjectManager);
		m_cmbProjectManager.SetItemData(nIndex, (DWORD_PTR)oCurUser->lId);

	}
	SetTaskInfo();

	return;
}

BOOL CProjectsDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	switch (m_oActionMode)
	{
	case Modes::InsertMode:
		SetDialogElementsText();
		break;
	case Modes::UpdateMode:
		SetDialogElementsText();
		break;
	case Modes::PreviewMode:
		m_edbName.SetReadOnly();
		m_edbDescription.SetReadOnly();
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
		lProjectManagerId = m_cmbProjectManager.GetCurSel();
		if (lProjectManagerId == CB_ERR)
		{
			AfxMessageBox(_T("Select a Project manager"));
			return;
		}

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
			AfxMessageBox(_T("No changes were made"));
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
	return CDialogEx::OnCancel();
}

void CProjectsDialog::OnCancel()
{
	return CDialogEx::OnCancel();
}

// CProjectsDialog message handlers

void CProjectsDialog::SetTaskInfo()
{
	short sState, sTotalEffort = 0;
	for (int i = 0; i < m_oTasksArray.GetSize(); i++)
	{
		TASKS* oCurTask = m_oTasksArray[i];
		if (oCurTask->lProjectId == m_oProject.lId)
		{
			m_lsbTasks.AddString(oCurTask->szName);
			sTotalEffort += oCurTask->sEffort;
			short sTaskState = oCurTask->sState;
			if (sTaskState == 0 || sTaskState == 1)
			{
				sState = 1;
			}
			else if (sTaskState == 2)
			{
				sTaskState = 0;
			}
			else
			{
				AfxMessageBox(_T("Invalid sState valuse on tasks. Cant assign state on project"));
				break;
			}
		}
	}
	m_sttState.SetWindowTextW(_T("%d", sState));
	m_sttTotalEffort.SetWindowTextW(_T("%i", sTotalEffort));
}

void CProjectsDialog::OnBnClickedBtnAddTask()
{
	TASKS* oTask = new TASKS();
	CTasksDialog oProjectsDialog(*oTask, m_oNewTasksArray, Modes::InsertMode, m_oUsersArray, m_oProject);
	int nResult = oProjectsDialog.DoModal();
	if (nResult == MODAL_OK)
	{
		m_oTasksArray.Add(oTask);
		SetTaskInfo();
		return;
	}
}
