#pragma once

#pragma once

#include "BaseODL.h"

#include "GraphODLFwd.h"

#include "CornerODLFwd.h"
#include "WallODLFwd.h"
//#include "RoomFwd.h"


class	GraphODL : public CBaseODL
{
public:

	GraphODL(const SRenderContextWPtr& rc);

	~GraphODL();

public:

	CornerODLList	GetAllCorners();

	WallODLList		GetAllWalls();

	WallODLList		GetWallsOnCorner(const CornerODLSPtr& corner);

	CornerODLSPtr	CreateCorner(const gp_Pnt& position);

	CornerODLSPtr	CreateCornerBySplitWall(const WallODLSPtr& toSplit, const gp_Pnt& position);

	bool			RemoveCorner(const CornerODLSPtr& corner);

	WallODLSPtr		AddWall(const CornerODLSPtr& corner1, const CornerODLSPtr& corner2);

	bool			RemoveWall(const WallODLSPtr& wall);

private:

	Graph	Graph_;
};