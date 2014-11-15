#pragma once

#include "RenderController/IRoomLayoutODLBaseCtrller.h"
#include "ODL/GraphODLFwd.h"
#include "ODL/DoorODL.h"

class	RoomLayoutDoorController : public IRoomLayoutODLBaseCtrller
{

	class	Imp;
	std::unique_ptr<Imp>	ImpUPtr_;

public:

	RoomLayoutDoorController(const GraphODLWPtr& graphODL, const SRenderContextWPtr& rc);
	~RoomLayoutDoorController();

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

typedef	std::shared_ptr<RoomLayoutDoorController>	RoomLayoutDoorControllerSPtr;