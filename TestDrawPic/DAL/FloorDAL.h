#ifndef FloorDAL_h__
#define FloorDAL_h__

#pragma once
#include "basedal.h"
class CFloorDAL :
	public CBaseDAL
{
public:
	CFloorDAL(CDBConnection* pCon);
	~CFloorDAL(void);
};

typedef	std::shared_ptr<CFloorDAL>	CFloorDALSPtr;
typedef	std::weak_ptr<CFloorDAL>	CFloorDALWPtr;
#endif // FloorDAL_h__
