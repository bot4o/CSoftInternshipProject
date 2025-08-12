#pragma once
#include "pch.h"
#include "resource.h"
#include "DLLExport.h"
#include "ProjectsStruct.h"
#include "TasksStruct.h"


/////////////////////////////////////////////////////////////////////////////
// TASKS

/// <summary>Таблична структура от данни за таблица TASKS</summary>
class DomainDLL_EXP CProjectDetails
{
    // Constructor / Destructor
    // ----------------
public:
    CProjectDetails(PROJECTS& oProject, CTasksTypedPtrArray& oTasksArray) : m_oProject(oProject), m_oTasksArray(oTasksArray) {}
    ~CProjectDetails();

    // Methods
    // ----------------
    PROJECTS& GetProject()
    {
        return m_oProject;
    }
    CTasksTypedPtrArray& GetTasks() 
    {
        return m_oTasksArray;
    }
    // Members
    // ----------------
private:
    PROJECTS& m_oProject;
    CTasksTypedPtrArray& m_oTasksArray;
};
