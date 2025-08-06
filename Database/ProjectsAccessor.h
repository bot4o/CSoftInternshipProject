#pragma once
#include "Resource.h"
#include "pch.h"
#include "ProjectsStruct.h"
#include "atldbcli.h"

#define PROJECTS_ACCESSORS_COUNT 2 //ID and all others
#define PROJECTS_IDENTITY_ACCESSOR_INDEX 0 //ID index
#define PROJECTS_DATA_ACCESSOR_INDEX 1 //All others first index?
#define PROJECTS_COLUMN_ENTRY_ID 1 // Column index
#define PROJECTS_COLUMN_ENTRY_UPDATE_COUNTER 2  //---//---
#define PROJECTS_COLUMN_ENTRY_NAME 3 
#define PROJECTS_COLUMN_ENTRY_DESCRIPTION 4
#define PROJECTS_COLUMN_ENTRY_PROJECT_MANAGER_ID 5 
#define PROJECTS_COLUMN_ENTRY_STATE 6
#define PROJECTS_COLUMN_ENTRY_TOTAL_EFFORT 7 

/////////////////////////////////////////////////////////////////////////////
// CProjectsAccessor

/// <summary>Клас за достъп до таблицата PROJECTS.</summary>  
class CProjectsAccessor
{
    // Methods
    // ----------------
public:
    /// <summary>GET method за достъп до m_recProject.</summary>  
    PROJECTS& GetRecord()
    {
        return m_recProject;
    }
    // Macros
    // ----------------
protected:
    /// <summary>Макрос дефиниращ публичен достъп до аксесор за определен тип клас.</summary>  
    BEGIN_ACCESSOR_MAP(CProjectsAccessor, PROJECTS_ACCESSORS_COUNT)
        BEGIN_ACCESSOR(PROJECTS_IDENTITY_ACCESSOR_INDEX, true)
        COLUMN_ENTRY(PROJECTS_COLUMN_ENTRY_ID, m_recProject.lId)
        END_ACCESSOR()

        BEGIN_ACCESSOR(PROJECTS_DATA_ACCESSOR_INDEX, true)
            COLUMN_ENTRY(PROJECTS_COLUMN_ENTRY_UPDATE_COUNTER, m_recProject.lUpdateCounter)
            COLUMN_ENTRY(PROJECTS_COLUMN_ENTRY_NAME, m_recProject.szName)
            COLUMN_ENTRY(PROJECTS_COLUMN_ENTRY_DESCRIPTION, m_recProject.szDescription)
            COLUMN_ENTRY(PROJECTS_COLUMN_ENTRY_PROJECT_MANAGER_ID, m_recProject.lProjectManagerId)
            COLUMN_ENTRY(PROJECTS_COLUMN_ENTRY_STATE, m_recProject.bState)
            COLUMN_ENTRY(PROJECTS_COLUMN_ENTRY_TOTAL_EFFORT, m_recProject.sTotalEffort)
        END_ACCESSOR()
    END_ACCESSOR_MAP()

    // Members
    // ----------------
protected:
    /// <summary>PROJECT проментлива за заявките</summary>  
    PROJECTS m_recProject;
};
