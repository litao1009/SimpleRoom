#pragma once

#include "RenderController/IRoomLayoutODLBaseCtrller.h"
#include "ODL/GraphODLFwd.h"
#include "ODL/WindowODL.h"

class	RoomLayoutWindowCtrller : public IRoomLayoutODLBaseCtrller
{

	class	Imp;
	std::unique_ptr<Imp>	ImpUPtr_;

public:

	RoomLayoutWindowCtrller(const GraphODLWPtr& graphODL, const SRenderContextWPtr& rc);
	~RoomLayoutWindowCtrller();

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

typedef	std::shared_ptr<RoomLayoutWindowCtrller>	RoomLayoutWindowCtrllerSPtr;