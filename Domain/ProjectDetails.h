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
    CProjectDetails();
    ~CProjectDetails();
    // Members
    // ----------------
    /// <summary>Уникален идентификатор за ред</summary>
    PROJECTS& m_oProject;
    CTasksTypedPtrArray& m_oTasksArray;
};
