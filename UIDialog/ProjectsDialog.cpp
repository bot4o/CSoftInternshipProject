#include "pch.h"
#include "afxdialogex.h"
#include "ProjectsDialog.h"
#include "TasksDialog.h"
#include "../UIView/ProjectsView.h"
/////////////////////////////////////////////////////////////////////////////
// CProjectsDialog

// Macros
// ----------------
IMPLEMENT_DYNAMIC(CProjectsDialog, CDialogEx)

BEGIN_MESSAGE_MAP(CProjectsDialog, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_ADD_TASK, &CProjectsDialog::OnBnClickedBtnAddTask)
END_MESSAGE_MAP()

// Constructor / Destructor
// ----------------
CProjectsDialog::CProjectsDialog(PROJECTS& oSelectedProject, Modes oActionMode, CUsersTypedPtrArray& oUsersArray, 
	CTasksTypedPtrArray& oTasksArray, CWnd* pParent)
	: CDialogEx(IDD_PROJECTS_DIALOG, pParent), m_oProject(oSelectedProject), m_oActionMode(oActionMode), m_oUsersArray(oUsersArray), m_oTasksArray(oTasksArray)
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
}
bool CProjectsDialog::ValidateDialog(CString strName, CString strDescription, long lProjectManager, short sState, short sTotalEffort)
{
	CString strOldName, strOldDescription;
	long lOldProjectManager;
	short sOldState, sOldTotalEffort;

	strOldName = m_oProject.szName;
	strOldDescription = m_oProject.szDescription;
	lOldProjectManager = m_oProject.lProjectManagerId;
	sOldState = m_oProject.sState;
	sOldTotalEffort = m_oProject.sTotalEffort;
	if (strOldName == strName && strOldDescription == strDescription && lOldProjectManager == lProjectManager && 
		sOldState == sState && sOldTotalEffort == sTotalEffort)
	{
		AfxMessageBox(_T("No changes were made"));
		return false;
	}
	return true;
}
void CProjectsDialog::SetDialogElementsText()
{
	m_edbName.SetWindowTextW(m_oProject.szName);
	m_edbDescription.SetWindowTextW(m_oProject.szDescription);

	short sState = 0;
	short sTotalEffort = 0;
	for (int i = 0; i < m_oUsersArray.GetSize(); i++)
	{
		USERS* oCurUser = m_oUsersArray[i];
		CString strProjectManager = oCurUser->szName;
		int nIndex = m_cmbProjectManager.AddString(strProjectManager);
		m_cmbProjectManager.InsertString(nIndex, strProjectManager);
	}
	for (int i = 0; i < m_oTasksArray.GetSize(); i++)
	{
		TASKS* oCurTask = m_oTasksArray[i];
		if (oCurTask->lProjectId == m_oProject.lId)
		{
			sTotalEffort += oCurTask->sEffort;
			short sCurState = oCurTask->sState;
			if (sState == 0 || sState == 1)
			{
				sState == 1;
			}
			else if (sState == 2)
			{
				sState == 0;
			}
			else
			{
				AfxMessageBox(_T("Invalid sState valuse on tasks. Cant assign state on project"));
				break;
			}
		}
	}
	m_sttState.SetWindowTextW(_T("%i", sState));
	m_sttTotalEffort.SetWindowTextW(_T("%i", sTotalEffort));

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
		CString strName;
		m_edbName.GetWindowText(strName);
		CString strDescription;
		m_edbDescription.GetWindowText(strDescription);
		long lProjectManagerId = 0;
		CString strProjectManager;
		m_cmbProjectManager.GetWindowText(strProjectManager);
		for (int i = 0; i < m_oUsersArray.GetSize(); i++)
		{
			USERS* oCurUser = m_oUsersArray[i];
			if (strProjectManager == oCurUser->szName)
			{
				lProjectManagerId == oCurUser->lId;
				break;
			}
		}

		CString strState;
		m_sttState.GetWindowText(strState);
		short sState = _ttoi(strState);

		CString strTotalEffort;
		m_sttTotalEffort.GetWindowText(strTotalEffort);
		short sTotalEffort = _ttoi(strTotalEffort);


		if (!ValidateDialog(strName, strDescription, lProjectManagerId, sState, sTotalEffort))
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
	return CDialogEx::OnCancel();
}

void CProjectsDialog::OnCancel()
{
	return CDialogEx::OnCancel();
}

// CProjectsDialog message handlers

void CProjectsDialog::OnBnClickedBtnAddTask()
{
	TASKS oTask = TASKS();
	CTasksDialog oProjectsDialog(oTask, Modes::UpdateMode);
	// TODO: Add your control notification handler code here
}
