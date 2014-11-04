#ifndef FloorODL_h__
#define FloorODL_h__

#pragma once

#include "BaseODL.h"
#include "FloorODLFwd.h"


class FloorODL : public CBaseODL
{
public:

	FloorODL(const SRenderContextWPtr& rc);
	~FloorODL(void);

public:

	static	FloorODLSPtr	CreateByFace(SRenderContextWPtr renderContext, const TopoDS_Shape& floorFace);

public:
	
	virtual	EObjectDisplayLayerType	GetType() const { return EODLT_FLOOR; }

public:

	void SetDefaultTexture();
};

#endif // FloorODL_h__
