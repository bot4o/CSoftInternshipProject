#include "pch.h"
#include "TasksAppService.h"
#include "ProjectsAppService.h"
#include "ProjectsAccessor.h"
#include "UsersAccessor.h"
#include "TasksAccessor.h"
#include "Table.h"


#define TABLE_NAME _T("PROJECTS")
// Constructor / Destructor
// ----------------
CProjectsAppService::CProjectsAppService()
{

}
CProjectsAppService::~CProjectsAppService()
{

}

// Methods
// ----------------

/// <summary>Извежда всички потребители в базата</summary>  
bool CProjectsAppService::SelectAll(CProjectsTypedPtrArray& oProjectsArray)
{
	if (!CBaseTable<PROJECTS, CProjectsAccessor>(TABLE_NAME).SelectAll(oProjectsArray))
	{
		AfxMessageBox(_T("Error at the m_oProjectsTable.SelectAll() in the application layer"));
		return false;
	}
	return true;
}

/// <summary>Извежда потребител от базата според ID</summary>  
bool CProjectsAppService::SelectWhereID(const long lID, PROJECTS& recProject)
{
	if (!CBaseTable<PROJECTS, CProjectsAccessor>(TABLE_NAME).SelectWhereID(lID, recProject))
	{
		AfxMessageBox(_T("Error at the m_oProjectsTable.SelectWhereID() in the application layer"));
		return false;
	}
	return true;
}

/// <summary>Променя длъжноста на потребител от базата според ID</summary>  
bool CProjectsAppService::UpdateWhereID(const long lID, PROJECTS& recProject)
{
	if (!CBaseTable<PROJECTS, CProjectsAccessor>(TABLE_NAME).UpdateWhereID(lID, recProject))
	{
		AfxMessageBox(_T("Error at the m_oProjectsTable.UpdateWhereID() in the application layer"));
		return false;
	}
	return true;
}

/// <summary>Вмъква нов потребител в базата</summary>  
bool CProjectsAppService::Insert(PROJECTS& recProject)
{
	if (!CBaseTable<PROJECTS, CProjectsAccessor>(TABLE_NAME).Insert(recProject))
	{
		AfxMessageBox(_T("Error at the m_oProjectsTable.Insert() in the application layer"));
		return false;
	}
	return true;
}

/// <summary>Изтрива потребител от базата според ID</summary>  
bool CProjectsAppService::DeleteWhereID(const long lID)
{
	if (!CBaseTable<PROJECTS, CProjectsAccessor>(TABLE_NAME).DeleteWhereID(lID))
	{
		AfxMessageBox(_T("Error at the m_oProjectsTable.DeleteWhereID() in the application layer"));
		return false;
	}
	return true;
}