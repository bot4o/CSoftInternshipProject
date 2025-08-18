#pragma once
#include "DLLExport.h"
#include "pch.h"
#include "Resource.h"
#include "UsersStruct.h"
#include "ProjectsStruct.h"
#include "TasksStruct.h"
#include "ProjectDetails.h"
#include "AutoCleanArray.h"


/////////////////////////////////////////////////////////////////////////////
// CProjectsDocument

class DocumentDLL_EXP CProjectsAndTasksDocument : public CDocument
{
	// Macros
	// ----------------
	DECLARE_DYNCREATE(CProjectsAndTasksDocument)

	// Constructor / Destructor
	// ----------------
public:
	CProjectsAndTasksDocument();
	virtual ~CProjectsAndTasksDocument();

	// Methods
	// ----------------
	CProjectsTypedPtrArray& GetProjects();
	CTasksTypedPtrArray& GetTasks();
	CUsersTypedPtrArray& GetUsers();

	/// <summary>Loads all projects from the PROJECTS table</summary>  
	bool LoadAllProjects();
	/// <summary>Alters the role of a specific project in the PROJECTS table</summary>  
	bool UpdateProject(const long lID, PROJECTS& recProject);
	/// <summary>Removes a specific project from the PROJECTS table</summary>  
	bool DeleteProject(const long lID);
	/// <summary>Gets the already populated m_oProjectsArray for the ProjectsView</summary>  


	/// <summary>Loads all Task from the TASKS table</summary>  
	bool LoadAllTasks();
	/// <summary>Alters the role of a specific Task in the TASKS Table</summary>  
	bool UpdateTask(const long lID, TASKS& recTask);
	/// <summary>Removes a specific Task from the Task table</summary>  
	bool DeleteTask(const long lID);
	/// <summary>Gets the already populated m_oTasksArray for the TasksView</summary>
	bool AddProjectWithTasks(CProjectDetails& oProjectDetails);

	bool UpdateProjectWithTasks(const long lProjectID, CProjectDetails& oProjectDetails);

	bool DeleteProjectWithTasks(const long lProjectID, CProjectDetails& oProjectDetails);
private:
	bool LoadAllUsers();
	// Overrides
	// ----------------
public:
	/// <summary>Дава възможност да заредим данните</summary>  
	BOOL OnNewDocument() override;

	// Members
	// ----------------
private:
	CProjectsTypedPtrArray m_oProjectsArray;
	CTasksTypedPtrArray m_oTasksArray;
	CUsersTypedPtrArray m_oUsersArray;
};