#ifndef DBConnection_h__
#define DBConnection_h__

#pragma once

#include <atlstr.h>
#include <vector>
#include <algorithm>
#include "sqlite3.h"
#ifdef _USE_OLE_ADO
#import "C:\Program Files (x86)\Common Files\System\ado\msado28.tlb"  \
	no_namespace  rename("EOF","adoEOF") rename("BOF","adoBOF")
#endif // _USE_OLE_ADO
using namespace std;

//数据库类型
enum DBTypeEnum
{
	DBT_SQLITE=1,
	DBT_OLE=2
};
//连接状态，暂时莫得啥子实际用途
enum DBStatusEnum
{
	DBS_OPEN_SUCCESS=1,
	DBS_OPEN_ALREADY=2,
	DBS_CLOSED=3,
	DBS_BUSY=4,
	DBS_READ_REFUSE=5
};

typedef struct RowInfo
{
	vector<CString> ColList;
}RowInfo;

typedef struct ReturnInfo
{
	vector<CString> Columns;
	vector<RowInfo> RowList;
} ReturnInfo;

class CDBConnection
{
public:
	CDBConnection();
	virtual ~CDBConnection(void);

public:
	static CDBConnection& GetConnection()
	{
		static CDBConnection pCon;
		return pCon;
	}

public:
	DBStatusEnum Open(const TCHAR* strConnectionString, DBTypeEnum dbType=DBT_SQLITE);
	DBStatusEnum Close();
	HRESULT GetTable(CString strSQL,ReturnInfo& arrReturn);
	//$(SolutionDir)externals/cryptoPP/include
	//link:cryptoPP.lib/cryptoPP
	//HRESULT GetTable(std::wstring strWSQL, ReturnInfo& arrReturn);
	int ExecuteCmd(CString strSQL);
	//int ExecuteCmd(std::wstring strWSQL);
	HRESULT GetLastError();
	HRESULT GetLastError(CString& strLastError);
	//事务
	DBStatusEnum BeginTrans();
	DBStatusEnum CommitTrans();
	DBStatusEnum RollbackTrans();

	INT GetColIndex(TCHAR* strColName,ReturnInfo& rtInfo);
	CString GetColValue(TCHAR* strColName,ReturnInfo& rtInfo,INT nRowIndex);
private:
	DBTypeEnum m_dbType;
#ifdef _USE_OLE_ADO
	_ConnectionPtr  m_pADOConn;
#endif
	
	sqlite3* m_pSLTConn;
	CString m_strLastError;
	HRESULT m_lLastError;
	int _result_open;

	DBStatusEnum m_dbStatus;

};

#endif // DBConnection_h__
