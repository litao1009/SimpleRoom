#ifndef WallFaceDAL_h__
#define WallFaceDAL_h__

#pragma once
#include "basedal.h"
class CWallFaceDAL :
	public CBaseDAL
{
public:
	CWallFaceDAL(CDBConnection* pCon);
	~CWallFaceDAL(void);
};

typedef	std::shared_ptr<CWallFaceDAL>	CWallFaceDALSPtr;
typedef	std::weak_ptr<CWallFaceDAL>		CWallFaceDALWPtr;
#endif // WallFaceDAL_h__
