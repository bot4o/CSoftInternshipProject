#include "pch.h"
#include "ProjectsAppService.h"
#include "ProjectsAccessor.h"
#include "UsersAccessor.h"
#include "TasksAccessor.h"
#include "Table.h"
#include "ProjectDetails.h"


#define TABLE_NAME_PROJECTS _T("PROJECTS")
#define TABLE_NAME_TASKS _T("TASKS")
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
bool CProjectsAppService::SelectAllProjects(CProjectsTypedPtrArray& oProjectsArray)
{
	if (!CBaseTable<PROJECTS, CProjectsAccessor>(TABLE_NAME_PROJECTS).SelectAll(oProjectsArray))
	{
		AfxMessageBox(_T("Error at the m_oProjectsTable.SelectAll() in the application layer"));
		return false;
	}
	return true;
}

/// <summary>Извежда потребител от базата според ID</summary>  
bool CProjectsAppService::SelectProjectByID(const long lID, PROJECTS& recProject)
{
	if (!CBaseTable<PROJECTS, CProjectsAccessor>(TABLE_NAME_PROJECTS).SelectWhereID(lID, recProject))
	{
		AfxMessageBox(_T("Error at the m_oProjectsTable.SelectWhereID() in the application layer"));
		return false;
	}
	return true;
}

/// <summary>Променя длъжноста на потребител от базата според ID</summary>  
bool CProjectsAppService::UpdateProjectByID(const long lID, PROJECTS& recProject)
{
	if (!CBaseTable<PROJECTS, CProjectsAccessor>(TABLE_NAME_PROJECTS).UpdateWhereID(lID, recProject))
	{
		AfxMessageBox(_T("Error at the m_oProjectsTable.UpdateWhereID() in the application layer"));
		return false;
	}
	return true;
}

/// <summary>Вмъква нов потребител в базата</summary>  
bool CProjectsAppService::InsertProject(PROJECTS& recProject)
{
	if (!CBaseTable<PROJECTS, CProjectsAccessor>(TABLE_NAME_PROJECTS).Insert(recProject))
	{
		AfxMessageBox(_T("Error at the m_oProjectsTable.Insert() in the application layer"));
		return false;
	}
	return true;
}

/// <summary>Изтрива потребител от базата според ID</summary>  
bool CProjectsAppService::DeleteProjectByID(const long lID)
{
	if (!CBaseTable<PROJECTS, CProjectsAccessor>(TABLE_NAME_PROJECTS).DeleteWhereID(lID))
	{
		AfxMessageBox(_T("Error at the m_oProjectsTable.DeleteWhereID() in the application layer"));
		return false;
	}
	return true;
}

/// <summary>Извежда всички потребители в базата</summary>  
bool CProjectsAppService::SelectAllTasks(CTasksTypedPtrArray& oTasksArray)
{
	if (!CBaseTable<TASKS, CTasksAccessor>(TABLE_NAME_TASKS).SelectAll(oTasksArray))
	{
		AfxMessageBox(_T("Error at the m_oTasksTable.SelectAll() in the application layer"));
		return false;
	}
	return true;
}

/// <summary>Извежда потребител от базата според ID</summary>  
bool CProjectsAppService::SelectTaskByID(const long lID, TASKS& recTask)
{
	if (!CBaseTable<TASKS, CTasksAccessor>(TABLE_NAME_TASKS).SelectWhereID(lID, recTask))
	{
		AfxMessageBox(_T("Error at the m_oTasksTable.SelectWhereID() in the application layer"));
		return false;
	}
	return true;
}

/// <summary>Променя длъжноста на потребител от базата според ID</summary>  
bool CProjectsAppService::UpdateTaskByID(const long lID, TASKS& recTask)
{
	if (!CBaseTable<TASKS, CTasksAccessor>(TABLE_NAME_TASKS).UpdateWhereID(lID, recTask))
	{
		AfxMessageBox(_T("Error at the m_oTasksTable.UpdateWhereID() in the application layer"));
		return false;
	}
	return true;
}

/// <summary>Вмъква нов потребител в базата</summary>  
bool CProjectsAppService::InsertTask(TASKS& recTask)
{
	if (!CBaseTable<TASKS, CTasksAccessor>(TABLE_NAME_TASKS).Insert(recTask))
	{
		AfxMessageBox(_T("Error at the m_oTasksTable.Insert() in the application layer"));
		return false;
	}
	return true;
}

/// <summary>Изтрива потребител от базата според ID</summary>  
bool CProjectsAppService::DeleteTaskByID(const long lID)
{
	if (!CBaseTable<TASKS, CTasksAccessor>(TABLE_NAME_TASKS).DeleteWhereID(lID))
	{
		AfxMessageBox(_T("Error at the m_oTasksTable.DeleteWhereID() in the application layer"));
		return false;
	}
	return true;
}

bool CProjectsAppService::AddProjectWithTasks(CProjectDetails& oProjectDetails)
{
	PROJECTS& oProject = oProjectDetails.GetProject();
	CTasksTypedPtrArray& oTasksArray = oProjectDetails.GetTasks();

	if (!InsertProject(oProject))
	{
		AfxMessageBox(_T("Error at the m_oTasksTable.InsertProject() in the application layer"));
		return false;
	}
		

	for (int i = 0; i < oTasksArray.GetSize(); ++i)
	{
 		TASKS* pTask = oTasksArray[i];
		pTask->lProjectId = oProject.lId;
		if (!InsertTask(*pTask))
		{
			return false;
		}
	}

	return true;
}
bool CProjectsAppService::UpdateProjectWithTasks(const long lProjectID, CProjectDetails& oProjectDetails)
{
	PROJECTS& oProject = oProjectDetails.GetProject();
	CTasksTypedPtrArray& oTasksArray = oProjectDetails.GetTasks();

	if (!UpdateProjectByID(lProjectID, oProject))
	{
		AfxMessageBox(_T("Error at the m_oTasksTable.UpdateProjectByID() on UpdateProjectWithTasks in the application layer"));
		return false;
	}

	for (int i = 0; i < oTasksArray.GetSize(); ++i)
	{
		TASKS* pTask = oTasksArray[i];
		if (pTask->lId == 0)
		{
			//ID = 0 -> newly created task;
			if (!InsertTask(*pTask))
			{
				AfxMessageBox(_T("Error at the m_oTasksTable.InsertTask() on UpdateProjectWithTasks in the application layer"));
				return false;
			}
				
		}
		long lTaskId = pTask->lId;
		if (!UpdateTaskByID(lTaskId, *pTask))
		{
			AfxMessageBox(_T("Error at the m_oTasksTable.UpdateTaskByID() on UpdateProjectWithTasks in the application layer"));
			return false;
		}
	}
	return true;
}
bool CProjectsAppService::DeleteProjectWithTasks(const long lProjectID, CProjectDetails& oProjectDetails)
{
	PROJECTS& oProject = oProjectDetails.GetProject();
	CTasksTypedPtrArray& oProjectTasksArray = oProjectDetails.GetTasks();

	for (int i = 0; i < oProjectTasksArray.GetSize(); i++)
	{
		if (!CProjectsAppService().DeleteTaskByID(oProjectTasksArray[i]->lId))
		{
			AfxMessageBox(_T("Error at the CTasksAppService().DeleteWhereID() in the document layer"));
			return false;
		}
	}

	if (!CProjectsAppService().DeleteProjectByID(lProjectID))
	{
		AfxMessageBox(_T("Error at the CProjectsAppService().DeleteWhereID() in the document layer"));
		return false;
	}
	return true;
}

