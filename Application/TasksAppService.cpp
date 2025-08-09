#include "pch.h"
#include "TasksAppService.h"
#include "TasksAccessor.h"
#include "Table.h"


#define TABLE_NAME _T("TASKS")
// Constructor / Destructor
// ----------------
CTasksAppService::CTasksAppService()
{

}
CTasksAppService::~CTasksAppService()
{

}

// Methods
// ----------------

/// <summary>Извежда всички потребители в базата</summary>  
bool CTasksAppService::SelectAll(CTasksTypedPtrArray& oTasksArray)
{
	if (!CBaseTable<TASKS, CTasksAccessor>(TABLE_NAME).SelectAll(oTasksArray))
	{
		AfxMessageBox(_T("Error at the m_oTasksTable.SelectAll() in the application layer"));
		return false;
	}
	return true;
}

/// <summary>Извежда потребител от базата според ID</summary>  
bool CTasksAppService::SelectWhereID(const long lID, TASKS& recTask)
{
	if (!CBaseTable<TASKS, CTasksAccessor>(TABLE_NAME).SelectWhereID(lID, recTask))
	{
		AfxMessageBox(_T("Error at the m_oTasksTable.SelectWhereID() in the application layer"));
		return false;
	}
	return true;
}

/// <summary>Променя длъжноста на потребител от базата според ID</summary>  
bool CTasksAppService::UpdateWhereID(const long lID, TASKS& recTask)
{
	if (!CBaseTable<TASKS, CTasksAccessor>(TABLE_NAME).UpdateWhereID(lID, recTask))
	{
		AfxMessageBox(_T("Error at the m_oTasksTable.UpdateWhereID() in the application layer"));
		return false;
	}
	return true;
}

/// <summary>Вмъква нов потребител в базата</summary>  
bool CTasksAppService::Insert(TASKS& recTask)
{
	if (!CBaseTable<TASKS, CTasksAccessor>(TABLE_NAME).Insert(recTask))
	{
		AfxMessageBox(_T("Error at the m_oTasksTable.Insert() in the application layer"));
		return false;
	}
	return true;
}

/// <summary>Изтрива потребител от базата според ID</summary>  
bool CTasksAppService::DeleteWhereID(const long lID)
{
	if (!CBaseTable<TASKS, CTasksAccessor>(TABLE_NAME).DeleteWhereID(lID))
	{
		AfxMessageBox(_T("Error at the m_oTasksTable.DeleteWhereID() in the application layer"));
		return false;
	}
	return true;
}