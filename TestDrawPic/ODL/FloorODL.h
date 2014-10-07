#ifndef FloorODL_h__
#define FloorODL_h__

#pragma once

#include "BaseODL.h"

#include "TopoDS_Shape.hxx"

class	CFloorODL;
typedef	std::shared_ptr<CFloorODL>	CFloorODLSPtr;
typedef	std::weak_ptr<CFloorODL>	CFloorODLWPtr;

class CFloorODL : public CBaseODL
{
public:

	CFloorODL();
	~CFloorODL(void);

public:

	typedef	std::vector<irr::core::vector3df>	PointList;

public:

	static	CFloorODLSPtr	CreateFloorByPath(SRenderContextWPtr renderContext, const TopoDS_Shape& bottomFace, const PointList& pntList, const gp_Pnt& center);

	static	CFloorODLSPtr	CreateFloorByRect(SRenderContextWPtr renderContext, const irr::core::vector3df& first, const irr::core::vector3df& last, const irr::core::vector3df& thickVec, const gp_Pnt& center);

public:
	
	virtual	EObjectDisplayLayerType	GetType() const { return EODLT_FLOOR; }

	virtual void	UpdateSweeping();

public:

	void SetDefaultTexture();
};

#endif // FloorODL_h__
