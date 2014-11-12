#ifndef RoomLayoutDoorController_h__
#define RoomLayoutDoorController_h__

#pragma once

#include "irrEngine/IRenderController.h"
#include "ODL/BaseODLFwd.h"

class	RoomLayoutDoorController : public IRenderController
{

	class	Imp;
	std::unique_ptr<Imp>	ImpUPtr_;

public:

	enum EDoorState
	{
		EDS_NONE,
		EDS_CREATE,
		EDS_PICKING
	};

public:

	RoomLayoutDoorController();
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

	void	SetRootODL(BaseODLWPtr root) { RootODL_ = root; }

public:

	EDoorState				State_;
	irr::core::vector2di	CursorIPos_;
	BaseODLWPtr				RootODL_;
};

typedef	std::shared_ptr<RoomLayoutDoorController>	DoorControllerSPtr;

#endif // RoomLayoutDoorController_h__
