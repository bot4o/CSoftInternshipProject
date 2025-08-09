#pragma once
#include "DLLExport.h"
#include "pch.h"
#include "resource.h"
#include "afxdialogex.h"
#include "TasksStruct.h"
#include "DialogModes.h"


// CTasksDialog dialog

class CTasksDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CTasksDialog)

public:
	CTasksDialog(TASKS& oSelectedTask, Modes nActionMode, CWnd* pParent = nullptr);   // standard constructor
	virtual ~CTasksDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TASKS_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private: 
	TASKS& m_oTask; 
	Modes m_oActionMode;
};
