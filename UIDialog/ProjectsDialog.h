#pragma once
#include "DLLExport.h"
#include "pch.h"
#include "resource.h"
#include "afxdialogex.h"
#include "ProjectsStruct.h"
#include "TasksStruct.h"
#include "DialogModes.h"
#include "ProjectStates.h"
#include "TaskStates.h"
#include "UsersStruct.h"
#include "ProjectDetails.h"

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
	CProjectsDialog(CProjectDetails& oProjectDetails, Modes oActionMode, CUsersTypedPtrArray& oUsersArray,
		CTasksTypedPtrArray& oTasksArray,  CWnd* pParent = nullptr);   // standard constructor
	virtual ~CProjectsDialog();

	// Methods
	// ----------------
public:
	void SetDialogElementsText();
	void SetTaskInfo();
	void InsertTaskToProject(TASKS& oTask);
	void UpdateTaskFromProject(const int nSelIndex, TASKS& oTask);
	void DeleteTaskFromProject(const int selIndex);
	bool ValidateDialog(const PROJECTS& oValidateProject);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	virtual void OnAbort();
	afx_msg void OnLbnSelchangeList1();
	afx_msg void OnBnClickedBtnAddTask();
	afx_msg void OnBnClickedBtnDeleteTask();
	afx_msg void OnBnClickedBtnEditTask();
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
	CTasksTypedPtrArray&m_oTasksArray;
	CTasksTypedPtrArray& m_oProjectTasksArray;

	CEdit m_edbName;
	CEdit m_edbDescription;
	CComboBox m_cmbProjectManager;
	CStatic m_sttState;
	CStatic m_sttTotalEffort;
	CListBox m_lsbTasks;
	CButton m_btnEdit;
	CButton m_btnDelete;
	CButton m_btnAdd;
};
