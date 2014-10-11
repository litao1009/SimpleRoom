#ifndef DoorController_h__
#define DoorController_h__

#pragma once

#include "irrEngine/IRenderController.h"
#include "ODL/BaseODLFwd.h"

class	DoorController : public IRenderController
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

	DoorController();
	~DoorController();

public://IRenderController

	virtual	void	Init();

	virtual	bool	OnPostEvent(const irr::SEvent& evt);

	virtual	void	OnResize();

	virtual bool	PreRender3D();

	virtual void	PostRender3D();

	virtual bool	PreRender2D();

	virtual void	PostRender2D();

public:

	void	SetRootODL(CBaseODLWPtr root) { RootODL_ = root; }

public:

	EDoorState				State_;
	irr::core::vector2di	CursorIPos_;
	CBaseODLWPtr			RootODL_;
};

typedef	std::shared_ptr<DoorController>	DoorControllerSPtr;

#endif // DoorController_h__
