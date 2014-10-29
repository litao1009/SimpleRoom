#pragma once

#include "BaseODL.h"
#include "CornerODLFwd.h"

#include "GraphODL.h"

class	CornerODL : public CBaseODL
{
public:

	typedef	Graph::vertex_descriptor	VertexIndex;

public:

	CornerODL(const SRenderContextWPtr& rc);
	~CornerODL();

public:

	virtual	EObjectDisplayLayerType	GetType() const { return EODLT_CORNER; }

public:

	const gp_Pnt&	GetPosition() const { return Position_; }

	void	SetPosition(const gp_Pnt& pos) { Position_ = pos; }

	const VertexIndex&	GetIndex() const { return GraphIndex_; }

	void	SetIndex(const VertexIndex& val) { GraphIndex_ = val; }

private:

	VertexIndex	GraphIndex_;
	gp_Pnt		Position_;
};