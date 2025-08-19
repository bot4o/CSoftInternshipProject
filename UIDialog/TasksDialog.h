#pragma once
#include "DLLExport.h"
#include "pch.h"
#include "resource.h"
#include "afxdialogex.h"
#include "TasksStruct.h"
#include "DialogModes.h"
#include "UsersStruct.h"
#include "ProjectsStruct.h"
#include "ProjectDetails.h"
#include "TaskStates.h"


/////////////////////////////////////////////////////////////////////////////
// CTasksDialog
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
	// <summary>Поставя начални стойности на диалога</summary>
	void SetDialogElementsText();
	// <summary>Проверява дали диалога е валиден</summary>
	bool ValidateDialog(const TASKS& oValidateTask);
	// <summary>Методът който се извиква при начално зареждане на диалога</summary>
	virtual BOOL OnInitDialog();
	///<summary>Ок бутонът от диалога</summary>
	virtual void OnOK();
	///<summary>Cancel бутонът от диалога</summary>
	virtual void OnCancel();
	///<summary>Close бутонът от диалога</summary>
	virtual void OnAbort();

	//EVENTS
public:
	afx_msg void OnBnClickedRdbTaskState1();
	afx_msg void OnBnClickedRdbTaskState2();
	afx_msg void OnBnClickedRdbTaskState3();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	// Members
	// ----------------
private:
	TASKS& m_oTask;
	PROJECTS& m_oProject;
	CUsersTypedPtrArray& m_oUsersArray;
	TaskStates m_oRdbCheck;
	Modes m_oActionMode;
	CStatic m_sttProject;
	CEdit m_edbName;
	CEdit m_edbDesc;
	CComboBox m_cmbUsers;
	CButton m_rdbState1;
	CButton m_rdbState2;
	CButton m_rdbState3;
	CEdit m_edbEffort;

};