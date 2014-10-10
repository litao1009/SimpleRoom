#ifndef ObjGroupODL_h__
#define ObjGroupODL_h__

#pragma once

#include "BaseODL.h"

class CGroupODL : public CBaseODL
{
public:

	typedef	std::vector<irr::core::vector3df>	PointList;

public:

	CGroupODL(void);
	~CGroupODL(void);

public:

	virtual	EObjectDisplayLayerType	GetType() const { return EODLT_GROUP; }

public:

	static CBaseODLSPtr	CreateByWallPath( SRenderContextSPtr& rc, const TopoDS_Shape& bottomFace, const PointList& pntList, float wallHeight );

	static CBaseODLSPtr	CreateByRect( SRenderContextSPtr& rc, const irr::core::vector3df& firstPnt, const irr::core::vector3df& lastPnt, float wallThick, float wallHeight );
};

typedef	std::shared_ptr<CGroupODL>	CGroupODLSPtr;

#endif // ObjGroupODL_h__
