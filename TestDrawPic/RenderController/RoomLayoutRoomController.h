#pragma once

#include "RenderController/IRoomLayoutODLBaseCtrller.h"
#include "ODL/GraphODLFwd.h"
#include "ODL/RoomODL.h"

class	RoomLayoutRoomController : public IRoomLayoutODLBaseCtrller
{

	class	Imp;
	std::unique_ptr<Imp>	ImpUPtr_;

public:

	RoomLayoutRoomController(const GraphODLWPtr& graphODL, const SRenderContextWPtr& rc);
	~RoomLayoutRoomController();

public://IRenderController

	virtual	void	Init();

	virtual	bool	OnPostEvent(const irr::SEvent& evt);

	virtual bool	PreRender3D();

	virtual void	PostRender3D();

public:

	virtual	bool	CheckValid();

private:

	GraphODLWPtr			RootODL_;
};

typedef	std::shared_ptr<RoomLayoutRoomController>	RoomLayoutRoomControllerSPtr;