#pragma once
#include "pch.h"
#include "Resource.h"
#include "ProjectsStruct.h"
#include "TasksStruct.h"
#include "UsersStruct.h"
#include "ProjectDetails.h"
#include "SessionManager.h"

/////////////////////////////////////////////////////////////////////////////
// CSampleClass

class ApplicationDLL_EXP CProjectsAppService
{
	// Constructor / Destructor
	// ----------------
public:
	CProjectsAppService();
	virtual ~CProjectsAppService();

	// Methods
	// ----------------
	/// <summary>Извежда всички проекти в базата</summary>  
	bool SelectAllProjects(CProjectsTypedPtrArray& oProjectsArray);
	/// <summary>Извежда проект от базата</summary>  
	bool SelectProjectByID(const long lID, PROJECTS& recProject);
	/// <summary>Променя длъжноста на проект от базата</summary>  
	bool UpdateProjectByID(const long lID, PROJECTS& recProject);
	/// <summary>Вмъква нов проект в базата</summary>  
	bool InsertProject(PROJECTS& recProject);
	/// <summary>Изтрива проект от базата</summary>  
	bool DeleteProjectByID(const long lID);

	/// <summary>Извежда всички задачи от базата</summary>  
	bool SelectAllTasks(CTasksTypedPtrArray& oTasksArray);
	/// <summary>Извежда задача от базата</summary>  
	bool SelectTaskByID(const long lID, TASKS& recTask);
	/// <summary>Променя задача от базата</summary>  
	bool UpdateTaskByID(const long lID, TASKS& recTask);
	/// <summary>Вмъква нова задача в базата</summary>  
	bool InsertTask(TASKS& recTask);
	/// <summary>Изтрива задача от базата</summary>  
	bool DeleteTaskByID(const long lID);

	bool GetProjectTasks(const long lProjectID, CTasksTypedPtrArray& oProjectsTasksArray);

	bool AddProjectWithTasks(CProjectDetails& oProjectDetails);

	bool UpdateProjectWithTasks( const long lProjectID, CProjectDetails& oProjectDetails);

	bool DeleteProjectWithTasks(const long lProjectID, CProjectDetails& oProjectDetails);

private:
	CSessionManager m_oSessionManager;
};