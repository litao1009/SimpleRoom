#pragma once

#include "RoomODLFwd.h"
#include "WallODLFwd.h"
#include "CornerODLFwd.h"

#include "BaseODL.h"


class	RoomODL : public CBaseODL
{
	class	Imp;
	std::unique_ptr<Imp>	ImpUPtr_;
public:

	RoomODL(const SRenderContextWPtr& rc);
	~RoomODL();

public:

	virtual	EObjectDisplayLayerType	GetType() const { return EODLT_ROOM; }

	virtual	void	Init() {}

	void	SetWallList(const WallODLList& list);

	void	SetCornerList(const CornerODLList& list);

	WallODLList	GetWallList() const;

	CornerODLList	GetCornerList() const;

	bool			Build();

	double			GetArea() const;

private:

	WallODLWList	WallList_;
	CornerODLWList	CornerList_;
};