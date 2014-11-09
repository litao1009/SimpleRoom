#pragma once

#include "irrEngine/IRenderController.h"
#include "ODL/GraphODLFwd.h"

class	RoomLayoutBrowserCtrller : public IRenderController
{
	class	Imp;
	std::unique_ptr<Imp>	ImpUPtr_;

public:

	RoomLayoutBrowserCtrller(const GraphODLWPtr& graphODL);
	~RoomLayoutBrowserCtrller();

public:

	virtual void	Init();

	virtual bool	OnPostEvent(const irr::SEvent& evt);

	virtual bool	PreRender3D();

	virtual void	PostRender3D();
};