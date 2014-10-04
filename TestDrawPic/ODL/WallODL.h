#ifndef WallODL_h__
#define WallODL_h__

#pragma once

#include "BaseODL.h"

#include "TopoDS_Shape.hxx"

class CWallODL : public CBaseODL
{
public:

	CWallODL();
	~CWallODL(void);

public:

	typedef	std::vector<irr::core::vector3df>	PointList;

public:

	static	ChildrenList	CreateWallByBottomFace(SRenderContextWPtr renderContext, const TopoDS_Shape& bottomFace, const PointList& pntList, float wallHeight);

	static	ChildrenList	CreateWallByRectRange(SRenderContextWPtr renderContext, const irr::core::vector3df& first, const irr::core::vector3df& last, const irr::core::vector3df& thickVec, float wallHeight);

public:
	
	virtual	EObjectDisplayLayerType	GetType() const { return EODLT_WALL; }

	virtual void	UpdateSweeping();

public:

	void	SetDefaultTexture();
};

typedef	std::shared_ptr<CWallODL>	CWallODLSPtr;
typedef	std::weak_ptr<CWallODL>		CWallODLWPtr;

#endif // WallODL_h__
