#pragma once

#include "RenderController/IRoomLayoutODLBaseCtrller.h"
#include "ODL/BaseODLFwd.h"

class	RoomLayoutPillarController : public IRoomLayoutODLBaseCtrller
{

	class	Imp;
	std::unique_ptr<Imp>	ImpUPtr_;

public:

	RoomLayoutPillarController(const BaseODLSPtr& baseODL, const SRenderContextWPtr& rc);
	~RoomLayoutPillarController();

public://IRenderController

	virtual	void	Init();

	virtual	bool	OnPostEvent(const irr::SEvent& evt);

	virtual bool	PreRender3D();

	virtual void	PostRender3D();

public:

	virtual	bool	CheckValid();

private:

	BaseODLWPtr		RootODL_;
};

typedef	std::shared_ptr<RoomLayoutPillarController>	RoomLayoutPilarControllerSPtr;