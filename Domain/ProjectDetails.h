#pragma once
#include "DLLExport.h"
#include "pch.h"
#include "resource.h"
#include "ProjectsStruct.h"
#include "TasksStruct.h"


/////////////////////////////////////////////////////////////////////////////
// CProjectDetails

/// <summary>Таблична структура от данни за таблица TASKS</summary>
class DomainDLL_EXP CProjectDetails
{
    // Constructor / Destructor
    // ----------------
public:
    CProjectDetails(PROJECTS& oProject, CTasksTypedPtrArray& oTasksArray);
    ~CProjectDetails(); 

    // Methods
    // ----------------
    PROJECTS& GetProject();
    CTasksTypedPtrArray& GetTasks();
    // Members
    // ----------------
private:
    PROJECTS& m_oProject;
    CTasksTypedPtrArray& m_oTasksArray;
};
