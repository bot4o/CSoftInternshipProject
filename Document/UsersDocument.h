#pragma once
#include "DLLExport.h"
#include "pch.h"
#include "Resource.h"
#include "UsersStruct.h"

/////////////////////////////////////////////////////////////////////////////
// CUsersDocument

class DocumentDLL_EXP CUsersDocument : public CDocument
{
// Macros
// ----------------
	DECLARE_DYNCREATE(CUsersDocument)

// Constructor / Destructor
// ----------------
public:
	CUsersDocument();
	virtual ~CUsersDocument();

// Methods
// ----------------
	/// <summary>Adds a new user to the USERS table</summary>  
	bool AddUser(USERS& recUser);
	/// <summary>Loads all users from the USERS table</summary>  
	bool LoadAllUsers();
	/// <summary>Alters the role of a specific user in the USERS table</summary>  
	bool UpdateUser(const long lID, USERS& recUser);
	/// <summary>Removes a specific user from the USERS table</summary>  
	bool DeleteUser(const long lID);
	/// <summary>Gets the already populated m_oUsersArray for the UsersView</summary>  
	CUsersTypedPtrArray& GetUsers();


// Overrides
// ----------------
	/// <summary>Дава възможност да заредим данните</summary>  
	BOOL OnNewDocument() override;

// Members
// ----------------
private:
	CUsersTypedPtrArray m_oUsersArray;
};