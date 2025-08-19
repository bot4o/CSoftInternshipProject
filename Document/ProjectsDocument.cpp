#include "pch.h"
#include "ProjectsDocument.h"
#include "../Database/ProjectsAccessor.h"
#include "../Database/TasksAccessor.h"
#include "UsersAppService.h"
#include "ProjectsAppService.h"
#include "DialogModes.h"
#include "UsersStruct.h"

#define TABLE_NAME_PROJECTS _T("PROJECTS")
#define TABLE_NAME_TASKS _T("TASKS")

/////////////////////////////////////////////////////////////////////////////
// CProjectsDocument
// 

// Macros
// ----------------
IMPLEMENT_DYNCREATE(CProjectsDocument, CDocument)

// Constructor / Destructor
// ----------------
CProjectsDocument::CProjectsDocument()
{
}

CProjectsDocument::~CProjectsDocument()
{
}

// Methods
// ----------------
//CRUD

bool CProjectsDocument::LoadAllProjects()
{
	if (!CProjectsAppService().SelectAllProjects(m_oProjectsArray))
	{
		AfxMessageBox(_T("Error at the CProjectsAppService().SelectAll() in the document layer"));
		return false;
	}
	return true;
}

bool CProjectsDocument::UpdateProject(const long lID, PROJECTS& oRecProject)
{
	if (!CProjectsAppService().UpdateProjectByID(lID, oRecProject))
	{
		AfxMessageBox(_T("Error at the CProjectsAppService().UpdateWhereID() in the document layer"));

		return false;
	}
	UpdateAllViews(nullptr, (long)Modes::UpdateMode, (CObject*)&oRecProject);
	return true;
}

bool CProjectsDocument::DeleteProject(const long lID)
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

bool CProjectsDocument::LoadAllTasks()
{
	if (!CProjectsAppService().SelectAllTasks(m_oTasksArray))
	{
		AfxMessageBox(_T("Error at the CTasksAppService().SelectAll() in the document layer"));
		return false;
	}
	return true;
}

bool CProjectsDocument::UpdateTask(const long lID, TASKS& oRecTask)
{
	if (!CProjectsAppService().UpdateTaskByID(lID, oRecTask))
	{
		AfxMessageBox(_T("Error at the CTasksAppService().UpdateWhereID() in the document layer"));

		return false;
	}
	UpdateAllViews(nullptr, (long)Modes::UpdateMode, (CObject*)&oRecTask);
	return true;
}

bool CProjectsDocument::DeleteTask(const long lID)
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

CProjectsTypedPtrArray& CProjectsDocument::GetProjects()
{
	return m_oProjectsArray;
}

CTasksTypedPtrArray& CProjectsDocument::GetTasks()
{
	return m_oTasksArray;
}

CUsersTypedPtrArray& CProjectsDocument::GetUsers()
{
	return m_oUsersArray;
}

bool CProjectsDocument::LoadAllUsers()
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

bool CProjectsDocument::GetProjectTasks(const long lProjectID, CTasksTypedPtrArray& oProjectsTasksArray)
{
	if (!CProjectsAppService().GetProjectTasks(lProjectID, oProjectsTasksArray))
	{
		AfxMessageBox(_T("Error at AddProjectWithTasks() in the Document Layer"));
		return false;
	}
	return true;
}

bool CProjectsDocument::AddProjectWithTasks(CProjectDetails& oProjectDetails)
{
	if (!CProjectsAppService().AddProjectWithTasks(oProjectDetails))
	{
		AfxMessageBox(_T("Error at AddProjectWithTasks() in the Document Layer"));
		return false;
	}
	UpdateAllViews(nullptr, (long)Modes::InsertMode, (CObject*)&oProjectDetails);
	return true;
}
bool CProjectsDocument::UpdateProjectWithTasks(const long lProjectID, CProjectDetails& oProjectDetails)
{
	if (!CProjectsAppService().UpdateProjectWithTasks(lProjectID, oProjectDetails))
	{
		AfxMessageBox(_T("Error at AddProjectWithTasks() in the Document Layer"));
		return false;
	}
	UpdateAllViews(nullptr, (long)Modes::UpdateMode, (CObject*)&oProjectDetails);

	return true;
}
bool CProjectsDocument::DeleteProjectWithTasks(const long lProjectID, CProjectDetails& oProjectDetails)
{
	if (!CProjectsAppService().DeleteProjectWithTasks(lProjectID, oProjectDetails))
	{
		AfxMessageBox(_T("Error at AddProjectWithTasks() in the Document Layer"));
		return false;
	}
	UpdateAllViews(nullptr, (long)Modes::DeleteMode, (CObject*)&oProjectDetails);

	return true;
}

// Overrides
// ----------------
BOOL CProjectsDocument::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	if (!LoadAllUsers()) {
		AfxMessageBox(_T("Cound't initialize all users into this document"));
		return FALSE;
	}
	if (!LoadAllProjects()) {
		AfxMessageBox(_T("Cound't initialize all projects into this document"));
		return FALSE;
	}
	return TRUE;
}