#include "pch.h"
#include "atldbcli.h"
#include "UsersTable.h"

bool CUsersTable::LoadAllUsers(CUsersTypedPtrArray& oUsersArray)
{
	CDataSource oDataSource; // връзка с БД
	CSession oSession; // Сесия

	CDBPropSet oDBPropSet(DBPROPSET_DBINIT);
	oDBPropSet.AddProperty(DBPROP_INIT_DATASOURCE, _T("DESKTOP-94MG5T1"));	// сървър
	oDBPropSet.AddProperty(DBPROP_AUTH_USERID, _T("bobbi"));			// потребител
	//DBPROP_AUTH_INTEGRATED
	oDBPropSet.AddProperty(DBPROP_AUTH_PASSWORD, _T("windows authentication"));// парола // windows authentication
	oDBPropSet.AddProperty(DBPROP_INIT_CATALOG, _T("Internship_DB"));	// база данни
	oDBPropSet.AddProperty(DBPROP_AUTH_PERSIST_SENSITIVE_AUTHINFO, false);
	oDBPropSet.AddProperty(DBPROP_INIT_LCID, 1033L);
	oDBPropSet.AddProperty(DBPROP_INIT_PROMPT, static_cast<short>(4));

	// Свързваме се към базата данни
	HRESULT hResult = oDataSource.Open(_T("SQLOLEDB.1"), &oDBPropSet);
	if (FAILED(hResult))
	{
		AfxMessageBox(_T("Unable to connect to SQL Server database. Error: %d"), hResult);
		return false;
	}

	// Отваряме сесия
	hResult = oSession.Open(oDataSource);
	if (FAILED(hResult))
	{
		AfxMessageBox(_T("Unable to open new session for the SQL Server database. Error: %d", hResult));
		oDataSource.Close();

		return false;
	}

	// Конструираме заявката
	CString strQuery = _T("SELECT * FROM CUSTOMERS");

	// Изпълняваме командата
	hResult = m_oCommand.Open(oSession, strQuery, &oDBPropSet);
	if (FAILED(hResult))
	{
		AfxMessageBox(_T("Unable to open new session for the SQL Server database.Error:% d"), hResult);

		oSession.Close();
		oDataSource.Close();

		return false;
	}
	// Прочитаме всички данни
	while ((hResult = m_oCommand.MoveNext()) == S_OK)
	{
		USERS* pUser = new USERS(m_oCommand.GetRecUser());
		if (pUser != NULL)
		{
			//*pUser = m_oCommand.m_recUser;
			oUsersArray.Add(pUser);
		}

	}
	if (hResult != DB_S_ENDOFROWSET)
	{
		AfxMessageBox(_T("Error reading data from the SQL Server database. Error: %d"), hResult);
		return false;
	}

	// Затваряме командата, сесията и връзката с базата данни. 
	m_oCommand.Close();
	oSession.Close();
	oDataSource.Close();

	return true;
}

bool CUsersTable::UpdateUser(USERS* pUser)
{
	CDataSource oDataSource; // връзка с БД
	CSession oSession; // Сесия

	CDBPropSet oDBPropSet(DBPROPSET_DBINIT);
	oDBPropSet.AddProperty(DBPROP_INIT_DATASOURCE, _T("DESKTOP-94MG5T1"));	// сървър
	oDBPropSet.AddProperty(DBPROP_AUTH_USERID, _T("bobbi"));			// потребител
	//DBPROP_AUTH_INTEGRATED
	oDBPropSet.AddProperty(DBPROP_AUTH_PASSWORD, _T("windows authentication"));// парола // windows authentication
	oDBPropSet.AddProperty(DBPROP_INIT_CATALOG, _T("Internship_DB"));	// база данни
	oDBPropSet.AddProperty(DBPROP_AUTH_PERSIST_SENSITIVE_AUTHINFO, false);
	oDBPropSet.AddProperty(DBPROP_INIT_LCID, 1033L);
	oDBPropSet.AddProperty(DBPROP_INIT_PROMPT, static_cast<short>(4));

	// Свързваме се към базата данни
	HRESULT hResult = oDataSource.Open(_T("SQLOLEDB.1"), &oDBPropSet);
	if (FAILED(hResult))
	{
		AfxMessageBox(_T("Unable to connect to SQL Server database. Error: %d"), hResult);
		return false;
	}

	// 1) lock
	hResult = oSession.StartTransaction();
	if (FAILED(hResult))
	{
		AfxMessageBox(_T("Unable to start new transaction for the SQL Server database. Error: %d", hResult));
		oSession.Close();
		oDataSource.Close();
		return false;
	}

	// Отваряме сесия
	hResult = oSession.Open(oDataSource);
	if (FAILED(hResult))
	{
		AfxMessageBox(_T("Unable to open new session for the SQL Server database. Error: %d", hResult));
		oDataSource.Close();

		return false;
	}

	// Конструираме заявката
	CString strQuery;
	//?
	strQuery.Format(_T("SELECT * FROM CUSTOMERS WITH(...) WHERE ID = %d"), 1);

	// Настройка на типа на Rowset-а
	CDBPropSet oUpdateDBPropSet(DBPROPSET_ROWSET);
	oUpdateDBPropSet.AddProperty(DBPROP_CANFETCHBACKWARDS, true);
	oUpdateDBPropSet.AddProperty(DBPROP_IRowsetScroll, true);
	oUpdateDBPropSet.AddProperty(DBPROP_IRowsetChange, true);
	oUpdateDBPropSet.AddProperty(DBPROP_UPDATABILITY, DBPROPVAL_UP_CHANGE | DBPROPVAL_UP_INSERT | DBPROPVAL_UP_DELETE);

	// Изпълняваме командата
	hResult = m_oCommand.Open(oSession, strQuery, &oUpdateDBPropSet);
	if (FAILED(hResult))
	{
		AfxMessageBox(_T("Unable to execute command in the SQL Server database. Error: %d", hResult));
		oSession.Abort();
		oSession.Close();
		oDataSource.Close();
		return false;
	}

	hResult = m_oCommand.MoveFirst();
	if (FAILED(hResult))
	{
		AfxMessageBox(_T("Unable to Move to the first record for the executed command in the SQL Server database. Error: %d", hResult));
		oSession.Abort();

		m_oCommand.Close();
		oSession.Close();
		oDataSource.Close();

		return false;
	}

	//ako sa ravni update counterite na podadeniq user otvun s toku shto izcheteniq uiser ot bazata danni - prodyljavame s vsichko nadolo. ako ne ( da prekusnem obrabotkata da rowbacknem tranzakciqta i signalizirame na potrebitelq che trqbwa da si prezaredi dannite)
	// ВЪПРОС: Какво стъпки следва да извършим преди да инкрементираме m_lUpdateCounter?
	// 2) check Update counter value != update counter from db

	if (pUser->lUpdateCounter == m_oCommand.GetRecUser().lUpdateCounter)
	{
		long temp = m_oCommand.GetRecUser().lUpdateCounter;
		temp++; // DB

		hResult = m_oCommand.SetData(USERS_DATA_ACCESSOR_INDEX);
		if (FAILED(hResult))
		{
			AfxMessageBox(_T("Unable to set data in the SQL Server database. Error: %d", hResult));
			oSession.Abort();

			m_oCommand.Close();
			oSession.Close();
			oDataSource.Close();

			return false;
		}
	}

	//commit transaction

	oSession.Commit();

	m_oCommand.Close();
	oSession.Close();
	oDataSource.Close();

	return true;
}
