#pragma once

#pragma once

#include "BaseODL.h"

#include "GraphODLFwd.h"

#include "CornerODLFwd.h"
#include "WallODLFwd.h"
#include "RoomODLFwd.h"


class	GraphODL : public BaseODL
{
	class	Imp;
	std::unique_ptr<Imp>	ImpUPtr_;

public:

	GraphODL(const SRenderContextWPtr& rc);

	~GraphODL();

public:

	virtual	EObjectDisplayLayerType	GetType() const { return EODLT_GRAPH; }

public:

	CornerODLList	GetAllCorners();

	WallODLList		GetAllWalls();

	RoomODLList		GetAllRooms();

	WallODLList		GetWallsOnCorner(const CornerODLSPtr& corner);

	CornerODLSPtr	CreateCorner(const gp_Pnt& position);

	CornerODLSPtr	CreateCornerBySplitWall(const WallODLSPtr& toSplit, const gp_Pnt& position, bool researchRoom = true, bool updateMesh = true);

	bool			RemoveCorner(const CornerODLSPtr& corner);

	WallODLSPtr		AddWall(const CornerODLSPtr& corner1, const CornerODLSPtr& corner2, bool researchRoom = true, bool updateMesh = true );

	bool			RemoveWall(const WallODLSPtr& wall, bool needMerge = true, bool researchRoom = true, bool updateMesh = true );

	void			MergeWallIfNeeded(const CornerODLSPtr& corner, bool researchRoom = true, bool updateMesh = true );

	void			SearchRooms();

	void			UpdateWallMeshIfNeeded();

private:

	Graph	Graph_;
};