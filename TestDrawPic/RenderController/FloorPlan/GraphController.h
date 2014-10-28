#pragma once

#include "CornerFwd.h"
#include "WallFwd.h"

#include "gp_Pnt.hxx"

class	GraphController
{
	class	Imp;
	std::unique_ptr<Imp>	ImpUPtr_;

public:

	GraphController();

	~GraphController();

public:

	static	GraphController&	GetInstance();

public:

	CornerList	GetAllCorners();

	WallList	GetAllWalls();

	WallList	GetWallsOnCorner(const CornerSPtr& corner);

	CornerSPtr	CreateCorner(const gp_Pnt& position);

	CornerSPtr	CreateCornerBySplitWall(const WallSPtr& toSplit, const gp_Pnt& position);

	bool		RemoveCorner(const CornerSPtr& corner);

	WallSPtr	AddWall(const CornerSPtr& corner1, const CornerSPtr& corner2);

	bool		RemoveWall(const WallSPtr& wall);
};