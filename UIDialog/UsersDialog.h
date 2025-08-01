#pragma once
#include "DLLExport.h"
#include "pch.h"
#include "resource.h"
#include "afxdialogex.h"
#include "UsersStruct.h"
#include "DialogModes.h"

/////////////////////////////////////////////////////////////////////////////
// CUsersDialog

class UIDialogDLL_EXP CUsersDialog : public CDialogEx 
{
	// Macros
	// ----------------
	DECLARE_DYNAMIC(CUsersDialog)
	DECLARE_MESSAGE_MAP()

	// Constants
	// ----------------


	// Constructor / Destructor
	// ----------------
public:
	//CUsersDialog(CWnd* pParent = nullptr);   // standard constructor
	CUsersDialog(USERS& oSelectedUser, Modes oActionMode, CWnd* pParent = nullptr);
	virtual ~CUsersDialog();


	// Methods
	// ----------------
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	virtual void OnAbort();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_USERS_DIALOG };
#endif
	// Overrides
	// ----------------


	// Members
	// ----------------
private:
	Modes m_oActionMode;
	USERS& m_oUser;
	CEdit m_edbName;
	CEdit m_edbEmail;
	CEdit m_edbJobTitle;
};