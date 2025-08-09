#pragma once
#include "pch.h"
#include "Resource.h"
#include "TasksStruct.h"

/////////////////////////////////////////////////////////////////////////////
// CSampleClass

class ApplicationDLL_EXP CTasksAppService
{
	// Constructor / Destructor
	// ----------------
public:
	CTasksAppService();
	virtual ~CTasksAppService();

	// Methods
	// ----------------
	/// <summary>Извежда всички потребители в базата</summary>  
	bool SelectAll(CTasksTypedPtrArray& oTasksArray);
	/// <summary>Извежда потребител от базата според ID</summary>  
	bool SelectWhereID(const long lID, TASKS& recTask);
	/// <summary>Променя длъжноста на потребител от базата според ID</summary>  
	bool UpdateWhereID(const long lID, TASKS& recTask);
	/// <summary>Вмъква нов потребител в базата</summary>  
	bool Insert(TASKS& recTask);
	/// <summary>Изтрива потребител от базата според ID</summary>  
	bool DeleteWhereID(const long lID);
};