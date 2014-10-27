#ifndef Wall_h__
#define Wall_h__

#pragma once

#include "WallFwd.h"

#include "TopoDS_Edge.hxx"

class	Wall : public std::enable_shared_from_this<Wall>
{
public:

public:

	bool	IsBezierCurve() const;

	float	GetLength() const;

	TopoDS_Edge	GetEdge() const;
};

#endif // Wall_h__