#pragma once

#include "Resource.h"  
#include "pch.h"  
#include "atldbcli.h"
#include "ProjectsStruct.h"  
#include "ProjectsAccessor.h"
#include "DBConnection.h"
#include "DLLExport.h"

#define TABLE_PROJECTS _T("PROJECTS")
#define SQL_SELECT_ALL_NOLOCK _T("SELECT * FROM %s WITH (NOLOCK)")
#define SQL_SELECT_ALL _T("SELECT * FROM %s")
#define SQL_SELECT_BY_ID _T("SELECT * FROM %s WITH (ROWLOCK, UPDLOCK, HOLDLOCK) WHERE ID = %d")
#define SQL_SELECT_EMPTY _T("SELECT TOP 0 * FROM %s")


/////////////////////////////////////////////////////////////////////////////
// CProjectsTable

/// <summary>Клас за работа с таблица PROJECTS</summary>  
class DatabaseDLL_EXP CProjectsTable
{

    // Constructor / Destructor
    // ----------------
public:
    CProjectsTable();
    virtual ~CProjectsTable();

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

    // Members
    // ----------------
private:
    /// <summary>Помага да изпълним команди по сесията</summary>  
    CCommand<CAccessor<CProjectsAccessor>> m_oCommand;
    // <summary>От коя таблица ще се извличат данни в заявките</summary>
    CString strTable = TABLE_PROJECTS;
    /// <summary>Самата сесия</summary
    CSession m_oSession;
};
