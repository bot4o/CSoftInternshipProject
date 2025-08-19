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

bool CProjectsDocument::LoadAllProjects()
{
	if (!CProjectsAppService().SelectAllProjects(m_oProjectsArray))
	{
		AfxMessageBox(_T("Error at the CProjectsAppService().SelectAll() in the document layer"));
		return false;
	}
	return true;
}

CProjectsTypedPtrArray& CProjectsDocument::GetProjects()
{
	return m_oProjectsArray;
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