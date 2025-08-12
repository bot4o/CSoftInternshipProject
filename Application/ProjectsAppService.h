#pragma once
#include "pch.h"
#include "../Database/Table.h"
#include "Resource.h"
#include "ProjectDetails.h"
#include "ProjectsStruct.h"
#include "TasksStruct.h"
#include "UsersStruct.h"


/////////////////////////////////////////////////////////////////////////////
// CSampleClass

template<typename TRecord, typename TAccessor>
class ApplicationDLL_EXP CProjectsAppService
{
	// Constructor / Destructor
	// ----------------
public:
	CProjectsAppService(const CString& strTableName) : m_strTable(strTableName) {}
	virtual ~CProjectsAppService() {}

	// Methods
	// ----------------
	/// <summary>Извежда всички потребители в базата</summary>  
	bool SelectAll(CTypedPtrArray<CPtrArray, TRecord*>& oRecordArray)
	{
		if (!CBaseTable<TRecord, TAccessor>(strTableName).SelectAll(oRecordArray))
		{
			AfxMessageBox(_T("Error at the m_oProjectsTable.SelectAll() in the application layer"));
			return false;
		}
		return true;
	}
	/// <summary>Извежда потребител от базата според ID</summary>  
	bool SelectWhereID(const long lID, TRecord& oRecord)
	{
		if (!CBaseTable<TRecord, TAccessor>(strTableName).SelectWhereID(lID, oRecord))
		{
			AfxMessageBox(_T("Error at the m_oProjectsTable.SelectWhereID() in the application layer"));
			return false;
		}
		return true;
	}
	/// <summary>Променя длъжноста на потребител от базата според ID</summary>  
	bool UpdateWhereID(const long lID, TRecord& oRecord)
	{
		if (!CBaseTable<TRecord, TAccessor>(strTableName).UpdateWhereID(lID, oRecord))
		{
			AfxMessageBox(_T("Error at the m_oProjectsTable.UpdateWhereID() in the application layer"));
			return false;
		}
		return true;
	}
	/// <summary>Вмъква нов потребител в базата</summary>  
	bool Insert(TRecord& oRecord)
	{
		if (!CBaseTable<TRecord, TAccessor>(strTableName).Insert(oRecord))
		{
			AfxMessageBox(_T("Error at the m_oProjectsTable.Insert() in the application layer"));
			return false;
		}
		return true;
	}
	/// <summary>Изтрива потребител от базата според ID</summary>  
	bool DeleteWhereID(const long lID) 
	{
		if (!CBaseTable<TRecord, TAccessor>(strTableName).DeleteWhereID(lID))
		{
			AfxMessageBox(_T("Error at the m_oProjectsTable.DeleteWhereID() in the application layer"));
			return false;
		}
		return true;
	}
	}
};