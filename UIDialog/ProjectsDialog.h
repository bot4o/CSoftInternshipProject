#pragma once
#include "DLLExport.h"
#include "pch.h"
#include "resource.h"
#include "afxdialogex.h"
#include "ProjectsStruct.h"
#include "TasksStruct.h"
#include "DialogModes.h"
#include "UsersStruct.h"

/////////////////////////////////////////////////////////////////////////////
// CUsersDialog

class UIDialogDLL_EXP CProjectsDialog : public CDialogEx
{
	// Macros
	// ----------------
	DECLARE_DYNAMIC(CProjectsDialog)
	DECLARE_MESSAGE_MAP()

	// Constructor / Destructor
	// ----------------
public:
	CProjectsDialog(PROJECTS& oSelectedProject, Modes oActionMode, CUsersTypedPtrArray& oUsersArray,
		CTasksTypedPtrArray& oTasksArray,  CWnd* pParent = nullptr);   // standard constructor
	virtual ~CProjectsDialog();

	// Methods
	// ----------------
	void SetDialogElementsText();
	bool ValidateDialog(CString strName, CString strDescription, long lProjectManager, short sState, short sTotalEffort);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	virtual void OnAbort();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROJECTS_DIALOG };
#endif

	// Members
	// ----------------
private:
	Modes m_oActionMode;
	PROJECTS& m_oProject;
	CUsersTypedPtrArray& m_oUsersArray;
	CTasksTypedPtrArray& m_oTasksArray;
	CEdit m_edbName;
	CEdit m_edbDescription;
	CComboBox m_cmbProjectManager;
	CStatic m_sttState;
	CStatic m_sttTotalEffort;
public:
	afx_msg void OnBnClickedBtnAddTask();
};
