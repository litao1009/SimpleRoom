#ifndef DrawLineRoomCtrller_h__
#define DrawLineRoomCtrller_h__

#pragma once

#include "irrEngine/IRenderController.h"
#include "ODL/GraphODLFwd.h"


class	DrawLineRoomCtrller : public IRenderController
{
	class	Imp;
	std::unique_ptr<Imp>	ImpUPtr_;

public:

	DrawLineRoomCtrller(const GraphODLWPtr& graphODL);
	~DrawLineRoomCtrller();

public:

	virtual void	Init();

	virtual bool	OnPostEvent(const irr::SEvent& evt);

	virtual bool	PreRender3D();

	virtual void	PostRender3D();

public:

	void	Reset();
};
#endif // DrawLineRoomCtrller_h__
