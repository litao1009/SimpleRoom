#pragma once

#include "irrEngine/IRenderController.h"
#include "ODL/BaseODLFwd.h"

class	RoomLayoutDoorController : public IRenderController
{

	class	Imp;
	std::unique_ptr<Imp>	ImpUPtr_;

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

	irr::core::vector2di	CursorIPos_;
	BaseODLWPtr				RootODL_;
};