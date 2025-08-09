// TasksDialog.cpp : implementation file
//
#include "pch.h"
#include "afxdialogex.h"
#include "TasksDialog.h"


// CTasksDialog dialog

IMPLEMENT_DYNAMIC(CTasksDialog, CDialogEx)

CTasksDialog::CTasksDialog(TASKS& oSelectedTask, Modes oActionMode, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TASKS_DIALOG, pParent), m_oTask(oSelectedTask), m_oActionMode(oActionMode)
{

}

CTasksDialog::~CTasksDialog()
{
}

void CTasksDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTasksDialog, CDialogEx)
END_MESSAGE_MAP()


// CTasksDialog message handlers
