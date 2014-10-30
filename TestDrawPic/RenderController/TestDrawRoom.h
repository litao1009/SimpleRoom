#ifndef TestDrawRoom_h__
#define TestDrawRoom_h__

#pragma once

#include "irrEngine/IRenderController.h"
#include "ODL/GraphODLFwd.h"


class	TestDrawRoomCtrller : public IRenderController
{
	class	Imp;
	std::unique_ptr<Imp>	ImpUPtr_;

public:

	TestDrawRoomCtrller(const GraphODLWPtr& graphODL);
	~TestDrawRoomCtrller();

public:

	virtual void	Init();

	virtual bool	OnPostEvent(const irr::SEvent& evt);

	virtual bool	PreRender3D();

	virtual void	PostRender3D();
};
#endif // TestDrawRoom_h__
