#pragma once

#include "RenderController/IRoomLayoutODLBaseCtrller.h"
#include "ODL/GraphODLFwd.h"
#include "ODL/WallODL.h"

class	RoomLayoutWallCtrller : public IRoomLayoutODLBaseCtrller
{

	class	Imp;
	std::unique_ptr<Imp>	ImpUPtr_;

public:

	RoomLayoutWallCtrller(const GraphODLWPtr& graphODL, const SRenderContextWPtr& rc);
	~RoomLayoutWallCtrller();

public://IRenderController

	virtual	void	Init();

	virtual	bool	OnPostEvent(const irr::SEvent& evt);

	virtual bool	PreRender3D();

	virtual void	PostRender3D();

public:

	virtual	bool	CheckValid();

private:

	irr::core::vector2di	CursorIPos_;
	GraphODLWPtr			RootODL_;
};

typedef	std::shared_ptr<RoomLayoutWallCtrller>	RoomLayoutWallCtrllerSPtr;