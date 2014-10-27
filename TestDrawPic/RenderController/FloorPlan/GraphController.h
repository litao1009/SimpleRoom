#pragma once

#include "Graph.h"

#include "gp_Pnt.hxx"

class	GraphController
{
public:

public:

	static	GraphController&	GetInstance();

public:

	CornerList	GetAllCorners();

	WallList	GetAllWalls();

	WallList	GetWallsOnCorner(const CornerSPtr& corner);

	bool		AddCornerToTempPath(const CornerSPtr& corner);

	CornerSPtr	CreateCorner(const gp_Pnt& position);

	CornerSPtr	FindCorner(const gp_Pnt& position);

	bool		RemoveCorner(const CornerSPtr& corner);

	CornerList	GetTempPathCorners() const;

	bool		UpdatePath();
};