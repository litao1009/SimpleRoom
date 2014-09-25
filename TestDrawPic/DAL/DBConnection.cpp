#include "stdafx.h"
#include "DBConnection.h"

CDBConnection::CDBConnection()
{
	m_dbStatus=DBS_CLOSED;
#ifdef _USE_OLE_ADO
	CoInitialize(NULL);
	m_pADOConn=NULL;
#endif
	m_pSLTConn=NULL;
	m_dbType=DBT_SQLITE;
	m_lLastError=0;
	m_strLastError=_T("");
	
}

CDBConnection::~CDBConnection(void)
{
	Close();
#ifdef _USE_OLE_ADO
	CoUninitialize();
	m_pADOConn=NULL;
#endif
	m_dbStatus=DBS_CLOSED;
	m_pSLTConn=NULL;
	m_dbType=DBT_SQLITE;
}

DBStatusEnum CDBConnection::Open(const TCHAR* strConnectionString,DBTypeEnum dbType)
{
	m_lLastError=0;
	Close();
	m_dbType=dbType;
	DBStatusEnum dbsRet;
	dbsRet=DBS_READ_REFUSE;
	switch (m_dbType)
	{
	case DBT_SQLITE:
		{
			if (DBS_CLOSED)
			{
				CStringArray arrList;
				CString strArr=strConnectionString;
				while(TRUE)
				{
					int nPos=strArr.Find(_T(";"));
					if ( nPos < 0 )
					{
						if ( strArr!= _T("") )
							arrList.Add(strArr) ; 
						break; 
					}
					CString sItem = strArr.Left( nPos ) ; 
					arrList.Add( sItem ) ; 
					strArr = strArr.Right(strArr.GetLength() - nPos - 1 ) ; 
				}
#ifdef _UNICODE
				_result_open = sqlite3_open16(arrList[0].GetBuffer(), &m_pSLTConn);
#else
				_result_open = sqlite3_open(arrList[0].GetBuffer(), &m_pSLTConn);
#endif
				arrList[0].ReleaseBuffer();
				if (arrList.GetCount()>1)
				{
					if(_result_open == SQLITE_OK)
					{
#ifdef _UNICODE
						int nPwdLen = WideCharToMultiByte(CP_ACP, 0, arrList[1].GetBuffer(), arrList[1].GetLength(), NULL, 0, NULL, NULL);
						arrList[1].ReleaseBuffer();
						char* bufPwd = new char[nPwdLen + 1];
						WideCharToMultiByte(CP_ACP, 0, arrList[1].GetBuffer(), nPwdLen, bufPwd, nPwdLen, NULL, NULL);   
						arrList[1].ReleaseBuffer();
						bufPwd[nPwdLen] = '\0';
						_result_open = sqlite3_key(m_pSLTConn, bufPwd, nPwdLen);
						delete[] bufPwd;
#else
						_result_open = sqlite3_key(m_pSLTConn, arrList[1].GetBuffer();, arrList[1].GetLength());
						arrList[1].ReleaseBuffer();
#endif

					}
				}
				
				if (_result_open == SQLITE_OK)
				{
					dbsRet=DBS_OPEN_SUCCESS;
					m_dbStatus=DBS_OPEN_SUCCESS;
				}
				else 
				{
					m_strLastError = sqlite3_errmsg(m_pSLTConn);
					m_lLastError=_result_open;
				}
			}
		}
		break;
		case DBT_OLE:
		{
#ifdef _USE_OLE_ADO
			try
			{
				HRESULT hr = m_pADOConn.CreateInstance(_T("ADODB.Connection"));
				if (FAILED(hr))
				{
					_com_error e(hr);
					m_strLastError=e.ErrorMessage();
					m_lLastError=e.Error();
					return dbsRet;
				}
				hr= m_pADOConn->Open(_bstr_t(strConnectionString),_T(""),_T(""),adModeUnknown);
				if (FAILED(hr))
				{
					_com_error e(hr);
					m_strLastError=e.ErrorMessage();
					m_lLastError=e.Error();
					return dbsRet;
				}

				
			}
			catch(...)
			{
				return dbsRet;
			}
			m_dbStatus=DBS_OPEN_SUCCESS;
			dbsRet=DBS_OPEN_SUCCESS;
#endif
		}
		break;
	}
	return dbsRet;
}

DBStatusEnum CDBConnection::Close()
{
	m_lLastError=0;
	DBStatusEnum dbsRet;
	dbsRet=DBS_CLOSED;
	switch (m_dbType)
	{
		case DBT_SQLITE:
		{
			if (m_pSLTConn)
			{
				sqlite3_close(m_pSLTConn);
				m_pSLTConn = NULL;
			}
			dbsRet=DBS_CLOSED;
			m_dbStatus=DBS_CLOSED;
		}
		break;
		case DBT_OLE:
		{
			HRESULT hResult=0;
#ifdef _USE_OLE_ADO
			try
			{
				if(m_pADOConn!=NULL)
				{
					if(m_pADOConn->State!=adStateClosed)
					{
						hResult=m_pADOConn->Close(); 
					}
					m_pADOConn.Release();
				}
				dbsRet=DBS_CLOSED;
				m_dbStatus=DBS_CLOSED;
			}
			catch(_com_error e)
			{
				m_strLastError=e.ErrorMessage();
				m_lLastError=e.Error();
			}
#endif
			
		}
		break;
	}
	return dbsRet;
}

HRESULT CDBConnection::GetTable(CString strSQL,ReturnInfo& arrReturn)
{
	m_lLastError=0;
	HRESULT hReturn=S_OK;
	switch (m_dbType)
	{
	case DBT_SQLITE:
		{
			sqlite3_stmt *statement;
			try
			{
#ifdef _UNICODE
				int nRet=sqlite3_prepare16_v2(m_pSLTConn, strSQL, -1, &statement, NULL);
#else
				int nRet=sqlite3_prepare_v2(m_pSLTConn, strSQL, -1, &statement, NULL);
#endif
				if (nRet==SQLITE_OK)
				{
					int nCount=sqlite3_column_count(statement);
					for (int i=0;i<nCount;i++)
					{
						CString strColName;
#ifdef _UNICODE
						strColName.Format(_T("%s"),sqlite3_column_name16(statement,i));
#else
						strColName.Format(_T("%s"),sqlite3_column_name(statement,i));
#endif
						INT nDotPos=strColName.Find('.');
						if (nDotPos>=0)
						{
							strColName.Delete(0, nDotPos+1);
						}
						arrReturn.Columns.push_back(strColName);
					}
					//遍历数据
					while (sqlite3_step(statement) == SQLITE_ROW) 
					{
						RowInfo rdInfo;
						for (int i=0;i<nCount;i++)
						{
							CString strInput=_T("");
							try
							{
#ifdef _UNICODE
								strInput.Format(_T("%s"),sqlite3_column_text16(statement, i));
#else
								strInput.Format(_T("%s"),sqlite3_column_text(statement, i));
#endif
							}
							catch (exception&)
							{
								strInput=_T("");
							}
							rdInfo.ColList.push_back(strInput);
						}
						arrReturn.RowList.push_back(rdInfo);
					}
					sqlite3_reset(statement);
					sqlite3_finalize(statement);
				}
				else
				{
					m_strLastError = sqlite3_errmsg(m_pSLTConn);
					hReturn=S_FALSE;
				}
				m_lLastError=nRet;
				return nRet;
			}
			catch (exception* e)
			{
				m_strLastError= e->what();
				hReturn=S_FALSE;
				m_lLastError=-1;
			}
		}
		break;
	case DBT_OLE:
		{
#ifdef _USE_OLE_ADO
			_RecordsetPtr   pRecordset = NULL;
			HRESULT hr;
			_variant_t RecordsAffected;
			pRecordset.CreateInstance(_uuidof(Recordset));
			try
			{
				pRecordset = m_pADOConn->Execute ((_bstr_t)strSQL , &RecordsAffected , adCmdUnknown);
				long nColCount,i;
				Fields *fls=NULL;
				BSTR bstrColName;
				hr=pRecordset->get_Fields(&fls);
				if (SUCCEEDED(hr))
					fls->get_Count(&nColCount);
				arrReturn.Columns.clear();
				arrReturn.RowList.clear();
				for (i=0;i<nColCount;i++)
				{
					DataTypeEnum dt;
					Field *fd=NULL;
					VARIANT nIndex;
					nIndex.vt=VT_I4;
					nIndex.lVal=i;
					fls->get_Item(nIndex,&fd);
					fd->get_Type(&dt);
					fls->Item[i]->get_Name(&bstrColName);
					CString strColName;
#ifdef _UNICODE
					strColName.Format(_T("%s"),W2T(bstrColName));
#else
					strColName=bstrColName;
#endif
					arrReturn.Columns.push_back(strColName);
				}
				while(!pRecordset->adoEOF)
				{
					RowInfo rdInfo;
					for (int m=0;m<nColCount;m++)
					{
						variant_t vIndex =(long)m;
						_variant_t var=pRecordset->GetCollect(vIndex);
						_bstr_t str;
						CString strValue=_T("");
						if(var.vt!=VT_NULL)
						{
							str= _bstr_t(var);
#ifdef _UNICODE
							strValue.Format(_T("%s"),W2T(str));
#else
							strValue=(LPCSTR)str;
#endif
						}
						rdInfo.ColList.push_back(strValue);
					}
					arrReturn.RowList.push_back(rdInfo);
					pRecordset->MoveNext();
				}
			}
			catch (_com_error e)
			{
				m_strLastError = e.ErrorMessage();
				m_lLastError=e.Error();
				hReturn=S_FALSE;
			}
			pRecordset->Close();
			pRecordset.Release();
			pRecordset=NULL;
#endif
			
		}
		break;
	}
	return hReturn;
}

//HRESULT CDBConnection::GetTable(std::wstring strWSQL, ReturnInfo& arrReturn)
//{
//	//auto cm = FastEncrypt::AES_GenerateKey();
//	//auto cm2 = FastEncrypt::AES_CFBEncrypt(cm, );
//	//FastEncrypt::AES_CFBDecrypt()
//	//auto cm3 = FastEncrypt::RSA_GenerateKey();
//	//auto cm4 = FastEncrypt::RSA_PubEncrypt(cm3.)
//	return GetTable(strWSQL.c_str(), arrReturn);
//}

HRESULT CDBConnection::GetLastError()
{
	return m_lLastError;
}

HRESULT CDBConnection::GetLastError(CString& strLastError)
{
	strLastError=m_strLastError;
	return m_lLastError;
}

DBStatusEnum CDBConnection::BeginTrans()
{
	DBStatusEnum dbRet=DBS_OPEN_SUCCESS;
	switch (m_dbType)
	{
	case DBT_SQLITE:
		{
			
			if (sqlite3_exec(m_pSLTConn, "begin", 0, 0, 0) != SQLITE_OK) {
				m_strLastError=sqlite3_errmsg(m_pSLTConn);
				dbRet = DBS_READ_REFUSE;
			}
		}
		break;
	case DBT_OLE:
		{

		}
		break;
	}
	return dbRet;
}


DBStatusEnum CDBConnection::CommitTrans()
{
	DBStatusEnum dbRet=DBS_OPEN_SUCCESS;
	switch (m_dbType)
	{
	case DBT_SQLITE:
		{

			if (sqlite3_exec(m_pSLTConn, "commit", 0, 0, 0) != SQLITE_OK) {
				m_strLastError=sqlite3_errmsg(m_pSLTConn);
				dbRet = DBS_READ_REFUSE;
			}
		}
		break;
	case DBT_OLE:
		{

		}
		break;
	}
	return dbRet;
}


DBStatusEnum CDBConnection::RollbackTrans()
{
	DBStatusEnum dbRet=DBS_OPEN_SUCCESS;
	switch (m_dbType)
	{
	case DBT_SQLITE:
		{
			if (sqlite3_exec(m_pSLTConn, "rollback", 0, 0, 0) != SQLITE_OK) 
			{
				m_strLastError=sqlite3_errmsg(m_pSLTConn);
				dbRet = DBS_READ_REFUSE;
			}
		}
		break;
	case DBT_OLE:
		{

		}
		break;
	}
	return dbRet;
}

int CDBConnection::ExecuteCmd(CString strSQL)
{
	m_lLastError=0;
	INT nRet=0;
	switch (m_dbType)
	{
	case DBT_SQLITE:
		{
			sqlite3_stmt *statement;
#ifdef _UNICODE
			nRet=sqlite3_prepare16_v2(m_pSLTConn, strSQL, -1, &statement, NULL);
#else
			nRet=sqlite3_prepare_v2(m_pSLTConn, strSQL, -1, &statement, NULL);
#endif
			if (sqlite3_step(statement) != SQLITE_DONE) 
			{
				m_strLastError=sqlite3_errmsg(m_pSLTConn);
				nRet=-1;
			}
			sqlite3_reset(statement);
			sqlite3_finalize(statement);
		}
		break;
	case DBT_OLE:
		{
#ifdef _USE_OLE_ADO
			try
			{
				_variant_t RecordsAffected;
				m_pADOConn->Execute(_bstr_t(strSQL),&RecordsAffected,adCmdUnknown);
				nRet=RecordsAffected.intVal;
			}
			catch (_com_error e)
			{
				m_strLastError = e.ErrorMessage();
				m_lLastError=e.Error();
				nRet=-1;
			}
#endif
			
		}
		break;
	}
	return nRet;
}

INT CDBConnection::GetColIndex(TCHAR* strColName,ReturnInfo& rtInfo)
{
	m_lLastError=0;
	INT nColIndex=-1;
	for (INT i=0;i<(INT)rtInfo.Columns.size();i++)
	{	
		if (_tcsicmp(rtInfo.Columns[i].GetBuffer(),strColName)==0)
		{
			nColIndex=i;
			rtInfo.Columns[i].ReleaseBuffer();
			break;
		}
		rtInfo.Columns[i].ReleaseBuffer();
	}
	return nColIndex;
}

CString CDBConnection::GetColValue(TCHAR* strColName,ReturnInfo& rtInfo,INT nRowIndex)
{
	m_lLastError=0;
	INT nColIndex=-1;
	if (nRowIndex>=(INT)rtInfo.RowList.size())
	{
		m_strLastError = _T("RowIndex is bigger than size.");
		m_lLastError = -1;
		return _T("");
	}
	for (INT i=0; i<(INT)rtInfo.Columns.size(); i++)
	{
		if (_tcsicmp(rtInfo.Columns[i].GetBuffer(),strColName)==0)
		{
			nColIndex=i;
			rtInfo.Columns[i].ReleaseBuffer();
			break;
		}
		rtInfo.Columns[i].ReleaseBuffer();
	}
	if (nColIndex==-1)
	{
		m_strLastError = _T("Column not exsit.");
		m_lLastError = -1;
		return _T("");
	}
	else
	{
		if (_tcsicmp(rtInfo.RowList[nRowIndex].ColList[nColIndex],_T("(null)"))==0)
		{
			return _T("");
		}
		return rtInfo.RowList[nRowIndex].ColList[nColIndex];
	}
}