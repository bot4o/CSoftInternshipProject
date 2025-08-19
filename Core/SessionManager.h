#pragma once
#include "pch.h"  
#include "Resource.h"  
#include "atldbcli.h"
#include "DBConnection.h"
#include "DLLExport.h"


/////////////////////////////////////////////////////////////////////////////
// CSampleClass
template<typename TRecord, typename TAccessor>
class CSessionManager
{

	// Constants
	// ----------------


	// Constructor / Destructor
	// ---------------- 
public:
	CSessionManager()
	{
		OpenSession();
	}
	virtual ~CSessionManager()
	{
		CloseSession();
	}

	// Methods
	// ----------------
	bool OpenSession()
	{
		if (m_oSession.m_spOpenRowset != NULL)
		{
			return true; //Session opened already
		}
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
        return true;
	}
	void CloseSession()
	{
		if (m_oSession.m_spOpenRowset != NULL)
		{
			m_oCommand.Close();
			m_oSession.Close();
		}
	}
	bool ExecuteQuery(CString strSQLCommand, CDBPropSet* pPropSet = nullptr)
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
			hResult = m_oCommand.Open(m_oSession, strSQLCommand, pPropSet);
		}
		else
		{
			hResult = m_oCommand.Open(m_oSession, strSQLCommand);
		}
		
		return true;
	}
	bool BeginTransaction()
	{
		if (m_oSession.m_spOpenRowset == NULL) 
		{
			if (!OpenSession())
			{
				return false;
			}
		}
		XACTTRANSINFO oTransInfo;
		HRESULT hResult = m_oSession.GetTransactionInfo(&oTransInfo);
		if (hResult == XACT_E_NOTRANSACTION)
		{
			hResult = m_oSession.StartTransaction();
			if (FAILED(hResult))
			{
				CString strMessage;
				strMessage.Format(_T("Unable to start transaction. HRESULT: 0x%08X"), hResult);
				AfxMessageBox(strMessage);
				return false;
			}
			return true;
		}
		else if (SUCCEEDED(hResult))
		{
			return true;
		}
		else
		{
			return false;
		}
		
	}
	bool RollbackTransaction()
	{
		if (m_oSession.m_spOpenRowset == NULL)
		{
			if (!OpenSession())
			{
				return false;
			}
		}

		HRESULT hResult = m_oSession.Abort();
		if (FAILED(hResult))
		{
			CString strMessage;
			strMessage.Format(_T("Unable to abort transaction. HRESULT: 0x%08X"), hResult);
			AfxMessageBox(strMessage);
			return false;
		}
		return true;

	}
	bool CommitTransaction()
	{
		if (m_oSession.m_spOpenRowset == NULL)
		{
			if (!OpenSession())
			{
				return false;
			}
		}

		HRESULT hResult = m_oSession.Commit();
		if (FAILED(hResult))
		{
			CString strMessage;
			strMessage.Format(_T("Unable to commit transaction. HRESULT: 0x%08X"), hResult);
			AfxMessageBox(strMessage);
			return false;
		}
		return true;
	}

	TRecord& GetRecord()
	{
		return m_oCommand.GetRecord();
	}

	HRESULT InsertData(int nAccessorIndex)
	{
		HRESULT hResult = m_oCommand.Insert(nAccessorIndex);
		return hResult;
	}
	HRESULT MoveNext()
	{
		HRESULT hResult = m_oCommand.MoveNext();
		return hResult;
	}
	HRESULT MoveFirst()
	{
		HRESULT hResult = m_oCommand.MoveFirst();
		return hResult;
	}
	HRESULT SetData(int nAccessorIndex)
	{
		HRESULT hResult = m_oCommand.SetData(nAccessorIndex);
		return hResult;
	}
	HRESULT Delete()
	{
		HRESULT hResult = m_oCommand.Delete();
		return hResult;
	}

	// Members
	// ----------------
private:
	CCommand<CAccessor<TAccessor>> m_oCommand;
	/// <summary>Самата сесия</summary
	CSession m_oSession;
};