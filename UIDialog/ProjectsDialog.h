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
	CProjectsDialog(CProjectDetails& oProjectDetails, Modes oActionMode, CUsersTypedPtrArray& oUsersArray,  CWnd* pParent = nullptr);   // standard constructor
	virtual ~CProjectsDialog();

	// Methods
	// ----------------
public:
	///<summary>Поставя начални стойности на диалога</summary>
	void SetDialogElementsText();
	///<summary>Поставя начални стойности относно тасковете</summary>
	void SetTaskInfo();
	///<summary>Създава нова задача към проекта</summary>
	void InsertTaskToProject(TASKS& oTask);
	///<summary>Променя задача към проекта</summary>
	void UpdateTaskFromProject(const int nSelIndex, TASKS& oTask);
	///<summary>Изтрива задача към проекта</summary>
	void DeleteTaskFromProject(const int selIndex);
	///<summary>Проверява дали диалога е валиден</summary>
	bool ValidateDialog(const PROJECTS& oValidateProject);
	///<summary>Методът който се извиква при начално зареждане на диалога</summary>
	virtual BOOL OnInitDialog();
	///<summary>Ок бутонът от диалога</summary>
	virtual void OnOK();
	///<summary>Cancel бутонът от диалога</summary>
	virtual void OnCancel();
	///<summary>Close бутонът от диалога</summary>
	virtual void OnAbort();

	//EVENTS
public:
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
	CTasksTypedPtrArray& m_oProjectTasksArray;
	ProjectStates m_oProjectState;
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
