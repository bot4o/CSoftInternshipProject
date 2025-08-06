#include "pch.h"
#include "ProjectsTable.h"

////////////////////////////////////////////////////////////////////////////
// CProjectsTable

// Constructor / Destructor
// ----------------

CProjectsTable::CProjectsTable()
{
}

CProjectsTable::~CProjectsTable()
{
}

// Methods
// ----------------
bool CProjectsTable::SelectAll(CProjectsTypedPtrArray& oProjectsArray)
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
	if (oProjectsArray.GetSize() > 0)
	{
		//if array already has data 
		oProjectsArray.RemoveAll();
	}
	while ((hResult = m_oCommand.MoveNext()) == S_OK)
	{
		PROJECTS* pProject = new PROJECTS(m_oCommand.GetRecProject());
		if (pProject != NULL)
		{
			oProjectsArray.Add(pProject);
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

bool CProjectsTable::SelectWhereID(const long lID, PROJECTS& recProject)
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
		strMessage.Format(_T("Project with ID %d not found"), lID);
		AfxMessageBox(strMessage);
		m_oSession.Abort();
		m_oCommand.Close();
		m_oSession.Close();
		return false;
	}

	recProject = m_oCommand.GetRecProject();

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

bool CProjectsTable::UpdateWhereID(const long lID, PROJECTS& recProject)
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

	PROJECTS& recDatabaseProject = m_oCommand.GetRecProject();
	if (recProject.lUpdateCounter != recDatabaseProject.lUpdateCounter)
	{
		AfxMessageBox(_T("Update counters do not match in the database"));
		m_oSession.Abort();
		m_oCommand.Close();
		m_oSession.Close();
		return false;
	}

	recDatabaseProject = recProject;
	recDatabaseProject.lUpdateCounter += 1;
	recProject.lUpdateCounter += 1;

	hResult = m_oCommand.SetData(PROJECTS_DATA_ACCESSOR_INDEX);
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

bool CProjectsTable::Insert(PROJECTS& recProject)
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

	PROJECTS& oDatabaseProject = m_oCommand.GetRecProject();
	oDatabaseProject.lUpdateCounter = 0;
	_tcscpy_s(oDatabaseProject.szName, recProject.szName);
	_tcscpy_s(oDatabaseProject.szEmail, recProject.szEmail);
	_tcscpy_s(oDatabaseProject.szJobTitle, recProject.szJobTitle);

	hResult = m_oCommand.Insert(PROJECTS_DATA_ACCESSOR_INDEX);
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
	recProject.lId = m_oCommand.GetRecProject().lId;

	m_oCommand.Close();
	m_oSession.Close();
	return true;
}

bool CProjectsTable::DeleteWhereID(const long lID)
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