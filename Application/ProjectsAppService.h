#pragma once
#include "pch.h"
#include "Resource.h"
#include "ProjectsStruct.h"
#include "TasksStruct.h"
#include "UsersStruct.h"
#include "ProjectDetails.h"

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
	/// <summary>Извежда всички потребители в базата</summary>  
	bool SelectAllProjects(CProjectsTypedPtrArray& oProjectsArray);
	/// <summary>Извежда потребител от базата според ID</summary>  
	bool SelectProjectByID(const long lID, PROJECTS& recProject);
	/// <summary>Променя длъжноста на потребител от базата според ID</summary>  
	bool UpdateProjectByID(const long lID, PROJECTS& recProject);
	/// <summary>Вмъква нов потребител в базата</summary>  
	bool InsertProject(PROJECTS& recProject);
	/// <summary>Изтрива потребител от базата според ID</summary>  
	bool DeleteProjectByID(const long lID);

	bool SelectAllTasks(CTasksTypedPtrArray& oTasksArray);
	/// <summary>Извежда потребител от базата според ID</summary>  
	bool SelectTaskByID(const long lID, TASKS& recTask);
	/// <summary>Променя длъжноста на потребител от базата според ID</summary>  
	bool UpdateTaskByID(const long lID, TASKS& recTask);
	/// <summary>Вмъква нов потребител в базата</summary>  
	bool InsertTask(TASKS& recTask);
	/// <summary>Изтрива потребител от базата според ID</summary>  
	bool DeleteTaskByID(const long lID);

	bool AddProjectWithTasks(CProjectDetails& oProjectDetails);

	bool UpdateProjectWithTasks(const long lProjectID, CProjectDetails& oProjectDetails);

	bool DeleteProjectWithTasks(const long lProjectID, CProjectDetails& oProjectDetails);
};