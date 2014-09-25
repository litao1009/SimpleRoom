#ifndef SkinUnitDAL_h__
#define SkinUnitDAL_h__

#pragma once
#include "basedal.h"
class CSkinUnitDAL :
	public CBaseDAL
{
public:
	CSkinUnitDAL(CDBConnection* pCon);
	~CSkinUnitDAL(void);
};

typedef	std::shared_ptr<CSkinUnitDAL>	CSkinUnitDALSPtr;
typedef	std::weak_ptr<CSkinUnitDAL>		CSkinUnitDALWPtr;
#endif // SkinUnitDAL_h__
