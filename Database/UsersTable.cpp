﻿#include "pch.h"
#include "UsersTable.h"

////////////////////////////////////////////////////////////////////////////
// CUsersTable

// Constructor / Destructor
// ----------------

CUsersTable::CUsersTable()
{
}

CUsersTable::~CUsersTable()
{
}

// Methods
// ----------------
bool CUsersTable::SelectAll(CUsersTypedPtrArray& oUsersArray)
{
	CDataSource oDataSource = CDBConnection::GetDataSource();

	HRESULT hResult = m_oSession.Open(oDataSource);
	if (FAILED(hResult))
	{
		CString strMessage;
		strMessage.Format(_T("Unable to open session for the SQL Server database. HRESULT: 0x%08X"), hResult);
		AfxMessageBox(strMessage);
		return false;
	}

	CString strQuery;
	strQuery.Format(SQL_SELECT_ALL_NOLOCK, strTable);

	hResult = m_oCommand.Open(m_oSession, strQuery);
	if (FAILED(hResult))
	{
		CString msg;
		msg.Format(_T("Unable to execute command for the SQL Server database.Error: %d"), hResult);
		AfxMessageBox(msg);
		m_oSession.Close();
		oDataSource.Close();

		return false;
	}

	while ((hResult = m_oCommand.MoveNext()) == S_OK)
	{
		USERS* pUser = new USERS(m_oCommand.GetRecUser());
		if (pUser != NULL)
		{
			oUsersArray.Add(pUser);
		}
	}
	if (hResult != DB_S_ENDOFROWSET)
	{
		CString strMessage;
		strMessage.Format(_T("Error reading data from the SQL Server database. Error: %d"), hResult);
		AfxMessageBox(strMessage);
		AfxMessageBox(_T(""), hResult);
		return false;
	}

	m_oCommand.Close();
	m_oSession.Close();
	oDataSource.Close();

	return true;
}
bool CUsersTable::SelectWhereID(const long lID, USERS& recUser)
{
	CDataSource oDataSource = CDBConnection::GetDataSource();

	HRESULT hResult = m_oSession.Open(oDataSource);
	if (FAILED(hResult))
	{
		CString strMessage;
		strMessage.Format(_T("Unable to open session for the SQL Server database. HRESULT: 0x%08X"), hResult);
		AfxMessageBox(strMessage);
		return false;
	}

	hResult = m_oSession.StartTransaction();
	if (FAILED(hResult))
	{
		AfxMessageBox(_T("Unable to start new transaction for the SQL Server database. Error: %d", hResult));
		m_oSession.Close();
		oDataSource.Close();
		return false;
	}

	CString strQuery;
	strQuery.Format(SQL_SELECT_BY_ID, strTable, lID);

	CDBPropSet oUpdateDBPropSet(DBPROPSET_ROWSET);
	oUpdateDBPropSet.AddProperty(DBPROP_CANFETCHBACKWARDS, true);
	oUpdateDBPropSet.AddProperty(DBPROP_IRowsetScroll, true);
	oUpdateDBPropSet.AddProperty(DBPROP_IRowsetChange, true);
	oUpdateDBPropSet.AddProperty(DBPROP_UPDATABILITY, DBPROPVAL_UP_CHANGE | DBPROPVAL_UP_INSERT | DBPROPVAL_UP_DELETE);

	hResult = m_oCommand.Open(m_oSession, strQuery, &oUpdateDBPropSet);
	if (FAILED(hResult))
	{
		CString strMessage;
		strMessage.Format(_T("Unable to execute command for the SQL Server database. HRESULT: 0x%08X"), hResult);

		m_oSession.Abort();
		m_oSession.Close();
		oDataSource.Close();

		AfxMessageBox(strMessage);
		return false;
	}
	
	hResult = m_oCommand.MoveFirst();
	if (hResult != S_OK)
	{
		CString strMessage;
		strMessage.Format(_T("User with ID %d not found"), lID);
		AfxMessageBox(strMessage);
		m_oSession.Abort();
		m_oCommand.Close();
		m_oSession.Close();
		oDataSource.Close();

		return false;
	}
	recUser = m_oCommand.GetRecUser();


	hResult = m_oSession.Commit();
	if (FAILED(hResult))
	{
		CString strMessage;
		strMessage.Format(_T("Failed to commit transaction. Error: %d"), hResult);
		AfxMessageBox(strMessage);
		m_oSession.Abort();
		m_oSession.Close();
		oDataSource.Close();
		return false;
	}	m_oCommand.Close();
	m_oSession.Close();
	oDataSource.Close();
	return true;
}
bool CUsersTable::UpdateWhereID(const long lID, USERS& recUser) 
{
	CDataSource oDataSource = CDBConnection::GetDataSource();

	HRESULT hResult = m_oSession.Open(oDataSource);
	if (FAILED(hResult))
	{
		CString strMessage;
		strMessage.Format(_T("Unable to open session for the SQL Server database. HRESULT: 0x%08X"), hResult);
		AfxMessageBox(strMessage);
		return false;
	}

	hResult = m_oSession.StartTransaction();
	if (FAILED(hResult))
	{
		AfxMessageBox(_T("Unable to start new transaction for the SQL Server database. Error: %d", hResult));
		m_oSession.Close();
		oDataSource.Close();
		return false;
	}

	CString sUpdateQuery;
	sUpdateQuery.Format(SQL_SELECT_BY_ID, strTable, lID);

	CDBPropSet oUpdateDBPropSet(DBPROPSET_ROWSET);
	oUpdateDBPropSet.AddProperty(DBPROP_CANFETCHBACKWARDS, true);
	oUpdateDBPropSet.AddProperty(DBPROP_IRowsetScroll, true);
	oUpdateDBPropSet.AddProperty(DBPROP_IRowsetChange, true);
	oUpdateDBPropSet.AddProperty(DBPROP_UPDATABILITY, DBPROPVAL_UP_CHANGE | DBPROPVAL_UP_INSERT | DBPROPVAL_UP_DELETE);

	hResult = m_oCommand.Open(m_oSession, sUpdateQuery, &oUpdateDBPropSet);
	if (FAILED(hResult))
	{
		CString strMessage;
		strMessage.Format(_T("Failed to update user record. Error: %d"), hResult);
		AfxMessageBox(strMessage);
		m_oSession.Abort();
		m_oSession.Close();
		oDataSource.Close();
		return false;
	}
	USERS& recDatabaseUser = m_oCommand.GetRecUser();
	if (recUser.lUpdateCounter != recDatabaseUser.lUpdateCounter)
	{
		AfxMessageBox(_T("Update counters do not match in the database"));
		m_oSession.Abort();
		m_oCommand.Close();
		m_oSession.Close();
		oDataSource.Close();
		return false;
	}

	recDatabaseUser = recUser;
	recDatabaseUser.lUpdateCounter += 1;

	hResult = m_oCommand.SetData(USERS_DATA_ACCESSOR_INDEX);
	if (FAILED(hResult))
	{
		AfxMessageBox(_T("Unable to set data in the SQL Server database. Error: %d", hResult));
		m_oSession.Abort();
		m_oCommand.Close();
		m_oSession.Close();
		oDataSource.Close();
		return false;
	}

	hResult = m_oSession.Commit();
	if (FAILED(hResult))
	{
		CString strMessage;
		strMessage.Format(_T("Failed to commit transaction. Error: %d"), hResult);
		AfxMessageBox(strMessage);
		m_oSession.Abort();
		m_oSession.Close();
		oDataSource.Close();
		return false;
	}	m_oCommand.Close();
	m_oSession.Close();
	oDataSource.Close();
	return true;
}
bool CUsersTable::Insert(const USERS& recUser) 
{
	CDataSource oDataSource = CDBConnection::m_oDataSource;

	HRESULT hResult = m_oSession.Open(oDataSource);
	if (FAILED(hResult))
	{
		CString strMessage;
		strMessage.Format(_T("Unable to open session for the SQL Server database. HRESULT: 0x%08X"), hResult);
		AfxMessageBox(strMessage);
		return false;
	}

	CString strQuery;
	strQuery.Format(SQL_SELECT_EMPTY, strTable);

	CDBPropSet oUpdateDBPropSet(DBPROPSET_ROWSET);
	oUpdateDBPropSet.AddProperty(DBPROP_CANFETCHBACKWARDS, true);
	oUpdateDBPropSet.AddProperty(DBPROP_IRowsetScroll, true);
	oUpdateDBPropSet.AddProperty(DBPROP_IRowsetChange, true);
	oUpdateDBPropSet.AddProperty(DBPROP_UPDATABILITY, DBPROPVAL_UP_CHANGE | DBPROPVAL_UP_INSERT | DBPROPVAL_UP_DELETE);

	hResult = m_oCommand.Open(m_oSession, strQuery, &oUpdateDBPropSet);
	if (FAILED(hResult))
	{
		CString strMessage;
		strMessage.Format(_T("Failed to insert user record. Error: %d"), hResult);
		AfxMessageBox(strMessage);
		m_oCommand.Close();
		m_oSession.Close();
		oDataSource.Close();
		return false;
	}

	USERS& oDatabaseUser = m_oCommand.GetRecUser();
	oDatabaseUser.lUpdateCounter = 0;
	_tcscpy_s(oDatabaseUser.szName, recUser.szName);
	_tcscpy_s(oDatabaseUser.szEmail, recUser.szEmail);
	_tcscpy_s(oDatabaseUser.szJobTitle, recUser.szJobTitle);

	hResult = m_oCommand.Insert(USERS_DATA_ACCESSOR_INDEX);

	if (FAILED(hResult))
	{
		AfxMessageBox(_T("Unable to set data in the SQL Server database. Error: %d", hResult));

		m_oCommand.Close();
		m_oSession.Close();
		oDataSource.Close();
		return false;
	}

	m_oSession.Close();
	oDataSource.Close();
	return true;
}
bool CUsersTable::DeleteWhereID(const long lID) 
{
	CDataSource oDataSource = CDBConnection::GetDataSource();

	HRESULT hResult = m_oSession.Open(oDataSource);
	if (FAILED(hResult))
	{
		CString strMessage;
		strMessage.Format(_T("Unable to open session for the SQL Server database. HRESULT: 0x%08X"), hResult);
		AfxMessageBox(strMessage);
		return false;
	}

	hResult = m_oSession.StartTransaction();
	if (FAILED(hResult))
	{
		AfxMessageBox(_T("Unable to start new transaction for the SQL Server database. Error: %d", hResult));
		m_oSession.Close();
		oDataSource.Close();
		return false;
	}

	CString strQuery;
	strQuery.Format(SQL_SELECT_BY_ID, strTable,  lID);

	hResult = m_oCommand.Open(m_oSession, strQuery);
	if (FAILED(hResult))
	{
		CString strMessage;
		strMessage.Format(_T("Failed to delete user record. Error: %d"), hResult);
		AfxMessageBox(strMessage);
		m_oSession.Abort();
		m_oSession.Close();
		oDataSource.Close();
		return false;
	}

	hResult = m_oCommand.MoveNext();
	if (hResult != S_OK) 
	{
		AfxMessageBox(_T("No user found with the specified ID to delete."));
		m_oCommand.Close();
		m_oSession.Abort();
		m_oSession.Close();
		oDataSource.Close();
		return true;
	}
	m_oCommand.Delete();

	hResult = m_oSession.Commit();
	if (FAILED(hResult)) 
	{
		CString strMessage;
		strMessage.Format(_T("Failed to commit transaction. Error: %d"), hResult);
		AfxMessageBox(strMessage);
		m_oSession.Abort();
		m_oSession.Close();
		oDataSource.Close();
		return false;
	}
	m_oCommand.Close();
	m_oSession.Close();
	oDataSource.Close();
	return true;
}

// Overrides
// ----------------