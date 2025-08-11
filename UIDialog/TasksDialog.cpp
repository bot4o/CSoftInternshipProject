#include "pch.h"
#include "afxdialogex.h"
#include "TasksDialog.h"


/////////////////////////////////////////////////////////////////////////////
// CSampleClass

// Macros
// ----------------
IMPLEMENT_DYNAMIC(CTasksDialog, CDialogEx)

BEGIN_MESSAGE_MAP(CTasksDialog, CDialogEx)
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
	DDX_Control(pDX, IDC_RDB_TASK_STATE_2, m_rdbState);
	DDX_Control(pDX, IDC_RDB_TASK_STATE_3, m_rdbState3);
	DDX_Control(pDX, IDC_EDB_TASK_EFFORT, m_edbEffort);
	DDX_Control(pDX, IDC_STT_PROJECT, m_sttProject);
}
bool CTasksDialog::ValidateDialog(const TASKS& oValidateTask)
{
	if (m_oTask.lId == oValidateTask.lId &&
		_tcscmp(m_oTask.szName, oValidateTask.szName) == 0 &&
		_tcscmp(m_oTask.szDescription, oValidateTask.szDescription) == 0 &&
		m_oTask.lProjectId == oValidateTask.lProjectId &&
		m_oTask.lUserId == oValidateTask.lUserId &&
		m_oTask.sState == oValidateTask.sState &&
		m_oTask.sEffort == oValidateTask.sEffort)
	{
		return false;
	}
	return true;
}
void CTasksDialog::SetDialogElementsText()
{
	m_edbName.SetWindowTextW(m_oTask.szName);
	m_edbDesc.SetWindowTextW(m_oTask.szDescription);
	m_edbEffort.SetWindowTextW(_T("%i", m_oTask.sEffort));
	
	for (int i = 0; i < m_oUsersArray.GetSize(); i++)
	{		
		USERS* oCurUser = m_oUsersArray[i];
		CString strTaskUser = oCurUser->szName;
		int nIndex = m_cmbUsers.AddString(strTaskUser);
		m_cmbUsers.SetItemData(nIndex, (DWORD_PTR)oCurUser->lId);
	}

	switch (m_oTask.sState)
	{
	case (short)RadioButtonState::Waiting:
		OnBnClickedRdbTaskState1();
		break;
	case (short)RadioButtonState::InProgress:
		OnBnClickedRdbTaskState2();
		break;
	case (short)RadioButtonState::Finished:
		OnBnClickedRdbTaskState3();
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
		lUserId = m_cmbUsers.GetCurSel();
		sState = sRdbCheck;

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
	sRdbCheck = (short)RadioButtonState::Waiting;
	m_rdbState.GetCheck();
	// TODO: Add your control notification handler code here
}

void CTasksDialog::OnBnClickedRdbTaskState2()
{
	sRdbCheck = (short)RadioButtonState::InProgress;
	m_rdbState.GetCheck();

	// TODO: Add your control notification handler code here
}

void CTasksDialog::OnBnClickedRdbTaskState3()
{
	sRdbCheck = (short)RadioButtonState::Finished;
	m_rdbState.GetCheck();
	// TODO: Add your control notification handler code here
}


