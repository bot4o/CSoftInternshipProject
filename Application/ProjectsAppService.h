#pragma once
#include "pch.h"
#include "../Database/Table.h"
#include "Resource.h"
#include "ProjectDetails.h"
#include "ProjectsStruct.h"
#include "TasksStruct.h"
#include "UsersStruct.h"

#define TABLE_NAME_PROJECTS _T("PROJECTS")
#define TABLE_NAME_TASKS _T("TASKS")
/////////////////////////////////////////////////////////////////////////////
// CSampleClass

template<typename TRecord, typename TAccessor>
class ApplicationDLL_EXP CProjectsAppService
{
	// Constructor / Destructor
	// ----------------
public:
	CProjectsAppService() {}
	CProjectsAppService(const CString& strTableName) : m_strTable(strTableName) {}
	virtual ~CProjectsAppService() {}

	// Methods
	// ----------------
	/// <summary>Извежда всички потребители в базата</summary>  
	bool SelectAll(CTypedPtrArray<CPtrArray, TRecord*>& oRecordArray)
	{
		if (!CBaseTable<TRecord, TAccessor>(m_strTable).SelectAll(oRecordArray))
		{
			AfxMessageBox(_T("Error at the m_oProjectsTable.SelectAll() in the application layer"));
			return false;
		}
		return true;
	}
	/// <summary>Извежда потребител от базата според ID</summary>  
	bool SelectWhereID(const long lID, TRecord& oRecord)
	{
		if (!CBaseTable<TRecord, TAccessor>(m_strTable).SelectWhereID(lID, oRecord))
		{
			AfxMessageBox(_T("Error at the m_oProjectsTable.SelectWhereID() in the application layer"));
			return false;
		}
		return true;
	}
	/// <summary>Променя длъжноста на потребител от базата според ID</summary>  
	bool UpdateWhereID(const long lID, TRecord& oRecord)
	{
		if (!CBaseTable<TRecord, TAccessor>(m_strTable).UpdateWhereID(lID, oRecord))
		{
			AfxMessageBox(_T("Error at the m_oProjectsTable.UpdateWhereID() in the application layer"));
			return false;
		}
		return true;
	}
	/// <summary>Вмъква нов потребител в базата</summary>  
	bool Insert(TRecord& oRecord)
	{
		if (!CBaseTable<TRecord, TAccessor>(m_strTable).Insert(oRecord))
		{
			AfxMessageBox(_T("Error at the m_oProjectsTable.Insert() in the application layer"));
			return false;
		}
		return true;
	}
	/// <summary>Изтрива потребител от базата според ID</summary>  
	bool DeleteWhereID(const long lID) 
	{
		if (!CBaseTable<TRecord, TAccessor>(m_strTable).DeleteWhereID(lID))
		{
			AfxMessageBox(_T("Error at the m_oProjectsTable.DeleteWhereID() in the application layer"));
			return false;
		}
		return true;
	}

	static bool AddProjectWithTasks(CProjectDetails& oProjectDetails)
	{

		/*PROJECTS oProject = oProjectDetails.GetProject();
		CTasksTypedPtrArray& oTasksArray = oProjectDetails.GetTasks();*/
		PROJECTS oProject = PROJECTS();

		if (!CProjectsAppService<PROJECTS, CProjectsAccessor>(TABLE_NAME_PROJECTS).Insert(oProject))
		{
			AfxMessageBox(_T("Couldn't insert Project at appservice layer"));
			return false;
		}

		CTasksTypedPtrArray oTasksArray = CTasksTypedPtrArray();
		for (int i = 0; i < oTasksArray.GetSize(); ++i)
		{
			TASKS* pTask = oTasksArray[i];
			if (!CProjectsAppService<TASKS, CTasksAccessor>(TABLE_NAME_TASKS).Insert(*pTask))
			{
				AfxMessageBox(_T("Couldn't insert Task at appservice layer"));
				return false;
			}
		}

		return true;

	}
	static bool UpdateProjectWithTasks(const long lProjectID, CProjectDetails& oProjectDetails)
	{
		PROJECTS& oProject = oProjectDetails.GetProject();
		CTasksTypedPtrArray& oTasksArray = oProjectDetails.GetTasks();

		if (!CProjectsAppService<PROJECTS, CProjectsAccessor>(TABLE_NAME_PROJECTS).UpdateWhereID(lProjectID, oProject))
		{
			AfxMessageBox(_T("Couldn't update Project at appservice layer"));
			return false;
		}

		for (int i = 0; i < oTasksArray.GetSize(); ++i)
		{
			TASKS* pTask = oTasksArray[i];
			if (!CProjectsAppService<TASKS, CTasksAccessor>(TABLE_NAME_TASKS).UpdateWhereID(pTask->lId, *pTask))
			{
				AfxMessageBox(_T("Couldn't update Task at appservice layer"));
				return false;
			}
		}

		return true;

	}

	// Members
	// ----------------
	CString m_strTable;
};