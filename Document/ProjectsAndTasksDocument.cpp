#include "pch.h"
#include "ProjectsAndTasksDocument.h"
#include "../Database/ProjectsAccessor.h"
#include "../Database/TasksAccessor.h"
#include "UsersAppService.h"
#include "ProjectsAppService.h"
#include "DialogModes.h"
#include "UsersStruct.h"

#define TABLE_NAME_PROJECTS _T("PROJECTS")
#define TABLE_NAME_TASKS _T("TASKS")

/////////////////////////////////////////////////////////////////////////////
// CProjectsAndTasksDocument
// 

// Macros
// ----------------
IMPLEMENT_DYNCREATE(CProjectsAndTasksDocument, CDocument)

// Constructor / Destructor
// ----------------
CProjectsAndTasksDocument::CProjectsAndTasksDocument()
{
}

CProjectsAndTasksDocument::~CProjectsAndTasksDocument()
{
}

// Methods
// ----------------
//CRUD

bool CProjectsAndTasksDocument::LoadAllProjects()
{
	if (!CProjectsAppService().SelectAllProjects(m_oProjectsArray))
	{
		AfxMessageBox(_T("Error at the CProjectsAppService().SelectAll() in the document layer"));
		return false;
	}
	return true;
}

bool CProjectsAndTasksDocument::UpdateProject(const long lID, PROJECTS& oRecProject)
{
	if (!CProjectsAppService().UpdateProjectByID(lID, oRecProject))
	{
		AfxMessageBox(_T("Error at the CProjectsAppService().UpdateWhereID() in the document layer"));

		return false;
	}
	UpdateAllViews(nullptr, (long)Modes::UpdateMode, (CObject*)&oRecProject);
	return true;
}

bool CProjectsAndTasksDocument::DeleteProject(const long lID)
{
	if (!CProjectsAppService().DeleteProjectByID(lID))
	{
		AfxMessageBox(_T("Error at the CProjectsAppService().DeleteWhereID() in the document layer"));

		return false;
	}
	for (int i = 0; i < m_oProjectsArray.GetSize(); i++)
	{
		if (lID == m_oProjectsArray[i]->lId)
		{
			m_oProjectsArray.RemoveAt(i);
			break;
		}
	}
	UpdateAllViews(nullptr, long(Modes::DeleteMode), (CObject*)&m_oProjectsArray[0]);
	return true;
}

bool CProjectsAndTasksDocument::LoadAllTasks()
{
	if (!CProjectsAppService().SelectAllTasks(m_oTasksArray))
	{
		AfxMessageBox(_T("Error at the CTasksAppService().SelectAll() in the document layer"));
		return false;
	}
	return true;
}

bool CProjectsAndTasksDocument::UpdateTask(const long lID, TASKS& oRecTask)
{
	if (!CProjectsAppService().UpdateTaskByID(lID, oRecTask))
	{
		AfxMessageBox(_T("Error at the CTasksAppService().UpdateWhereID() in the document layer"));

		return false;
	}
	UpdateAllViews(nullptr, (long)Modes::UpdateMode, (CObject*)&oRecTask);
	return true;
}

bool CProjectsAndTasksDocument::DeleteTask(const long lID)
{
	if (!CProjectsAppService().DeleteTaskByID(lID))
	{
		AfxMessageBox(_T("Error at the CTasksAppService().DeleteWhereID() in the document layer"));

		return false;
	}
	for (int i = 0; i < m_oTasksArray.GetSize(); i++)
	{
		if (lID == m_oTasksArray[i]->lId)
		{
			m_oTasksArray.RemoveAt(i);
			break;
		}
	}
	UpdateAllViews(nullptr, (long)Modes::DeleteMode, (CObject*)&m_oTasksArray[0]);
	return true;
}

CProjectsTypedPtrArray& CProjectsAndTasksDocument::GetProjects()
{
	return m_oProjectsArray;
}

CTasksTypedPtrArray& CProjectsAndTasksDocument::GetTasks()
{
	return m_oTasksArray;
}

CUsersTypedPtrArray& CProjectsAndTasksDocument::GetUsers()
{
	return m_oUsersArray;
}

bool CProjectsAndTasksDocument::LoadAllUsers()
{
	if (!CUsersAppService().SelectAll(m_oUsersArray))
	{
		return false;
	}
	return true;
}

void UpdateAllViews(CView* pSender, LPARAM lHint = 0L, CObject* pHint = NULL)
{

}

// Overrides
// ----------------
BOOL CProjectsAndTasksDocument::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	if (!LoadAllUsers()) {
		AfxMessageBox(_T("Cound't initialize all users into this document"));
		return FALSE;
	}
	if (!LoadAllTasks()) {
		AfxMessageBox(_T("Cound't initialize all users into this document"));
		return FALSE;
	}
	if (!LoadAllProjects()) {
		AfxMessageBox(_T("Cound't initialize all users into this document"));
		return FALSE;
	}
	return TRUE;
}

bool CProjectsAndTasksDocument::AddProjectWithTasks(CProjectDetails& oProjectDetails)
{
	if (!CProjectsAppService().AddProjectWithTasks(oProjectDetails))
	{
		AfxMessageBox(_T("Error at AddProjectWithTasks() in the Document Layer"));
		return false;
	}
	return true;
}
bool CProjectsAndTasksDocument::UpdateProjectWithTasks(const long lProjectID, CProjectDetails& oProjectDetails)
{
	if(!CProjectsAppService().UpdateProjectWithTasks(lProjectID, oProjectDetails))
	{
		AfxMessageBox(_T("Error at AddProjectWithTasks() in the Document Layer"));
		return false;
	}
	return true;
}
