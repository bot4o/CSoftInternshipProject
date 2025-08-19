#include "pch.h"
#include "ProjectsAppService.h"
#include "ProjectsAccessor.h"
#include "UsersAccessor.h"
#include "TasksAccessor.h"
#include "BaseTable.h"
#include "ProjectDetails.h"
#include "SessionManager.h"


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
	if (!CBaseTable<PROJECTS, CProjectsAccessor>(&m_oSessionManager, TABLE_NAME_PROJECTS).SelectAll(oProjectsArray))
	{
		AfxMessageBox(_T("Error at the m_oProjectsTable.SelectAll() in the application layer"));
		return false;
	}
	return true;
}

/// <summary>Извежда потребител от базата според ID</summary>  
bool CProjectsAppService::SelectProjectByID(const long lID, PROJECTS& recProject)
{
	if (!CBaseTable<PROJECTS, CProjectsAccessor>(&m_oSessionManager, TABLE_NAME_PROJECTS).SelectWhereID(lID, recProject))
	{
		AfxMessageBox(_T("Error at the m_oProjectsTable.SelectWhereID() in the application layer"));
		return false;
	}
	return true;
}

/// <summary>Променя длъжноста на потребител от базата според ID</summary>  
bool CProjectsAppService::UpdateProjectByID(const long lID, PROJECTS& recProject)
{
	if (!CBaseTable<PROJECTS, CProjectsAccessor>(&m_oSessionManager, TABLE_NAME_PROJECTS).UpdateWhereID(lID, recProject))
	{
		AfxMessageBox(_T("Error at the m_oProjectsTable.UpdateWhereID() in the application layer"));
		return false;
	}
	return true;
}

/// <summary>Вмъква нов потребител в базата</summary>  
bool CProjectsAppService::InsertProject(PROJECTS& recProject)
{
	if (!CBaseTable<PROJECTS, CProjectsAccessor>(&m_oSessionManager, TABLE_NAME_PROJECTS).Insert(recProject))
	{
		AfxMessageBox(_T("Error at the m_oProjectsTable.Insert() in the application layer"));
		return false;
	}
	return true;
}

/// <summary>Изтрива потребител от базата според ID</summary>  
bool CProjectsAppService::DeleteProjectByID(const long lID)
{
	if (!CBaseTable<PROJECTS, CProjectsAccessor>(&m_oSessionManager, TABLE_NAME_PROJECTS).DeleteWhereID(lID))
	{
		AfxMessageBox(_T("Error at the m_oProjectsTable.DeleteWhereID() in the application layer"));
		return false;
	}
	return true;
}

/// <summary>Извежда всички потребители в базата</summary>  
bool CProjectsAppService::SelectAllTasks(CTasksTypedPtrArray& oTasksArray)
{
	if (!CBaseTable<TASKS, CTasksAccessor>(&m_oSessionManager, TABLE_NAME_TASKS).SelectAll(oTasksArray))
	{
		AfxMessageBox(_T("Error at the m_oTasksTable.SelectAll() in the application layer"));
		return false;
	}
	return true;
}

/// <summary>Извежда потребител от базата според ID</summary>  
bool CProjectsAppService::SelectTaskByID(const long lID, TASKS& recTask)
{
	if (!CBaseTable<TASKS, CTasksAccessor>(&m_oSessionManager, TABLE_NAME_TASKS).SelectWhereID(lID, recTask))
	{
		AfxMessageBox(_T("Error at the m_oTasksTable.SelectWhereID() in the application layer"));
		return false;
	}
	return true;
}

/// <summary>Променя длъжноста на потребител от базата според ID</summary>  
bool CProjectsAppService::UpdateTaskByID(const long lID, TASKS& recTask)
{
	if (!CBaseTable<TASKS, CTasksAccessor>(&m_oSessionManager, TABLE_NAME_TASKS).UpdateWhereID(lID, recTask))
	{
		AfxMessageBox(_T("Error at the m_oTasksTable.UpdateWhereID() in the application layer"));
		return false;
	}
	return true;
}

/// <summary>Вмъква нов потребител в базата</summary>  
bool CProjectsAppService::InsertTask(TASKS& recTask)
{
	if (!CBaseTable<TASKS, CTasksAccessor>(&m_oSessionManager, TABLE_NAME_TASKS).Insert(recTask))
	{
		AfxMessageBox(_T("Error at the m_oTasksTable.Insert() in the application layer"));
		return false;
	}
	return true;
}

/// <summary>Изтрива потребител от базата според ID</summary>  
bool CProjectsAppService::DeleteTaskByID(const long lID)
{
	if (!CBaseTable<TASKS, CTasksAccessor>(&m_oSessionManager, TABLE_NAME_TASKS).DeleteWhereID(lID))
	{
		AfxMessageBox(_T("Error at the m_oTasksTable.DeleteWhereID() in the application layer"));
		return false;
	}
	return true;
}

bool CProjectsAppService::AddProjectWithTasks(CProjectDetails& oProjectDetails)
{
	HRESULT hResult = m_oSessionManager.BeginTransaction();
	if (FAILED(hResult))
	{
		CString strMessage;
		strMessage.Format(_T("Failed to start transaction. Error: %d", hResult));
		AfxMessageBox(strMessage);
		m_oSessionManager.CloseSession();
		return false;
	}

	PROJECTS& oProject = oProjectDetails.GetProject();
	CTasksTypedPtrArray& oTasksArray = oProjectDetails.GetTasks();

	if (!InsertProject(oProject))
	{
		AfxMessageBox(_T("Error at the m_oTasksTable.InsertProject() in the application layer"));
		//m_oSessionManager.RollbackTransaction();
		return false;
	}

	for (int i = 0; i < oTasksArray.GetSize(); ++i)
	{
		TASKS* pTask = oTasksArray[i];
		pTask->lProjectId = oProject.lId;
		if (!InsertTask(*pTask))
		{
			AfxMessageBox(_T("Error inserting task"));
			//m_oSessionManager.RollbackTransaction();
			return false;
		}
	}

	hResult = m_oSessionManager.CommitTransaction();
	if (FAILED(hResult))
	{
		CString strMessage;
		strMessage.Format(_T("Failed to commit transaction. At AddProjectWithTasks Error: %d", hResult));
		AfxMessageBox(strMessage);
		m_oSessionManager.CloseSession();
		return false;
	}
	return true;
}
bool CProjectsAppService::UpdateProjectWithTasks(const long lProjectID, CProjectDetails& oProjectDetails)
{
	PROJECTS& oProject = oProjectDetails.GetProject();
	CTasksTypedPtrArray& oTasksArray = oProjectDetails.GetTasks();

	HRESULT hResult = m_oSessionManager.BeginTransaction();
	if (FAILED(hResult))
	{
		CString strMessage;
		strMessage.Format(_T("Failed to begin transaction. Error: %d", hResult));
		AfxMessageBox(strMessage);
		m_oSessionManager.CloseSession();
		return false;
	}

	if (!UpdateProjectByID(lProjectID, oProject))
	{
		m_oSessionManager.RollbackTransaction();
		AfxMessageBox(_T("Error at the m_oTasksTable.UpdateProjectByID() on UpdateProjectWithTasks in the application layer"));
		return false;
	}

	for (int i = 0; i < oTasksArray.GetSize(); ++i)
	{
		TASKS* pTask = oTasksArray[i];
		if (pTask->lId == 0)
		{
			if (!InsertTask(*pTask))
			{
				m_oSessionManager.RollbackTransaction();
				AfxMessageBox(_T("Error at the m_oTasksTable.InsertTask() on UpdateProjectWithTasks in the application layer"));
				return false;
			}

		}
		long lTaskId = pTask->lId;
		if (!UpdateTaskByID(lTaskId, *pTask))
		{
			m_oSessionManager.RollbackTransaction();
			AfxMessageBox(_T("Error at the m_oTasksTable.UpdateTaskByID() on UpdateProjectWithTasks in the application layer"));
			return false;
		}
	}
	hResult = m_oSessionManager.CommitTransaction();
	if (FAILED(hResult))
	{
		CString strMessage;
		strMessage.Format(_T("Failed to commit transaction. Error: %d", hResult));
		AfxMessageBox(strMessage);
		m_oSessionManager.CloseSession();
		return false;
	}

	return true;
}
bool CProjectsAppService::DeleteProjectWithTasks(const long lProjectID, CProjectDetails& oProjectDetails)
{
	PROJECTS& oProject = oProjectDetails.GetProject();
	CTasksTypedPtrArray& oProjectTasksArray = oProjectDetails.GetTasks();

	HRESULT hResult = m_oSessionManager.BeginTransaction();
	if (FAILED(hResult))
	{
		CString strMessage;
		strMessage.Format(_T("Failed to begin transaction. Error: %d", hResult));
		AfxMessageBox(strMessage);
		m_oSessionManager.CloseSession();
		return false;
	}

	for (int i = 0; i < oProjectTasksArray.GetSize(); i++)
	{
		if (!DeleteTaskByID(oProjectTasksArray[i]->lId))
		{
			m_oSessionManager.RollbackTransaction();
			AfxMessageBox(_T("Error at the CTasksAppService().DeleteWhereID() in the document layer"));
			return false;
		}
	}

	if (!DeleteProjectByID(lProjectID))
	{
		m_oSessionManager.RollbackTransaction();
		AfxMessageBox(_T("Error at the CProjectsAppService().DeleteWhereID() in the document layer"));
		return false;
	}

	hResult = m_oSessionManager.CommitTransaction();
	if (FAILED(hResult))
	{
		CString strMessage;
		strMessage.Format(_T("Failed to commit transaction. Error: %d", hResult));
		AfxMessageBox(strMessage);
		m_oSessionManager.CloseSession();
		return false;
	}

	return true;
}

bool CProjectsAppService::GetProjectTasks(const long lProjectID, CTasksTypedPtrArray& oProjectsTasksArray)
{
	CTasksTypedPtrArray oTasksArray;

	if (!SelectAllTasks(oTasksArray))
	{
		AfxMessageBox(_T("Error at retriving all tasks"));
		return false;

	}
	for (int i = 0; i < oTasksArray.GetSize(); i++)
	{
		if (oTasksArray[i]->lProjectId == lProjectID)
		{
			TASKS* oTask = new TASKS(*oTasksArray[i]);
			oProjectsTasksArray.Add(oTask);
		}
	}

	return true;
}
