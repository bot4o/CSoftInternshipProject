#include "pch.h"
#include "TasksTable.h"

////////////////////////////////////////////////////////////////////////////
// CTasksTable

// Constructor / Destructor
// ----------------

CTasksTable::CTasksTable()
{
}

CTasksTable::~CTasksTable()
{
}

// Methods
// ----------------
bool CTasksTable::SelectAll(CTasksTypedPtrArray& oTasksArray)
{
	CDataSource& oDataSource = CDBConnection::GetDataSource();

	//Check for connection because its the first query exacuted;
	if (oDataSource.m_spInit == nullptr)
	{
		CDBConnection::OpenConnection();
	}

	HRESULT hResult = m_oSession.Open(oDataSource);
	if (FAILED(hResult))
	{
		CString strMessage;
		strMessage.Format(_T("Unable to open session for the SQL Server database. HRESULT: 0x%08X"), hResult);
		AfxMessageBox(strMessage);
		return false;
	}

	CString strQuery;
	strQuery.Format(SQL_SELECT_ALL, strTable);

	//само тук се чупи винаги след изпълнение на INSERT
	hResult = m_oCommand.Open(m_oSession, strQuery);
	if (FAILED(hResult))
	{
		CString msg;
		msg.Format(_T("Unable to execute command for the SQL Server database.Error: %d"), hResult);
		AfxMessageBox(msg);
		m_oSession.Close();
		//oDataSource.Close();

		return false;
	}
	if (oTasksArray.GetSize() > 0)
	{
		//if array already has data 
		oTasksArray.RemoveAll();
	}
	while ((hResult = m_oCommand.MoveNext()) == S_OK)
	{
		TASKS* pTask = new TASKS(m_oCommand.GetRecTask());
		if (pTask != NULL)
		{
			oTasksArray.Add(pTask);
		}
	}
	if (hResult != DB_S_ENDOFROWSET)
	{
		CString strMessage;
		strMessage.Format(_T("Error reading data from the SQL Server database. Error: %d"), hResult);
		AfxMessageBox(strMessage);
		return false;
	}

	m_oCommand.Close();
	m_oSession.Close();
	return true;
}

bool CTasksTable::SelectWhereID(const long lID, TASKS& recTask)
{
	CDataSource& oDataSource = CDBConnection::GetDataSource();

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
		return false;
	}

	CString strQuery;
	strQuery.Format(SQL_SELECT_BY_ID, strTable, lID);

	CDBPropSet& oUpdateDBPropSet = CDBConnection::GetDbPropSet();

	hResult = m_oCommand.Open(m_oSession, strQuery, &oUpdateDBPropSet);
	if (FAILED(hResult))
	{
		CString strMessage;
		strMessage.Format(_T("Unable to execute command for the SQL Server database. HRESULT: 0x%08X"), hResult);
		AfxMessageBox(strMessage);
		m_oSession.Abort();
		m_oSession.Close();
		return false;
	}

	hResult = m_oCommand.MoveFirst();
	if (hResult != S_OK)
	{
		CString strMessage;
		strMessage.Format(_T("Task with ID %d not found"), lID);
		AfxMessageBox(strMessage);
		m_oSession.Abort();
		m_oCommand.Close();
		m_oSession.Close();
		return false;
	}

	recTask = m_oCommand.GetRecTask();

	hResult = m_oSession.Commit();
	if (FAILED(hResult))
	{
		CString strMessage;
		strMessage.Format(_T("Failed to commit transaction. Error: %d"), hResult);
		AfxMessageBox(strMessage);
		m_oSession.Abort();
		m_oSession.Close();
		return false;
	}

	m_oCommand.Close();
	m_oSession.Close();
	return true;
}

bool CTasksTable::UpdateWhereID(const long lID, TASKS& recTask)
{
	CDataSource& oDataSource = CDBConnection::GetDataSource();

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
		return false;
	}

	CString sUpdateQuery;
	sUpdateQuery.Format(SQL_SELECT_BY_ID, strTable, lID);

	CDBPropSet& oUpdateDBPropSet = CDBConnection::GetDbPropSet();

	hResult = m_oCommand.Open(m_oSession, sUpdateQuery, &oUpdateDBPropSet);
	if (FAILED(hResult))
	{
		CString strMessage;
		strMessage.Format(_T("Failed to update user record. Error: %d"), hResult);
		AfxMessageBox(strMessage);
		m_oSession.Abort();
		m_oSession.Close();
		return false;
	}
	//Ето тук
	//This move the "cursor" on the position of the first (and only) row of the query result
	hResult = m_oCommand.MoveNext();
	if (hResult == DB_S_ENDOFROWSET)
	{
		AfxMessageBox(_T("No user found with the specified ID to update."));
		m_oSession.Abort();
		m_oCommand.Close();
		m_oSession.Close();
		return true;
	}

	TASKS& recDatabaseTask = m_oCommand.GetRecTask();
	if (recTask.lUpdateCounter != recDatabaseTask.lUpdateCounter)
	{
		AfxMessageBox(_T("Update counters do not match in the database"));
		m_oSession.Abort();
		m_oCommand.Close();
		m_oSession.Close();
		return false;
	}

	recDatabaseTask = recTask;
	recDatabaseTask.lUpdateCounter += 1;
	recTask.lUpdateCounter += 1;

	hResult = m_oCommand.SetData(TASKS_DATA_ACCESSOR_INDEX);
	if (FAILED(hResult))
	{
		AfxMessageBox(_T("Unable to set data in the SQL Server database. Error: %d", hResult));
		m_oSession.Abort();
		m_oCommand.Close();
		m_oSession.Close();
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
		return false;
	}
	m_oCommand.Close();
	m_oSession.Close();
	return true;
}

bool CTasksTable::Insert(TASKS& recTask)
{
	CDataSource& oDataSource = CDBConnection::GetDataSource();

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

	CDBPropSet& oUpdateDBPropSet = CDBConnection::GetDbPropSet();

	hResult = m_oCommand.Open(m_oSession, strQuery, &oUpdateDBPropSet);
	if (FAILED(hResult))
	{
		CString strMessage;
		strMessage.Format(_T("Failed to insert user record. Error: %d"), hResult);
		AfxMessageBox(strMessage);
		m_oCommand.Close();
		m_oSession.Close();
		return false;
	}

	TASKS& oDatabaseTask = m_oCommand.GetRecTask();
	oDatabaseTask.lUpdateCounter = 0;
	_tcscpy_s(oDatabaseTask.szName, recTask.szName);
	_tcscpy_s(oDatabaseTask.szEmail, recTask.szEmail);
	_tcscpy_s(oDatabaseTask.szJobTitle, recTask.szJobTitle);

	hResult = m_oCommand.Insert(TASKS_DATA_ACCESSOR_INDEX);
	if (FAILED(hResult))
	{
		AfxMessageBox(_T("Unable to set data in the SQL Server database. Error: %d", hResult));
		m_oCommand.Close();
		m_oSession.Close();
		return false;
	}

	hResult = m_oCommand.MoveFirst();
	if (hResult != S_OK)
	{
		AfxMessageBox(_T("Unable to read inserted user again from the SQL Server database. Error: %d", hResult));
		m_oCommand.Close();
		m_oSession.Close();
		return true;
	}
	recTask.lId = m_oCommand.GetRecTask().lId;

	m_oCommand.Close();
	m_oSession.Close();
	return true;
}

bool CTasksTable::DeleteWhereID(const long lID)
{
	CDataSource& oDataSource = CDBConnection::GetDataSource();

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
		return false;
	}

	CString strQuery;
	strQuery.Format(SQL_SELECT_BY_ID, strTable, lID);

	CDBPropSet& oUpdateDBPropSet = CDBConnection::GetDbPropSet();

	hResult = m_oCommand.Open(m_oSession, strQuery, &oUpdateDBPropSet);
	if (FAILED(hResult))
	{
		CString strMessage;
		strMessage.Format(_T("Failed to delete user record. Error: %d"), hResult);
		AfxMessageBox(strMessage);
		m_oSession.Abort();
		m_oSession.Close();
		return false;
	}

	hResult = m_oCommand.MoveFirst();
	if (hResult != S_OK)
	{
		AfxMessageBox(_T("No user found with the specified ID"));
		m_oSession.Abort();
		m_oCommand.Close();
		m_oSession.Close();
		return true;
	}
	hResult = m_oCommand.Delete();
	if (FAILED(hResult))
	{
		AfxMessageBox(_T("Failed to delete the user."));
		m_oSession.Abort();
		m_oCommand.Close();
		m_oSession.Close();
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
		return false;
	}
	m_oCommand.Close();
	m_oSession.Close();
	return true;
}