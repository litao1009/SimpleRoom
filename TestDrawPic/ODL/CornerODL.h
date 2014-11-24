#pragma once

#include "BaseODL.h"
#include "CornerODLFwd.h"
#include "RoomODLFwd.h"
#include "GraphODL.h"

class	CornerODL : public BaseODL
{
	class	Imp;
	std::unique_ptr<Imp>	ImpUPtr_;
public:

	typedef	Graph::vertex_descriptor	VertexIndex;

public:

	CornerODL(const SRenderContextWPtr& rc);
	~CornerODL();

public:

	virtual	EObjectDisplayLayerType	GetType() const { return EODLT_CORNER; }

	virtual	void	Init();

public:

	const gp_Pnt&	GetPosition() const { return Position_; }

	void	SetPosition(const gp_Pnt& pos);

	const VertexIndex&	GetIndex() const { return GraphIndex_; }

	void	SetIndex(const VertexIndex& val) { GraphIndex_ = val; }

	RoomODLList	GetRooms() const;

	void		AddRoom(const RoomODLSPtr& room);

	void		RemoveRoom(const RoomODLSPtr& room);

public:

	void	SetVisible(bool vis);

	bool	GetVisible() const;

private:

	VertexIndex	GraphIndex_;
	gp_Pnt		Position_;
};