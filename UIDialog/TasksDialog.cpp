#include "pch.h"
#include "TasksDialog.h"

#define RDB_CHECKED 1
#define RDB_UNCHECKED 0

/////////////////////////////////////////////////////////////////////////////
// CSampleClass

// Macros
// ----------------
IMPLEMENT_DYNAMIC(CTasksDialog, CDialogEx)

BEGIN_MESSAGE_MAP(CTasksDialog, CDialogEx)
//	ON_BN_CLICKED(IDC_RDB_TASK_STATE_1, &CTasksDialog::OnBnClickedRdbTaskState1)
//	ON_BN_CLICKED(IDC_RDB_TASK_STATE_2, &CTasksDialog::OnBnClickedRdbTaskState2)
//	ON_BN_CLICKED(IDC_RDB_TASK_STATE_3, &CTasksDialog::OnBnClickedRdbTaskState3)
ON_BN_CLICKED(IDC_RDB_TASK_STATE_1, &CTasksDialog::OnBnClickedRdbTaskState1)
ON_BN_CLICKED(IDC_RDB_TASK_STATE_2, &CTasksDialog::OnBnClickedRdbTaskState2)
ON_BN_CLICKED(IDC_RDB_TASK_STATE_3, &CTasksDialog::OnBnClickedRdbTaskState3)
END_MESSAGE_MAP()
// Constructor / Destructor
// ----------------

CTasksDialog::CTasksDialog(TASKS& oSelectedTask, Modes oActionMode, CUsersTypedPtrArray& oUsersArray, PROJECTS& oProject, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TASKS_DIALOG, pParent), m_oTask(oSelectedTask), m_oActionMode(oActionMode), m_oUsersArray(oUsersArray), m_oProject(oProject)
{

}

CTasksDialog::~CTasksDialog()
{
}


// Methods
// ----------------
void CTasksDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDB_TASK_NAME, m_edbName);
	DDX_Control(pDX, IDC_EDB_TASK_DESC, m_edbDesc);
	DDX_Control(pDX, IDC_CMB_TASK_USER, m_cmbUsers);
	DDX_Control(pDX, IDC_RDB_TASK_STATE_1, m_rdbState1);
	DDX_Control(pDX, IDC_RDB_TASK_STATE_2, m_rdbState2);
	DDX_Control(pDX, IDC_RDB_TASK_STATE_3, m_rdbState3);
	DDX_Control(pDX, IDC_EDB_TASK_EFFORT, m_edbEffort);
	DDX_Control(pDX, IDC_STT_PROJECT, m_sttProject);
}
bool CTasksDialog::ValidateDialog(const TASKS& oValidateTask)
{
	if (_tcslen(oValidateTask.szName) == 0 || _tcslen(oValidateTask.szDescription) == 0 || oValidateTask.lUserId == -1 || oValidateTask.sEffort == 0 || oValidateTask.sState == -1)
	{
		AfxMessageBox(_T("Please fill each field"));
		return false;
	}
	if (_tcscmp(m_oTask.szName, oValidateTask.szName) == 0 &&
		_tcscmp(m_oTask.szDescription, oValidateTask.szDescription) == 0 &&
		m_oTask.lProjectId == oValidateTask.lProjectId &&
		m_oTask.lUserId == oValidateTask.lUserId &&
		m_oTask.sState == oValidateTask.sState &&
		m_oTask.sEffort == oValidateTask.sEffort)
	{
		AfxMessageBox(_T("No changes were made"));
		return false;
	}
	return true;
}
void CTasksDialog::SetDialogElementsText()
{
	m_edbName.SetWindowTextW(m_oTask.szName);
	m_edbDesc.SetWindowTextW(m_oTask.szDescription);
	CString strEdbEffort;
	strEdbEffort.Format(_T("%i"), m_oTask.sEffort);
	m_edbEffort.SetWindowTextW(strEdbEffort);

	int selIndex = -1;

	for (int i = 0; i < m_oUsersArray.GetSize(); i++)
	{		
		USERS* oCurUser = m_oUsersArray[i];
		int nIndex = m_cmbUsers.AddString(oCurUser->szName);
		nIndex = m_cmbUsers.FindStringExact(-1, oCurUser->szName); // get actual index after sort
		m_cmbUsers.SetItemData(nIndex, (DWORD_PTR)oCurUser->lId);	
		if (oCurUser->lId == m_oTask.lUserId)
			selIndex = nIndex;
	}
	if (selIndex != -1)
	{
		m_cmbUsers.SetCurSel(selIndex);
	}

	switch (m_oTask.sState)
	{
	case (short)TaskStates::Waiting:
		m_rdbState1.SetCheck(RDB_CHECKED);
		m_oRdbCheck = TaskStates::Waiting;
		break;
	case (short)TaskStates::InProgress:
		m_rdbState2.SetCheck(RDB_CHECKED);
		m_oRdbCheck = TaskStates::InProgress;
		break;
	case (short)TaskStates::Finished:
		m_rdbState3.SetCheck(RDB_CHECKED);
		m_oRdbCheck = TaskStates::Finished;
		break;
	default:
		break;
	}
	return;
}

BOOL CTasksDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	switch (m_oActionMode)
	{
	case Modes::InsertMode:
		m_rdbState3.EnableWindow(FALSE);
		SetDialogElementsText();
		break;
	case Modes::UpdateMode:
		SetDialogElementsText();
		break;
	case Modes::PreviewMode:
		m_edbName.SetReadOnly();
		m_edbDesc.SetReadOnly();
		m_edbEffort.SetReadOnly();
		SetDialogElementsText();
		break;
	default:
		break;
	}

	return TRUE;
}

void CTasksDialog::OnOK()
{
	if (m_oActionMode != Modes::PreviewMode) {
		TASKS oValidateTask;
		CString strName, strDesc, strEffort;
		long lProjectId, lUserId = 0;
		short sState, sEffort = 0;

		m_edbName.GetWindowText(strName);
		m_edbDesc.GetWindowText(strDesc);
		m_edbEffort.GetWindowText(strEffort);
		sEffort = _ttoi(strEffort);
		lProjectId = m_oProject.lId;
		int nIndex = m_cmbUsers.GetCurSel();
		lUserId = m_cmbUsers.GetItemData(nIndex);
		sState = (short)m_oRdbCheck;

		oValidateTask = TASKS();
		_tcscpy_s(oValidateTask.szName, strName);
		_tcscpy_s(oValidateTask.szDescription, strDesc);
		oValidateTask.lProjectId = lProjectId;
		oValidateTask.lUserId = lUserId;
		oValidateTask.sState = sState;
		oValidateTask.sEffort = sEffort;


		if (!ValidateDialog(oValidateTask))
		{
			return;
		}

		_tcscpy_s(m_oTask.szName, strName);
		_tcscpy_s(m_oTask.szDescription, strDesc);
		m_oTask.lProjectId = lProjectId;
		m_oTask.lUserId = lUserId;
		m_oTask.sState = sState;
		m_oTask.sEffort = sEffort; 
	}
	return CDialogEx::OnOK();
}

void CTasksDialog::OnAbort()
{
	return CDialogEx::OnCancel();
}

void CTasksDialog::OnCancel()
{
	return CDialogEx::OnCancel();
}



// Overrides
// ----------------
void CTasksDialog::OnBnClickedRdbTaskState1()
{
	m_oRdbCheck = TaskStates::Waiting;
}

void CTasksDialog::OnBnClickedRdbTaskState2()
{
	m_oRdbCheck = TaskStates::InProgress;
}

void CTasksDialog::OnBnClickedRdbTaskState3()
{
	m_oRdbCheck = TaskStates::Finished;
}
