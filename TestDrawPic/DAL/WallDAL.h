#ifndef WallDAL_h__
#define WallDAL_h__

#pragma once
#include "basedal.h"
class CWallDAL :
	public CBaseDAL
{
public:
	CWallDAL(CDBConnection* pCon);
	~CWallDAL(void);
};

typedef	std::shared_ptr<CWallDAL>	CWallDALSPtr;
typedef	std::weak_ptr<CWallDAL>		CWallDALWPtr;
#endif // WallDAL_h__
