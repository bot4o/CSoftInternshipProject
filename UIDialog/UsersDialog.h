#pragma once
#include "DLLExport.h"
#include "pch.h"
#include "resource.h"
#include "afxdialogex.h"
#include "UsersStruct.h"
#include "../UIView/UsersView.h"

// CUsersDialog dialog

class UIDialogDLL_EXP CUsersDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CUsersDialog)

public:
	CUsersDialog(CWnd* pParent = nullptr);   // standard constructor
	CUsersDialog(USERS* oSelectedUser, bool m_mode, CWnd* pParent = nullptr);
	virtual ~CUsersDialog();


	virtual BOOL OnInitDialog();
	virtual void OnOK();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_USERS_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	//true - Edit mode;
	//false - Insert mode;
	bool m_mode;
	USERS m_oUser;

	CEdit m_edbName;
	CEdit m_edbEmail;
	CEdit m_edbJobTitle;
};
