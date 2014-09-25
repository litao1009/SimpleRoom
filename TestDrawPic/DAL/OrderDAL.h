#ifndef OrderDAL_h__
#define OrderDAL_h__

#pragma once
#include "basedal.h"
class COrderDAL :
	public CBaseDAL
{
public:
	COrderDAL(CDBConnection* pCon);
	~COrderDAL(void);
};

typedef	std::shared_ptr<COrderDAL>	COrderDALSPtr;
typedef	std::weak_ptr<COrderDAL>	COrderDALWPtr;
#endif // OrderDAL_h__
