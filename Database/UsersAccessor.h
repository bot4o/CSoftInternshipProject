#pragma once
#include "Resource.h"
#include "pch.h"
#include "UsersStruct.h"
#include "atldbcli.h"

#define USERS_ACCESSORS_COUNT 2 //ID and all others
#define USERS_IDENTITY_ACCESSOR_INDEX 0 //ID index
#define USERS_DATA_ACCESSOR_INDEX 1 //All others first index?
#define USERS_COLUMN_ENTRY_ID 1 // Column index
#define USERS_COLUMN_ENTRY_UPDATE_COUNTER 2  //---//---
#define USERS_COLUMN_ENTRY_USER_NAME 3 
#define USERS_COLUMN_ENTRY_USER_EMAIL 4 
#define USERS_COLUMN_ENTRY_JOB_TITLE 5 

/////////////////////////////////////////////////////////////////////////////
// CUsersAccessor

/// <summary>Клас за достъп до таблицата USERS.</summary>  
class CUsersAccessor
{
// Constants
// ----------------


// Constructor / Destructor
// ----------------
public:
    //CUsersAccessor();
    //virtual ~CUsersAccessor();

// Methods
// ----------------
    /// <summary>GET method за достъп до m_recUser.</summary>  
    USERS GetRecUser()  
    {
        return m_recUser;
    }
    /// <summary>SET method за достъп до m_recUser.</summary>  
    bool SetRecUser(USERS oNewUser) 
    {
        m_recUser = oNewUser;
    }

// Overrides
// ----------------


// Members
// ----------------
protected:
    /// <summary>Макрос дефиниращ публичен достъп до аксесор за определен тип клас.</summary>  
    BEGIN_ACCESSOR_MAP(CUsersAccessor, USERS_ACCESSORS_COUNT)
        BEGIN_ACCESSOR(USERS_IDENTITY_ACCESSOR_INDEX, true)
        COLUMN_ENTRY(USERS_COLUMN_ENTRY_ID, m_recUser.lId)
        END_ACCESSOR()

        BEGIN_ACCESSOR(USERS_DATA_ACCESSOR_INDEX, true)
        COLUMN_ENTRY(USERS_COLUMN_ENTRY_UPDATE_COUNTER, m_recUser.lUpdateCounter)
        COLUMN_ENTRY(USERS_COLUMN_ENTRY_USER_NAME, m_recUser.szName)
        COLUMN_ENTRY(USERS_COLUMN_ENTRY_USER_EMAIL, m_recUser.szEmail)
        COLUMN_ENTRY(USERS_COLUMN_ENTRY_JOB_TITLE, m_recUser.szJobTitle)
        END_ACCESSOR()
    END_ACCESSOR_MAP()
protected:
    /// <summary>USER проментлива за заявките</summary>  
    USERS m_recUser;
};
