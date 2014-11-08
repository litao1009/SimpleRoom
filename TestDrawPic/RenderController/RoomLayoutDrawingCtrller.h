#ifndef RoomLayoutDrawingCtrller_h__
#define RoomLayoutDrawingCtrller_h__

#pragma once

#include "irrEngine/IRenderController.h"
#include "ODL/GraphODLFwd.h"


class	RoomLayoutDrawingCtrller : public IRenderController
{
	class	Imp;
	std::unique_ptr<Imp>	ImpUPtr_;

public:

	RoomLayoutDrawingCtrller(const GraphODLWPtr& graphODL);
	~RoomLayoutDrawingCtrller();

public:

	virtual void	Init();

	virtual bool	OnPostEvent(const irr::SEvent& evt);

	virtual bool	PreRender3D();

	virtual void	PostRender3D();

public:

	void	Reset();
};
#endif // RoomLayoutDrawingCtrller_h__
