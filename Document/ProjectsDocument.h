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

class DocumentDLL_EXP CProjectsDocument : public CDocument
{
	// Macros
	// ----------------
	DECLARE_DYNCREATE(CProjectsDocument)

	// Constructor / Destructor
	// ----------------
public:
	CProjectsDocument();
	virtual ~CProjectsDocument();

	// Methods
	// ----------------
	CProjectsTypedPtrArray& GetProjects();
	CTasksTypedPtrArray& GetTasks();
	CUsersTypedPtrArray& GetUsers();

	/// <summary>Loads all projects from the PROJECTS table</summary>  
	bool LoadAllProjects();
	/// <summary>Loads all users from the PROJECTS table</summary
	bool LoadAllUsers();

	/// <summary>Метод за взимане на задачите по проект</summary>  
	bool GetProjectTasks(const long lProjectID, CTasksTypedPtrArray& oProjectsTasksArray);
	/// <summary>Метод за добавяне проект и неговите</summary>  
	bool AddProjectWithTasks(CProjectDetails& oProjectDetails);
	/// <summary>Метод за промяна проект и неговите</summary>  
	bool UpdateProjectWithTasks(const long lProjectID, CProjectDetails& oProjectDetails);
	/// <summary>Метод за изтриване проект и неговите</summary>  
	bool DeleteProjectWithTasks(const long lProjectID, CProjectDetails& oProjectDetails);
	
	// Overrides
	// ----------------
public:
	/// <summary>Дава възможност да заредим данните</summary>  
	BOOL OnNewDocument() override;

	// Members
	// ----------------
private:
	/// <summary>Масив с всички проекти от базата</summary>  
	CProjectsTypedPtrArray m_oProjectsArray;
	/// <summary>Масив с всички потребители от базата</summary
	CUsersTypedPtrArray m_oUsersArray;
};