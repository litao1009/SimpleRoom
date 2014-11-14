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

	virtual	void	OnResize();

	virtual bool	PreRender3D();

	virtual void	PostRender3D();

	virtual bool	PreRender2D();

	virtual void	PostRender2D();

public:

	virtual	bool	Valid();

private:

	irr::core::vector2di	CursorIPos_;
	GraphODLWPtr			RootODL_;
};

typedef	std::shared_ptr<RoomLayoutDoorController>	RoomLayoutDoorControllerSPtr;