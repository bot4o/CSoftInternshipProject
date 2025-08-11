#pragma once
#include "DLLExport.h"
#include "pch.h"
#include "resource.h"
#include "afxdialogex.h"
#include "TasksStruct.h"
#include "DialogModes.h"
#include "UsersStruct.h"
#include "ProjectsStruct.h"

/////////////////////////////////////////////////////////////////////////////
// CTasksDialog
enum class RadioButtonState
{
	Waiting = 0,
	InProgress = 1,
	Finished = 2,
};
class CTasksDialog : public CDialogEx
{

	// Macros
	// ----------------
	DECLARE_DYNAMIC(CTasksDialog)
	DECLARE_MESSAGE_MAP()
	// Constants
	// ----------------


	// Constructor / Destructor
	// ----------------
public:
	CTasksDialog(TASKS& oSelectedTask, Modes nActionMode, CUsersTypedPtrArray& oUsersArray, PROJECTS& oProject, CWnd* pParent = nullptr);   // standard constructor
	virtual ~CTasksDialog();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TASKS_DIALOG };
#endif

	// Methods
	// ----------------
public:
	void SetDialogElementsText();
	bool ValidateDialog(const TASKS& oValidateTask);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	virtual void OnAbort();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// Overrides
	// ----------------


	// Members
	// ----------------
private:
	TASKS& m_oTask;
	PROJECTS& m_oProject;
	CUsersTypedPtrArray& m_oUsersArray;

	short sRdbCheck = -1;

	Modes m_oActionMode;
	CStatic m_sttProject;
	CEdit m_edbName;
	CEdit m_edbDesc;
	CComboBox m_cmbUsers;
	CButton m_rdbState1;
	CButton m_rdbState;
	CButton m_rdbState3;
	CEdit m_edbEffort;
public:
	afx_msg void OnBnClickedRdbTaskState1();
	afx_msg void OnBnClickedRdbTaskState2();
	afx_msg void OnBnClickedRdbTaskState3();
};