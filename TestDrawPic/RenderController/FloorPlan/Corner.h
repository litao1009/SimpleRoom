#ifndef Corner_h__
#define Corner_h__

#pragma once

#include "CornerFwd.h"
#include "Graph.h"

#include "gp_Pnt.hxx"

class	Corner : public std::enable_shared_from_this<Corner>
{
public:

	typedef	Graph::vertex_descriptor	VertexIndex;

public:

public:

	const gp_Pnt&	GetPosition() const { return Position_; }

	void	SetPosition(const gp_Pnt& pos) { Position_ = pos; }

	const VertexIndex&	GetIndex() const { return GraphIndex_; }

	void	SetIndex(const VertexIndex& val) { GraphIndex_ = val; }

private:

	VertexIndex	GraphIndex_;
	gp_Pnt		Position_;
};

#endif // Corner_h__