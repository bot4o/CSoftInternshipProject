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
bool CUsersDialog::ValidateDialog(CString strName, CString strEmail, CString strJobTitle)
{
	int nAtPos = strEmail.Find(_T('@'));
	int nLastAtPos = strEmail.ReverseFind(_T('@'));

	if (nAtPos <= 0 || nAtPos != nLastAtPos || nAtPos >= strEmail.GetLength() - 1)
	{
		AfxMessageBox(_T("Invalid Email Format"));
		return false;
	}
	int nDotPos = strEmail.Find(_T('.'), nAtPos + 2);
	if (nDotPos == -1 || nDotPos >= strEmail.GetLength() - 1)
	{
		AfxMessageBox(_T("Invalid Email Format"));
		return false;
	}

	CString strOldName, strOldEmail, strOldJobTitle;
	strOldName = m_oUser.szName;
	strOldEmail = m_oUser.szEmail;
	strOldJobTitle = m_oUser.szJobTitle;
	if (strOldName == strName && strOldEmail == strEmail && strOldJobTitle == strJobTitle)
	{
		AfxMessageBox(_T("No changes were made"));
		return false;
	}
	return true;
}
void CUsersDialog::SetEditBoxText() 
{
	m_edbName.SetWindowTextW(m_oUser.szName);

	m_edbEmail.SetWindowTextW(m_oUser.szEmail);

	m_edbJobTitle.SetWindowTextW(m_oUser.szJobTitle);

	return;
}
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
		SetEditBoxText();
		break;
	case UpdateMode:
		SetEditBoxText();
		break;
	case PreviewMode:
		m_edbName.SetReadOnly();
		m_edbEmail.SetReadOnly();
		m_edbJobTitle.SetReadOnly();
		SetEditBoxText();
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

	if (!ValidateDialog(strName, strEmail, strJobTitle)) 
	{
		return;
	}

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