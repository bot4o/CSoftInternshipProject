#include "pch.h"
#include "ProjectsAndTasksDocument.h"
#include "ProjectsAppService.h"
#include "TasksAppService.h"
#include "DialogModes.h"
#include "UsersStruct.h"


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
bool CProjectsAndTasksDocument::AddProject(PROJECTS& oRecProject)
{
	if (!CProjectsAppService().Insert(oRecProject))
	{
		AfxMessageBox(_T("Error at the CProjectsAppService().Insert() in the document layer"));
		return false;
	}
	m_oProjectsArray.Add(new PROJECTS(oRecProject));
	UpdateAllViews(nullptr, (long)Modes::InsertMode, (CObject*)&oRecProject);
	return true;
}

bool CProjectsAndTasksDocument::LoadAllProjects()
{
	if (!CProjectsAppService().SelectAll(m_oProjectsArray))
	{
		AfxMessageBox(_T("Error at the CProjectsAppService().SelectAll() in the document layer"));
		return false;
	}
	return true;
}

bool CProjectsAndTasksDocument::UpdateProject(const long lID, PROJECTS& oRecProject)
{
	if (!CProjectsAppService().UpdateWhereID(lID, oRecProject))
	{
		AfxMessageBox(_T("Error at the CProjectsAppService().UpdateWhereID() in the document layer"));

		return false;
	}
	UpdateAllViews(nullptr, (long)Modes::UpdateMode, (CObject*)&oRecProject);
	return true;
}

bool CProjectsAndTasksDocument::DeleteProject(const long lID)
{
	if (!CProjectsAppService().DeleteWhereID(lID))
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

bool CProjectsAndTasksDocument::AddTask(TASKS& oRecTask)
{
	if (!CTasksAppService().Insert(oRecTask))
	{
		AfxMessageBox(_T("Error at the CTasksAppService().Insert() in the document layer"));
		return false;
	}
	m_oTasksArray.Add(new TASKS(oRecTask));
	UpdateAllViews(nullptr, (long)Modes::InsertMode, (CObject*)&oRecTask);
	return true;
}

bool CProjectsAndTasksDocument::LoadAllTasks()
{
	if (!CTasksAppService().SelectAll(m_oTasksArray))
	{
		AfxMessageBox(_T("Error at the CTasksAppService().SelectAll() in the document layer"));
		return false;
	}
	return true;
}

bool CProjectsAndTasksDocument::UpdateTask(const long lID, TASKS& oRecTask)
{
	if (!CTasksAppService().UpdateWhereID(lID, oRecTask))
	{
		AfxMessageBox(_T("Error at the CTasksAppService().UpdateWhereID() in the document layer"));

		return false;
	}
	UpdateAllViews(nullptr, (long)Modes::UpdateMode, (CObject*)&oRecTask);
	return true;
}

bool CProjectsAndTasksDocument::DeleteTask(const long lID)
{
	if (!CTasksAppService().DeleteWhereID(lID))
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

bool CProjectsAndTasksDocument::FillUsersArray()
{
	if (!CProjectsAppService().SelectAllUsers(m_oUsersArray))
	{
		return false;
	}
	return true;
}
bool CProjectsAndTasksDocument::FillTasksArray()
{
	if (!CProjectsAppService().SelectAllTasks(m_oTasksArray))
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
	FillUsersArray();
	FillTasksArray();
	if (!LoadAllProjects()) {
		AfxMessageBox(_T("Cound't initialize all users into this document"));
		return FALSE;
	}
	return TRUE;
}