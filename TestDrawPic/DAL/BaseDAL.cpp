#include "stdafx.h"
#include "BaseDAL.h"

#include <string>

CBaseDAL::CBaseDAL(CDBConnection* pCon)
{
	m_pCon = pCon;
}


CBaseDAL::~CBaseDAL()
{
}

HRESULT CBaseDAL::GetLastError(CString& strLastError)
{
	return m_pCon->GetLastError(strLastError);
}


HRESULT CBaseDAL::GetLastError()
{
	return m_pCon->GetLastError();
}

void CBaseDAL::BeginTrans()
{
	m_pCon->BeginTrans();
}

void CBaseDAL::CommitTrans()
{
	m_pCon->CommitTrans();
}

void CBaseDAL::RollbackTrans()
{
	m_pCon->RollbackTrans();
}

float CBaseDAL::StrToFloat(CString& strInput)
{
	if (strInput.GetLength()==0)
	{
		return 0.0f;
	}
	auto fRet = std::stod(strInput.GetBuffer());
	//return _ttof(strInput);
	strInput.ReleaseBuffer();
	return (float)fRet;
}