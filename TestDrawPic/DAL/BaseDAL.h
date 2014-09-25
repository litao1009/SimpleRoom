#ifndef BaseDAL_h__
#define BaseDAL_h__

#pragma once
#include "DBConnection.h"

class CBaseDAL
{
public:
	CBaseDAL(CDBConnection* pCon);
	virtual ~CBaseDAL();

public:
	HRESULT GetLastError(CString& strLastError);
	HRESULT GetLastError();
	void BeginTrans();
	void CommitTrans();
	void RollbackTrans();
	void FilterSQLSearchKey(CString& strInput);

	float StrToFloat(CString& strInput);

public:
	virtual HRESULT GetInfo(int nID){return S_OK;};
protected:
	CDBConnection* m_pCon;
};

typedef	std::shared_ptr<CBaseDAL>	CBaseDALSPtr;
typedef	std::weak_ptr<CBaseDAL>	CBaseDALWPtr;
#endif // BaseDAL_h__
