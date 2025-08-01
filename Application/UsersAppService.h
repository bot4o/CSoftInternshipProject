#pragma once
#include "DLLExport.h"
#include "pch.h"
#include "Resource.h"
#include "UsersStruct.h"
/////////////////////////////////////////////////////////////////////////////
// CSampleClass

class ApplicationDLL_EXP CUsersAppService
{

	// Constants
	// ----------------


	// Constructor / Destructor
	// ----------------
public:
	CUsersAppService();
	virtual ~CUsersAppService();


	// Methods
	// ----------------
	/// <summary>Извежда всички потребители в базата</summary>  
	bool SelectAll(CUsersTypedPtrArray& oUsersArray);
	/// <summary>Извежда потребител от базата според ID</summary>  
	bool SelectWhereID(const long lID, USERS& recUser);
	/// <summary>Променя длъжноста на потребител от базата според ID</summary>  
	bool UpdateWhereID(const long lID, USERS& recUser);
	/// <summary>Вмъква нов потребител в базата</summary>  
	bool Insert(const USERS& recUser);
	/// <summary>Изтрива потребител от базата според ID</summary>  
	bool DeleteWhereID(const long lID);

	// Overrides
	// ----------------


	// Members
	// ----------------
};