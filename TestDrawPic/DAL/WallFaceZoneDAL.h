#ifndef WallFaceZoneDAL_h__
#define WallFaceZoneDAL_h__

#pragma once
#include "basedal.h"
class CWallFaceZoneDAL :
	public CBaseDAL
{
public:
	CWallFaceZoneDAL(CDBConnection* pCon);
	~CWallFaceZoneDAL(void);
};

typedef	std::shared_ptr<CWallFaceZoneDAL>	CWallFaceZoneDALSPtr;
typedef	std::weak_ptr<CWallFaceZoneDAL>		CWallFaceZoneDALWPtr;
#endif // WallFaceZoneDAL_h__
