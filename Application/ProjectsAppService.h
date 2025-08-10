#pragma once
#include "pch.h"
#include "Resource.h"
#include "ProjectsStruct.h"
#include "TasksStruct.h"
#include "UsersStruct.h"

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
	bool SelectAll(CProjectsTypedPtrArray& oProjectsArray);
	/// <summary>Извежда потребител от базата според ID</summary>  
	bool SelectWhereID(const long lID, PROJECTS& recProject);
	/// <summary>Променя длъжноста на потребител от базата според ID</summary>  
	bool UpdateWhereID(const long lID, PROJECTS& recProject);
	/// <summary>Вмъква нов потребител в базата</summary>  
	bool Insert(PROJECTS& recProject);
	/// <summary>Изтрива потребител от базата според ID</summary>  
	bool DeleteWhereID(const long lID);
};