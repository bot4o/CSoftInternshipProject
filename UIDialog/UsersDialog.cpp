// UsersDialog.cpp : implementation file
//

#include "pch.h"
#include "afxdialogex.h"
#include "UsersDialog.h"


// CUsersDialog dialog

IMPLEMENT_DYNAMIC(CUsersDialog, CDialogEx)

CUsersDialog::CUsersDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_USERS_DIALOG, pParent)
{

}
CUsersDialog::CUsersDialog(USERS* oSelectedUser, bool mode, CWnd* pParent)
	: CDialogEx(IDD_USERS_DIALOG, pParent), m_oUser(*oSelectedUser), m_mode(mode)
{ 

}

CUsersDialog::~CUsersDialog()
{
}

void CUsersDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDB_NAME, m_edbName);
	DDX_Control(pDX, IDC_EDB_EMAIL, m_edbEmail);
	DDX_Control(pDX, IDC_EDB_JOB_TITLE, m_edbJobTitle);
}


BEGIN_MESSAGE_MAP(CUsersDialog, CDialogEx)
END_MESSAGE_MAP()


// CUsersDialog message handlers

BOOL CUsersDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	//UPDATE
	if (m_mode == true)
	{
		m_edbName.SetWindowTextW(_T(""));
		m_edbEmail.SetWindowTextW(_T(""));
		m_edbJobTitle.SetWindowTextW(_T(""));

		m_edbName.SetWindowTextW(m_oUser.szName);
		m_edbEmail.SetWindowTextW(m_oUser.szEmail);
		m_edbJobTitle.SetWindowTextW(m_oUser.szJobTitle);
	}
	else //INSERT
	{
		m_edbName.SetWindowTextW(_T(""));
		m_edbEmail.SetWindowTextW(_T(""));
		m_edbJobTitle.SetWindowTextW(_T(""));
	}
	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CUsersDialog::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	CString strName; 
	m_edbName.GetWindowText(strName);
	CString strEmail;
	m_edbName.GetWindowText(strEmail);
	CString strJobTitle;
	m_edbName.GetWindowText(strJobTitle);

	_tcscpy_s(m_oUser.szName, strName);
	_tcscpy_s(m_oUser.szEmail, strEmail);
	_tcscpy_s(m_oUser.szJobTitle, strJobTitle);

	CDialogEx::OnOK();
}
