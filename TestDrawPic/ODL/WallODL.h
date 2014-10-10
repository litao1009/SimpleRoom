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

	static	CBaseODLSPtr	CreateByBottomFace(SRenderContextWPtr renderContext, const TopoDS_Shape& bottomFace, const gp_Dir& wallPathDir, float wallHeight);

public:
	
	virtual	EObjectDisplayLayerType	GetType() const { return EODLT_WALL; }

	virtual void	UpdateSweeping();

public:

	void	SetDefaultTexture();
};

typedef	std::shared_ptr<CWallODL>	CWallODLSPtr;
typedef	std::weak_ptr<CWallODL>		CWallODLWPtr;

#endif // WallODL_h__
