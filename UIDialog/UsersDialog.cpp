#include "pch.h"
#include "afxdialogex.h"
#include "UsersDialog.h"


/////////////////////////////////////////////////////////////////////////////
// CUsersDialog

// Macros
// ----------------
IMPLEMENT_DYNAMIC(CUsersDialog, CDialogEx)

BEGIN_MESSAGE_MAP(CUsersDialog, CDialogEx)
END_MESSAGE_MAP()
// Constructor / Destructor
// ----------------
CUsersDialog::CUsersDialog(USERS& oSelectedUser, Modes oActionMode, CWnd* pParent)
	: CDialogEx(IDD_USERS_DIALOG, pParent), m_oUser(oSelectedUser), m_oActionMode(oActionMode)
{
}

CUsersDialog::~CUsersDialog()
{
}

// Methods
// ----------------
void CUsersDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDB_NAME, m_edbName);
	DDX_Control(pDX, IDC_EDB_EMAIL, m_edbEmail);
	DDX_Control(pDX, IDC_EDB_JOB_TITLE, m_edbJobTitle);
}
BOOL CUsersDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	switch (m_oActionMode)
	{
	case InsertMode:
		m_edbName.SetWindowTextW(_T(""));
		m_edbEmail.SetWindowTextW(_T(""));
		m_edbJobTitle.SetWindowTextW(_T(""));
		break;
	case UpdateMode:
		m_edbName.SetWindowTextW(m_oUser.szName);
		m_edbEmail.SetWindowTextW(m_oUser.szEmail);
		m_edbJobTitle.SetWindowTextW(m_oUser.szJobTitle);
		break;
	case PreviewMode:
		m_edbName.SetReadOnly();
		m_edbEmail.SetReadOnly();
		m_edbJobTitle.SetReadOnly();
		m_edbName.SetWindowTextW(m_oUser.szName);
		m_edbEmail.SetWindowTextW(m_oUser.szEmail);
		m_edbJobTitle.SetWindowTextW(m_oUser.szJobTitle);
		break;
	default:
		break;
	}

	return TRUE;
}

void CUsersDialog::OnOK()
{
	if (m_oActionMode == PreviewMode) {
		return CDialogEx::OnOK();
	}
	CString strName;
	m_edbName.GetWindowText(strName);
	CString strEmail;
	m_edbEmail.GetWindowText(strEmail);
	CString strJobTitle;
	m_edbJobTitle.GetWindowText(strJobTitle);

	_tcscpy_s(m_oUser.szName, strName);
	_tcscpy_s(m_oUser.szEmail, strEmail);
	_tcscpy_s(m_oUser.szJobTitle, strJobTitle);

	return CDialogEx::OnOK();
}

void CUsersDialog::OnAbort()
{
	return CDialogEx::OnCancel(); 
}

void CUsersDialog::OnCancel()
{
	return CDialogEx::OnCancel();  
}

// Overrides
// ----------------