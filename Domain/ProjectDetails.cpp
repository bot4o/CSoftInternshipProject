// CProjectDetails.cpp
#include "pch.h"
#include "ProjectDetails.h"

/////////////////////////////////////////////////////////////////////////////
// CProjectDetails

    // Constructor / Destructor
    // ----------------
CProjectDetails::CProjectDetails(PROJECTS& oProject, CTasksTypedPtrArray& oTasksArray)
    : m_oProject(oProject), m_oTasksArray(oTasksArray)
{

}


CProjectDetails::~CProjectDetails() {
    // Add any cleanup code if needed
}

    // Methods
    // ----------------
PROJECTS& CProjectDetails::GetProject()
{
    return m_oProject;
}
CTasksTypedPtrArray& CProjectDetails::GetTasks()
{
    return m_oTasksArray;
}
