#pragma once
#include "pch.h"  
#include "Resource.h"  
#include "atldbcli.h"
#include "../Database/DBConnection.h"
#include "DLLExport.h"

/////////////////////////////////////////////////////////////////////////////
// CSessionManager

class CSessionManager
{
	// Constructor / Destructor
	// ---------------- 
public:
	CSessionManager() : m_bInTransaction(false)
	{
		OpenSession();
	}
	virtual ~CSessionManager()
	{
		CloseSession();
	}

	// Methods
	// ----------------
	///<summary>Проверява връзката за базата и отваря нова сесия</summary>
	bool OpenSession()
	{
		if (m_oSession.m_spOpenRowset == NULL)
		{
			CDataSource& oDataSource = CDBConnection::GetDataSource();
			if (oDataSource.m_spInit == nullptr)
			{
				if (!CDBConnection::OpenConnection())
				{
					AfxMessageBox(_T("Failed to initialize data source."));
					return false;
				}
			}

			HRESULT hResult = m_oSession.Open(oDataSource);
			if (FAILED(hResult))
			{
				CString strMessage;
				strMessage.Format(_T("Unable to open session. HRESULT: 0x%08X"), hResult);
				AfxMessageBox(strMessage);
				return false;
			}
		}

        return true;
	}
	///<summary>Затваря сесията</summary>
	void CloseSession()
	{
		if (m_oSession.m_spOpenRowset != NULL)
		{
			m_oSession.Close();
		}
	}
	///<summary>Стартира танзакцията</summary>
	bool BeginTransaction()
	{
		if (m_bInTransaction)
		{
			return false;
		}
		if (!OpenSession())
		{
			return false;
		}
		HRESULT hResult = m_oSession.StartTransaction();
		if (FAILED(hResult))
		{
			CString strMessage;
			strMessage.Format(_T("Unable to start transaction. HRESULT: 0x%08X"), hResult);
			AfxMessageBox(strMessage);
		}
		m_bInTransaction = true;
		return true;
	}
	///<summary>Прекъсва танзакцията</summary>
	bool RollbackTransaction()
	{
		if (!m_bInTransaction)
		{
			return false;
		}
		if (!OpenSession())
		{
			return false;
		}
		HRESULT hResult = m_oSession.Abort();
		if (FAILED(hResult))
		{
			CString strMessage;
			strMessage.Format(_T("Unable to abort transaction. HRESULT: 0x%08X"), hResult);
			AfxMessageBox(strMessage);
		}
		m_bInTransaction = false;
		return true;
	}

	///<summary>Приключва танзакцията</summary>
	bool CommitTransaction()
	{
		if (!m_bInTransaction)
		{
			return false;
		}
		if (!OpenSession())
		{
			return false;
		}
		HRESULT hResult = m_oSession.Commit();
		if (FAILED(hResult))
		{
			CString strMessage;
			strMessage.Format(_T("Unable to commit transaction. HRESULT: 0x%08X"), hResult);
			AfxMessageBox(strMessage);
			return false;
		}
		m_bInTransaction = false;
		return true;
	}
	///<summary>Изпълнява завка</summary>
	template <typename TAccessor>
	bool ExecuteQuery(CString strSQLCommand, CCommand<CAccessor<TAccessor>>& oCommand, CDBPropSet* pPropSet = nullptr)
	{
		if (m_oSession.m_spOpenRowset == NULL)
		{
			if (!OpenSession())
			{
				return false;
			}
		}
		HRESULT hResult = S_OK;

		if (pPropSet != nullptr)
		{
			hResult = oCommand.Open(m_oSession, strSQLCommand, pPropSet);
		}
		else
		{
			hResult = oCommand.Open(m_oSession, strSQLCommand);
		}
		
		return true;
	}

	// Members
	// ----------------
private:
	///<summary>Сесия</summary>
	CSession m_oSession;
	///<summary>За проверка за съществуваща транзакция</summary>
	bool m_bInTransaction;
};