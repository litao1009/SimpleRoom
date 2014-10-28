#ifndef Wall_h__
#define Wall_h__

#pragma once

#include "WallFwd.h"
#include "CornerFwd.h"

#include "Graph.h"

#include "gp_Pnt.hxx"
#include "TopoDS_Edge.hxx"

#include <stdint.h>
#include <array>

class	Wall : public std::enable_shared_from_this<Wall>
{
	class	Imp;

public:

	typedef	std::array<gp_Pnt, 6>	MeshPoints;

	typedef	Graph::edge_descriptor	EdgeIndex;

	Wall( const CornerSPtr& firstCorner, const CornerSPtr& secondCorner,  float wallThickness = 200.f );

public:

	bool		IsBezierCurve() const;

	TopoDS_Edge	GetEdge() const;

	const EdgeIndex&	GetIndex() const { return GraphIndex_; }

	void		SetIndex(const EdgeIndex& val) { GraphIndex_ = val; }

	CornerWPtr	GetFirstCorner() const { return FirstCorner_; }

	CornerWPtr	GetSecondCorner() const { return SecondCorner_; }

	void		SetThickness(float val) { Thickness_ = val; }

	float	GetThickness() const { return Thickness_; }

	void		UpdateMesh();

	const MeshPoints&	GetMeshPoints() { return MeshPoints_; }

private:

	EdgeIndex	GraphIndex_;

	CornerWPtr	FirstCorner_;
	CornerWPtr	SecondCorner_;

	float		Thickness_;
	MeshPoints	MeshPoints_;
};

#endif // Wall_h__