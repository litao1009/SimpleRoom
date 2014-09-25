#ifndef DesignDAL_h__
#define DesignDAL_h__

#pragma once
#include "basedal.h"
class CDesignDAL :
	public CBaseDAL
{
public:
	CDesignDAL(CDBConnection* pCon);
	~CDesignDAL(void);
};

typedef	std::shared_ptr<CDesignDAL>	CDesignDALSPtr;
typedef	std::weak_ptr<CDesignDAL>	CDesignDALWPtr;
#endif // DesignDAL_h__
