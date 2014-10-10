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

	static	CFloorODLSPtr	CreateByFace(SRenderContextWPtr renderContext, const TopoDS_Shape& wallFace, const TopoDS_Shape& floorFace);

public:
	
	virtual	EObjectDisplayLayerType	GetType() const { return EODLT_FLOOR; }

	virtual void	UpdateSweeping();

public:

	void SetDefaultTexture();
};

#endif // FloorODL_h__
